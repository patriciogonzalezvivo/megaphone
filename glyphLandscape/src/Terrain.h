//
//  Terrain.h
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/4/12.
//
//

#ifndef TERRAIN
#define TERRAIN

#include "ofMain.h"
#include "SuperGlyph.h"
#include "ofxFX.h"

class Terrain : public ofRectangle {
public:
    Terrain();
    
    void    allocate( ofRectangle &_rect );
    void    createFromGlyph( SuperGlyph &_glyph );

    void    update();
    void    draw();
    
    float   topAltitud;
    float   resolution;
    
    ofFbo       heightFbo;
    ofFbo       colorFbo;
    ofVboMesh   mesh;
private:
    ofxNormals  normals;
    ofxBlur     blur;

    ofFloatPixels   colors;
};

#endif
