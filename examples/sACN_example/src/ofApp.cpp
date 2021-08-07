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
    
    gui.setup();
    gui.setPosition(ofGetWidth()-gui.getWidth()-5, 5);
    gui.setFillColor(ofColor::lightPink);
    gui.setHeaderBackgroundColor(ofColor::orangeRed);
    gui.add(label.setup("Filter", "controls"));
    
    gui.add(intSlider.setup("Resolution", 12, 3, 64));
    gui.add(floatSlider.setup("Size", 105., 0., 300.));
    
    gui.add(toggle.setup("Show Video", false));
    gui.add(shader.setup("Show Shader", false));
    
    //    gui.add(intSlider.setup("Vector Size", 70, 10, 300));
    //    gui.add(fontPos.setup("Vector Position", ofVec2f(ofGetWidth()/3, ofGetHeight()/3), ofVec2f(0, 0), ofVec2f(sw, sh)));
    
    gui.add(vec2Slider.setup("Position", ofVec2f(ofGetWidth()/3, ofGetHeight()/3), ofVec2f(0, 0), ofVec2f(sw, sh)));
    gui.add(Colour.setup("Colour", ofColor(255, 2, 1), ofColor(0, 0, 0), ofColor(255, 255, 255)));//ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));
    
    
    int numRings = 2;
    circle.clear();
    circle.resize(numRings);
    
    ring.clear();
    ring.resize(numRings);
    
    circle[0].setup(130, 100, -ofGetWidth()/12*2, -ofGetHeight()/12*2-30, 0);
    circle[1].setup(121, 100, -ofGetWidth()/12*2, -ofGetHeight()/12*2-70, 0);
    //    circle[2].setup(111, 130, -ofGetWidth()/12*2, -ofGetHeight()/12*2-30, 0);
    glm::vec2 startingPos = glm::vec2(ofGetWidth()/8*3, ofGetHeight()/2);
    
    hand.setup('z', 300, 5, startingPos);
    svgShape.setup("164.svg", 1., 10, startingPos);
    xmlShape.setup("points.xml", 1., 300, glm::vec2(170, 140));
    
    //    vector<vector<glm::vec2>> positions;
    //    vector<glm::vec2> sizes;
    
    //positions[0].push_back(glm::vec2(circle[0].getPosition()));
    
    
    for ( int i = 0; i < numRings; i++)    {
        //        ring[i].push_back(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(((node.getStageWidth()/8)/2)+(i*(node.getStageWidth()/8)), node.getStageCenterY()-(30*5), 60, 5));
        
        ring[i].insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, circle[i].getPosition(), circle[i].getSize(), 5)));
    }
    //    fonty.insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, hand.getPosition(), hand.getSize(), 5)));
    svg.insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, svgShape.getPosition(), svgShape.getSize(), 5)));
    xml.insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, xmlShape.getPosition(), 170, 5)));
    
    
    //    //FFT
    bufferSize = 2048;
    //    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);
    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());
    
    
    ofSoundStreamSettings settings;
    //    settings.setApi(ofSoundDevice::Api::OSX_CORE);
    //    auto devices = soundStream.getDeviceList();
    std::vector<ofSoundDevice> devices = soundStream.getDeviceList();
    settings.setInDevice(devices[1]);
    settings.setOutDevice(devices[0]);
    settings.setInListener(this);
    settings.bufferSize = bufferSize;
    settings.sampleRate = 44100;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 1;
    //    ofSoundStreamSetup(settings);
    soundStream.setup(settings);
    soundStream.printDeviceList();
    
    for (int i = 0; i < devices.size(); i++) {
        std::cout << "devices[" << ofToString(i) << "] = " << ofToString(devices[i]) << std::endl;
    }
    
    //    ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);
    
    fftSmoothed.clear();
    fftSmoothed.resize(fft->getBinSize());
    
    ofDisableArbTex();
    
    mTexture.allocate(512,2,GL_RED, false);
    fbo.allocate(sw/4, sh/4, GL_RGBA32F);
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    mTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    shadertoy.load("shaders/Shader.glsl");
    shadertoy.setTexture(0, mTexture);
    shadertoy.setDimensions(sw/4, sh/4);
    shadertoy.setAdvanceTime(true);
    
    fingers.load("movies/fingers.mov");
    fingers.setLoopState(OF_LOOP_NORMAL);
    fingers.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    node.stageUpdate();
    
    fingers.update();
    //    gray.update();
    
    node.beginStage();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(intSlider);
    ofSetColor(Colour);
    ofDrawCircle(vec2Slider->x, vec2Slider->y, floatSlider);
    if(toggle){
        if (!fingers.isInitialized())   {
            fingers.load("movies/fingers.mov");
            fingers.setLoopState(OF_LOOP_NORMAL);
            fingers.play();
        }
        ofSetColor(255);
        fingers.draw(0, 0, sw, sh);
    }else{
        fingers.close();
    }
    if(shader){
        ofSetColor(255);
        fbo.draw(0, 0, sw, sh);
        //shadertoy.draw(0, 0, sw, sh);
    }else{
        //        shadertoy.end();
    }
    ofDisableBlendMode();
    ofSetColor(245, 58, 135);
    ofPopStyle();
    node.endStage();
    
    vector<vector<u_char>> values;
    vector<vector<u_char>> values2;
    values.resize(170*3);
    values2.resize(170*3);
    values.clear();
    values2.clear();
    
    int chCounter;
    chCounter = 0;
    
    for (int i = 0; i < ring.size(); i++)
    {
        vector<ofColor> col = node.getStagePixels(ring[i].at(0)->getPixelCoordinates(), ring[i].at(0)->colors);
        
        //        values[i].resize(circle[i].circleSize());
        
        for (int j = 0; j < circle[i].getSize(); j++)
        {
            //            values[i].push_back(col[j][0]);
            //            values[i].push_back(col[j][1]);
            //            values[i].push_back(col[j][2]);
        }
        
        //        node.setChannels(1 , values[i].data(), values[i].size(), 2);
        //        chCounter += values[i].size();
        //        std::cout << values[0][30];
        
        //        values[i].clear();
        
    }
    //    node.update();
    
    //    values.clear();
    
    //    for (int i = 0; i < fonty.size(); i++)
    //    {
    //        vector<ofColor> col = node.getStagePixels(fonty.at(0)->getPixelCoordinates(), fonty.at(0)->colors);
    //    }
    for (int i = 0; i < svg.size(); i++)
    {
        vector<ofColor> col;
        col.clear();
        col = node.getStagePixels(svg.at(0)->getPixelCoordinates(), svg.at(0)->colors);
    }
    for (int i = 0; i < xml.size(); i++)
    {
        vector<ofColor> col;
        col.resize(xmlShape.getSize());
        col.clear();
        col = node.getStagePixels(xml.at(0)->getPixelCoordinates(), xml.at(0)->colors);
        //        values.clear();
        for (int j = 0; j < 170; j++)
        {
            values[i].push_back(col[j][0]);
            values[i].push_back(col[j][1]);
            values[i].push_back(col[j][2]);
            values2[i].push_back(col[j+170][0]);
            values2[i].push_back(col[j+170][1]);
            values2[i].push_back(col[j+170][2]);
        }
        
        node.setChannels(1 , values[i].data(), values[i].size(), 1);
        node.setChannels(1 , values2[i].data(), values2[i].size(), 2);
        
        chCounter += values2[i].size();
    }
    node.update();
    
    vector<float> dB;
    dB.resize(middleBins.size());
    vector<float> clamped;
    clamped.resize(middleBins.size());
    float dB_min = -60;
    float dB_max = -20;
    
    ////    // Time to smoooooth our the FFT singal ;)
    for (int i = 0;i < middleBins.size(); i++){
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.85f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < middleBins[i]) fftSmoothed[i] = middleBins[i];
        
        dB[i] = 20 * log10(fftSmoothed[i]);
        clamped[i] = ofMap(dB[i], dB_min, dB_max, -0.f, 0.5f);//(1. / (dB_max - dB_min) * (dB[i] - dB_min), 0, 1.)
    }
    
    mTexture.loadData(clamped.data(), 512, 2, GL_RED);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    node.drawStage(hide);
    
    int drawCnt = 0;
    for (int i = 0; i < ring.size(); i++)   {
        ring[i][0]->drawGrabRegion(hide);
        //    }
        //    for (int i = 0; i < ring.size(); i++)   {
        ring[i].at(0)->draw(node.getStageWidth()+(i*10), 10);
        drawCnt++;
    }
    //    for (int i = 0; i < fonty.size(); i++)  {
    //        fonty[i]->drawGrabRegion(hide);
    //        //    }
    //        //    for (int i = 0; i < fonty.size(); i++)  {
    //        fonty.at(0)->draw(node.getStageWidth()+(drawCnt * 10), 10);
    //        drawCnt++;
    //    }
    for (int i = 0; i < svg.size(); i++)  {
        svg[i]->drawGrabRegion(hide);
        //    }
        //    for (int i = 0; i < fonty.size(); i++)  {
        svg.at(0)->draw(node.getStageWidth()+(drawCnt * 10), 10);
        drawCnt++;
    }
    for (int i = 0; i < xml.size(); i++)  {
        xml[i]->drawGrabRegion(hide);
        //    }
        //    for (int i = 0; i < fonty.size(); i++)  {
        xml.at(0)->draw(node.getStageWidth()+(drawCnt * 10), 10);
        drawCnt++;
    }
    gui.draw();
    
    fbo.begin();
    shadertoy.draw(0, 0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'q':
        shadertoy.load("shaders/fftView.glsl");
        //            shadertoy.setTexture(0, mTexture);
        break;
    }
    switch (key) {
        case 'w':
        shadertoy.load("shaders/Shader.glsl");
        //            shadertoy.setTexture(0, mTexture);
        break;
    }
    switch (key) {
        case 'e':
        shadertoy.load("shaders/oil.glsl");
        //            shadertoy.setTexture(0, mTexture);
        break;
    }
    switch (key) {
        case 'r':
        shadertoy.load("shaders/blobs.glsl");
        //            shadertoy.setTexture(0, mTexture);
        break;
    }
    switch (key) {
        case 't':
        shadertoy.load("shaders/fbm.glsl");
        //            shadertoy.setTexture(0, mTexture);
        break;
    }
    switch (key) {
        case 'y':
        shadertoy.load("shaders/clouds.glsl");
        //            shadertoy.setTexture(0, mTexture);
        break;
    }
    switch (key) {
        case 'u':
        shadertoy.load("shaders/quantum.glsl");
        //            shadertoy.setTexture(0, mTexture);
        break;
    }
    switch (key) {
        case 'i':
        shadertoy.load("shaders/cine.glsl");
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
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
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
void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
    float maxValue = 0;
    for(int i = 0; i < bufferSize; i++) {
        if(abs(input[i]) > maxValue) {
            maxValue = abs(input[i]);
        }
    }
    for(int i = 0; i < bufferSize; i++) {
        input[i] /= maxValue;
    }
    
    fft->setSignal(input);
    
    float* curFft = fft->getAmplitude();
    memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
    
    maxValue = 0;
    for(int i = 0; i < fft->getBinSize(); i++) {
        if(abs(audioBins[i]) > maxValue) {
            maxValue = abs(audioBins[i]);
        }
    }
    for(int i = 0; i < fft->getBinSize(); i++) {
        audioBins[i] /= maxValue;
    }
    
    soundMutex.lock();
    middleBins = audioBins;
    soundMutex.unlock();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    node.exit();
    ofSoundStreamClose();
}

