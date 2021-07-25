//
//	ofxSACN_led_mapper.h
//
//  Modified by bluejaylouche on 19/7/21.
//
//	Modified by Eduard Frigola on 24/05/21 https://github.com/PlaymodesStudio/ofxE131Client
//
//	ORIGINAL: https://github.com/sosolimited/Cinder-DMX/blob/master/src/sACN/E131Client.h
//  E131Client.h
//
//  Created by Alex Olivier on 04/05/16.
//
//	Some code from: https://github.com/tobiasebsen/ofxArtNode/blob/master/src/ofxArtNode.h

#pragma once

#include "ofMain.h"
#include "ofxUDPManager.h"
#include "ofxCustomPixelObject.hpp"

class ofxSACN_led_mapper {
public:
	
	void setup(string host = "2.255.255.255", string mask = "255.0.0.0", string destIp = "239.255.0.1");
	
	void setChannel(int channel, u_char value, int universe = 1);
	void setChannels(int startChannel, u_char* values, size_t size, int universe = 1);
	
	void update();
	void enableAutoSending(bool iEnable); // Whether or not data is automatically
	// sent each update frame
	void setFramerate(float iFPS); // Data sending framerate, by default this is 30 fps
	
	void setUseFramerate(bool iEnable) { useFramerate = iEnable; }
	
	void setPriority(int priority);
	void setCid(const std::vector<char> cid);
	void setSourceName(std::string name);
	bool isConnected() { return _isConnected; }
    void exit();
    
    
    //--------------------------------------------------------------
    /// \brief Update
    //--------------------------------------------------------------
    void stageUpdate();
    
    //--------------------------------------------------------------
    /// \brief Draw
    //--------------------------------------------------------------
    void draw();
    
    //--------------------------------------------------------------
    /// \brief Setup the Pixel Stage
    /**
     @param width : width of the stage
     @param height : height of the stage
     */
    //--------------------------------------------------------------
    void setupStage(int width, int height);
    
    // New Method of Accessing screen pixels
    //--------------------------------------------------------------
    /// \brief Open the Pixel Stage Buffer
    //--------------------------------------------------------------
    void beginStage();
    
    //--------------------------------------------------------------
    /// \brief Close the Pixel Stage Buffer
    //--------------------------------------------------------------
    void endStage();
    
    //--------------------------------------------------------------
    /// \brief Draw Stage
    /**
     @param bool drawGrid : whether to draw the grid or not
     @param int gridSpace : size of the grid
     */
    //--------------------------------------------------------------
    void drawStage(bool drawGrid,int gridSpace = 50);
    
    //--------------------------------------------------------------
    // \brief Get the Stage Width
    //--------------------------------------------------------------
    int getStageWidth();
    
    //--------------------------------------------------------------
    // \brief Get the Stage Height
    //--------------------------------------------------------------
    int getStageHeight();
    
    //--------------------------------------------------------------
    // \brief Get the Stage Center X
    //--------------------------------------------------------------
    int getStageCenterX();
    
    //--------------------------------------------------------------
    // \brief Get the Stage Center Y
    //--------------------------------------------------------------
    int getStageCenterY();
    
    //--------------------------------------------------------------
    // \brief Get the Stage Center
    //--------------------------------------------------------------
    glm::vec2 getStageCenter();
    //--------------------------------------------------------------
    // \brief Get a raw representation of the stage pixels
    //--------------------------------------------------------------
    ofPixels getStagePixels();
    //--------------------------------------------------------------
//    ofColor getStageColor(vector <ofColor> &colorData);
    
    //--------------------------------------------------------------
    // \brief Get the Stage pixels from specific points
    /**
     @param vector <ofPoint> pixels : position of pixels on the stage
     @param vector <ofColor> colorData : color data from the specific points on the stage
     */
    //--------------------------------------------------------------
    vector<ofColor> getStagePixels(vector<glm::vec2> pixels,vector <ofColor> &colorData);
    
    vector <string> error;
	
private:
	void connectUDP(char* destIp);
	void sendDMX();
	void setUniverse(int universe);
	void setLengthFlags();
	
	bool shouldSendData(float iTimeSinceDataSent);
	
	// Data structure to store payload and sequenece num for each universe
	struct UniverseData {
		char universeSequenceNum;
		std::array<char, 512> payload;
	};
	
	// BIG ENDIAN
	std::array<char, 638> sac_packet = {{
		// ROOT LAYER (RLP)
		0x00, 0x10,                   // Define RLP Preamble Size
		0x00, 0x00,                   // RLP Post amble size
		0x41, 0x53, 0x43, 0x2d, 0x45, // E131 Packet identifier
		0x31, 0x2e, 0x31, 0x37, 0x00, 0x00, 0x00, 0x00,
		0x00, // FILL OUT, low 12 bits = PDU length
		// High 4 bits = 0x7
		0x00, 0x00, 0x00, 0x04, // Identifies RLP Data as 1.31 Protocol PDU
		0x00, 0x00, 0x00, 0x00, // CID, Sender's unique CID
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		
		// FRAMING LAYER
		0x00, 0x00,             // Low 12 bits = PDU length, High 4 bits=07
		0x00, 0x00, 0x00, 0x02, // Identifies 1.31 data as DMP protocol PDU
		
		0x00, 0x00, 0x00, 0x00, // User assigned name of source
		0x00, 0x00, 0x00, 0x00, // UTF-8 string, null-terminated
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		
		0x64,       // DATA PRIORITY, 0-200, default of 100 (0x64)
		0x00, 0x00, // RESERVED, transmitters send 0, receivers ignore
		0x00,       // Sequence number to detect dupliate or out of order packets
		0x00, // Options flag, bit 7 = preview data, bit 6 = stream terminated
		0x00, 0x00, // UNIVERSE number
		
		// DMP Layer
		0x00, 0x00, // Protocol flags + length, Low 12 bits = PDU length
		// High 4 bits = 0x7
		0x02,                    // Identifies DMP set property message PDU
		static_cast<char>(0xA1), // Identifies format of address and data
		0x00, 0x00, // Indicates DMX_start.  Code is at DMP address 0.
		0x00, 0x01, // Indicates each property is 1 octet / byte
		0x02, 0x01, // Indicates 1+the number of slots in packet
		0x00,       // DMX start code (0 is standard)
		char(512)   // DMX payload (all 512 channels)
	}};
	
	int packet_length = 638; // Length when all 512 DMX channels are sent
	int destPort = 5568;     // Default port for sACN protocol!
	int priority = 100;
	
	// Framerate-lock sending or send as fast as we can
	bool useFramerate = false;
	bool autoSendingEnabled = true;
	
	float dataSendInterval = 0;
	float lastDataSendTime = 0;
	float framerate = 30.0f;
	
	bool _isConnected = false;
	
	float timeSinceDataSent = 0;
	clock_t timer;
	
	float elapsedTime = 0;
	bool udpSetup = false;
	
	bool loggedException = false;
	
	std::string ipAddress;
	ofxUDPManager udp;
    char* clientIp;
	
	std::map<int, UniverseData> universePackets;
    
    ofTrueTypeFont labels;
    int numberOfFadecandys;
    void connect();
    void disconnect();
    ofFbo screenCapture;
    int _stageWidth;
    int _stageHeight;
    unsigned char * screenPixels;
    
    float startTime;

};
