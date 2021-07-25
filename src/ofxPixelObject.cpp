//
//  ofxPixelObject.cpp
//  sACN_Mapped
//
//  Created by bluejaylouche on 19/7/21.
//
//  Adapted from: https://github.com/DHaylock/ofxOPC

#include "ofxPixelObject.hpp"
//--------------------------------------------------------------
vector <ofColor> ofxPixelObject::colorData()
{
    // Transmit Data
    return colors;
}
//--------------------------------------------------------------
vector<glm::vec2> ofxPixelObject::getPixelCoordinates()
{
    colors.clear();
    return pos;
}
//--------------------------------------------------------------
void ofxPixelObject::drawGrabRegion(bool hideArea)
{
    
}
//--------------------------------------------------------------
void ofxPixelObject::draw(int x, int y)
{
    
}
