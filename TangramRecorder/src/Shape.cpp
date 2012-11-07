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
    
    rotationTargetX = 0.0;
    rotationTargetY = 0.0;
    rotationTargetZ = 0.0;
    
    bDebug = NULL;
}

void Shape::makeNumber( int _num, float _scale ){
    clear();
    
    pos.set(0,0);
    org.set(0,0);
    
    //  Choose of Shapes
    //
    switch (_num) {
        case 0:
            addVertex(ofVec3f(0, 0));
            addVertex(ofVec3f(2, 2));
            addVertex(ofVec3f(0, 4));
            break;
        case 1:
            addVertex(ofVec3f(0, 0));
            addVertex(ofVec3f(4, 0));
            addVertex(ofVec3f(2, 2));
            break;
        case 2:
            addVertex(ofVec3f(4, 0));
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(3, 1));
            break;
        case 3:
            addVertex(ofVec3f(2, 2));
            addVertex(ofVec3f(3, 1));
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(3, 3));
            addVertex(ofVec3f(2, 2));
            break;
        case 4:
            addVertex(ofVec3f(1, 3));
            addVertex(ofVec3f(2, 2));
            addVertex(ofVec3f(3, 3));
            break;
        case 5:
            addVertex(ofVec3f(2, 4));
            addVertex(ofVec3f(0, 4));
            addVertex(ofVec3f(1, 3));
            addVertex(ofVec3f(1, 3));
            addVertex(ofVec3f(3, 3));
            addVertex(ofVec3f(2, 4));
            break;
        case 6:
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(4, 4));
            addVertex(ofVec3f(2, 4));
            break;
    }
    close();
    
    //  Centered
    //
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= ofPoint(2,2);
        getVertices()[i] *= _scale;
    }
    org = pos = this->getCentroid2D();
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= org;
    }
    ofRectangle rect = getBoundingBox();
    size = MAX(rect.width,rect.height);
    
    //  Colors Palette
    //
    vector<ofColor> colors;
    colors.push_back(ofColor(245, 180, 69));
    colors.push_back(ofColor(221, 35, 66));
    colors.push_back(ofColor(93, 191, 185));
    colors.push_back(ofColor(206, 116, 172));
    colors.push_back(ofColor(86, 172, 220));
    colors.push_back(ofColor(0, 55, 115));
    color = colors[ _num%colors.size() ];
}

void Shape::makeFromLimb(int _limbNum, Glyph &_glyph ){
    
    Limb limb = _glyph.limbs()[_limbNum];
    for (int i = 0; i < limb.coords().size(); i++){
        ofPoint coord = limb.coords()[i];
        coord *= _glyph.scale();
        this->addVertex(coord);
    }
    close();
    
    org = pos = this->getCentroid2D();
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= org;
    }
    
    ofRectangle rect = limb.getBoundingBox();
    size = MAX(this->getBoundingBox().width,this->getBoundingBox().height);
    
    color = _glyph.limbs()[_limbNum].color();
}

void Shape::flipH(){
    rotationTargetX += 180;
    while (rotationTargetX >= 360) {
        rotationTargetX -= 360;
    }
}

void Shape::flipV(){
    rotationTargetY += 180;
    while (rotationTargetY >= 360) {
        rotationTargetY -= 360;
    }
}

void Shape::turnLeft(){
    if ( rotationTargetZ < 5){
        rotationTargetZ = 360;
    }
    
    rotationTargetZ -= 5;
}

void Shape::turnRight(){
    rotationTargetZ += 5;
    
    while (rotationTargetZ >= 360) {
        rotationTargetZ -= 360;
    }
}

//------------------------------------------------------------
void Shape::addForce(ofPoint &_force){
    acc += _force;
}

//------------------------------------------------------------
void Shape::update(){
    
    rotationX = ofLerp(rotationX, rotationTargetX, 0.1);
    rotationY = ofLerp(rotationY, rotationTargetY, 0.1);
    rotationZ = ofLerp(rotationZ, rotationTargetZ, 0.1);
    
    pos.x = ofLerp(pos.x,org.x,0.1);
    pos.y = ofLerp(pos.y,org.y,0.1);
    pos.z = ofLerp(pos.z,org.y,0.1);
    
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
    
    ofRotateX(rotationX);
    ofRotateY(rotationY);
    ofRotateZ(rotationZ);
    
    ofFill();
    ofSetColor(color);
    ofBeginShape();
    for (int i = 0; i < getVertices().size(); i++) {
        ofVertex(getVertices()[i]);
    }
    ofEndShape(true);
    ofPopMatrix();
    
    if(bDebug){
        ofPushMatrix();
        ofTranslate(org);
        ofSetColor(255);
        ofLine(-5,0,5,0);
        ofLine(0, -5, 0, 5);
        
        ofRotateX(rotationTargetX);
        ofRotateY(rotationTargetY);
        ofRotateZ(rotationTargetZ);
        
        ofEnableSmoothing();
        ofNoFill();
        ofBeginShape();
        for (int i = 0; i < getVertices().size(); i++) {
            ofVertex(getVertices()[i]);
        }
        ofEndShape(true);
        
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(pos);
        
        ofFill();
        ofSetColor(255,100);
        ofCircle(0,0, 7);
        
        ofDrawBitmapString("x: "+ofToString(rotationTargetX), 15,-15);
        ofDrawBitmapString("y: "+ofToString(rotationTargetY), 15,0);
        ofDrawBitmapString("z: "+ofToString(rotationTargetZ), 15,15);
        
        ofPopMatrix();
        ofDisableSmoothing();
    }
    
    ofPopStyle();
}

bool Shape::mouseHover(){
    return inside(ofGetMouseX()-ofGetWidth()*0.5-pos.x,
                  ofGetMouseY()-ofGetHeight()*0.5-pos.y);
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
void Shape::infinitWalls(){
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
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