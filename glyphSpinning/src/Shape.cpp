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

void Shape::makeFromLimb(int _limbNum, Glyph &_glyph ){
    
    ofPolyline contour;
    
    Limb limb = _glyph.limbs()[_limbNum];
    for (int i = 0; i < limb.coords().size(); i++){
        ofPoint coord = limb.coords()[i];
        
        coord *= _glyph.scale();
        
        contour.addVertex(coord);
    }
    
    contour.close();
    
    ofRectangle rect = limb.getBoundingBox();
    
    org = pos = _glyph.pos() + contour.getCentroid2D();
    size = MAX(contour.getBoundingBox().width,contour.getBoundingBox().height);
    
    for(int k = 0; k < contour.getVertices().size(); k++){
        wParticle newDot;
        ofPoint position = contour.getVertices()[k] - contour.getCentroid2D();
        newDot.set(position);
        stringDots.push_back(newDot);
    }
    
    color = _glyph.limbs()[_limbNum].color();
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
    
    for (int i = 0; i < stringDots.size(); i++){
        
        float radius = pct * 200;
        float angle = ofGetElapsedTimef() * (1 + i / 20.0);
        
        float x = stringDots[i].origin.x + radius * cos(angle* 0.9);    //  We move the points to spin round the
        float y = stringDots[i].origin.y + radius * -sin(angle* 0.8);   //  default position
        
        stringDots[i].moveTo(x, y);
    }
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
    
    ofFill();
    ofSetColor(color);
    ofBeginShape();
    for (int i = 0; i < stringDots.size(); i++) {
        ofVertex(stringDots[i]);
    }
    ofEndShape(true);
    ofPopMatrix();
    
    if (bDebug != NULL) {
        if(*bDebug){
            ofSetColor(255,0,0);
            ofCircle(0,0, 2);
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