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

    ofSetFrameRate(30);
    
    fingers.load("movies/fingers.mp4");
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
    gui.add(Colour.setup("Colour", ofColor(255, 2, 1), ofColor(0, 0, 0), ofColor(255, 255, 255)));//ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));


    int numRings = 3;
    circle.clear();
    circle.resize(numRings);

    ring.clear();
    ring.resize(numRings);

    butt.clear();
    butt.resize(numRings);

    circle[0].setup(61, 70, -ofGetWidth()/12*2, -ofGetHeight()/12*2-30, 0);
    circle[1].setup(130, 100, -ofGetWidth()/12*2, -ofGetHeight()/12*2-70, 0);
    circle[2].setup(111, 130, -ofGetWidth()/12*2, -ofGetHeight()/12*2-30, 0);
    glm::vec2 startingPos = glm::vec2(ofGetWidth()/8*3, ofGetHeight()/2);
    
    hand.setup('z', 300, 5, startingPos);
    
//    vector<vector<glm::vec2>> positions;
//    vector<glm::vec2> sizes;
    
    //positions[0].push_back(glm::vec2(circle[0].getPosition()));
    
    
    for ( int i = 0; i < numRings; i++)    {
        //ring[i].push_back(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(((node.getStageWidth()/8)/2)+(i*(node.getStageWidth()/8)), node.getStageCenterY()-(30*5), 60, 5));

        ring[i].insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, circle[i].getPosition(), circle[i].circleSize(), 5)));
    }
    fonty.insert(std::pair<int, ofxPixelObject*>(0,new ofxCustomPixelObject(0, 0, hand.getPosition(), hand.circleSize(), 5)));


    //FFT
    //fft.setup(16384);
    bufferSize = 2048;
//    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);
    //fft.setUseNormalization(true);
    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());
    ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);
    
    fftSmoothed.clear();
    fftSmoothed.resize(fft->getBinSize());
    
    ofDisableArbTex();
    
    mTexture.allocate(fft->getBinSize(),2,GL_RED, false);
    fbo.allocate(sw/4, sh/4, GL_RGBA32F);
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
//    mTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    shadertoy.load("shaders/Shader.glsl");
    shadertoy.setTexture(0, mTexture);
    shadertoy.setDimensions(sw/4, sh/4);
    shadertoy.setAdvanceTime(true);
    
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
            fingers.load("movies/fingers.mp4");
            fingers.setLoopState(OF_LOOP_NORMAL);
            fingers.play();
        }
        ofColor c;
        ofSetColor(c);
        fingers.draw(0, 0, sw, sh);
    }else{
        fingers.close();
    }
    if(shader){
//        ofSetColor(255);
        fbo.draw(0, 0, sw, sh);
        //shadertoy.draw(0, 0, sw, sh);
    }else{
        shadertoy.end();
    }
    ofDisableBlendMode();
    ofSetColor(245, 58, 135);
    ofPopStyle();
    node.endStage();
    
    butt.clear();
    int chCounter;
    chCounter = 0;
    
    for (int i = 0; i < ring.size(); i++)
    {
        vector<ofColor> col = node.getStagePixels(ring[i].at(0)->getPixelCoordinates(), ring[i].at(0)->colors);

        butt[i].clear();
//        butt[i].resize(circle[i].circleSize());

        for (int j = 0; j < circle[i].circleSize(); j++)
        {
            butt[i].push_back(col[j][0]);
            butt[i].push_back(col[j][1]);
            butt[i].push_back(col[j][2]);
//            butt[i].push_back(0);

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
//    fft.update();
//
//    vector<float>& buffer = *fft.getBins();
//
//    unsigned char signal[1024];
//    for (int i = 0; i < 512; i++) {
//        signal[i] = (unsigned char) (buffer.at(i) * 80.0); // FFT
//    }
//    for (int i = 0; i < 512; i++) {
//        float audioSig = fft.getAudio()[i];
//        audioSig = (audioSig * 0.5 + 0.5) * 254.0;
//        signal[512+i] = (unsigned char) audioSig;
//    }
//
    
    vector<float> dB;
    dB.resize(middleBins.size());
    vector<float> clamped;
    clamped.resize(middleBins.size());
    float dB_min = -80;
    float dB_max = -30;
    


//    // Time to smoooooth our the FFT singal ;)
    for (int i = 0;i < middleBins.size(); i++){
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.96f;

        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < middleBins[i]) fftSmoothed[i] = middleBins[i];
        
        dB[i] = 20 * log10(fftSmoothed[i]);
        clamped[i] = ofMap(dB[i], dB_min, dB_max, 0.f, 0.5f);//(1. / (dB_max - dB_min) * (dB[i] - dB_min), 0, 1.)
        
        
    }
    //std::cout << dB[100];
    
    
    //        fftSmoothed[512+255] = (fftSmoothed[512+254] + fftSmoothed[512+256]) >> 1.f;
    // get vector of the right size
//    std::vector<unsigned char> data(sizeof(middleBins));
//    // copy the bytes
//    std::memcpy(data.data(), &middleBins, sizeof(middleBins));
//
//    std::cout << "bin values" << data[100];
////    return data;
////    std::transform(data.begin(), data.end(), data.begin(),
////                   std::bind(std::ratio_divide<T>(), std::placeholders::_1, 3));
//
////    data.std::transform
//
////    std::vector<double> myarray;
////    int myconstant{1000};
////    std::transform(data.begin(), data.end(), data.begin(), std::divides<double>(myconstant);
//    std::vector<unsigned char> scaled(sizeof(middleBins));
//
//    for (int i = 0; i < data.size(); i ++)  {
//        scaled[i] = data[i]/1000000;
////        scaled.push_back(data.data().at(i)/1000)
//    }
    
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
    for (int i = 0; i < fonty.size(); i++)  {
        fonty[i]->drawGrabRegion(hide);
//    }
//    for (int i = 0; i < fonty.size(); i++)  {
        fonty.at(0)->draw(node.getStageWidth()+(drawCnt * 10), 10);
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
}

