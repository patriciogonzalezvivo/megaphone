#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    width = 1500;
    height = 400;
    
    ofSetWindowShape(width, height);
    
    //  SOUND
    //
	soundStream.setup(this, 0, 2, 44100, 256, 4);
    smoothedVol     = 0.0;
    micBar.setLabel("mic");
    
    time    = 0.0;
    zoom    = 1.0;
    angle   = 0.0;
    bDebug  = true;
    
    ofSeedRandom(0);
    
    glyph.create(100);
    terrain.createFromGlyph(glyph);
    terrain.resolution  = 2;
    terrain.topAltitud  = 300;
    
    cameraFocus.pos = glyph.getPathPositionAt(0.0);
    light.setPosition(ofVec3f(0,0,300));
//    light.rotate(54, ofVec3f(1,0,0.0));
//    light.setDiffuseColor(ofColor(255));
//    light.setAmbientColor(ofColor(50,21,33));
}


//--------------------------------------------------------------
void testApp::update(){
    time += 0.1;
    
    if (zoom < 10)
        zoom += 0.01;
    
    //  update mic data
    //
    micBar.setValue(smoothedVol);
    if (bDebug){
        micBar.updateGUI();
    }
    
    headPos = glyph.getPathPositionAt( abs(sin(ofGetElapsedTimef()*0.001)) );
    cameraFocus.addAttractionForce( headPos, 1000, 0.1);
    cameraFocus.addRepulsionForce( headPos, 1, 0.5);
    cameraFocus.update();
    
    float angleTo = atan2(cameraFocus.vel.y,cameraFocus.vel.x);
    float diffAngle = (angleTo - angle);
    if (diffAngle < -PI) diffAngle += TWO_PI;
    if (diffAngle > PI) diffAngle -= TWO_PI;
    angle  += 0.005 * diffAngle;
    
    //angle = ofLerp(angle, PI+atan2(cameraFocusFocus.vel.y,cameraFocus.vel.x), 0.005);
    
    terrain.heightFbo.begin();
    ofPushMatrix();
    ofTranslate(terrain.width*0.5, terrain.height*0.5);
    ofSetColor(255,10);
    float soundVal = micBar.getActiveValue()*10;
    ofPoint pos = headPos + ofPoint(ofRandom(-soundVal,soundVal),ofRandom(-soundVal,soundVal));
    ofCircle(pos, soundVal);
    ofPopMatrix();
    terrain.heightFbo.end();
    terrain.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::black,ofColor::gray,OF_GRADIENT_LINEAR);
    
    //  Draw Shapes
    //
    cam.begin();
    ofPushMatrix();
    ofRotate(-70,1.0,0.0,0.0);
    ofScale(zoom,zoom);
    ofRotate(ofRadToDeg(angle), 0, 0, 1);
    ofTranslate(cameraFocus.pos*-1.0);
    
    //glyph.draw();
    //ofTranslate(0, 0,-1);
    ofSetColor(255);
    light.enable();
    terrain.draw();
    light.disable();
    
    
//    ofSetColor(0,50);
//    terrain.mesh.drawWireframe();
    
    ofPopMatrix();
    
    //  Draw cameraFocus Focus Center
    //
    ofPushMatrix();
    ofTranslate(cameraFocus.pos*-1.0);
    cameraFocus.draw();
    ofPopMatrix();
    
    cam.end();
    
    //  Draw the average volume
    //
    if(bDebug){
        ofSetColor(255);
        micBar.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
        glyph.create(100);
        terrain.createFromGlyph(glyph);
        zoom = 1.0;
    } else if (key == 'd'){
        bDebug = !bDebug;
    } else if (key == 'g'){
        glyph.bDebug = !glyph.bDebug;
    }
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
    float left[bufferSize];
    float right[bufferSize];
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}