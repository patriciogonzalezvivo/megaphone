//
//  Tangram.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/6/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com. All rights reserved.
//

#include "Tangram.h"

Tangram::Tangram(){
    bDebug = false;
    bEdit = false;
    selectedLimb = -1;
    
    ofAddListener(ofEvents().mousePressed, this, &Tangram::_mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &Tangram::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &Tangram::_mouseReleased);
    ofAddListener(ofEvents().keyPressed, this, &Tangram::_keyPressed);
}

bool Tangram::load( string _file ){
    
    ofxXmlSettings XML;
    if (XML.loadFile(_file)){
        
        int total = XML.getNumTags("shape");
        
        //  If not have 7 shapes... delete everthing and make them
        //
        if ( total != limbs.size()){
            limbs.clear();
            for (int i = 0; i < total; i++){
                Shape newShape;
                newShape.makeNumber(i);
                limbs.push_back( newShape );
                XML.popTag();
            }
        }
        
        //  Just load the new positions and rotations
        //
        for (int i = 0; i < total; i++){
            if ( XML.pushTag("shape",i ) ){
                
                limbs[i].setPosition(ofPoint(XML.getValue("pos:x", 0.0),
                                             XML.getValue("pos:y", 0.0),
                                             XML.getValue("pos:z", 0.0)));
                limbs[i].setRotation(ofPoint(XML.getValue("rot:x", 0.0),
                                             XML.getValue("rot:y", 0.0),
                                             XML.getValue("rot:z", 0.0)));
                XML.popTag();
            }
        }
        
        _makeInsidePath();
        
        return true;
    } else {
        return false;
    }
}

bool Tangram::save( string _file ){
    ofxXmlSettings XML;
    XML.loadFile(_file);
    
    for (int i = 0; i < limbs.size(); i++){
        XML.addTag("shape");
    }
    
    for (int i = 0; i < limbs.size(); i++){
        if ( XML.pushTag("shape",i ) ){
            
            XML.setValue("pos:x", limbs[i].getPosition().x);
            XML.setValue("pos:y", limbs[i].getPosition().y);
            XML.setValue("pos:z", limbs[i].getPosition().z);
            XML.setValue("rot:x", limbs[i].getRotation().x);
            XML.setValue("rot:y", limbs[i].getRotation().y);
            XML.setValue("rot:z", limbs[i].getRotation().z);
            
            XML.popTag();
        }
    }
    return XML.saveFile(_file);
}

void Tangram::createFromGlyph( Glyph *_glyph ){
    limbs.clear();
    
    for (int i = 0; i < _glyph->limbs().size(); i++){
        Shape newShape;
        newShape.makeFromLimb(i, *_glyph );
        limbs.push_back( newShape );
    }
    
    _makeInsidePath();
}

void Tangram::createSet(){
    limbs.clear();
    
    for ( int i = 0; i < 7; i++){
        Shape newShape;
        newShape.makeNumber(i);
        limbs.push_back( newShape );
    }
    
    _makeInsidePath();
}

void Tangram::_makeInsidePath(){
    
    insidePath.clear();
    for (int i = 0; i < limbs.size(); i++){
        for (int j = 0; j < limbs[i].getVertices().size(); j++) {
            ofPoint vert = limbs[i].getVertices()[j];
            insidePath.addVertex( vert );
        }
    }
    insidePath.close();
    perimeter = insidePath.getPerimeter();
    area = insidePath.getArea();
    
    this->set( insidePath.getBoundingBox() );
}

void Tangram::update(){
    for(int i = 0; i < limbs.size(); i++){

        if (bEdit){
            limbs[i].bDebug = limbs[i].mouseHover();
        }
//        limbs[i].addAttractionForce( limbs[i].org, area*2.0, 0.1);

        if (bDebug)
            limbs[i].bDebug = limbs[i].mouseHover();
        
        limbs[i].update();
    }
}

void Tangram::draw(){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i].draw();
    }
}

int Tangram::getShapeNumberAt( ofPoint _pos ){
    int rta = -1;
    for(int i = 0; i < limbs.size(); i++){
        if (limbs[i].inside(_pos) ){
            rta = i;
            break;
        }
    }
    return rta;
}

