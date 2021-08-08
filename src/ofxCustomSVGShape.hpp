//
//  ofxCustomSVGShape.hpp
//  damn_butts
//
//  Created by administrator on 30/7/21.
//

#ifndef ofxCustomSVGShape_hpp
#define ofxCustomSVGShape_hpp

#include <stdio.h>
#include <ofMain.h>
#include "ofxSvg.h"


class ofxCustomSVGShape{
    
public:
    
    void setup(std::string path, float scale, int ledCount, glm::vec2 position);
    void drawSampleRegion(bool hideArea);
    void ledObject();
    void draw(int x, int y);
    vector<glm::vec2> getPosition();
    int getSize();
    
//    int _scale;
    vector<glm::vec2> pos;
    
    ofxSVG svg;
//    float step;
//    vector<ofPolyline> outlines;

//    typedef ofPath ofTTFCharacter;
    
private:
    
};

#endif /* ofxCustomSVGShape_hpp */
