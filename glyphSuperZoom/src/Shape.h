//
//  Shape.h
//
//  Created by Patricio Gonzalez Vivo on 11/1/12.
//
//

#ifndef SHAPE
#define SHAPE

#include "ofMain.h"
#include "Glyph.h"

class Shape : public ofPolyline {
public:
    Shape();
    
    void makeFromLimb(int _limbNum, Glyph &_glyph);
    
    void    addForce( ofPoint &_force );
    
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(ofPoint p, float radius, float scale);
    void    addRepulsionForce(Shape &p, float scale);
    void    addRepulsionForce(Shape &p, float radius, float scale);
    void    addAttractionForce(Shape &p, float radius, float scale);
    void    addClockwiseForce(Shape &p, float radius, float scale);
    void    addCounterClockwiseForce(Shape &p, float radius, float scale);
    
    void    infinitWalls();
    void    bounceOffWalls();
    
    void update();
    void draw();
    
    ofPoint pos;
    ofPoint vel;
    ofPoint acc;
    
    bool    *bDebug;
    
private:
    ofColor color;    
    ofPoint org;

    float   scale;
    float   size;
    
    float   damping;
    float   rotationX;
    float   rotationY;
    float   rotationZ;
};

#endif
