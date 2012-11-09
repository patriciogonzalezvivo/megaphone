//
//  ControlsInterface.h
//  ofxMTL2DMapping
//
//  Created by Elie Zananiri on 12-04-18.
//

#pragma once

#include "ofxUI.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class ControlsInterface
{
    protected:
        ofxUIScrollableCanvas *_canvas;
        string _file;
    
        static const int kSliderHeight = 16;
        static const int kSpacerHeight = 20;
        static const int kToggleSize = 16;
        static const int kBottomSpacerHeight = 100; // padding to be able to scroll until the end/bottom of the UI canvas
    
        bool getToggleValue(const string& name);
        float getSliderValue(const string& name);
        ofPoint get2DPadValue(const string& name);
    
    
    public:
        ControlsInterface(int width, ofColor color, const string& file);
    
        void save();
        void load();
        
        void enable();
        void disable();
        void toggle();
        bool isEnabled();
        bool isHit(int x, int y);

};
