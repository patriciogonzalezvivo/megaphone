//
//  Bubbles.h
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/2/12.
//
//

#ifndef glyphsOnSpline_Bubbles_h
#define glyphsOnSpline_Bubbles_h

#include "ofMain.h"

#include "Shape.h"
#include "vectorField.h"

class Bubbles {
public:
    
    Bubbles();
    virtual ~Bubbles();
	
    void    setup(int _width, int _height, int _scale);
    void    generate( vector<Shape> &_shapes, int _amount );
    
    void    clean();
    
    void    update();
    
    void    draw();
    
    bool    *bDebug;
private:
    vector<Shape*>  particles;
    vectorField     VF;
    
};

#endif
