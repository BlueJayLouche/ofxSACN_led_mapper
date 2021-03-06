//
//  fontClass.cpp
//  sACN_Mapper
//
//  Created by bluejaylouche on 23/7/21.
//

#include "ofxCustomFontShape.hpp"
void ofxCustomFontShape::setup(uint32_t letter, int scale, int ledCount, glm::vec2 position)
{
//    _scale = scale;
    font.load("/Users/administrator/Developer/of_v0.11.0_osx_release/apps/myApps/fontingAbout/bin/data/fonts/The Fortune.otf", scale, true, true, true);
    
    bool vflip = true;
    bool filled = false;
    pos.clear();
    
    ofTTFCharacter nLetter = font.getCharacterAsPoints(letter, vflip, filled);
    
    glm::vec2 adjustedPos = position;
    
    vector<ofPolyline> nPolylines = nLetter.getOutline();
    
    for (int j = 0; j<nPolylines.size(); j++){
        ofPolyline outline = nPolylines[j].getResampledByCount(ledCount);
        outline.translate(adjustedPos);
        for (int k = 0; k < outline.size(); k++){
            
            pos.push_back(outline[k]);
//            std::cout << "value: " << pos.at(k) << endl;

        }
    }
}
//--------------------------------------------------------------
vector<glm::vec2> ofxCustomFontShape::getPosition()
{
    return pos;
}//--------------------------------------------------------------
int ofxCustomFontShape::getSize()
{
    return pos.size();
}
