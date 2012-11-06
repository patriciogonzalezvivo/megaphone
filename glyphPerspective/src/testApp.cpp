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
    
    //  SOUND
    //
	soundStream.setup(this, 0, 2, 44100, 256, 4);
    smoothedVol     = 0.0;
    micBar.setLabel("mic");
    
    glyph = new Glyph();
    glyph->setScale(100);
    
    for (int i = 0; i < TOTAL; i++){
        float alpha = ofMap((float)i, 0, TOTAL-1, 1.0, 0.0);
        SuperGlyph newGlyph;
        newGlyph.createFromGlyph( glyph, 1.0-alpha, ofFloatColor(0.5));//(215.0/255.0,124.0/255.0,0.0));
        superGlyphs.push_back(newGlyph);
    }
    
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
    
    if (ofGetFrameNum()%10 == 0){
        vals.push_back(micBar.getActiveValue());
        while(vals.size() > superGlyphs.size()){
            vals.erase(vals.begin());
        }
    }
    
    for (int i = 0; i < superGlyphs.size(); i++){
        if (vals[i] > 0.001){
            superGlyphs[superGlyphs.size()-1-i].addCentralRepulsionForce( vals[i] );
        }
        superGlyphs[superGlyphs.size()-1-i].update();
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(127);
    //ofBackgroundGradient(ofColor::white, ofColor::gray);
    //ofBackgroundGradient(ofColor(255,200,100),ofColor(150.0,100,0));
    
    cam.begin();
    ofPushMatrix();
//    ofTranslate(width*0.5, height*0.5);
    ofTranslate(0, 0, TOTAL*-50.0);
    for (int i = superGlyphs.size()-1; i >= 0; i--){
        ofTranslate(0, 0, 50);
        ofScale(0.99, 0.99);
        superGlyphs[i].draw();
        
    }
    ofPopMatrix();
    cam.end();
    
    //  Draw the average volume
    //
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
        
        for (int i = 0; i < superGlyphs.size(); i++){
            float alpha = ofMap((float)i, 0, TOTAL-1, 1.0, 0.0);
            superGlyphs[i].createFromGlyph( glyph, 1.0-alpha, ofFloatColor(0.5));//ofFloatColor(215.0/255.0,124.0/255.0,0.0));
        }
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