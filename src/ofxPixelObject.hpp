//
//  ofxPixelObject.hpp
//  sACN_Mapped
//
//  Created by bluejaylouche on 19/7/21.
//
//  Adapted from: https://github.com/DHaylock/ofxOPC

#ifndef ofxPixelObject_hpp
#define ofxPixelObject_hpp

#include <stdio.h>
#include "ofMain.h"

class ofxPixelObject {
public:
    
    ofxPixelObject(){}
    virtual ~ofxPixelObject(){}
    //! Return Data Method
    vector <ofColor> colorData();
    
    //! Return the locations of the Pixels
    vector<glm::vec2> getPixelCoordinates();
    
    // Variables
    int size;
    int radius;
    int _spacing;
    float offsetX;
    float offsetY;
    float actualX;
    float actualY;
    //vector<glm::vec3> _pos;
    int _cols;
    int _rows;
    
    
    // Hold the Captured Colors
    vector <ofColor> colors;
    // Hold the Position of our Capture points
    vector<glm::vec2> pos;
    
    virtual void drawGrabRegion(bool hideArea);
    virtual void draw(int x,int y);
    
private:
    
    
};

#endif /* ofxPixelObject_hpp */
