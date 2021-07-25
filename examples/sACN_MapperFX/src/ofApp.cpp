#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    node.setup("192.168.1.55", "255.255.255.0", "192.168.1.200");
    node.setUseFramerate(false);
    node.setSourceName("ofxE131ClientExample");
    node.setupStage(sw, sh);
    hide = false;

    ofSetFrameRate(60);
    
//    ofDisableArbTex();
//
//    mTexture.allocate(512,2,GL_RED, false);
    fbo.allocate(sw/4, sh/4, GL_RGBA);
    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
//    mTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
    
//    shadertoy.load("shaders/Shader.glsl");
//    shadertoy.setTexture(0, mTexture);

    //ofSetFrameRate(60);
//    shadertoy.setAdvanceTime(true);
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    gray.allocate(sw/4, sh/4);
//    normals.allocate(sw, sh);
    
    fingers.load("movies/fingers.mov");
    
    fingers.setLoopState(OF_LOOP_NORMAL);
    fingers.play();
    
    gui.setup();
    gui.setPosition(ofGetWidth()-gui.getWidth()-5, 5);
    gui.setFillColor(ofColor::lightPink);
    gui.setHeaderBackgroundColor(ofColor::orangeRed);
    gui.add(label.setup("Filter", "controls"));

    gui.add(intSlider.setup("Resolution", 12, 3, 64));
    gui.add(floatSlider.setup("Size", 105., 0., 300.));
    
    gui.add(toggle.setup("Show Video", false));
    gui.add(shader.setup("Show Shader", false));
    
    gui.add(intSlider.setup("Vector Size", 70, 10, 300));
    gui.add(fontPos.setup("Vector Position", ofVec2f(ofGetWidth()/3, ofGetHeight()/3), ofVec2f(0, 0), ofVec2f(sw, sh)));

    gui.add(vec2Slider.setup("Position", ofVec2f(ofGetWidth()/3, ofGetHeight()/3), ofVec2f(0, 0), ofVec2f(sw, sh)));
    gui.add(Colour.setup("Colour", ofColor(255, 2, 1), ofColor(0, 0, 0), ofColor(255, 255, 255)));

    int numRings = 2;
    circle.clear();
    circle.resize(numRings);
    
    ring.clear();
    ring.resize(numRings);
    
    butt.clear();
    butt.resize(numRings);
    
    circle[0].setup(61, 70, -ofGetWidth()/12*2, -ofGetHeight()/12*2-30, 0);
    circle[1].setup(121, 100, -ofGetWidth()/12*2, -ofGetHeight()/12*2-70, 0);
    //    circle[2].setup(111, 130, -ofGetWidth()/12*2, -ofGetHeight()/12*2-30, 0);
    glm::vec2 startingPos = glm::vec2(ofGetWidth()/8*3, ofGetHeight()/2);
    
    hand.setup('z', 300, 5, startingPos);
    
    //    vector<vector<glm::vec2>> positions;
    //    vector<glm::vec2> sizes;
    
    //positions[0].push_back(glm::vec2(circle[0].getPosition()));
    
    
    for ( int i = 0; i < numRings; i++)    {
        ring[i].insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, circle[i].getPosition(), circle[i].circleSize(), 5)));
    }
    fonty.insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, hand.getPosition(), hand.circleSize(), 5)));

    //FFT
    fft.setup(16384);
    fft.setUseNormalization(true);
    
    for (int i = 0; i < 512; i++){
        fftSmoothed[i] = 0;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    node.stageUpdate();
    fingers.update();
    gray.update();
    
//    normals << gray;
//    normals.update();
    
    node.beginStage();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(intSlider);
    ofSetColor(Colour);
    ofDrawCircle(vec2Slider->x, vec2Slider->y, floatSlider);
    if(toggle){
        if (!fingers.isInitialized())   {
            fingers.load("movies/fingers.mov");
            fingers.play();
        }
        ofColor c;
        ofSetColor(c);
        fingers.draw(0, 0, sw, sh);
    }else{
        fingers.close();
    }
    if(shader){
        fbo.draw(0, 0, sw, sh);
    }else{
//        shadertoy.end();
    }
    ofDisableBlendMode();
    ofSetColor(245, 58, 135);
    ofPopStyle();
    node.endStage();

     //    butt.clear();
    int chCounter;
    chCounter = 0;
    
    for (int i = 0; i < ring.size(); i++)
    {
        vector<ofColor> col = node.getStagePixels(ring[i].at(0)->getPixelCoordinates(), ring[i].at(0)->colors);
        //
        butt[i].clear();
        //        butt[i].resize(circle[i].circleSize());
        
        for (int j = 0; j < circle[i].circleSize(); j++)
        {
            butt[i].push_back(col[j][0]);
            butt[i].push_back(col[j][1]);
            butt[i].push_back(col[j][2]);
        }
        
        node.setChannels(chCounter+1 , butt[i].data(), butt[i].size(), 1);
        chCounter += butt[i].size();
    }
    node.update();
    
    for (int i = 0; i < fonty.size(); i++)
    {
        vector<ofColor> col = node.getStagePixels(fonty.at(0)->getPixelCoordinates(), fonty.at(0)->colors);
    }
    
    //FFT
    fft.update();

    vector<float>& buffer = fft.getBins();

    unsigned char signal[1024];
    for (int i = 0; i < 512; i++) {
        signal[i] = (unsigned char) (buffer.at(i) * 80.0); // FFT
    }
    for (int i = 0; i < 512; i++) {
        float audioSig = fft.getAudio()[i];
        audioSig = (audioSig * 0.5 + 0.5) * 254.0;
        signal[512+i] = (unsigned char) audioSig;
    }

    // Time to smoooooth our the FFT singal ;)
    for (int i = 0;i < 512; i++){
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.96f;

        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < signal[i]) fftSmoothed[i] = signal[i];
        
    }
//    mTexture.loadData(fftSmoothed, 512, 2, GL_RED);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofBackground(0);
//    gray.draw(sw+20, sh+20);

    node.drawStage(hide);
    
//    for (int j = 0; j < ring.size(); j++)
//        {
        for (int i = 0; i < ring.size(); i++)
        {
            ring[i][0]->drawGrabRegion(hide);
        }
//    }
    for (int i = 0; i < ring.size(); i++)
    {
        ring[i].at(0)->draw(node.getStageWidth()+(i*25), 10);
    }
    for (int i = 0; i < fonty.size(); i++)
    {
        fonty[i]->drawGrabRegion(hide);
    }
    for (int i = 0; i < fonty.size(); i++)
    {
        fonty.at(0)->draw(node.getStageWidth()+(50), 10);
    }
    gui.draw();
    
    fbo.begin();
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    ofSetColor(255);
    gray.draw(0, 0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'q':
//            shadertoy.load("shaders/ShaderTest copy 6.glsl");
//            shadertoy.setTexture(0, mTexture);
            break;
    }
    switch (key) {
        case 'w':
//            shadertoy.load("shaders/Shader.glsl");
//            shadertoy.setTexture(0, mTexture);
            break;
    }
    switch (key) {
        case ' ':
            hide = !hide;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//    node.beginStage();
    gray.setK( ofMap(mouseX,0,sw,0.005,0.095,true) );
    gray.setF( ofMap(mouseY,0,sh,0.01,0.028,true) );
//    node.endStage();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    gray.begin();
    ofSetColor(ofNoise( ofGetElapsedTimef() )*255);
    ofDrawCircle(x/4, y/4, 3);
    gray.end();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::exit()
{
    node.exit();
}

