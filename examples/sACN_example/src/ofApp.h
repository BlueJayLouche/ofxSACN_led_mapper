#pragma once

#include "ofMain.h"
#include "ofxSACN_led_mapper.h"
#include "ofxCustomPixelRing.hpp"
#include "ofxCustomFontShape.hpp"
#include "ofxCustomSVGShape.hpp"
#include "ofxShapeFromXML.hpp"
#include "ofxGui.h"
#include "ofxShadertoy.h"
#include "ofxFft.h"
//#include "ofxSvg.h"
#include "ofMain.h"



class ofApp : public ofBaseApp{
    private:
    ofxShadertoy shadertoy;
    ofTexture mTexture;
    //    ofTexture picImage;
    public:
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
    void audioReceived(float* input, int bufferSize, int nChannels);
    void exit();
    
    ofxSACN_led_mapper node;
    ofEventListeners listeners;
    
    //ofImage original;
    ofVideoPlayer fingers;
    int sw = ofGetWidth()/3*2;
    int sh = ofGetHeight()/3*2;
    
    vector<ofxCustomPixelRing> circle;
    deque<map<int, ofxPixelObject*>> ring;
    map<int, ofxPixelObject*> fonty;
    map<int, ofxPixelObject*> svg;
    map<int, ofxPixelObject*> xml;
    
    ofxCustomFontShape hand;
    ofxCustomSVGShape svgShape;
    ofxShapeFromXML xmlShape;
    
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
    
    int bufferSize;
    ofxFft* fft;
    ofMutex soundMutex;
    vector<float> drawBins, middleBins, audioBins;
    vector<float> fftSmoothed;
    
    ofSoundStream soundStream;
    
};
