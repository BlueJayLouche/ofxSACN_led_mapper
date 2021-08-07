//
//  ofxShapeFromXML.hpp
//  damn_butts
//
//  Created by administrator on 1/8/21.
//

#ifndef ofxShapeFromXML_hpp
#define ofxShapeFromXML_hpp

#include <stdio.h>
#include <ofMain.h>

class ofxShapeFromXML{
    
public:
    
    void setup(std::string path, int scale, int ledCount, glm::vec2 position);
    void drawSampleRegion(bool hideArea);
    void ledObject();
    void draw(int x, int y);
    vector<glm::vec2> getPosition();
    int getSize();
    
//    int _ledCount;
    ofPolyline line;
    vector<glm::vec2> pos;
    ofXml xml;

//    std::vector<ofMesh> strokes;

    //    float step;
    //    vector<ofPolyline> outlines;
    
    //    typedef ofPath ofTTFCharacter;
    
private:
    
};
#endif /* ofxShapeFromXML_hpp */
