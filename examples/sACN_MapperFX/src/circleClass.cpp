//
//  circleClass.cpp
//  sACN_Mapped
//
//  Created by Andrew Cherry on 19/7/21.
//

#include "circleClass.hpp"
void circleClass::setup(int circleSize,int radius, int shiftX, int shiftY, int spacing)
{
    size = circleSize;
    //memcpy(size,circleSize,sizeof(circleSize));
    _spacing = spacing;

    int actualX = ofGetWidth()/2+shiftX;
    int actualY = ofGetHeight()/2+shiftY;

    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    pos.resize(size);
    pos.clear();

    for (int i = 0; i < size; i++) {
        float angle = (1.0 * i) * (2.0 * M_PI)/(1.0 * size);
        // Generate the position of the grabber points
        float rx = actualX + (radius * sin(angle));
        float ry = actualY + (radius * cos(angle));
        
        pos.push_back(glm::vec2(rx,ry));
    }
}

void circleClass::drawSampleRegion(bool hideArea)//, ofMesh mesh, vector<ofPoint> pos)
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
        std::cout << "get Here! " << endl;

    }
}

//--------------------------------------------------------------
void circleClass::ledObject()
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
            ofSetColor(ofColor::red);
        }
        ofDrawCircle(5,0+(i*_spacing),2);
    }
}
//--------------------------------------------------------------
void circleClass::draw(int x, int y)
{
    // Where to draw the Strip!
    ofPushMatrix();
    ofTranslate(x, y);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2+5,0);
    ledObject();
    ofPopMatrix();
    ofPopMatrix();
}
//--------------------------------------------------------------
vector<glm::vec2> circleClass::getPosition()
{
    return pos;
}//--------------------------------------------------------------
int circleClass::circleSize()
{
    return size;
}


