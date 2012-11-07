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
    
    void    setPosition( ofPoint _pos ){ org = _pos;};
    void    setRotation( ofPoint _rot ){ rotationTargetX = _rot.x; rotationTargetZ = _rot.y; rotationTargetZ = _rot.z;};
    ofPoint getRotation(){return ofPoint(rotationTargetX,rotationTargetY,rotationTargetZ);};
    ofPoint getPosition(){return org;};
    
    bool    mouseHover();
    
    void    addForce( ofPoint &_force );
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(ofPoint p, float radius, float scale);

    void    infinitWalls();
    void    bounceOffWalls();
    
    void    update();
    void    draw();
    
    bool    bDebug;
    
private:
    ofFloatColor color, targetColor;
    ofPoint org,pos;
    
    ofPoint vel,acc;
    
    float   rotationX, rotationTargetX;
    float   rotationY, rotationTargetY;
    float   rotationZ, rotationTargetZ;
    
    float   scale;
    float   size;
    
    float   damping;
};

#endif
