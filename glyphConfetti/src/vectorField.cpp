#include "vectorField.h"



//------------------------------------------------------------------------------------
vectorField::vectorField(){
    field = NULL;
    
    x = 0;
    y = 0;
}

vectorField::~vectorField(){
    if (field != NULL){
        delete [] field;
    }
}

//------------------------------------------------------------------------------------
void vectorField::setupField(int _cols, int _rows, int _width, int _height){
	
	cols           = _cols;
	rows            = _rows;
	width           = _width;
	height          = _height;
	
	if (field != NULL){
        delete [] field;
    }
	
	nTotal = cols * rows;
	field = new ofPoint[nTotal];
    clear();
}

ofPoint& vectorField::operator[](int _index){
    if (( _index > 0 ) && (_index < nTotal))
        return field[_index];
    else {
        ofPoint empty = ofPoint(0,0,0);
        return empty;
    }
}

//------------------------------------------------------------------------------------
void vectorField::clear(){
    for (int i = 0; i < nTotal; i++){
        field[i].set(0,0);
    }
}


//------------------------------------------------------------------------------------
void vectorField::fadeField(float fadeAmount){
	for (int i = 0; i < nTotal; i++){
        field[i].set(field[i].x*fadeAmount,field[i].y*fadeAmount);
    }
}

//------------------------------------------------------------------------------------
void vectorField::randomizeField(float scale){
	for (int i = 0; i < nTotal; i++){
        // random between -1 and 1
        float x = (float)(ofRandom(-1,1)) * scale;
        float y = (float)(ofRandom(-1,1)) * scale;
        field[i].set(x,y);
    }
}

void vectorField::noiseField( float _scale  ,float _speed, float _turbulence, bool _signed){
    
    float t = ofGetElapsedTimef() * 0.5;
    
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++){
            // pos in array
            int pos = j * cols + i;
            
            float normx = ofNormalize(i, 0, cols);
            float normy = ofNormalize(j, 0, rows);

            float u, v;
            if (_signed){
                u = ofSignedNoise(t + TWO_PI, normx * _turbulence + TWO_PI, normy * _turbulence + TWO_PI);
                v = ofSignedNoise(t - TWO_PI, normx * _turbulence - TWO_PI, normy * _turbulence + TWO_PI);
            } else {
                u = ofNoise(t + TWO_PI, normx * _turbulence + TWO_PI, normy * _turbulence + TWO_PI);
                v = ofNoise(t - TWO_PI, normx * _turbulence - TWO_PI, normy * _turbulence + TWO_PI);
            }
            
            ofPoint force = ofPoint(u,v,0.0) ;
            field[ pos ] = field[ pos ] * (1.0-_scale) + force * _scale;
        }
    }
}

void vectorField::AngleNoiseField( float _angle, float _turbulence, float _scale ){
    
    float t = ofGetElapsedTimef() * 0.5;
    
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++){
            // pos in array
            int pos = j * cols + i;
            
            float normx = ofNormalize(i, 0, cols);
            float normy = ofNormalize(j, 0, rows);
            
            float a = _angle + ofSignedNoise(t, normx * _turbulence, normy * _turbulence);
            

            ofPoint force = ofPoint(cos(a),sin(a),0.0) ;
            field[ pos ] = field[ pos ] * (1.0-_scale) + force * _scale;
        }
    }
}

//------------------------------------------------------------------------------------
void vectorField::draw(){
	
    float scalex = (float)width / (float)cols;
    float scaley = (float)height / (float)rows;
	
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++){

            // pos in array
            int pos = j * cols + i;
            // pos externally
            float px = 	i * scalex;
            float py = 	j * scaley;
            float px2 = px + field[pos].x * 5;
            float py2 = py + field[pos].y * 5;
			
            ofLine(px,py, px2, py2);
			
			
			// draw an baseline to show direction
			// get the line as vector, calculate length, then normalize. 
			// rotate and draw based on length
			
			ofVec2f line;
			line.set(px2-px, py2-py);
			float length = line.length();
			line.normalize();
			line.rotate(90);  // these are angles in degrees
			ofLine(px - line.x*length*0.2, py - line.y*length*0.2, px + line.x*length*0.2, py + line.y*length*0.2);
        }
    }
}


//------------------------------------------------------------------------------------
int vectorField::getIndexFor(float xpos, float ypos){
    // convert xpos and ypos into pcts =
	float xPct = xpos / (float)width;
	float yPct = ypos / (float)height;
	
	// if we are less then 0 or greater then 1 in x or y, return no force.
	if ((xPct < 0 || xPct > 1) || (yPct < 0 || yPct > 1)){
		return 0;
	}
	
    // where are we in the array
    int fieldPosX = (int)(xPct * cols);
    int fieldPosY = (int)(yPct * rows);
    
    // saftey :)
    fieldPosX = MAX(0, MIN(fieldPosX, cols-1));
    fieldPosY = MAX(0, MIN(fieldPosY, rows-1));
    
    // pos in array
    return fieldPosY * cols + fieldPosX;
}

