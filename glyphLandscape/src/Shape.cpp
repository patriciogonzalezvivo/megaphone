//
//  Shape.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/1/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com All rights reserved.
//

#include "Shape.h"

Shape::Shape(){
    size = 20;
    
    bDebug = NULL;
}

void Shape::makeFromLimb(int _limbNum, Glyph &_glyph){
    
    Limb limb = _glyph.limbs()[_limbNum];
    for (int i = 0; i < limb.coords().size(); i++){
        ofPoint coord = limb.coords()[i];
        
        coord *= _glyph.scale();
        
        this->addVertex(coord);
    }
    
    close();
    
    ofRectangle rect = limb.getBoundingBox();
    
    pos = _glyph.pos() + this->getCentroid2D();
    size = MAX(this->getBoundingBox().width,this->getBoundingBox().height);
    
    color = _glyph.limbs()[_limbNum].color();
}

void Shape::setAnchorPercent(float _xPct, float _yPct){
    ofRectangle rect = this->getBoundingBox();
    pos.x = rect.x + rect.width*_xPct;
    pos.y = rect.y + rect.height*_yPct;
}

//------------------------------------------------------------
void Shape::draw(){
    ofPushStyle();
    
    ofPushMatrix();
    
    ofTranslate(pos);
    
    ofPushMatrix();
    
    ofTranslate(-this->getCentroid2D());
    ofFill();
    ofSetColor(color);
    ofBeginShape();
    for (int i = 0; i < getVertices().size(); i++) {
        ofVertex(getVertices()[i]);
    }
    ofEndShape(true);
    ofPopMatrix();
    
    if (bDebug != NULL) {
        if(*bDebug){
            ofDisableSmoothing();
            ofSetColor(255,0,0);
            ofCircle(0,0, 2);
            
            ofSetColor(255,0,0,100);
            ofPushMatrix();
            ofTranslate(-this->getCentroid2D());
            ofNoFill();
            ofRect(this->getBoundingBox());
            ofPopMatrix();
            ofEnableSmoothing();
        }
    }
    
    ofPopMatrix();
    ofPopStyle();
}

