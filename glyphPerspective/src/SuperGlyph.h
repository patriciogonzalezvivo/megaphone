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

class SuperGlyph : public ofRectangle {
public:
    SuperGlyph();
    
    void        createFromGlyph( Glyph *_glyph, float _alpha, ofFloatColor _target );
    
    ofPoint     getPathPositionAt( float _pct );
    int         getShapeNumberAt( float _pct);
    int         getShapeNumberAt( ofPoint _pos);
    ofPoint     getNormalPoint(ofPoint p, ofPoint a, ofPoint b);
    ofPoint     getGetClosePath( ofPoint _pos );
    
    void        addCentralRepulsionForce( float _amount );
    
    void        update();
    void        draw();

    bool        bDebug;
private:
    vector<Shape>   limbs;
    ofPolyline      insidePath;
    float           perimeter;
    float           area;
};
#endif
