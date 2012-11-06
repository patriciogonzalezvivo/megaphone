//
//  Bubbles.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/2/12.
//
//

#include "Bubbles.h"

bool comparisonFunction(  Shape * a, Shape * b ) {
	return a->pos.x < b->pos.x;
}

Bubbles::Bubbles(){
    bDebug = NULL;
}

Bubbles::~Bubbles(){
    clean();
}

void Bubbles::setup(int _width, int _height, int _scale){
    VF.setupField(_width/_scale,_height/_scale,_width, _height);
    VF.noiseField();
}

void Bubbles::generate(vector<Shape> &_shapes, int _amount){
    for (int i = 0; i < _amount; i++){
        Shape *newShape = new Shape( _shapes[i%_shapes.size()] );
        ofPoint force = ofPoint(ofRandom(-1,1), ofRandom(-_amount,-_amount*3));
        newShape->addForce( force );
        particles.push_back( newShape );
    }
}

void Bubbles::clean(){
    for(int i = 0; i < particles.size(); i++){
        delete particles[i];
    }
    
    particles.clear();
}

void Bubbles::update(){
    VF.AngleNoiseField(-HALF_PI, 3, 0.9);
    VF.addOutwardCircle(ofGetWidth()*0.5, ofGetHeight()*0.9, 500, 1.0);
    
    //  Particle-Particle interaction
    //
	sort( particles.begin(), particles.end(), comparisonFunction );
	
    for (int i = 0; i < particles.size(); i++){
		for (int j = i-1; j >= 0; j--){
            if ( fabs(particles[j]->pos.x - particles[i]->pos.x) > 20) break;
            particles[i]->addRepulsionForce( *particles[j], 0.1);
		}
	}
    
    
    for (int i = particles.size()-1; i >= 0 ; i--){
        
        if ( VF.inside(particles[i]->pos)){
            ofPoint force = VF.getForceFromPos( particles[i]->pos  );
            particles[i]->addForce( force );
            
            //        particles[i]->addRepulsionForce(ofPoint(ofGetMouseX(), ofGetMouseY()), 100, 0.9);
            //        particles[i]->bounceOffWalls();
            
            particles[i]->update();
        } else {
            particles.erase(particles.begin()+i);
        }
	}
}

void Bubbles::draw(){
    if (bDebug != NULL){
        if (*bDebug){
            ofDisableSmoothing();
            ofSetColor(100);
            VF.draw();
            ofEnableSmoothing();
        }
    }
    
    for (int i = 0; i < particles.size(); i++){
        ofSetColor(0);
		particles[i]->draw();
	}
}