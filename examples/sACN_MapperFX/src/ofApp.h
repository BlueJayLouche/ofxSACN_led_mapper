#pragma once

#include "ofMain.h"
#include "ofxE131Client.h"
#include "circleClass.hpp"
#include "fontClass.hpp"
#include "ofxGui.h"
//#include "ofxShadertoy.h"
#include "ofxEasyFft.h"
#include "ofMain.h"
#include "ofxGrayScott.h"
#include "ofxNormals.h"
//#include "ofxFire"


class ofApp : public ofBaseApp{
private:
//    ofxShadertoy shadertoy;
//    ofTexture mTexture;
    //ofTexture picImage;
public:
    ofxCustomPixelObject * extracted();
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    ofxE131Client node;
    ofEventListeners listeners;
    vector<vector<u_char>> butt;
    
    //ofImage original;
    ofVideoPlayer fingers;
    int sw = ofGetWidth()/3*2;
    int sh = ofGetHeight()/3*2;
    
    vector<circleClass> circle;
    deque<map<int, ofxPixelObject*>> ring;
    map<int, ofxPixelObject*> fonty;

    fontClass hand;
    
    bool hide;
    
    ofxPanel gui;
    
    ofxIntSlider intSlider;
    ofxIntSlider fontSize;
    ofxFloatSlider floatSlider;
    
    ofxToggle toggle;
    ofxToggle shader;
    ofxLabel label;
    
    ofxVec2Slider vec2Slider;
    ofxVec2Slider fontPos;

    ofxColorSlider Colour;
    ofFbo fbo;
    
    ofxEasyFft fft;
    unsigned char fftSmoothed[512];

    ofxGrayScott gray;
//    ofxNormals normals;

};
