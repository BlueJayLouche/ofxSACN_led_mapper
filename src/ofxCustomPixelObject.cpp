//
//  ofxCustomPixelObject.cpp
//  sACN_Mapped
//
//  Created by bluejaylouche on 19/7/21.
//
//  Adapted from: https://github.com/DHaylock/ofxOPC

#include "ofxCustomPixelObject.hpp"
//--------------------------------------------------------------
ofxCustomPixelObject::ofxCustomPixelObject()
{
    
}
//--------------------------------------------------------------
ofxCustomPixelObject::~ofxCustomPixelObject()
{
    
}
//--------------------------------------------------------------
ofxCustomPixelObject::ofxCustomPixelObject(int offsetx, int offsety, vector<glm::vec2> positions, int numberOfLeds, int spacing)
{
    // Setup Positioning
    size = numberOfLeds;
    offsetX = 5;
    offsetY = 5;
    _spacing = spacing;
    
    actualX = offsetX+offsetx;
    actualY = offsetY+offsety;
    
    pos.clear();
    pos.resize(positions.size());
    pos = positions;
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for (unsigned int i = 1; i < pos.size(); i++)
    {
        //std::cout << "get Here! " << endl;

        glm::vec2 thisPoint = pos[i-1];
        glm::vec2 nextPoint = pos[i];
        glm::vec2 direction = (nextPoint - thisPoint);
        glm::vec3 rotation = glm::vec3(0.f, 0.f, 1.f);
        
        float distance = direction.length();
        glm::vec2 unitDirection = glm::normalize(direction);
        glm::vec3 toTheLeft = glm::rotate(glm::vec3((unitDirection),0.f), glm::radians(-90.f), rotation);
        glm::vec3 toTheRight = glm::rotate(glm::vec3((unitDirection),0.f), glm::radians(90.f), rotation);
//        glm::vec3 toTheRight = glm::rotate(unitDirection, glm::radians(90.f), rotation);

        glm::vec3 leftPoint = (glm::vec2(thisPoint),0.f)+toTheLeft*5;
        glm::vec3 rightPoint = (glm::vec2(thisPoint),0.f)+toTheRight*5;
        mesh.addVertex(glm::vec3(leftPoint.x, leftPoint.y, leftPoint.z));
        mesh.addVertex(glm::vec3(rightPoint.x, rightPoint.y, rightPoint.z));
    }
    
}
//--------------------------------------------------------------
void ofxCustomPixelObject::setupLedObject(int offsetx, int offsety, vector<glm::vec2> positions, int numberOfLeds, int spacing)
{
    // Setup Positioning
    size = numberOfLeds;
    offsetX = 5;
    offsetY = 5;
    _spacing = spacing;
    
    actualX = offsetX+offsetx;
    actualY = offsetY+offsety;
    
    pos.clear();
    pos.resize(positions.size());
    pos = positions;
}
//--------------------------------------------------------------
void ofxCustomPixelObject::drawGrabRegion(bool hideArea)
{
    if (hideArea == true) {
        // Draw Interaction Area
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(255, 255);
        ofPopStyle();
        
        // Visualise the Grabber
        ofSetColor(255, 175);
        ofNoFill();
    }
    else
    {
        // Visualise the Grabber
        ofSetColor(0, 175);
        ofNoFill();
    }
    
    ofPushStyle();
    ofSetColor(0, 50);
    mesh.draw();
    ofPopStyle();
    
    for (int i = 0; i < pos.size(); i++)
    {
        ofDrawCircle(pos[i],2);
    }
}
//--------------------------------------------------------------
void ofxCustomPixelObject::ledObject()
{
    ofFill();
    ofSetColor(100,175);
    ofDrawRectangle(0, 0, 10, size*_spacing);
    
    for (int i = 0; i < size; i++)
    {
        ofFill();
        if (!colors.empty()) {
            ofSetColor(colors[i]);
        }
        else {
            ofSetColor(ofColor::midnightBlue);
        }
        ofDrawCircle(5,0+(i*_spacing),2);
    }
}
//--------------------------------------------------------------
void ofxCustomPixelObject::draw(int x, int y)
{
    // Where to draw the Strip!
    ofPushMatrix();
    ofTranslate(x, y);
    ofPushMatrix();
    ofTranslate(offsetX+5,0);
    ledObject();
    ofPopMatrix();
    ofPopMatrix();
}
//--------------------------------------------------------------
//void ofxCustomPixelObject::translate(int _x, int _y, int _s)//, vector<glm::vec2> positions, int numberOfLeds, int spacing)
//{
//
//~ofxCustomPixelObject()->translate(*this,_x,_y,_s);
//
////    // Setup Positioning
////    //size = numberOfLeds;
////    offsetX = 5;
////    offsetY = 5;
////    //_spacing = spacing;
////    
////    actualX = offsetX+offsetx;
////    actualY = offsetY+offsety;
////    
//////    pos.clear();
//////    pos.resize(positions.size());
////    pos = pos;
//}
