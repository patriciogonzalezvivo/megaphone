//
//  Shape.h
//
//  Created by Patricio Gonzalez Vivo on 11/1/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com All rights reserved.
//

#ifndef SHAPE
#define SHAPE

#include "ofMain.h"
#include "Glyph.h"

class Shape : public ofPolyline {
public:
    Shape();
    
    void    makeFromLimb(int _limbNum, Glyph &_glyph);
    void    setAnchorPercent(float _xPct, float _yPct);
    void    draw();
    
    ofPoint pos;
    bool    *bDebug;
    ofColor color;
    
private:
    
    ofPoint org;

    float   scale;
    float   size;
};

#endif
