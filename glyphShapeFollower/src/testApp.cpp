#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    width = 300;
    height = 800;
    
    ofSetWindowShape(width, height);
    
    time    = 0.0;
    zoom    = 1.0;
    angle   = 0.0;
    bDebug  = true;
    
    ofSeedRandom(0);
    createGlyph();
    cameraFocus.pos.set(0,0);
}

void testApp::createGlyph(){
    glyphShapes.clear();
    
    //  Make a new Glyph
    //
    Glyph *glyph = new Glyph();
    glyph->setScale(60);
    
    //  Transform it to Shapes
    //
    for (int i = 0; i < glyph->limbs().size(); i++){
        Shape newShape;
        newShape.makeFromLimb(i, *glyph);
        newShape.bDebug = &bDebug;
        glyphShapes.push_back( newShape );
    }
    
    //  Make a path from the shapes
    //
    path.clear();
    for (int i = 0; i < glyphShapes.size(); i++){
        for (int j = 0; j < glyphShapes[i].getVertices().size(); j++) {
            ofPoint vert = glyphShapes[i].getVertices()[j];
            path.addVertex( vert );
        }
    }
    path.close();
}

//--------------------------------------------------------------
void testApp::update(){
    time += 0.1;
    
    if (zoom < 10)
        zoom += 0.01;
    
    headPos = getPathPositionAt(time);
    cameraFocus.addAttractionForce( headPos, 1000, 0.1);
    cameraFocus.addRepulsionForce( headPos, 1, 0.5);
    cameraFocus.update();
    
    float angleTo = atan2(cameraFocus.vel.y,cameraFocus.vel.x);
    float diffAngle = (angleTo - angle);
    if (diffAngle < -PI) diffAngle += TWO_PI;
    if (diffAngle > PI) diffAngle -= TWO_PI;
    angle  += 0.005 * diffAngle;
    
    //angle = ofLerp(angle, PI+atan2(cameraFocusFocus.vel.y,cameraFocus.vel.x), 0.005);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

ofPoint	testApp::getPathPositionAt( float &_miles ){
	float total = path.getPerimeter();
    
    while (_miles >= total){
        _miles = abs(_miles-total);
    }
    
	ofPoint pos;
    float previus = 0.0;
    
	for (int i = 0; i < path.getVertices().size()-1; i++){
        ofPoint diff = path.getVertices()[i+1] - path.getVertices()[i];
        float lenght = diff.length();
        
		if (_miles >= previus && _miles <= previus + lenght ){
			
			float pct = (_miles - previus)/lenght;
			
			// figure out where we are between a and b
			pos.x = (1-pct) * path.getVertices()[i].x + (pct) * path.getVertices()[i+1].x;
			pos.y = (1-pct) * path.getVertices()[i].y + (pct) * path.getVertices()[i+1].y;
            
            break;
		}
        
        previus += lenght;
	}
    
    return pos;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::black,ofColor::gray,OF_GRADIENT_LINEAR);
    
    //  Draw Shapes
    //
    ofPushMatrix();
    ofTranslate(width*0.5, height*0.5);
    ofScale(zoom,zoom);
    ofRotate(ofRadToDeg(angle), 0, 0, 1);
    ofTranslate(cameraFocus.pos*-1.0);
    for(int i = 0; i < glyphShapes.size(); i++){
        glyphShapes[i].draw();
    }
    
    if (bDebug){
        ofSetColor(0,50);
        path.draw();
        
        ofNoFill();
        ofSetColor(255,0,0);
        ofCircle(headPos, 1);
    }
    ofPopMatrix();
    
    //  Draw cameraFocus Focus Center
    //
    ofPushMatrix();    
    ofTranslate(width*0.5, height*0.5);
    ofTranslate(cameraFocus.pos*-1.0);
    cameraFocus.draw();
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
        createGlyph();
        zoom = 1.0;
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