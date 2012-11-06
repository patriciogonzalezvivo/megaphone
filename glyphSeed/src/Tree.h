//
//  Tree.h
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/3/12.
//
//

#ifndef TREE
#define TREE

#include "ofMain.h"
#include "Shape.h"

struct Branch{
    ofPoint base;
    ofPoint tail;
};

class Tree {
public:
    
    Tree();
    ~Tree();
    
    void    clear();
    void    create(ofPoint _pos, float _scale);
    
    void    update();
    void    draw();
    
    ofPoint base;
    float   pct;
    
    bool    bDebug;
    
private:
    vector<Shape>   shapes;
    vector<Branch>  branches;
    
    float           *angles;
    float           *radios;
    float           *pcts;
};


#endif
