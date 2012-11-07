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
    
    void    flipH();
    void    flipV();
    void    turnLeft();
    void    turnRight();
    
    void    setPosition( ofPoint _pos ){ targetPos = _pos;};
    void    setRotation( ofPoint _rot ){ targetRotationX = _rot.x; targetRotationZ = _rot.y; targetRotationZ = _rot.z;};
    ofPoint getRotation(){return ofPoint(targetRotationX,targetRotationY,targetRotationZ);};
    ofPoint getPosition(){return targetPos;};
    
    bool    mouseHover();
    
    void    update();
    void    draw();
    
    bool    bDebug;
    
private:
    ofFloatColor color, targetColor;
    ofPoint pos, targetPos;
    
    float   rotationX, targetRotationX;
    float   rotationY, targetRotationY;
    float   rotationZ, targetRotationZ;
    
    float   scale;
    float   size;
    
    float   damping;
};

#endif
