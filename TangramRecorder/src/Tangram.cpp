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
        
        _centerShapes();
        
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
    
    _centerShapes();
}

void Tangram::createSet(){
    limbs.clear();
    
    for ( int i = 0; i < 7; i++){
        Shape newShape;
        newShape.makeNumber(i);
        limbs.push_back( newShape );
    }
    
    _centerShapes();
}

void Tangram::_centerShapes(){
    
    ofPolyline centers;
    for (int i = 0; i < limbs.size(); i++){
        centers.addVertex(limbs[i].getPosition());
    }
    
    ofPoint center = centers.getBoundingBox().getCenter();
    
    for(int i = 0; i < limbs.size();i++){
        limbs[i].setPosition( limbs[i].getPosition() -center);
    }
}

void Tangram::update(){
    for(int i = 0; i < limbs.size(); i++){

        if (bEdit){
            limbs[i].bDebug = limbs[i].mouseHover();
        }

        if (bDebug)
            limbs[i].bDebug = true;
        
        limbs[i].update();
    }
}

void Tangram::draw(){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i].draw();
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