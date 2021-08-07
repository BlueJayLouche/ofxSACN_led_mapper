//
//  ofxShapeFromXML.cpp
//  damn_butts
//
//  Created by administrator on 1/8/21.
//

#include "ofxShapeFromXML.hpp"
void ofxShapeFromXML::setup(std::string path, int scale, int ledCount, glm::vec2 position)
{
    
    if(!xml.load(path)){
        ofLogError() << "Couldn't load file";
    }

    pos.resize(ledCount);
    pos.clear();

    // select all strokes and iterate through them
    // for each stroke, create a new mesh
    auto strokesXml = xml.find("//drawing/stroke");
    for(auto & stroke: strokesXml){

        auto pts = stroke.getChildren("pt");
        for(auto & pt: pts){
            auto x = pt.getAttribute("x").getIntValue();
            auto y = pt.getAttribute("y").getIntValue();

            line.addVertex(glm::vec3(x,y,0));
        }
    }
    
//    _ledCount = ledCount;

    glm::vec2 adjustedPos = position;

    ofPolyline smoother = line.getResampledByCount(ledCount);
    smoother.translate(adjustedPos);
    smoother.scale(scale, scale);
    for (int i = 0; i < smoother.size(); i++){
        pos.push_back(smoother[i]);
    }
}
//--------------------------------------------------------------
vector<glm::vec2> ofxShapeFromXML::getPosition()
{
    return pos;
}//--------------------------------------------------------------
int ofxShapeFromXML::getSize()
{
    return pos.size();
}
