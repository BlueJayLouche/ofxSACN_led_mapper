#include "ofxSACN_led_mapper.h"

#ifdef WIN32
#include <iphlpapi.h>
#include <codecvt>
#else
#include <ifaddrs.h>
#endif

void ofxSACN_led_mapper::setup(string host, string mask, string destIp)
{
	ipAddress = host;
    // Universally Unique Identifier generated at:  https://www.uuidgenerator.net/
    const vector<char> cid = { char(0x74), char(0x68), char(0x69), char(0x72), char(0x64), char(0x41), char(0x78), char(0x69), char(0x73), char(0x53), char(0x61), char(0x6c), char(0x6f), char(0x6e), char(0x35), char(0x35) };

    setSourceName("Sosolimited device");
    setCid(cid);
    setLengthFlags();
    setPriority(200); // Default top priority
    setUniverse(1);
    
    // If this an example it will use this resource
    labels.load( "../../../resources/verdana.ttf", 13);
    
    // If not it will searching the data directory for the resource5
    if (!labels.isLoaded()) {
        labels.load("fonts/verdana.ttf", 13);
    }

    // Set default data send interval to be equivalent to 30 Hz
    if (framerate > 0) {
        dataSendInterval = 1.0f / framerate;
    }

    // Create a timer for timing cycling and data sending
    timer = clock();
	
    // Connect to UDP endpoint
    clientIp = &destIp[0];
    connectUDP(clientIp);
}

void ofxSACN_led_mapper::connectUDP(char* destIp)
{
    if (!udpSetup) {
		udp.Create();
		udp.SetEnableBroadcast(false);
		udp.SetReuseAddress(true);
		udp.SetNonBlocking(true);
		udp.SetSendBufferSize(4096);
		udp.SetTimeoutSend(1);
		udp.ConnectMcast(destIp, destPort);
    }
}

void ofxSACN_led_mapper::setChannel(int channel, u_char value, int universe)
{
	setChannels(channel, &value, 1, universe);
}


void ofxSACN_led_mapper::setChannels(int startChannel, u_char* values, size_t size, int universe)
{
    // Check if universe is already in our map
    if (universePackets.count(universe) == 0) {
        std::array<char, 512> emptyPayload = { { char(512) } };

        // Create packet
        UniverseData data = UniverseData{
            char(0),
            emptyPayload
        };

        universePackets[universe] = data;
    }

    setUniverse(universe);

    // Expect values between 1 - 512, inclusive
    if ((startChannel > 0) && (startChannel <= 512)) {

        auto &dataPacket = universePackets.at(universe);
		for(int channel = startChannel ; channel < startChannel+size && channel <= 512; channel++){
			dataPacket.payload.at(channel - 1) = (char)*(values+channel-startChannel);
		}
    }
    else {
        ofLog() << "Channel must be between 1 and 512 for DMX protocol. " << startChannel;
        error.push_back("DMX OverLoad!: Channel must be between 1 and 512 for DMX protocol.");
    }
}

void ofxSACN_led_mapper::setUniverse(int universe)
{
    // Set header with appropriate universe, high and low byte
    try {
        sac_packet.at(113) = universe >> 8; // Grabs high byte
        sac_packet.at(114) = universe; // Just the low byte
    }
    catch (std::exception &e) {
        cout << "set universe" << endl;
        error.push_back("set universe");

    }
}

void ofxSACN_led_mapper::setPriority(int priority)
{
    if ((priority >= 0) && (priority <= 200)) {
        sac_packet.at(108) = char(priority);
    }
    else {
		ofLog() << "Priority must be between 0-200";
        error.push_back("Priority must be between 0-200");

    }
}

// This should remain the same per physical device
void ofxSACN_led_mapper::setCid(const std::vector<char> cid)
{
    int length = cid.size();

    if (length != 16) {
        ofLog() << "CID must be of length 16!";
        error.push_back("CID must be of length 16!");
        return;
    }

    int start_index = 22;
    for (int i = 0; i < 16; i++) {
        sac_packet.at(start_index + i) = cid[i];
    }
}

// UTF-8 encoded string, null terminated
void ofxSACN_led_mapper::setSourceName(std::string name)
{
    char *cstr = new char[name.length() + 1];
    std::strcpy(cstr, name.c_str());

    int max_length = strlen(cstr);

    // Field is 64 bytes, with a null terminator
    if (max_length > 63) {
        max_length = 63;
    };

    for (int i = 0; i < max_length; i++) {
        sac_packet.at(44 + i) = cstr[i];
    }
    sac_packet.at(107) = '\n';
}

