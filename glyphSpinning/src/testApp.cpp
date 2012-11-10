#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    width = 300;
    height = 800;
    ofSetWindowShape(width, height);
    
    ofSeedRandom(0); 
    
	soundStream.setup(this, 0, 2, 44100, 256, 4);
    smoothedVol     = 0.0;
    micBar.setLabel("mic");
    
    glyph = new Glyph();
    glyph->setScale(100);
    superGlyph.createFromGlyph( glyph );

    bDebug = true;
}

//--------------------------------------------------------------
void testApp::update(){
    //  update mic data
    //
    micBar.setValue(smoothedVol);
    if (bDebug){
        micBar.updateGUI();
    }
    
    superGlyph.pct = micBar.getActiveValue();
    superGlyph.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::black,ofColor::gray,OF_GRADIENT_LINEAR);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    superGlyph.draw();
    ofPopMatrix();
    
    if (bDebug){
        ofSetColor(255);
        micBar.draw();
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
void testApp::keyPressed(int key){
    
    if (key == ' '){
        delete glyph;
        glyph = new Glyph();
        glyph->setScale(100);
        superGlyph.createFromGlyph(glyph);
    } else if (key == 's'){
        bDebug = !bDebug;
    }
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