//
//  Particle.h
//
//  Created by Patricio Gonzalez Vivo on 11/3/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com All rights reserved.
//

#ifndef PARTICLE
#define PARTICLE

#include "ofMain.h"

class Particle {
public:
    Particle();
    
    void    addForce( ofPoint &_force );
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(ofPoint p, float radius, float scale);
    
    void    infinitWalls();
    void    bounceOffWalls();
    
    void    update();
    void    draw();
    
    ofPoint pos, vel, acc;
    float   damping;
};


#endif
