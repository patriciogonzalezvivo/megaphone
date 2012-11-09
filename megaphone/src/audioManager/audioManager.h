#pragma once

#include "dywapitchtrack.h"
#include <vector.h>
#include "aubioAnalyzer.h"
#include "fft.h"
#include "fftOctaveAnalyzer.h"
#include "AubioOnsetDetector.h"
#include "statistics.h"


//---------------------------------------------------------
typedef struct {
    float pitch;
    float aubioPitch;
    float aubioRMS;
    float RMS;              // root mean square
    float pitchStdDev;      // how "pitchy" are we
    float * fftOctaves; 
    int nFftOctaves;
    
    float spectralCentroid;
    
    bool bVox;              // are we loud enough?
    
    
} analysisResults;


class valueChart{
public: 
    
    int maxValues;
    vector < float > values;
    bool bAutoMinMax;
    float min, max;
    ofRectangle bounds;
    string name;
    
    valueChart () {
        min = 0;
        max = 1;
        maxValues = 150;
    }
    
    void addValue(float value){
        values.push_back(value);
        if (values.size() > maxValues){
            values.erase(values.begin());
        }
    }
    
    void draw(){
        ofEnableAlphaBlending();
        ofSetColor(127, 50);
        ofFill();
        ofRect(bounds);
        ofNoFill();
        ofSetColor(255, 255);
        ofBeginShape();
        for (int i = 0; i < values.size(); i++){
           ofVertex( ofMap(i, 0, values.size()-1, bounds.x, bounds.x + bounds.width), 
                    ofMap(values[i], min, max, bounds.y + bounds.height, bounds.y, true));
        }
        ofEndShape();
        
        ofDrawBitmapStringHighlight(name, ofPoint(bounds.x, bounds.y) + ofPoint(0,bounds.height + 18));
    }
    
};



//---------------------------------------------------------
class audioManager {

public: 
    
    void setup();
    void update(float * audio, int bufferSize);
    void calculatePitch();
    void calculateRMS();
    void calculateFFT();
    void calculateOnset();
    
    
    // pitch tracker : 
    double * audioDataDouble;
    dywapitchtracker pitchtracker;
    float * audioData;
    int bufferSize;
    
    
    
    analysisResults resultsFromMainThread;
    analysisResults results;
    
    aubioAnalyzer AA;
   
    
    void calculateAubio();

    fft myfft;
    FFTOctaveAnalyzer FFTanalyzer;
    
    float * magnitude ;//[BUFFER_SIZE];
    float * phase;//[BUFFER_SIZE];
    float * power;// [BUFFER_SIZE];
    float * freq;//[BUFFER_SIZE];
    
    int nBuffers;
    float * largerBuffer;
    
    AubioOnsetDetector AOD;
    
    valueChart volume;
    valueChart onset;
    valueChart pitch;
    valueChart vox;
    valueChart spectralCentroid;
    
    vector < valueChart * > charts;
    
    
    void draw();
    
    
};






