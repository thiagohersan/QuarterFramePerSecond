#include "ofApp.h"

const ofVec2f ofApp::canvasSize = ofVec2f(50,50);

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
}

//--------------------------------------------------------------
void ofApp::update(){
    mCamera.update();
    mScene.update(mCamera);
    mScene.draw(mCanvas);
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
