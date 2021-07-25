//
//  ofxCustomPixelObject.hpp
//  sACN_Mapped
//
//  Created by bluejaylouche on 19/7/21.
//
//  Adapted from: https://github.com/DHaylock/ofxOPC

#ifndef ofxCustomPixelObject_hpp
#define ofxCustomPixelObject_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxPixelObject.hpp"

class ofxCustomPixelObject : public ofxPixelObject {
public:
    ofxCustomPixelObject();
    virtual ~ofxCustomPixelObject();
    
    //! Constructor
    ofxCustomPixelObject(int offsetx, int offsety, vector<glm::vec2> positions, int numberOfLeds, int spacing);
    
    //! Setup Strip
    void setupLedObject(int offsetx, int offsety, vector<glm::vec2> positions, int numberOfLeds, int spacing);
    void drawGrabRegion(bool hideArea);
    void draw(int offsetx, int offsety);
//    void translate(int offsetx, int offsety);
    
private:
    
    // Draw Unit
    void ledObject();
    
    ofMesh mesh;
};


#endif /* ofxCustomPixelObject_hpp */
