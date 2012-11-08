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
    
    void    makeNumber( int _num, float _scale = 100 );
    void    makeFromLimb(int _limbNum, Glyph &_glyph );
    
    void    setTargetPos( ofPoint _pos );
    void    setTargetRot( ofPoint _rot );
    ofPoint getTargetRot();
    ofPoint getTargetPos();
    float   getDistanceToTarget();
    bool    mouseHover();
    
    void    flipH();
    void    flipV();
    void    turnLeft();
    void    turnRight();
    
    void    addForce( ofPoint &_force );
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(ofPoint p, float radius, float scale);
    void    addRepulsionForce(Shape &p, float scale);
    void    addRepulsionForce(Shape &p, float radius, float scale);
    void    addAttractionForce(Shape &p, float radius, float scale);
    void    addClockwiseForce(Shape &p, float radius, float scale);
    void    addCounterClockwiseForce(Shape &p, float radius, float scale);
    
    void    update(float _posLerp, float _rotLerp);
    void    draw();
    
    bool    bDebug;
    
private:
    ofFloatColor color, targetColor;
    ofPoint pos, targetPos;
    ofPoint vel;
    ofPoint acc;
    
    float   rotationX, targetRotationX;
    float   rotationY, targetRotationY;
    float   rotationZ, targetRotationZ;
    
    float   scale;
    float   size;
    float   damping;
};

#endif
