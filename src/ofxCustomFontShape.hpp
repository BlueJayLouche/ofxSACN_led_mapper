//
//  ofxCustomFontShape.hpp
//  sACN_Mapper
//
//  Created by bluejaylouche on 23/7/21.
//

#ifndef ofxCustomFontShape_hpp
#define ofxCustomFontShape_hpp

#include <stdio.h>
#include <ofMain.h>

class ofxCustomFontShape{
    
public:
    
    void setup(uint32_t letter, int size, int spacing, glm::vec2 position);
    void drawSampleRegion(bool hideArea);
    void ledObject();
    void draw(int x, int y);
    vector<glm::vec2> getPosition();
    int circleSize();
    
    int _size;
    vector<glm::vec2> pos;
    
    ofTrueTypeFont font;
    typedef ofPath ofTTFCharacter;
    
private:

};

#endif /* ofxCustomFontShape_hpp */