void ofxSACN_led_mapper::setLengthFlags()
{
    // 16-bit field with the PDU (Protocol Data Unit) length
    // encoded in the lower 12 bits
    // and 0x7 encoded in the top 4 bits

    // There are 3 PDUs in each packet (Root layer, Framing Layer, and DMP layer)

    // To calculate PDU length for RLP section, subtract last index in DMP layer
    // (637 for full payload) from the index before RLP length flag (15)

    // Set length for
    short val = 0x026e; // Index 637-15 = 622 (0x026e)
    char lowByte = 0xff & val; // Get the lower byte
    char highByte = (0x7 << 4) | (val >> 8); // bitshift so 0x7 is in the top 4 bits

    // Set length for Root Layer (RLP)

    sac_packet.at(16) = highByte;
    sac_packet.at(17) = lowByte;

    val = 0x0258; // Index 637-37 = 600 (0x0258)
    lowByte = 0xff & val; // Get the lower byte
    highByte = (0x7 << 4) | (val >> 8); // bitshift so 0x7 is in the top 4 bits

    // Set length for Framing Layer

    sac_packet.at(38) = highByte;
    sac_packet.at(39) = lowByte; // different length!

    val = 0x020B; // Index 637-114 = 523 (0x020B)
    lowByte = 0xff & val; // Get the lower byte
    highByte = (0x7 << 4) | (val >> 8); // bitshift so 0x7 is in the top 4 bits

    // Set length for DMP Layer
    sac_packet.at(115) = highByte;
    sac_packet.at(116) = lowByte;
}

// Set our data sending framerate
void ofxSACN_led_mapper::setFramerate(float iFPS)
{
    if (iFPS > 0) {
        dataSendInterval = 1.0 / iFPS;
    }
    framerate = iFPS;
}

// Check if we should send data, according to our
// data framerate
bool ofxSACN_led_mapper::shouldSendData(float iTime)
{
    if (!useFramerate) {
        return true;
    }
    else {
        float diff = iTime - lastDataSendTime;

        if ((diff >= dataSendInterval) && (framerate > 0)) {
            return true;
        }
    }
    return false;
}

void ofxSACN_led_mapper::sendDMX()
{
    // Send for all universes
    for (auto &pair : universePackets) {
        auto universe = pair.first;
        auto &dataPacket = universePackets.at(universe);
        auto payload = dataPacket.payload;

        setUniverse(universe);

        // TODO: Probably a better way to do this.
        for (int i = 0; i < payload.size(); i++) {
            sac_packet.at(126 + i) = payload.at(i);
        }

        // Handle exceptions
        try {
            sac_packet.at(111) = dataPacket.universeSequenceNum;
			int xxx = universe >> 8 & 0x00ff;
			int yyy = universe & 0x00ff;
			//udp.ConnectMcast((char*)("239.255." + ofToString(xxx) + "." + ofToString(yyy)).c_str(), destPort);
			udp.SendAll(sac_packet.data(), packet_length);
			//udp.Close();
//            _socket->send(asio::buffer(sac_packet, packet_length));
            dataPacket.universeSequenceNum = dataPacket.universeSequenceNum + 1;
        }
        catch (std::exception &e) {
            if (!loggedException) {
                ofLog() << "Could not send sACN data - are you plugged into the device?";
                error.push_back("Could not send sACN data - are you plugged into the device?");
                loggedException = true;
            }
        }
    }
    // Increment current universe counter.
}

