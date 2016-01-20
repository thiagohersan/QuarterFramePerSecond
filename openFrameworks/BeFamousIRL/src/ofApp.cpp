#include "ofApp.h"

const ofVec2f ofApp::canvasSize = ofVec2f(50,50);

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);

    mCamera.setLiveView(false);
    mCamera.setup();

    mScene.setup(canvasSize);
    mCanvas.allocate(canvasSize.x, canvasSize.y, OF_IMAGE_COLOR);
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

void ofApp::keyPressed(int key){
    if(key == '!'){
    }
    else if(key == '@'){
    }
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
