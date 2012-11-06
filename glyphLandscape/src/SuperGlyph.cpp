//
//  SuperGlyph.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/4/12.
//
//

#include "SuperGlyph.h"

SuperGlyph::SuperGlyph(){
    bDebug = false;
}

void SuperGlyph::create(float _scale){
    limbs.clear();
    
    //  Make a new Glyph
    //
    Glyph *glyph = new Glyph();
    glyph->setScale(_scale);
    
    //  Transform it to Shapes
    //
    for (int i = 0; i < glyph->limbs().size(); i++){
        Shape newShape;
        newShape.makeFromLimb(i, *glyph);
        newShape.bDebug = &bDebug;
        limbs.push_back( newShape );
    }
    
    //  Make a path from the shapes
    //
    insidePath.clear();
    for (int i = 0; i < limbs.size(); i++){
        for (int j = 0; j < limbs[i].getVertices().size(); j++) {
            ofPoint vert = limbs[i].getVertices()[j];
            insidePath.addVertex( vert );
        }
    }
    insidePath.close();
    perimeter = insidePath.getPerimeter();
    
    this->set( insidePath.getBoundingBox() );
}

int SuperGlyph::getShapeNumberAt( ofPoint _pos ){
    int rta = -1;
    for(int i = 0; i < limbs.size(); i++){
        if (limbs[i].inside(_pos) ){
            rta = i;
            break;
        }
    }
    return rta;
}

int SuperGlyph::getShapeNumberAt( float _pct ){
    
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

ofPoint	SuperGlyph::getPathPositionAt( float _pct ){
    
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

ofPoint SuperGlyph::getNormalPoint(ofPoint p, ofPoint a, ofPoint b) {
    ofPoint ap = p - a;
    ofPoint ab = b - a;
    ab.normalize();
    ab *= ap.dot(ab);
    return a+ab;
}

ofPoint SuperGlyph::getGetClosePath(ofPoint _pos){
    
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

void SuperGlyph::draw(){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i].draw();
    }
    
    if (bDebug){
        ofSetColor(0,50);
        insidePath.draw();
    }
}