//
//  ofxCustomPixelRing.cpp
//  sACN_Mapped
//
//  Created by bluejaylouche on 19/7/21.
//

#include "ofxCustomPixelRing.hpp"
void ofxCustomPixelRing::setup(int ledCount,int radius, int shiftX, int shiftY, int spacing)
{
//    size = ledCount;
    //memcpy(size,circleSize,sizeof(circleSize));
    _spacing = spacing;

    int actualX = ofGetWidth()/2+shiftX;
    int actualY = ofGetHeight()/2+shiftY;

    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    pos.resize(ledCount);
    pos.clear();

    for (int i = 0; i < ledCount; i++) {
        float angle = (1.0 * i) * (2.0 * M_PI)/(1.0 * ledCount);
        // Generate the position of the grabber points
        float rx = actualX + (radius * sin(angle));
        float ry = actualY + (radius * cos(angle));
        
        pos.push_back(glm::vec2(rx,ry));
    }
}
//--------------------------------------------------------------
vector<glm::vec2> ofxCustomPixelRing::getPosition()
{
    return pos;
}//--------------------------------------------------------------
int ofxCustomPixelRing::getSize()
{
    return pos.size();
}


