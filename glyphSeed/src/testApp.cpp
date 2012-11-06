#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetWindowShape(300, 800);
    
    ofSeedRandom(0); 
    
    zoom = 3.0;
    tree.create(ofPoint(0,0), 10);
}

//--------------------------------------------------------------
void testApp::update(){

    tree.pct += 0.001; //ofMap(mouseY,0,ofGetHeight(),0.0,1.0,true);
    
    zoom -= 0.001;
    tree.update();

    //ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::black,ofColor::gray,OF_GRADIENT_LINEAR);
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.9);
    ofScale(zoom, zoom);
    tree.draw();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
        tree.create(ofPoint(0,0), 20);
        zoom = 3.0;
    } else if (key == 'd'){
        tree.bDebug = !tree.bDebug;
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