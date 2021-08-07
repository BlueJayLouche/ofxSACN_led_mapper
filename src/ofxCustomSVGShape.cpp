//
//  ofxCustomSVGShape.cpp
//  damn_butts
//
//  Created by administrator on 30/7/21.
//

#include "ofxCustomSVGShape.hpp"
void ofxCustomSVGShape::setup(std::string path, float scale, int ledCount, glm::vec2 position)
{
//    _size = scale;

    pos.clear();
    
    svg.load(path);
    
    glm::vec2 adjustedPos = position;
    
    for (ofPath p: svg.getPaths()){
        p.scale(scale, scale);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        const vector<ofPolyline>& lines = p.getOutline();
        for (int i = 0; i < lines.size(); i++)   {
            ofPolyline outline = lines[i].getResampledByCount(ledCount);
            outline.translate(adjustedPos);
            for (int j = 0; j < outline.size(); j++){
                pos.push_back(outline[j]);
            }
        }
    }
}
//--------------------------------------------------------------
vector<glm::vec2> ofxCustomSVGShape::getPosition()
{
    return pos;
}//--------------------------------------------------------------
int ofxCustomSVGShape::getSize()
{
    return pos.size();
}
