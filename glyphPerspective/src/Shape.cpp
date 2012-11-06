//
//  Shape.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/1/12.
//
//

#include "Shape.h"

Shape::Shape(){
    damping = 0.05f;
    size = 20;
    
    rotationX = 0.0;
    rotationY = 0.0;
    rotationZ = 0.0;
    
    bDebug = NULL;
}

void Shape::makeFromLimb(int _limbNum, Glyph &_glyph, float _alpha, ofFloatColor _target){
    
    Limb limb = _glyph.limbs()[_limbNum];
    for (int i = 0; i < limb.coords().size(); i++){
        ofPoint coord = limb.coords()[i];
        
        coord *= _glyph.scale();
        
        this->addVertex(coord);
    }
    
    close();
    
    ofRectangle rect = limb.getBoundingBox();
    
    org = pos = _glyph.pos() + this->getCentroid2D();
    size = MAX(this->getBoundingBox().width,this->getBoundingBox().height);
    
    color = _glyph.limbs()[_limbNum].color();
    color.lerp(_target, _alpha);
}

//------------------------------------------------------------
void Shape::addForce(ofPoint &_force){
    acc += _force;
}

//------------------------------------------------------------
void Shape::update(){
	
    vel += acc;
    vel *= 1.0f - damping;
    pos += vel;
    
    acc *= 0;
}

//------------------------------------------------------------
void Shape::draw(){
    ofPushStyle();
    
    ofPushMatrix();
    
    ofTranslate(pos);
    
    ofPushMatrix();
    ofRotateX(rotationX);
    ofRotateY(rotationY);
    ofRotateZ(rotationZ);
    
    ofTranslate(-this->getCentroid2D());
    ofFill();
    ofSetColor(color);
    ofBeginShape();
    for (int i = 0; i < getVertices().size(); i++) {
        ofVertex(getVertices()[i]);
    }
    ofEndShape(true);
    ofPopMatrix();
    
    if (bDebug != NULL) {
        if(*bDebug){
            ofDisableSmoothing();
            ofSetColor(255,0,0);
            ofCircle(0,0, 2);
            
            ofSetColor(255,0,0,100);
            ofPushMatrix();
            ofTranslate(-this->getCentroid2D());
            ofNoFill();
            ofRect(this->getBoundingBox());
            ofPopMatrix();
            ofEnableSmoothing();
        }
    }
    
    ofPopMatrix();
    ofPopStyle();
}

//------------------------------------------------------------
void Shape::infinitWalls(){
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
}

//------------------------------------------------------------
void Shape::addRepulsionForce(ofPoint posOfForce, float radius, float scale){
    
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
        acc += diff * scale * pct;
    }
}

//------------------------------------------------------------
void Shape::addAttractionForce(ofPoint posOfForce, float radius, float scale){
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Shape::addRepulsionForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc += diff * scale * pct;
		s.acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Shape::addRepulsionForce(Shape &s, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
    float radius    = size*2.f + s.size*2.f;
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc += diff * scale * pct;
		s.acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Shape::addAttractionForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
		s.acc += diff * scale * pct;
    }
	
}

//------------------------------------------------------------
void Shape::addClockwiseForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc.x -= diff.y * scale * pct;
        acc.y += diff.x * scale * pct;
		s.acc.x += diff.y * scale * pct;
        s.acc.y -= diff.x * scale * pct;
    }
}

//------------------------------------------------------------
void Shape::addCounterClockwiseForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc.x += diff.y * scale * pct;
        acc.y -= diff.x * scale * pct;
		s.acc.x -= diff.y * scale * pct;
        s.acc.y += diff.x * scale * pct;
		
    }
}

//------------------------------------------------------------
void Shape::bounceOffWalls(){
	
	// sometimes it makes sense to damped, when we hit
	bool bDampedOnCollision = true;
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (pos.x > maxx){
		pos.x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (pos.x < minx){
		pos.x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
	
	if (pos.y > maxy){
		pos.y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (pos.y < miny){
		pos.y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.3;
	}
	
}