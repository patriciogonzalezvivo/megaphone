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
    
    bDebug = false;
}

//--------------------------------------------------------------
void testApp::update(){
    for(int i = 0; i < groups.size(); i++){
        groups[i]->update();
    }
    
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::black,ofColor::gray,OF_GRADIENT_LINEAR);
    
    for(int i = 0; i < groups.size(); i++){
        groups[i]->draw();
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
        for(int i = 0; i < groups.size(); i++){
            groups[i]->rotate90();
        }
        
        Group *newGroup = new Group();
        groups.push_back(newGroup);
    } else if (key == 'd'){
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