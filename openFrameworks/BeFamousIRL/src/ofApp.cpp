#include "ofApp.h"

const ofVec2f ofApp::canvasSize = ofVec2f(PHOTO_DIM,PHOTO_DIM);

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofUnregisterKeyEvents((ofBaseApp*)this, OF_EVENT_ORDER_APP);
    ofUnregisterMouseEvents((ofBaseApp*)this, OF_EVENT_ORDER_APP);
    ofUnregisterTouchEvents((ofBaseApp*)this, OF_EVENT_ORDER_APP);

    mCamera.setLiveView(false);
    mCamera.setup();

    mScene.setup(canvasSize);
    mCanvas.allocate(canvasSize.x, canvasSize.y, OF_IMAGE_COLOR);

    // register the callback and the data ... HACK!!!
    (ofxEdsdk::Camera::onVolumeInfoChangedCallbackFunction) = &GifScene::staticCallbackWrapper;
    (ofxEdsdk::Camera::volumeInfoChangedCallbackParamater) = (void*)&mScene;

    mUDP.Create();
    mUDP.Connect("localhost", UDP_PORT);
    mUDP.SetNonBlocking(false);
}

//--------------------------------------------------------------
void ofApp::update(){
    mCamera.update();
    mScene.update(mCamera);
    mScene.draw(mCanvas);

    // send every other frame
    if(ofGetFrameNum()%2){
        ofPixels mPix = mCanvas.getPixels();
        int totalPixelCount = mCanvas.getWidth()*mCanvas.getHeight();
        for(int i=0; i<totalPixelCount; i++){
            frameBuffer[3*i+0] = mPix[4*i+0];
            frameBuffer[3*i+1] = mPix[4*i+1];
            frameBuffer[3*i+2] = mPix[4*i+2];
        }
        mUDP.Send((char*)frameBuffer, sizeof(frameBuffer));
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    mCanvas.draw(20,20, 500,500);
}

void ofApp::exit(){
    cout << "exit\n";
    mCamera.close();
    ofBaseApp::exit();
}
