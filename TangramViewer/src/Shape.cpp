//
//  Shape.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/1/12.
//
//

#include "Shape.h"

Shape::Shape(){
    size = 20;
    damping = 0.05f;
    
    rotationX = 0.0;
    rotationY = 0.0;
    rotationZ = 0.0;
    
    targetRotationX = 0.0;
    targetRotationY = 0.0;
    targetRotationZ = 0.0;
    
    bDebug = NULL;
}

void Shape::makeNumber( int _num, float _scale ){
    clear();
    
    pos.set(0,0);
    targetPos.set(0,0);
    
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
    targetPos = pos = this->getCentroid2D();
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= targetPos;
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
    
    targetPos = pos = this->getCentroid2D();
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= targetPos;
    }
    
    ofRectangle rect = limb.getBoundingBox();
    size = MAX(this->getBoundingBox().width,this->getBoundingBox().height);
    
    color = _glyph.limbs()[_limbNum].color();
}

float Shape::getDistanceToTarget(){
    return targetPos.distance(pos);
}

void Shape::flipH(){
    targetRotationX += 180;
    while (targetRotationX >= 360) {
        targetRotationX -= 360;
    }
}

void Shape::flipV(){
    targetRotationY += 180;
    while (targetRotationY >= 360) {
        targetRotationY -= 360;
    }
}

void Shape::turnLeft(){
    if ( targetRotationZ < 5){
        targetRotationZ = 360;
    }
    
    targetRotationZ -= 5;
}

void Shape::turnRight(){
    targetRotationZ += 5;
    
    while (targetRotationZ >= 360) {
        targetRotationZ -= 360;
    }
}

void Shape::setTargetPos( ofPoint _pos ){
    targetPos = _pos;
};

void Shape::setTargetRot( ofPoint _rot ){
    targetRotationX = _rot.x;
    targetRotationY = _rot.y;
    targetRotationZ = _rot.z;
};

ofPoint Shape::getTargetRot(){
    return ofPoint(targetRotationX,targetRotationY,targetRotationZ);
};

ofPoint Shape::getTargetPos(){
    return targetPos;
}

//------------------------------------------------------------
void Shape::addForce(ofPoint &_force){
    acc += _force;
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
        ofPoint dir = ofPoint(diff * scale * pct);
        addForce(dir);
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
        ofPoint dir = ofPoint(diff * scale * pct) * -1;
        addForce(dir);
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
        ofPoint dir = ofPoint(diff * scale * pct);
        addForce(dir);
		dir *= -1.0;
        s.addForce(dir);
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
        ofPoint dir = ofPoint(diff * scale * pct);
        addForce(dir);
		dir *= -1.0;
        s.addForce(dir);
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
        ofPoint dir = ofPoint(diff * scale * pct);
        s.addForce(dir);
		dir *= -1.0;
        addForce(dir);
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
        
        ofPoint dir = ofPoint(diff * scale * pct);
        s.addForce(dir);
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
void Shape::update(float _posLerp, float _rotLerp){
    
    rotationX = ofLerp(rotationX, targetRotationX, _rotLerp);
    rotationY = ofLerp(rotationY, targetRotationY, _rotLerp);
    rotationZ = ofLerp(rotationZ, targetRotationZ, _rotLerp);
    if ( getDistanceToTarget() > 10) {
        addAttractionForce(targetPos, 1000, 0.3);
    } else {
        pos.x = ofLerp(pos.x,targetPos.x,_posLerp);
        pos.y = ofLerp(pos.y,targetPos.y,_posLerp);
        pos.z = ofLerp(pos.z,targetPos.z,_posLerp);
    }
    
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
    
    ofRotateY(rotationY);
    ofRotateX(rotationX);
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
        ofTranslate(targetPos);
        ofSetColor(255);
        ofLine(-5,0,5,0);
        ofLine(0, -5, 0, 5);
    
        ofRotateY(targetRotationY);
        ofRotateX(targetRotationX);
        ofRotateZ(targetRotationZ);
        
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
        
        ofDrawBitmapString("x: "+ofToString(targetRotationX), 15,-15);
        ofDrawBitmapString("y: "+ofToString(targetRotationY), 15,0);
        ofDrawBitmapString("z: "+ofToString(targetRotationZ), 15,15);
        
        ofPopMatrix();
        ofDisableSmoothing();
    }
    
    ofPopStyle();
}

bool Shape::mouseHover(){
    return inside(ofGetMouseX()-ofGetWidth()*0.5-pos.x,
                  ofGetMouseY()-ofGetHeight()*0.5-pos.y);
}