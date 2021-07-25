//
//  circleClass.hpp
//  sACN_Mapped
//
//  Created by Andrew Cherry on 19/7/21.
//

#ifndef circleClass_hpp
#define circleClass_hpp

#include <stdio.h>
#include <ofMain.h>

class circleClass{

public:

    void setup(int circleSize,int radius, int shiftX, int shiftY, int spacing);
    void drawSampleRegion(bool hideArea);
    void ledObject();
    void draw(int x, int y);
    vector<glm::vec2> getPosition();
    int circleSize();

    int size;
    vector<glm::vec2> pos;

private:
    ofMesh mesh;
//    vector<int> actualX;
//    vector<ofPoint> innerRadius;
//    vector<ofPoint> outerRadius;
    vector<ofColor> colors;
    int _spacing;
};
#endif /* circleClass_hpp */
