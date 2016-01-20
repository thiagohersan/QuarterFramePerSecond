#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 320;  // try to grab at this size.
    camHeight = 240;

    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(camWidth, camHeight);

    videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    //ofSetVerticalSync(true);
    
    udpConnection.Create();
    udpConnection.Connect("localhost",9100);
    udpConnection.SetNonBlocking(false);
    
    imgToSend.allocate( camWidth, camHeight, OF_IMAGE_COLOR);

}


//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100, 100, 100);
    vidGrabber.update();

    if(vidGrabber.isFrameNew()){

        imgToSend.clone( ofImage(vidGrabber.getPixels()));
        imgToSend.resize(50, 50);
        
        
        for(int i=0;i<imgToSend.getWidth();i++)
        {
            for(int j=0;j<imgToSend.getHeight();j++)
            {
                ofColor color = imgToSend.getColor(i,j);
                int index = i + imgToSend.getWidth()*j;
                
                frameBuffer[index].r = color.r;
                frameBuffer[index].g = color.g;
                frameBuffer[index].b = color.b;
            }
        }
        int sizePack = sizeof(Packet) * imgToSend.getWidth()* imgToSend.getHeight();
        
       // string message="S<"+ofToString(sizePack)+ "<";
       // udpConnection.Send(message.c_str(),message.length());
        udpConnection.Send( (char *)&frameBuffer,  sizePack);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    vidGrabber.draw(0, 0);
    imgToSend.draw(camWidth,0,camWidth,camHeight);

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // in fullscreen mode, on a pc at least, the 
    // first time video settings the come up
    // they come up *under* the fullscreen window
    // use alt-tab to navigate to the settings
    // window. we are working on a fix for this...

    // Video settings no longer works in 10.7
    // You'll need to compile with the 10.6 SDK for this
    // For Xcode 4.4 and greater, see this forum post on instructions on installing the SDK
    // http://forum.openframeworks.cc/index.php?topic=10343
    if(key == 's' || key == 'S'){
        vidGrabber.videoSettings();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
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