ofPoint	vectorField::getForceFromPos(ofPoint pos){
    return getForceFromPos(pos.x,pos.y);
}

ofPoint vectorField::getForceFromPos(float xpos, float ypos){
    
    int pos = getIndexFor(xpos,ypos);
    
    if (pos == 0)
        return ofPoint(0,0);
	
	return ofPoint(field[pos].x * 0.1, field[pos].y * 0.1, 0.0 );  // scale here as values are pretty large.
}

//------------------------------------------------------------------------------------
void vectorField::addInwardCircle(float x, float y, float radius, float strength){
	
    // x y and radius are in external dimensions.  Let's put them into internal dimensions:
	// first convert to pct:
	
	float pctx			= x / (float)width;
	float pcty			= y / (float)height;
	float radiusPct		= radius / (float)width;
	
	// then, use them here:
    int fieldPosX		= (int)(pctx * (float)cols);
    int fieldPosY		= (int)(pcty * (float)rows);
	float fieldRadius	= (float)(radiusPct * cols);
	
	// we used to do this search through every pixel, ie:
	//    for (int i = 0; i < cols; i++){
	//    for (int j = 0; j < rows; j++){
	// but we can be smarter :)
	// now, as we search, we can reduce the "pixels" we look at by
	// using the x y +/- radius.
	// use min and max to make sure we don't look over the edges
	
	int startx	= MAX(fieldPosX - fieldRadius, 0);
	int starty	= MAX(fieldPosY - fieldRadius, 0);
	int endx	= MIN(fieldPosX + fieldRadius, cols);
	int endy	= MIN(fieldPosY + fieldRadius, rows);
	
	
    for (int i = startx; i < endx; i++){
        for (int j = starty; j < endy; j++){
			
            int pos = j * cols + i;
            float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                         (fieldPosY-j)*(fieldPosY-j));
            
			if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
			
			if (distance < fieldRadius){
				
				float pct = 1.0f - (distance / fieldRadius);
				
                float strongness = strength * pct;
                float unit_px = ( fieldPosX - i) / distance;
                float unit_py = ( fieldPosY - j) / distance;
                field[pos].x += unit_px * strongness;
                field[pos].y += unit_py * strongness;
            }
        }
    }
}


//------------------------------------------------------------------------------------
void vectorField::addOutwardCircle(float x, float y, float radius, float strength){
	
	
	// x y and radius are in external dimensions.  Let's put them into internal dimensions:
	// first convert to pct:
	
	float pctx			= x / (float)width;
	float pcty			= y / (float)height;
	float radiusPct		= radius / (float)width;
	
	// then, use them here:
    int fieldPosX		= (int)(pctx * (float)cols);
    int fieldPosY		= (int)(pcty * (float)rows);
	float fieldRadius	= (float)(radiusPct * cols);
	
	// we used to do this search through every pixel, ie:
	//    for (int i = 0; i < cols; i++){
	//    for (int j = 0; j < rows; j++){
	// but we can be smarter :)
	// now, as we search, we can reduce the "pixels" we look at by
	// using the x y +/- radius.
	// use min and max to make sure we don't look over the edges
	
	int startx	= MAX(fieldPosX - fieldRadius, 0);
	int starty	= MAX(fieldPosY - fieldRadius, 0);
	int endx	= MIN(fieldPosX + fieldRadius, cols);
	int endy	= MIN(fieldPosY + fieldRadius, rows);
    
	
    for (int i = startx; i < endx; i++){
        for (int j = starty; j < endy; j++){
            
            int pos = j * cols + i;
            float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                         (fieldPosY-j)*(fieldPosY-j));
            
			if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
			
			if (distance < fieldRadius){
                
				float pct = 1.0f - (distance / fieldRadius);
                float strongness = strength * pct;
                float unit_px = ( fieldPosX - i) / distance;
                float unit_py = ( fieldPosY - j) / distance;
                field[pos].x -= unit_px * strongness;
                field[pos].y -= unit_py * strongness;
            }
        }
    }
}