int Tangram::getShapeNumberAt( float _pct ){
    
    float previus = 0.0;
    float target = _pct * perimeter;
    
    for (int i = 0; i < limbs.size(); i++){
        
        for(int j = 0; j < limbs[i].getVertices().size()-1; j++){
            ofPoint diff = limbs[i].getVertices()[j+1] - limbs[i].getVertices()[j];
            float lenght = diff.length();
            
            if ( target >= previus && target <= previus + lenght ){
                return j;
            }
            previus += lenght;
        }
        
    }
    return -1;
}

ofPoint	Tangram::getPathPositionAt( float _pct ){
    
	ofPoint pos;
    float previus = 0.0;
    float target = _pct * perimeter;
    
	for (int i = 0; i < insidePath.getVertices().size()-1; i++){
        ofPoint diff = insidePath.getVertices()[i+1] - insidePath.getVertices()[i];
        float lenght = diff.length();
        
		if ( target >= previus && target <= previus + lenght ){
			
			float pct = ( target - previus)/lenght;
			
			pos.x = (1-pct) * insidePath.getVertices()[i].x + (pct) * insidePath.getVertices()[i+1].x;
			pos.y = (1-pct) * insidePath.getVertices()[i].y + (pct) * insidePath.getVertices()[i+1].y;
            
            break;
		}
        
        previus += lenght;
	}
    
    return pos;
}

ofPoint Tangram::getNormalPoint(ofPoint p, ofPoint a, ofPoint b) {
    ofPoint ap = p - a;
    ofPoint ab = b - a;
    ab.normalize();
    ab *= ap.dot(ab);
    return a+ab;
}

ofPoint Tangram::getGetClosePath(ofPoint _pos){
    
    ofPoint normal;
    ofPoint target;
    float minDist = 1000000;
    
    for (int i = 0; i < insidePath.getVertices().size()-1; i++) {
        
        ofPoint a = insidePath.getVertices()[i];
        ofPoint b = insidePath.getVertices()[i+1];
        
        ofPoint normalPoint = getNormalPoint(_pos, a, b);
        
        if (normalPoint.x < a.x || normalPoint.x > b.x) {
            normalPoint = b;
        }
        
        float distance = _pos.distance(normalPoint);
        
        if (distance < minDist) {
            minDist = distance;
            
            normal = normalPoint;
            
            ofPoint dir = b - a;
            dir.normalize();
            
            dir *= 10;
            target = normalPoint;
            target += dir;
        }
    }
    
    return target;
}

void Tangram::addCentralRepulsionForce( float _amount ){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i].addRepulsionForce( ofPoint(0,0), 500*_amount,0.3);
    }
}

//----------------------------------------------------------- Mouse
void Tangram::_mousePressed(ofMouseEventArgs &e){
	if(bEdit){
        for(int i = 0; i < limbs.size(); i++){
            if (limbs[i].mouseHover()){
                selectedLimb = i;
                break;
            }
        }
    }
}

void Tangram::_mouseDragged(ofMouseEventArgs &e){
    if (selectedLimb >= 0 && selectedLimb < limbs.size() ){
        limbs[ selectedLimb ].setPosition(ofPoint(ofGetMouseX()-ofGetWidth()*0.5,
                                                  ofGetMouseY()-ofGetHeight()*0.5));
    }
}

void Tangram::_mouseReleased(ofMouseEventArgs &e){
    if (bEdit){
        selectedLimb = -1;
    }
}

void Tangram::_keyPressed(ofKeyEventArgs &e){
    if (bEdit){
        if (selectedLimb >= 0 && selectedLimb < limbs.size() ){
            if (e.key == OF_KEY_RIGHT){
                limbs[ selectedLimb ].turnRight();
            } else if (e.key == OF_KEY_LEFT){
                limbs[ selectedLimb ].turnLeft();
            } else if (e.key == 'h' || e.key == OF_KEY_UP){
                limbs[ selectedLimb ].flipH();
            } else if (e.key == 'v' || e.key == OF_KEY_DOWN){
                limbs[ selectedLimb ].flipV();
            }
        }
    }
}