#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSeedRandom(0); 
    
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
    tangram.bEdit = true;

    bDebug = true;
}

//--------------------------------------------------------------
void testApp::update(){
    
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
    }
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