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

void SuperGlyph::createFromGlyph( Glyph *_glyph ){
    limbs.clear();
    //  Transform it to Shapes
    //
    for (int i = 0; i < _glyph->limbs().size(); i++){
        Shape newShape;
        newShape.makeFromLimb(i, *_glyph );
        newShape.bDebug = &bDebug;
        limbs.push_back( newShape );
    }
}

void SuperGlyph::addCentralRepulsionForce( float _amount ){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i].addRepulsionForce( ofPoint(0,0), 500*_amount,0.3);
    }
}

void SuperGlyph::update(){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i].addAttractionForce( limbs[i].org, 100*2.0, 0.1);
        //        limbs[i].addRepulsionForce( ofPoint(0,0), 1,1.0);
        limbs[i].pct = pct;
        limbs[i].update();
    }
}

void SuperGlyph::draw(){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i].draw();
    }
    
//    if (bDebug){
//        ofSetColor(0,50);
//        insidePath.draw();
//    }
}