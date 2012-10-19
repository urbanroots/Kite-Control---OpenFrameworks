#include <iomanip>
#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(255, 255, 255);
    
        
    serialCheck = serial.setup();       // Opens serial port at 115200 baudrate by default- returns false if failed to open serial 
    //serialCheck = serial.setup(4, 9600);  // use when arduino is connected directly to computer
    if(serialCheck) {
        serial.startContinuesRead(false);
        ofAddListener(serial.NEW_MESSAGE,this,&testApp::onNewMessage);
    }
    
    
    //portName = serial.getDeviceList()[0].getDeviceName();
    //serialCheck = mega.connect(portName);
    
    serial.flush();                     // flush serial port buffer
    ofSetFrameRate(30);                 // set Frame Rate
    ofSetVerticalSync(TRUE);            // Enable Verticle sync 
    
    if(serialCheck){
        serial.writeByte('x');
        serial.writeByte('/');
    }
    
    red = 233; blue = 162.73; green = 121.63; 
    
    //serial.close();         // close serial port
    //mega.disconnect();
    
    
    serial.listDevices();
    serialCommand = "n/a";
    incomingByte = 0;
    
    ofBackground(100);
    
    // Initialize OSC connection 
    //wiimote.setup( PORT );
    receiver.setup( PORT );
    
    // Initialize OSC and Serial ports
    //oscIn.setup(PORT);
    //oscOut.setup("10.10.3.218",9000);

    
    // Load images from ../data folder 
    limitReached.loadSound("WiiMouseDisconnected.mp3");
    bg.loadImage("grey.jpg");
    powerMeter.loadImage("powerMeter.png");
    logo.loadImage("kite.gif");
    kite.loadImage("naishKite.png");
    kite.resize(400, 400);

    
    // Initialize GUI stuff // 
    
    gui1_w = 340;
    
    serialSent = new ofxUILabel("n/a", OFX_UI_FONT_MEDIUM);
    serialReceived = new ofxUILabel(ofToString(incomingByte), OFX_UI_FONT_MEDIUM);
    quit = new ofxUIButton(20, ofGetHeight()-52, 32, 32, false, "QUIT");
    quit->setDrawFill(true);
    quit->setColorFill(ofColor(red,0,0));
    powerSlider = new ofxUISlider(300, 20, 0, 96, 96, "POWER");
    powerSlider->setIncrement(8);
   
    gui1 = new ofxUICanvas(0,0,gui1_w,ofGetHeight());		//ofxUICanvas(float x, float y, float width, float height)
    gui1->setPadding(20);
    
    rightMotorIn = new ofxUIButton(20, 20, false, "IN");
    rightMotorIn->setColorFill(ofColor(0,0,blue));
    rightMotorIn->setDrawFill(true);
    rightMotorOut = new ofxUIButton(20, 20, false, "OUT");
    rightMotorOut->setColorFill(ofColor(0,0,blue));
    rightMotorOut->setDrawFill(true);
    leftMotorIn = new ofxUIButton(20, 20, false, "IN");
    leftMotorIn->setColorFill(ofColor(0,0,blue));
    leftMotorOut = new ofxUIButton(20, 20, false, "OUT");
    leftMotorIn->setColorFill(ofColor(0,0,blue));
    bothMotorIn = new ofxUIButton(20, 20, false, "IN");
    bothMotorIn->setColorFill(ofColor(0,0,blue));
    bothMotorOut = new ofxUIButton(20, 20, false, "OUT");
    bothMotorIn->setColorFill(ofColor(0,0,blue));
    
    leftMotorPos = new ofxUILabel("", OFX_UI_FONT_MEDIUM);
    rightMotorPos = new ofxUILabel("", OFX_UI_FONT_MEDIUM);
    

    gui1->addWidgetDown(new ofxUILabel("KITE CONTROL ", OFX_UI_FONT_LARGE)); 
    gui1->addWidgetDown(new ofxUISpacer(300, 2));
    gui1->loadSettings("GUI/guiSettings.xml");
    //gui1->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    gui1->addWidgetDown(powerSlider);