// Update all strands, send data if it's time
// Update any test cycles
void ofxSACN_led_mapper::update()
{
    // Use system time for timing purposes
    clock_t thisTime = clock();
    float timeDiff = (thisTime - timer) / 100000.0f;

    elapsedTime += timeDiff;

    float iTime = elapsedTime;

    // If we've enabled auto data sending, send data if
    // enough time has passed according to our data send interval
    if (autoSendingEnabled && shouldSendData(iTime)) {
        lastDataSendTime = iTime;
        sendDMX();
    }

    timer = clock();
}
//--------------------------------------------------------------
void ofxSACN_led_mapper::setupStage(int width,int height)
{
    // If this an example it will use this resource
//    labels.load( "../../../resources/Verdana.ttf", 13);
    
    // If not it will searching the data directory for the resource5
//    if (!labels.isLoaded())
//    {
//        labels.load("Verdana.ttf", 13);
//    }
    
//    connectionAttempts = 0;
//    tryReconnecting = false;
//    startTime = ofGetElapsedTimeMillis();  // get the start time
//    endTime = 5000;// in
    
    _stageWidth = width;
    _stageHeight = height;
    
    screenPixels = new unsigned char [_stageWidth*_stageHeight*4];
    screenCapture.allocate(_stageWidth, _stageHeight,GL_RGBA);
    screenCapture.begin();
    ofClear(0);
    screenCapture.end();
}
//--------------------------------------------------------------
void ofxSACN_led_mapper::beginStage()
{
    screenCapture.begin();
    ofPushStyle();
    ofFill();
    ofSetColor(0, 0, 0);
    ofDrawRectangle(0, 0, _stageWidth, _stageHeight);
    ofPopStyle();
}
//--------------------------------------------------------------
void ofxSACN_led_mapper::endStage()
{
    glReadPixels(0, 0,screenCapture.getWidth(),screenCapture.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, screenPixels);
    screenCapture.end();
}
//--------------------------------------------------------------
int ofxSACN_led_mapper::getStageWidth()
{
    return _stageWidth;
}
//--------------------------------------------------------------
int ofxSACN_led_mapper::getStageHeight()
{
    return _stageHeight;
}
//--------------------------------------------------------------
int ofxSACN_led_mapper::getStageCenterX()
{
    return _stageWidth/2;
}
//--------------------------------------------------------------
int ofxSACN_led_mapper::getStageCenterY()
{
    return _stageHeight/2;
}
//--------------------------------------------------------------
glm::vec2 ofxSACN_led_mapper::getStageCenter()
{
    return glm::vec2(_stageWidth/2,_stageHeight/2);
}
//--------------------------------------------------------------
ofPixels ofxSACN_led_mapper::getStagePixels()
{
    ofPixels tempPixs;
    tempPixs.setFromPixels(screenPixels, _stageWidth, _stageHeight, OF_IMAGE_COLOR_ALPHA);
    return tempPixs;
}
//--------------------------------------------------------------
// NOT WORKING
//--------------------------------------------------------------
//ofColor ofxE131Client::getStageColor(vector <ofColor> &colorData)
//{
//    ofPixels tempPixs;
//    tempPixs.setFromPixels(screenPixels, _stageWidth, _stageHeight, OF_IMAGE_COLOR_ALPHA);
//    unsigned char* colors = tempPixs.getData();
////    int numChannels = original.getPixels().getNumChannels();
////    int pixelIndex = pixels.pos();
//    ofColor color;
//    for (int i = 0; i < 3; i++) {
//        color= ofColor(
//                        colors[i+0], // R
//                        colors[i+1], // G
//                        colors[i+2]  // B
//                        );
//    }
//
//    return color;
//}
//--------------------------------------------------------------
vector<ofColor> ofxSACN_led_mapper::getStagePixels(vector<glm::vec2> pixels,vector <ofColor> &colorData)
{
    ofPixels tempPixs;
    tempPixs.setFromPixels(screenPixels, _stageWidth, _stageHeight, OF_IMAGE_COLOR_ALPHA);
    
    for (int i = 0; i < pixels.size(); i++)
    {
        if (pixels[i].x < 0 || pixels[i].x > getStageWidth() || pixels[i].y < 0 || pixels[i].y > getStageHeight())
        {
            error.push_back("Pixels are outside the Stage Area");
            return;
        }
        else
        {
            colorData.push_back(tempPixs.getColor(pixels[i].x, pixels[i].y));
        }
    }
    return colorData;
}
//--------------------------------------------------------------
void ofxSACN_led_mapper::drawStage(bool drawGrid,int gridSpace)
{
    ofPushMatrix();
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::white);
    ofDrawRectangle(0, 0, _stageWidth, _stageHeight);
    ofFill();
    screenCapture.draw(0,0);
    ofSetColor(ofColor::white);
    
    stringstream ss;
//    ss << "Input Stage" << endl;
    
//    if (bSetupWithFadecandy)
//    {
//        string st = (isConnected()) ? "Fade Candy Connected " : "Fade Candy Not Connected";
//        ss << st << endl;
//    }
    
    
    for (int i = 0; i < error.size(); i++) ss << error[i] << endl;
    
    labels.drawString(ss.str(),10, _stageHeight+labels.getLineHeight());
    ofPopStyle();
    ofPopMatrix();
    
    if (drawGrid)
    {
        for (int y = 0; y < _stageHeight; y+=gridSpace)
        {
            for (int x = 0; x < _stageWidth; x+=gridSpace)
            {
                ofSetColor(50);
                ofDrawLine(x,0,x,_stageHeight);
                ofDrawLine(0,y,_stageWidth,y);
            }
        }
    }
}
//--------------------------------------------------------------
void ofxSACN_led_mapper::stageUpdate()
{
    error.clear();
    timer = ofGetElapsedTimeMillis() - startTime;
}
void ofxSACN_led_mapper::exit(){
    udp.Close();
}

