#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    width = 300;
    height = 800;
    scale  = 5;
    
    ofSetWindowShape(width, height);
    
    ofSeedRandom(0);    
    creatGlyph();
    
    bubbles.setup(width, height, 10);
    bubbles.bDebug = &bDebug;
    
    //  SOUND
    //
    //soundStream.listDevices();
	//if you want to set a different device id
	//soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	soundStream.setup(this, 0, 2, 44100, 256, 4);
    smoothedVol     = 0.0;
    micBar.setLabel("mic");
    
    bDebug = true;
}

void testApp::creatGlyph(){
    shapes.clear();
    
    Glyph glyph;
    glyph.setScale(scale);
    glyph.pos().set(width*0.5,height*0.9);
    
    for (int i = 0; i < glyph.limbs().size(); i++){
        Shape newShape;
        newShape.makeFromLimb(i, glyph);
        newShape.bDebug = &bDebug;
        shapes.push_back( newShape );
    }
}

//--------------------------------------------------------------
void testApp::update(){
    //  update mic data
    //
    micBar.setValue(smoothedVol);
    if (bDebug){
        micBar.updateGUI();
    }
    
    float amount = micBar.getValue();
    bubbles.generate( shapes , amount * 10);
    
    
    bubbles.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
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
void testApp::draw(){
    ofBackgroundGradient(ofColor::black,ofColor::gray,OF_GRADIENT_LINEAR);

    bubbles.draw();
    
    for(int i = 0; i < shapes.size(); i++){
        shapes[i].draw();
    }
    
    if (bDebug){
        //  Draw the average volume
        //
        ofSetColor(255);
        micBar.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
        creatGlyph();
    } else if (key == 'd'){
        bDebug = !bDebug;
    } else if (key == 'c'){
        bubbles.clean();
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