//    gui1->addWidgetDown(new ofxUILabel("Left Motor:", OFX_UI_FONT_MEDIUM));
//    gui1->addWidgetRight(leftMotorPos);
//    gui1->addWidgetRight(new ofxUILabel("Right Motor:", OFX_UI_FONT_MEDIUM));
//    gui1->addWidgetRight(rightMotorPos);

    
    gui1->addWidgetDown(new ofxUISpacer(200,2));
    gui1->addWidgetDown(new ofxUILabel("SERIAL", OFX_UI_FONT_MEDIUM)); 
    gui1->addWidgetDown(new ofxUILabel("Serial Sent:", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetRight(serialSent);  
    gui1->addWidgetDown(new ofxUILabel("Serial Received:", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetRight(serialReceived); 
    gui1->addWidgetDown(new ofxUISpacer(300, 5));
    gui1->addWidgetDown(new ofxUITextInput(300, "SERIAL COMMAND", "Enter Command...", OFX_UI_FONT_MEDIUM));
    
    
    
    gui1->addWidgetDown(new ofxUISpacer(250,2));
    gui1->addWidgetDown(new ofxUILabel("CALIBRATION", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetDown(new ofxUILabel("Left Motor:   ", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetRight(leftMotorIn);
    gui1->addWidgetRight(leftMotorOut);
    gui1->addWidgetDown(new ofxUILabel("Right Motor:  ", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetRight(rightMotorIn);
    gui1->addWidgetRight(rightMotorOut);    
    gui1->addWidgetDown(new ofxUILabel("Both Motors:", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetRight(bothMotorIn);
    gui1->addWidgetRight(bothMotorOut);
    
    ofAddListener(gui1->newGUIEvent, this, &testApp::guiEvent); 

    gui1->addWidget(quit);

    
    leftMotorInID = leftMotorIn->getID();
    leftMotorOutID = leftMotorOut->getID();
    rightMotorInID = rightMotorIn->getID();
    rightMotorOutID = rightMotorOut->getID();
    
    
    // motor Speed parameters
    low = 0;
    med = 1;
    max = 2;
    
    motorSpeed = 100;

    powerLevel = 96;
    turnValue = 0;
    
    incomingByte = 0;
    countCycles = 0;
    maxCycles = 3;
    
    
    posAvg.resize(maxCycles);
    
    
    // initialiaze control Boolians 
    
    pitchFlip = false;
    speedUp = false;
    slowDown = false;
    holdPower = false;
    holdPosition = false;
    holdMotorSpeed = false;
    keyDown = false;
    turn = false;
    Bbutton = false;
    arduinoReady = false;
    if(serialCheck) arduinoReady = true;
    axisChange = false;
    printByte = false;
    
    key = 0;
    
    // Initialize angle of rotation 
    theta = 0;
    holdAngle = 0;
    
    numBytes = 0;
    
    //std::system ("osascript -e 'tell app \"DarwiinRemote\" to activate' ");   // this code is for running apple script 
    
    
    // Initialize ofxGamePad Handler 
    ofxGamepadHandler::get()->enableHotplug();
	
	//CHECK IF THERE EVEN IS A GAMEPAD CONNECTED
	if(ofxGamepadHandler::get()->getNumPads()>0){
        ofxGamepad* pad = ofxGamepadHandler::get()->getGamepad(0);
        ofAddListener(pad->onAxisChanged, this, &testApp::axisChanged);
        ofAddListener(pad->onButtonReleased, this, &testApp::buttonPressed);        // These are opposite on purpose to be properly 
        ofAddListener(pad->onButtonPressed, this, &testApp::buttonReleased);        // calibrated   
	}
 
    // initialize x and y positions of red dot
    xpos = ofGetWidth()/2;
    ypos = ofGetHeight()/2;
    
    
}

void testApp::onNewMessage(string & message)
{
	cout << "Serial message: " << message << "\n";
    serialReceived->setLabel(message);
	
}

//--------------------------------------------------------------
void testApp::update(){
    
    if(powerLevel > 96) powerLevel = 96;
    if(powerLevel < 0) powerLevel = 0;
    
    
    serialSent->setLabel(serialCommand);
    powerSlider->setValue(powerLevel);
        
    // check for waiting OSC messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
        
        // get arguments
        for ( int i=0; i<m.getNumArgs(); i++ )
        {
            // display the argument - make sure we get the right type
            if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
                msg_string += ofToString( m.getArgAsInt32( i ) );
            else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
                msg_string += ofToString( m.getArgAsFloat( i ) );
            else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
                msg_string += m.getArgAsString( i );
        }
        
        //cout << m.getAddress();
        //cout << "   " << msg_string << endl;
        
        
        if(m.getAddress() == "/wii/2/accel/pry/0" ){
            yaw = ofToFloat(msg_string);
        }
        if(m.getAddress() == "/wii/2/accel/pry/1" ){
            roll = ofToFloat(msg_string);
        }
        if(m.getAddress() == "/wii/2/accel/pry/2" ){
            pitch = ofToFloat(msg_string);
            
        }
        
        
        if (m.getAddress() == "/gyrosc/gps") {
            cout.precision(4);
            cout << "Lat: " << m.getArgAsFloat(0) << " Long: " << m.getArgAsFloat(1) << endl;
        }
        
        if(m.getAddress() == "/gyrosc/gyro"){
            //cout << "pitch: " << m.getArgAsFloat(0) ;
            pitch = m.getArgAsFloat(0) / 1.5;
            //cout << "   roll: " << m.getArgAsFloat(1) << endl;;
            
            
            //cout << "   yaw: " << m.getArgAsFloat(2) << endl;
            yaw = m.getArgAsFloat(2);
        }
        if(m.getAddress() == "/gyrosc/comp"){
            currentHeading = ofToFloat(msg_string);
            if(currentHeading-lastHeading > 60){
                pitchFlip = !pitchFlip;
            }
            
            cout << "   HEADING:" << msg_string << endl;
            
            lastHeading = currentHeading;
            
        }
        
        
        
        /// *** iPhone Quaterion Data *** /// 
        if(m.getAddress() == "/gyrosc/quat"){
            iphoneQuat.set(m.getArgAsFloat(1), -m.getArgAsFloat(2), m.getArgAsFloat(3), m.getArgAsFloat(0));
        
        }
        
        if(m.getAddress() == "/FlightControl/Steering"){
            xpos = ofGetWidth()/2 + ofToFloat(msg_string) * ofGetWidth()/2; 
            //cout << "Xpos: " << xpos << endl;
            
            // Send turn command to Arduino
            
            posAvg.at(countCycles) = ofToFloat(msg_string);
            currentPos = ofToFloat(msg_string);
            turnValue = int(currentPos*10)*2;
            axisChange = true;
            
            
            if(serialCheck){    
                
                serial.flush();
                int t0 = ofGetElapsedTimeMillis();
                int t1;
                
                
                serialCommand = "t" + ofToString(int(currentPos*10)*2) + "/";
                unsigned char serialChar[serialCommand.size()];
                unsigned char * buf = serialChar;
                
                for(int i=0;i < serialCommand.size();i++) {
                    serialChar[i] = serialCommand[i];
                }
                
                //cout << currentPos << "  " << lastPos << endl;
                
                if(int(currentPos/0.1) != int(lastPos/0.1)){ 
                    
                    if(currentPos != 0){
                        for(int i=0;i < serialCommand.size();i++) {
                            cout << serialChar[i];
                            serial.writeByte(serialChar[i]);
                        }
                        cout << endl;
                    }else {
                        cout << "HOME" << endl;
                        serial.writeByte('t');
                        serial.writeByte('0');
                        serial.writeByte('/');
                        
                    }
                    
                    while(serial.available() <1)  // wait untill arduino is ready to recieve more data
                    {   
                        if(ofGetElapsedTimeMillis() - t0 > 50) break;
                    }
                    incomingByte = serial.readByte();  // Read data from serial port 
                    printByte = true;
                }
                
                lastPos = currentPos;
                
            }
            

        }
        if(m.getAddress() == "/FlightControl/Power"){
            ypos = ofGetHeight() - (1 - ofToFloat(msg_string))* ofGetHeight();
            //cout << "Ypos: " << ypos << endl;
            int power = 96 * ofToFloat(msg_string);
            
            writeToArduino("p" + ofToString(power) + "/");
        }
        
        if(m.getAddress() == "/Calibrate/leftIN" && msg_string == "1"){   
            powerLevel = 96;
            writeToArduino("b/");
            //cout << "b/" << endl;
        }
        if(m.getAddress() == "/Calibrate/leftIN" && msg_string == "0"){   
            powerLevel = 96;
            writeToArduino("S/");
            //cout << "b/" << endl;
        }
        
        if(m.getAddress() == "/Calibrate/leftOUT" && msg_string == "1"){
            powerLevel = 96;
            writeToArduino("a/");
            //cout << "a/" << endl;

        }
        if(m.getAddress() == "/Calibrate/leftOUT" && msg_string == "0"){   
            powerLevel = 96;
            writeToArduino("S/");
            //cout << "b/" << endl;
        }
        if(m.getAddress() == "/Calibrate/rightIN" && msg_string == "1"){
            powerLevel = 96;
            writeToArduino("c/");
            //cout << "c/" << endl;
        }
        if(m.getAddress() == "/Calibrate/rightIN" && msg_string == "0"){   
            powerLevel = 96;
            writeToArduino("S/");
            //cout << "b/" << endl;
        }
        if(m.getAddress() == "/Calibrate/rightOUT" && msg_string == "1"){
            powerLevel = 96;
            writeToArduino("d/");
            //cout << "d/" << endl;
        }
        if(m.getAddress() == "/Calibrate/rightOUT" && msg_string == "0"){   
            powerLevel = 96;
            writeToArduino("S/");
            //cout << "b/" << endl;
        }
        if(m.getAddress() == "/Calibrate/bothIN" && msg_string == "1"){
            powerLevel = 96;
            // Write to arduino a serial command to bring both motors in equal amounts
            //cout << "e/" << endl;

        }
        if(m.getAddress() == "/Calibrate/bothOUT" && msg_string == "1"){
            powerLevel = 96;
            // Write to arduino to let both motors out equal amounts 
            //cout << "f/" << endl;

        }
        

        msg_string = "";
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofEnableAlphaBlending();
    ofEnableSeparateSpecularLight();
    
    light.enable();
    
    // draw background
    ofSetColor(150);
    bg.draw(0,0,ofGetWidth(),ofGetHeight());
    
    // Draw kite logo 
    ofSetColor(255);
    logo.draw(gui1_w-170,gui1->getRect()->height-170,130,150);
    
    // draw rotated line to indicate rotation of WiiMote/Joystick
    ofSetColor(255,255,255);
    ofSetColor(0, 0, 255);
    
    ofVec3f yaxis(0,-1,0);
    ofVec3f xaxis(1,0,0);
    ofVec3f iphoneYaxis(0,-1,0);
    ofVec3f iphoneXaxis(1,0,0);

    
    ofPushMatrix();
    ofSetLineWidth(20);
    ofTranslate((ofGetWidth() + gui1_w)/2,ofGetHeight()/2);
    //ofRotateZ(-90);
    
    ofVec3f axis;
    
    float  angle;
    iphoneQuat.getRotate(angle, axis);
    ofRotate(angle, -axis.x, -axis.y, -axis.z);
    iphoneYaxis.rotate(angle, -axis);
    iphoneXaxis.rotate(angle, -axis);
    

    float rollAngle;
    rollAngle =  iphoneYaxis.angle(yaxis) ;
    //cout << "Actual Angle:  " << int(rollAngle) << "  ";
    
    // Quadrant 4
    if(iphoneYaxis.y > 0 && iphoneYaxis.x > 0){
        rollAngle = (iphoneYaxis.angle(yaxis) - 90)  ;
    }
    // Quadrant 3
    if(iphoneYaxis.y > 0 && iphoneYaxis.x < 0){
        rollAngle = 180 - rollAngle + 90;
    }   
    //Quadreent 1
    if(iphoneYaxis.y < 0 && iphoneYaxis.x > 0){
        rollAngle = - ( 90 - iphoneYaxis.angle(yaxis) )  ;
    }
    // Quadrent 2
    if(iphoneYaxis.y < 0 && iphoneYaxis.x < 0){
        rollAngle = - (90 +iphoneYaxis.angle(yaxis))  ;
    }
    
    //cout << " iPhoneX: " << int(10*iphoneYaxis.x) << "  iPhoneY: " << int(10*iphoneYaxis.y) << "  ";
    
    //writeToArduino("r" + ofToString(int(rollAngle)));
    //cout << "Roll Angle: " << int(rollAngle) << endl;
    //cout << "axis.x:  " << axis.x << "  axis.y: " << axis.y << "  axis.z:  " << axis.z << endl;
    
    ofSetColor(255,255,255);
    kite.draw(-kite.width/2, -kite.height/2);

    ofPopMatrix();
    

    ofSetColor(255, 255, 255);
   
    // Draw Error message if serial port not opened
    if(!serialCheck) {
        ofSetColor(255);
        ofDrawBitmapString("Failed to open Serial Port!", ((ofGetWidth()+304)/2)-100,90);  
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    this->key = key;
    if(key == OF_KEY_RETURN)
    {
        //gui1->toggleVisible();
    }
    
    if(key == OF_KEY_UP){
        powerLevel = powerLevel + 8;
        if(powerLevel > 96) powerLevel = 96;
        serialCommand = "p" + ofToString(powerLevel) + "/";
        writeToArduino(serialCommand);
    }
    if(key == OF_KEY_DOWN){
        powerLevel = powerLevel - 8;
        if(powerLevel < 0) powerLevel = 0;
        writeToArduino("p" + ofToString(powerLevel) + "/");
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
    
    this->key = 0;

}

//--------------------------------------------------------------
void testApp::axisChanged(ofxGamepadAxisEvent& e)
{
    cout << "AXIS " << e.axis << " VALUE " << ofToString(e.value) << endl;
    
    
    
    switch(e.axis){
        case 0:    // joystick  
            //countCycles++;
            //if(countCycles > maxCycles-1) countCycles = 0;
            xpos = ofGetWidth()/2 + e.value * ofGetWidth()/2; 
            //cout << "Joystick Pos:  " << e.value << endl;
            //theta = 95*e.value;
            posAvg.at(countCycles) = e.value;
            currentPos = e.value;
            turnValue = int(currentPos*10)*2;
            axisChange = true;
            
            //serialCheck = serial.setup();
            serial.flush();

            if(serialCheck){    
                
                int t0 = ofGetElapsedTimeMillis();
                int t1;
                
                    
                serialCommand = "t" + ofToString(int(currentPos*10)*2) + "/";
                unsigned char serialChar[serialCommand.size()];
                unsigned char * buf = serialChar;
                
                for(int i=0;i < serialCommand.size();i++) {
                    serialChar[i] = serialCommand[i];
                }
                
                //cout << currentPos << "  " << lastPos << endl;
                
                if(int(currentPos/0.1) != int(lastPos/0.1)){ 
                    
                    if(currentPos != 0){
                        for(int i=0;i < serialCommand.size();i++) {
                            cout << serialChar[i];
                            serial.writeByte(serialChar[i]);
                        }
                        cout << endl;
                    }else {
                        cout << "HOME" << endl;
                        serial.writeByte('t');
                        serial.writeByte('0');
                        serial.writeByte('/');
                        
                    }
                    
                    while(serial.available() <1)  // wait untill arduino is ready to recieve more data
                    {   
                        if(ofGetElapsedTimeMillis() - t0 > 100) break;
                    }
                    incomingByte = serial.readByte();  // Read data from serial port 
                    printByte = true;
                }
        
                lastPos = currentPos;
            
            }
       
            //serial.close();

            
            break;
        case 1: // Y-axis of Nunchuck
            
            break;    
        case 2: // Roll angle of Nunchuck
            
            break;
        case 3: // Pitch angle of WiiMote
            break;            
    }
    
}

//--------------------------------------------------------------

void testApp::buttonPressed(ofxGamepadButtonEvent& e)
{
	cout << "BUTTON " << e.button << " PRESSED" << endl;
    
    switch (e.button) {
        case 0: // Trigger Button Joystick  // A Button WiiMote
            holdPosition = false;
            break;
        case 1: // WiiMote - B Button 
            Bbutton = true;
            holdPosition_x = xpos;
            holdPosition = true;
            break;
        case 2: // Nunchuck - Z Button // Button 2
            
            powerLevel = powerLevel + 8;
            if(powerLevel > 96) powerLevel = 96;
            //speedMeter_y = ofGetHeight()/2 - speedMeter_h;
            //serialCheck = serial.setup();
            serialCommand = "p" + ofToString(powerLevel) + "/";
            writeToArduino(serialCommand);
            
            break;
        case 3: // Nunchuck - C Button // Button 3 
            powerLevel = powerLevel - 8;
            if(powerLevel < 0) powerLevel = 0;
            //serialCheck = serial.setup();
            writeToArduino("p" + ofToString(powerLevel) + "/");
            break;
    }
    
}

//--------------------------------------------------------------

void testApp::buttonReleased(ofxGamepadButtonEvent& e)
{
    switch (e.button) {
        case 0: // WiiMote - A Button
            //holdPosition = false;
            home = true;
            break;
        case 1: // WiiMote - B Button 
            Bbutton = false;
            holdAngle = theta;
            //serial.writeByte('s');          // send signal to reset motors to zero position 
            break;
        case 2: // Nunchuck - Z Button
            break;
        case 3: // Nunchuck - C Button
            break;
    }

	cout << "BUTTON " << e.button << " RELEASED" << endl;
}
//--------------------------------------------------------------

void testApp::exit()
{
    gui1->saveSettings("GUI/guiSettings.xml");     
    delete gui1;
    
   
    
}
//--------------------------------------------------------------

void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "BACKGROUND VALUE")	
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        ofBackground(slider->getScaledValue());
    }
    if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue()); 
        
    }
    if(e.widget->getName() == "QUIT")
    {
        ofxUIButton *quit = (ofxUIButton *) e.widget;
        if(quit->getValue())
        {
            exit();
            //ofExit();
        }
    }
    if(e.widget->getName() == "SERIAL COMMAND")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        
        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
        {
            writeToArduino(textinput->getTextString());
            serialSent->setLabel(serialCommand);            
        }
        if(serialCommand[0] == 'p'){
            powerLevel = int(ofToFloat(serialCommand.substr(1,2)));
        }
        if(serialCommand[0] == 't')
        {
            theta = ofToFloat(serialCommand.substr(1,3))/20 * 80;
            if(theta > 110) theta = 110;
            if(theta < -110) theta = - 110;
        }
    }    
    
        
    // send motor calibration commands to Arduino
    if(leftMotorIn->getValue()){   
        powerLevel = 96;
        writeToArduino("b/");
    }
    if(leftMotorOut->getValue()){
        powerLevel = 96;
        writeToArduino("a/");
    }
    if(rightMotorIn->getValue()){        
        powerLevel = 96;
        writeToArduino("c/");
    }
    if(rightMotorOut->getValue()){
        powerLevel = 96;
        writeToArduino("d/");
    }
    if(bothMotorIn->getValue()){
        powerLevel = 96;
        // Write to arduino a serial command to bring both motors in equal amounts
    }
    if(bothMotorOut->getValue()){
        powerLevel = 96;
        // Write to arduino to let both motors out equal amounts 
    }
    
    //cout << e.widget->getID() << endl;
    
    if(e.widget->getID() == leftMotorIn->getID()) if(!((ofxUIButton*)e.widget)->getValue()) writeToArduino("S/");
    if(e.widget->getID() == leftMotorOutID) if(!((ofxUIButton*)e.widget)->getValue()) writeToArduino("S/");
    if(e.widget->getID() == rightMotorInID) if(!((ofxUIButton*)e.widget)->getValue()) writeToArduino("S/");
    if(e.widget->getID() == rightMotorOutID) if(!((ofxUIButton*)e.widget)->getValue()) writeToArduino("S/");
 
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void testApp::writeToArduino(string command)
{
   
    serialCommand = command;
    if(serialCheck)
    {
        serial.flush();
        int t0 = ofGetElapsedTimeMillis();
        unsigned char serialChar[command.size()];
        
        for(int i=0;i < command.size();i++) {
            serialChar[i] = command[i];
        }
        for(int i=0;i < command.size();i++) {
            //cout << serialChar[i];
            serial.writeByte(serialChar[i]);
        }
        cout << endl;
        
        while(serial.available() <1)  // wait untill arduino is ready to recieve more data
        {   
            if(ofGetElapsedTimeMillis() - t0 > 100) break;
        }
        
        incomingByte = serial.readByte();  // Read data from serial port         
        //serial.close();
    }
    
    return;
    
}

float testApp::getAvg(vector<float> x)
{
    float sum = 0;
    for(int i=0;i<x.size();i++)
    {
        sum += x.at(i);
    }
    return sum/float(x.size());
}



