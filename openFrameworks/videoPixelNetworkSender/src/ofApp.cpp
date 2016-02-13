#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camDim = ofVec2f(320, 240);
    sendDim = ofVec2f(PHOTO_DIM, PHOTO_DIM);

    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        } else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(camDim.x, camDim.y);

    udpConnection.Create();
    udpConnection.Connect("192.168.0.200",9100);
    udpConnection.SetNonBlocking(false);
    
    imgToSend.allocate(camDim.x, camDim.y, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100, 100, 100);
    vidGrabber.update();

    if(vidGrabber.isFrameNew()){
        imgToSend.clone(ofImage(vidGrabber.getPixels()));
        float sFactor = max((float)(sendDim.x)/imgToSend.getWidth(), (float)(sendDim.y)/imgToSend.getHeight());
        imgToSend.resize(sFactor*imgToSend.getWidth(), sFactor*imgToSend.getHeight());
        imgToSend.crop((imgToSend.getWidth()-sendDim.x)/2.0, (imgToSend.getHeight()-sendDim.y)/2.0, sendDim.x, sendDim.y);

        for(int i=0; i<imgToSend.getHeight(); i++) {
            for(int j=0; j<imgToSend.getWidth(); j++) {
                ofColor color = imgToSend.getColor(j,i);
                int index = (j*3)+(imgToSend.getWidth()*i*3);

                frameBuffer[index+0] = color.r & 0xff;
                frameBuffer[index+1] = color.g & 0xff;
                frameBuffer[index+2] = color.b & 0xff;
            }
        }
        int sizePack = sizeof(frameBuffer);

        // string message="S<"+ofToString(sizePack)+ "<";
        // udpConnection.Send(message.c_str(),message.length());
        udpConnection.Send((char*)frameBuffer, sizePack);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    vidGrabber.draw(0, 0);
    imgToSend.draw(camDim.x,0,camDim.y,camDim.y);
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
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
