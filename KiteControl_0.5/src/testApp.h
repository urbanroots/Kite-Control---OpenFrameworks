#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGamepadHandler.h"
#include "ofxSerial.h"
#include "ofxSimpleSerial.h"
#include "ofxUI.h"
#include <sstream>

#define HOST "localhost" // This is host name of my local computer
#define PORT 8000       // This port must match the port number in OSCulator
#define DEVICE 0        // This serial port number must match the port of the XBEE
#define BAUDRATE 115200 // Buadrate must match that of the ARDUINO

class testApp : public ofBaseApp{
	
	
public:
    
    void setup();
    void update();
    void draw();
    
    void axisChanged(ofxGamepadAxisEvent &e);
    void buttonPressed(ofxGamepadButtonEvent &e);
    void buttonReleased(ofxGamepadButtonEvent &e);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);		
    
    // ofxUI stuff // 
    
    ofxUICanvas *gui1;
    //ofxUICanvas *gui2;
    ofxUILabel *serialSent;
    ofxUILabel *serialReceived;
    ofxUILabel *leftMotorPos;
    ofxUILabel *rightMotorPos;
    ofxUIButton *quit;
    ofxUISlider *powerSlider;
    ofxUIButton *rightMotorIn;
    ofxUIButton *rightMotorOut;
    ofxUIButton *leftMotorIn;
    ofxUIButton *leftMotorOut;
    ofxUIButton *bothMotorIn;
    ofxUIButton *bothMotorOut;
    
    
    int red, green, blue;
    int leftMotorInID,leftMotorOutID,rightMotorInID,rightMotorOutID;
    int turnValue;
    
    
    void exit(); 
    void guiEvent(ofxUIEventArgs &e);    
    
    
    
    // Define oF Objects // 
    ofxSimpleSerial serial;    
    
    ofxOscSender wiimote;
    //ofxOscMessage m;
    ofxOscReceiver receiver;
    
    ofxGamepad wii;
    
    float xpos;
    float ypos;
    
    float powerMeter_h;
    float powerMeter_w;
    float powerMeter_y;
    float powerMeter_x;
    
    
    
    ofArduino mega;

    ofLight light;
    ofSoundPlayer limitReached;
    
    ofImage arrowKeys;
    ofImage bg;
    ofImage powerMeter;
    ofImage logo;
    ofImage kite;
    
    ofQuaternion iphoneQuat;
    
    // Define Variables // 
    
    string depower;
    string power;
    string left;
    string right;
    string serialInfo;
    string xval;
    string yval;
    string zval;
    string serialCommand;
    string portName;
    string msg_string;
    
    unsigned char incomingByte;
    
    float arrowx;
    float arrowy;
    float arrow_h;
    float arrow_w;
    float holdPosition_x;
    float holdPower_y;
    float theta;
    float holdAngle;
    int motorSpeed;
    float speedMeter_y;
    float speedMeter_x;
    float speedMeter_h;
    float speedMeter_w;
    float currentPos;
    float lastPos;
    float gui1_w;
    
    float pitch;
    float roll;
    float yaw;
    float currentHeading;
    float lastHeading;
    
    int numBytes;
    int powerLevel;
    int low;
    int med; 
    int max;
    int key; 
    vector<float> posAvg;
    
    int countCycles;
    int maxCycles;
    
    bool pitchFlip;
    bool turn;
    bool printByte;
    bool holdMotorSpeed;
    bool holdPosition;
    bool holdPower;
    bool powerON;
    bool depowerON;
	bool moveright;
	bool moveleft;
    bool speedUp;
    bool slowDown;
    bool Bbutton;
    bool arduinoReady;
    
    
    bool keyDown;
    bool home;
    
    bool serialCheck;
    bool axisChange;
    
    //unsigned char* buf;
    
    void onNewMessage(string & message);
    
    void writeToArduino(string command);
    
    
    string floatToString(float value)
    {
        std::stringstream ss;
        ss << std::setprecision(2);
        ss << value;
        return ss.str();
    }

    float getAvg(vector<float> x);
    
    
    
};

