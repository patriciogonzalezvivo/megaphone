//
//  SuperGlyph.h
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/4/12.
//
//

#ifndef SUPERGLYPH
#define SUPERGLYPH

#include "ofMain.h"
#include "Shape.h"
#include "Glyph.h"

class SuperGlyph {
public:
    SuperGlyph();
    
    void        createFromGlyph( Glyph *_glyph );
    
    void        addCentralRepulsionForce( float _amount );
    
    void        update();
    void        draw();

    float       pct;
    bool        bDebug;
private:
    vector<Shape>   limbs;
};
#endif
