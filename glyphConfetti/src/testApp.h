#pragma once

#include "ofMain.h"

#include "Glyph.h"

#include "Shape.h"
#include "Bubbles.h"
#include "ThresholdBar.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //  Glyph Creator and transformer to Shape
    //
    void creatGlyph();
    
    //  Audio
    //
    void audioIn(float * input, int bufferSize, int nChannels);
    ofSoundStream   soundStream;
    float           smoothedVol;
    ThresholdBar    micBar;
    
    vector<Shape>   shapes;
    Bubbles         bubbles;
    
    int     width, height, scale;
    
    bool    bDebug;
};
