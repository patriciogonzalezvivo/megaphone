//
//  Terrain.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/4/12.
//
//

#include "Terrain.h"

Terrain::Terrain(){
    resolution  = 10;
    topAltitud  = 100;
}

void Terrain::allocate(ofRectangle &_rect){
    x = _rect.x;
    y = _rect.y;
    width = _rect.width;
    height = _rect.height;
    
    heightFbo.allocate(width, height);
    normals.allocate(width, height);
    blur.allocate(width, height);
}

void Terrain::createFromGlyph( SuperGlyph &_glyph ){
    allocate(_glyph);
    
    colorFbo;
    colorFbo.allocate(width, height);
    colorFbo.begin();
    ofClear(0);
    ofPushMatrix();
    ofTranslate(width*0.5, height*0.5);
    _glyph.draw();
    ofPopMatrix();
    colorFbo.end();
    
    colorFbo.getTextureReference().readToPixels(colors);
    
    heightFbo.begin();
    ofClear(0);
    heightFbo.end();
}

void Terrain::update(){
    //  update the information on the GPU and down loadit 
    //
    blur.setTexture(heightFbo.getTextureReference());
    blur.update();
    
    ofFloatPixels heightMap;
    heightMap.allocate(heightFbo.getWidth(),
                       heightFbo.getHeight(),
                       OF_PIXELS_RGBA);
    heightFbo.readToPixels(heightMap);
    
    normals << blur;
    normals.update();
    
    ofFloatPixels norms;
    norms.allocate(normals.getTextureReference().getWidth(),
                   normals.getTextureReference().getHeight(),
                   OF_PIXELS_RGBA);
    normals.getTextureReference().readToPixels(norms);
    
    // Generate Vertex Field
    //
    int nVertexCount = (int) ( width * height * 6 / ( resolution * resolution ) );
    
    ofVec3f *pVertices      = new ofVec3f[nVertexCount];
    ofVec2f *pTexCoords     = new ofVec2f[nVertexCount];
    ofVec3f *pNormals       = new ofVec3f[nVertexCount];
    ofFloatColor *pColors   = new ofFloatColor[nVertexCount];
    
    int nX, nY, nTri, nIndex=0;
    float flX, flY;
    
    for( nY = 0; nY < height-resolution ; nY += (int) resolution ){
        for( nX = 0; nX < width-resolution ; nX += (int) resolution ){
            for( nTri = 0; nTri < 6; nTri++ ){
                
                flX = (float) nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? resolution : 0.0f );
                flY = (float) nY + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? resolution : 0.0f );
                
                pVertices[nIndex].x = flX - ( width *0.5 );
                pVertices[nIndex].y = flY - ( height *0.5 );
                pVertices[nIndex].z = heightMap.getColor((int)flX, (int)flY).r * topAltitud;
                
                // 3	0 --- 1		nTri reference
                // | \	  \	  |
                // |   \	\ |
                // 4 --- 5	  2
                
                //  Normals by vert
                pNormals[nIndex].x = norms.getColor((int)flX, (int)flY).r;
                pNormals[nIndex].y = norms.getColor((int)flX, (int)flY).g;
                pNormals[nIndex].z = norms.getColor((int)flX, (int)flY).b;
                
                pTexCoords[nIndex].x = flX;// * textureScale;// / width;
                pTexCoords[nIndex].y = flY;// * textureScale;// / height;
                
                //  Colors by vert
                pColors[nIndex] = colors.getColor((int)flX, (int)flY);//ofColor(0,30);
                
                // Increment Our Index
                nIndex++;
            }
        }
    }
    
    mesh.clear();
    mesh.addTexCoords(pTexCoords, nVertexCount);
    mesh.addVertices(pVertices, nVertexCount);
    mesh.addNormals(pNormals, nVertexCount);
    mesh.addColors(pColors, nVertexCount);
    
    delete [] pVertices; pVertices = NULL;
    delete [] pTexCoords; pTexCoords = NULL;
    delete [] pNormals; pNormals = NULL;
    delete [] pColors; pColors = NULL;
}

void Terrain::draw(){
    
    ofPushStyle();
        glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
//    glEnable(GL_TEXTURE_2D);
    ofPushMatrix();
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    
//    colorFbo.getTextureReference().bind();
    mesh.draw();
//    colorFbo.getTextureReference().unbind();
    
    ofPopMatrix();
    
//    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
    
    ofPopStyle();
}