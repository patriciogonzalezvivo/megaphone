//
//  Tree.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/3/12.
//
//

#include "Tree.h"

Tree::Tree(){
    angles = NULL;
    radios = NULL;
    pcts = NULL;
    
    bDebug = true;
    pct = 1.0;
}

Tree::~Tree(){
    clear();
}

void Tree::clear(){
    shapes.clear();
    branches.clear();
    
    if (angles != NULL)
        delete [] angles;
    
    if (pcts != NULL)
        delete [] pcts;
    
    if (radios != NULL)
        delete [] radios;
}

void Tree::create(ofPoint _pos, float _scale){
    pct = 0.0;
    clear();
    
    cout << "Create a new Glyph" << endl;
    Glyph glyph;
    glyph.setScale(_scale);
    glyph.pos().set(_pos);
    
    ofRectangle bound = glyph.absBounds();
    bound.width *= _scale;
    bound.height *= _scale;
    
    cout << "Extract the Shapes of it"<< endl;
    for (int i = 0; i < glyph.limbs().size(); i++){
        Shape newShape;
        newShape.makeFromLimb(i, glyph);
        newShape.bDebug = &bDebug;
        shapes.push_back( newShape );
    }
    
    cout << "Calculate the angles, radios and percentages of them" << endl;
    base = _pos - ofPoint(0,bound.height*0.5);
    
    angles = new float[shapes.size()];
    radios = new float[shapes.size()];
    pcts = new float[shapes.size()];
    
    for(int i = 0; i < shapes.size(); i++){
        Branch newBranch;
        newBranch.base = shapes[i].pos;
        newBranch.tail = shapes[i].pos;
        branches.push_back(newBranch);
        
        ofPoint dist = base - shapes[i].pos;
        angles[i] = atan2(dist.y,dist.x);
        pcts[i] = dist.length()/sqrt( bound.width*bound.width + bound.height*bound.height );
        radios[i] = bound.width*(1.0-pcts[i])*2.0;
        cout << "a: "<< angles[i] << " pct: " << pcts[i] << endl;
    }
}

void Tree::update(){
    ofSetColor(255);
    for(int i = 0; i < shapes.size(); i++){
        branches[i].base = ofPoint(0, (800*-0.1*pct) * pcts[i] ) + base;
        branches[i].tail.x = cos(angles[i]) * radios[i];
        branches[i].tail.y = sin(angles[i]) * radios[i];
        branches[i].tail *= pct;
        branches[i].tail += branches[i].base;
    }
    
    for(int i = 0; i < shapes.size(); i++){
        shapes[i].addAttractionForce(branches[i].tail, 50, 0.1);
        shapes[i].update();
    }
}

void Tree::draw(){
    ofLine(base,branches[branches.size()-1].base);
    for(int i = 0; i < branches.size(); i++){
        ofLine(branches[i].base, branches[i].tail);
    }
    
    for(int i = 0; i < shapes.size(); i++){
        shapes[i].draw();
    }
}