//------------------------------------------------------------------------------------
void vectorField::addClockwiseCircle(float x, float y, float radius, float strength){
	
	
	
    // x y and radius are in external dimensions.  Let's put them into internal dimensions:
	// first convert to pct:
	
	float pctx			= x / (float)width;
	float pcty			= y / (float)height;
	float radiusPct		= radius / (float)width;
	
	// then, use them here:
    int fieldPosX		= (int)(pctx * (float)cols);
    int fieldPosY		= (int)(pcty * (float)rows);
	float fieldRadius	= (float)(radiusPct * cols);
	
	// we used to do this search through every pixel, ie:
	//    for (int i = 0; i < cols; i++){
	//    for (int j = 0; j < rows; j++){
	// but we can be smarter :)
	// now, as we search, we can reduce the "pixels" we look at by
	// using the x y +/- radius.
	// use min and max to make sure we don't look over the edges
	
	int startx	= MAX(fieldPosX - fieldRadius, 0);
	int starty	= MAX(fieldPosY - fieldRadius, 0);
	int endx	= MIN(fieldPosX + fieldRadius, cols);
	int endy	= MIN(fieldPosY + fieldRadius, rows);
	
	
    for (int i = startx; i < endx; i++){
        for (int j = starty; j < endy; j++){
			
            int pos = j * cols + i;
            float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                         (fieldPosY-j)*(fieldPosY-j));
            
			if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
			
			if (distance < fieldRadius){
				
				float pct = 1.0f - (distance / fieldRadius);
				
                float strongness = strength * pct;
                float unit_px = ( fieldPosX - i) / distance;
                float unit_py = ( fieldPosY - j) / distance;
                field[pos].x += unit_py * strongness;   /// Note: px and py switched, for perpendicular
                field[pos].y -= unit_px * strongness;
            }
        }
    }
}



//------------------------------------------------------------------------------------
void vectorField::addCounterClockwiseCircle(float x, float y, float radius, float strength){
	
	
	
    // x y and radius are in external dimensions.  Let's put them into internal dimensions:
	// first convert to pct:
	
	float pctx			= x / (float)width;
	float pcty			= y / (float)height;
	float radiusPct		= radius / (float)width;
	
	// then, use them here:
    int fieldPosX		= (int)(pctx * (float)cols);
    int fieldPosY		= (int)(pcty * (float)rows);
	float fieldRadius	= (float)(radiusPct * cols);
	
	// we used to do this search through every pixel, ie:
	//    for (int i = 0; i < cols; i++){
	//    for (int j = 0; j < rows; j++){
	// but we can be smarter :)
	// now, as we search, we can reduce the "pixels" we look at by
	// using the x y +/- radius.
	// use min and max to make sure we don't look over the edges
	
	int startx	= MAX(fieldPosX - fieldRadius, 0);
	int starty	= MAX(fieldPosY - fieldRadius, 0);
	int endx	= MIN(fieldPosX + fieldRadius, cols);
	int endy	= MIN(fieldPosY + fieldRadius, rows);
	
	
    for (int i = startx; i < endx; i++){
        for (int j = starty; j < endy; j++){
			
            int pos = j * cols + i;
            float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                         (fieldPosY-j)*(fieldPosY-j));
            
			if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
			
			if (distance < fieldRadius){
				
				float pct = 1.0f - (distance / fieldRadius);
				
                float strongness = strength * pct;
                float unit_px = ( fieldPosX - i) / distance;
                float unit_py = ( fieldPosY - j) / distance;
                field[pos].x -= unit_py * strongness;   /// Note: px and py switched, for perpendicular
                field[pos].y += unit_px * strongness;
            }
        }
    }
}


//------------------------------------------------------------------------------------
void vectorField::addVectorCircle(float x, float y, float vx, float vy, float radius, float strength){
	
	
	
	// x y and radius are in external dimensions.  Let's put them into internal dimensions:
	// first convert to pct:
	
	float pctx			= x / (float)width;
	float pcty			= y / (float)height;
	float radiusPct		= radius / (float)width;
	
	// then, use them here:
    int fieldPosX		= (int)(pctx * (float)cols);
    int fieldPosY		= (int)(pcty * (float)rows);
	float fieldRadius	= (float)(radiusPct * cols);
	
	// we used to do this search through every pixel, ie:
	//    for (int i = 0; i < cols; i++){
	//    for (int j = 0; j < rows; j++){
	// but we can be smarter :)
	// now, as we search, we can reduce the "pixels" we look at by
	// using the x y +/- radius.
	// use min and max to make sure we don't look over the edges
	
	int startx	= MAX(fieldPosX - fieldRadius, 0);
	int starty	= MAX(fieldPosY - fieldRadius, 0);
	int endx	= MIN(fieldPosX + fieldRadius, cols);
	int endy	= MIN(fieldPosY + fieldRadius, rows);
	
	
    for (int i = startx; i < endx; i++){
        for (int j = starty; j < endy; j++){
			
            int pos = j * cols + i;
            float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                         (fieldPosY-j)*(fieldPosY-j));
            
			if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
			
			if (distance < fieldRadius){
				
				float pct = 1.0f - (distance / fieldRadius);
                float strongness = strength * pct;
                field[pos].x += vx * strongness;
                field[pos].y += vy * strongness;
            }
        }
    }
}