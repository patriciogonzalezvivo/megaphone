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
//    tangram.createFromGlyph( glyph );
    tangram.createSet();
    tangram.bDebug = true;
    tangram.bEdit = false;

    cameraAngle = 0.0;
    tangramAngle = 0.0;
    
    //  CONTROL
    //
    gui.setup("Controls");
    gui.add(posLerp.setup("position_amount", 0.05, 0.0, 0.1));
    gui.add(rotLerp.setup("rotation_amount", 0.05, 0.0, 0.1));
    gui.add(camLerp.setup("camera_amount",0.05,0.0,0.1));
    gui.add(wakeUpDist.setup("wakeUpDist", 100, 50, 500));
    gui.loadFromFile("gui.xml");
    dataDir.listDir("tangramShape");
    selectedFile = 0;
    bDebug = true;
}

//--------------------------------------------------------------
void testApp::update(){
    
    cameraAngle = ofLerp(cameraAngle, tangramAngle, camLerp);
    tangram.update(posLerp, rotLerp, camLerp, wakeUpDist);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    ofRotateY(cameraAngle);
    tangram.draw();
    ofPopMatrix();
    
    //  Draw the average volume
    //
    if (bDebug){
        gui.draw();
    }
}

void testApp::exit(){
    gui.saveToFile("gui.xml");
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
//        delete glyph;
//        glyph = new Glyph();
//        glyph->setScale(100);
//        tangram.createFromGlyph( glyph );
        
        tangram.load( "tangramShape/" + dataDir.getFile(selectedFile).getFileName() );
        tangramAngle = ofRandom(360);
        tangram.rotateY(tangramAngle);
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