//
//  Group.h
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/3/12.
//
//

#ifndef GROUP
#define GROUP

#include "ofMain.h"

#include "Glyph.h"
#include "Shape.h"

class Group {
public:
    Group();
    
    void    create();
    
    void    rotate90();
    
    void    update();
    
    void    draw();
    
private:
    vector<Shape>   shapes;
    
    float   scale;
    float   rotate;
};

#endif
