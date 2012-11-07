#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSeedRandom(0); 
    
    //  AUDIO
    //
	soundStream.setup(this, 0, 2, 44100, 256, 4);
    smoothedVol     = 0.0;
    micBar.setLabel("mic");
    
    //  VISUAL
    //
    glyph = new Glyph();
    glyph->setScale(100);
    
    dataDir.listDir("tangramShape");
    selectedFile = 0;
    
//    tangram.createFromGlyph( glyph );
    tangram.createSet();
//    tangram.load("t6-20-14-1.tan");
    tangram.bDebug = true;
    
    
    
    bDebug = true;
}

//--------------------------------------------------------------
void testApp::update(){
    //  AUDIO
    //
    micBar.setValue(smoothedVol);
    if (bDebug){
        micBar.updateGUI();
    }
    
    //  VISUAL
    //
    tangram.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    
    tangram.draw();
    
    ofPopMatrix();
    
    //  Draw the average volume
    //
    if (bDebug){
        
        if (tangram.bEdit){
            ofDrawBitmapString("Click over a shape to move and by pressing H, V, LEFT and RIGHT at the same time you can rotate it and flip it", 15, 15);
        } else {
            ofDrawBitmapString("Press E to enter into EDITMODE or M to hide this message", 15, 15);
        }
        
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
//        delete glyph;
//        glyph = new Glyph();
//        glyph->setScale(100);
//        tangram.createFromGlyph( glyph );
        
        tangram.load( "tangramShape/" + dataDir.getFile(selectedFile).getFileName() );
        selectedFile = (selectedFile+1)%dataDir.size();
    } else if (key == 'm'){
        bDebug = !bDebug;
    } else if (key == 'd'){
        tangram.bDebug = !tangram.bDebug;
    } else if (key == 'e'){
        tangram.bEdit = !tangram.bEdit;
    } else if (key == 's'){
        string filename = "t"+ofToString(ofGetDay())+"-"+ofToString(ofGetHours())+"-"+ofToString(ofGetMinutes())+"-"+ofToString(ofGetSeconds());
        tangram.save("tangramShape/"+filename+".tan");
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