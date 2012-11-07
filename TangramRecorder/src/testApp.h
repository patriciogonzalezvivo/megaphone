#pragma once

#include "ofMain.h"

//  Visual
//
#include "Tangram.h"

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
    
    //  Visual
    //
    Glyph           *glyph;
    Tangram         tangram;

    ofDirectory     dataDir;
    int             selectedFile;
    
    int             width, height;
    bool            bDebug;
};
