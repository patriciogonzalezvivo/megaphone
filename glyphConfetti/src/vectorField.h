#ifndef VECTORFIELD_H
#define VECTORFIELD_H


#include "ofMain.h"

class vectorField : public ofRectangle {
public:
    
    vectorField();
    virtual ~vectorField();
		
    void    setupField(int _cols, int _rows, int _width, int _height);   // pass in internal dimensions and outer dimensions
	
    int     size();
    void    clear();
    void    fadeField(float _fadeAmount);
    void    randomizeField(float _scale);
    void    noiseField(float _scale = 1.0f, float _speed = 0.01, float _turbulence = 3, bool _signed = true);
    void    AngleNoiseField(float _angle, float _turbulence, float _scale);
    
    void    draw();

    ofPoint& operator [](int _index);
    
    int     getIndexFor(float _x, float _y);
    ofPoint	getForceFromPos(ofPoint pos);
    ofPoint	getForceFromPos(float xpos, float ypos);

    void    addOutwardCircle(float x, float y, float radius, float strength);
    void    addInwardCircle(float x, float y, float radius, float strength);
    void    addClockwiseCircle(float x, float y, float radius, float strength);
    void    addCounterClockwiseCircle(float x, float y, float radius, float strength);
    void    addVectorCircle(float x, float y, float vx, float vy, float radius, float strength);

private:
    ofPoint *field;
    
    int     cols, rows, nTotal;
};

#endif // VECTORFIELD_H
