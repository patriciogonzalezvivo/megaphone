//
//  Group.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/3/12.
//
//

#include "Group.h"

Group::Group(){
    scale = 0.0;
    rotate = 0.0;
    
    create();
}

void Group::create(){
    shapes.clear();
    
    Glyph glyph;
    glyph.setScale(1);
    glyph.pos().set(0,0);
    
    for (int i = 0; i < glyph.limbs().size(); i++){
        Shape newShape;
        newShape.makeFromLimb(i, glyph);
        shapes.push_back( newShape );
    }
}

void Group::rotate90(){
    rotate += 0.1;
}

void Group::update(){
    scale += 0.2;
    
    if (rotate > 0 && rotate < 90){
        rotate += 0.1;
    }
}

void Group::draw(){
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    ofRotateZ(rotate);
    ofScale(scale, scale);
    
    for(int i = 0; i < shapes.size(); i++){
        shapes[i].draw();
    }
    
    ofPopMatrix();
}
