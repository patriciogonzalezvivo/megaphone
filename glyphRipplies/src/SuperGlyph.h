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

class SuperGlyph : public ofRectangle {
public:
    SuperGlyph();
    
    void    create( float _scale);
    
    ofPoint     getPathPositionAt( float _pct );
    int         getShapeNumberAt( float _pct);
    int         getShapeNumberAt( ofPoint _pos);
    ofPoint     getNormalPoint(ofPoint p, ofPoint a, ofPoint b);
    ofPoint     getGetClosePath( ofPoint _pos );
    
    void        draw();
    
    bool        bDebug;
private:
    vector<Shape>   limbs;
    ofPolyline      insidePath;
    float           perimeter;
};
#endif
