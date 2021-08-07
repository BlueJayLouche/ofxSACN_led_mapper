//
//  ofxCustomPixelRing.hpp
//  sACN_Mapped
//
//  Created by bluejaylouche on 19/7/21.
//

#ifndef ofxCustomPixelRing_hpp
#define ofxCustomPixelRing_hpp

#include <stdio.h>
#include <ofMain.h>

class ofxCustomPixelRing{

public:

    void setup(int ledCount,int radius, int shiftX, int shiftY, int spacing);
    vector<glm::vec2> getPosition();
    int getSize();

    vector<glm::vec2> pos;

private:
    ofMesh mesh;
//    vector<int> actualX;
//    vector<ofPoint> innerRadius;
//    vector<ofPoint> outerRadius;
    vector<ofColor> colors;
    int _spacing;
};
#endif /* ofxCustomPixelRing_hpp */
