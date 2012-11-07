//
//  Tangram.h
//
//  Created by Patricio Gonzalez Vivo on 11/6/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef TANGRAM
#define TANGRAM

#include "ofMain.h"

#include "Glyph.h"
#include "Shape.h"

#include "ofxXmlSettings.h"

class Tangram : public ofRectangle {
public:
    Tangram();
    
    bool        load( string _file );
    bool        save( string _file );
    
    void        createSet();
    void        createFromGlyph( Glyph *_glyph );
    
    void        update();
    void        draw();
    
    bool        bDebug;
    bool        bEdit;
private:
    virtual void    _mousePressed(ofMouseEventArgs &e);
    virtual void    _mouseDragged(ofMouseEventArgs &e);
    virtual void    _mouseReleased(ofMouseEventArgs &e);
    virtual void    _keyPressed(ofKeyEventArgs &e);
    
    void            _centerShapes();
    
    vector<Shape>   limbs;
    
    float           perimeter;
    float           area;
    
    int             selectedLimb;
};

#endif
