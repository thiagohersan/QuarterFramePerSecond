#pragma once

#include "ofMain.h"
#include "ofxEdsdk.h"
#include "GifScene.h"
#include "ofxNetwork.h"

#define PHOTO_DIM 50
#define UDP_PORT 9100

class ofApp : public ofBaseApp{
    private:
        static const ofVec2f canvasSize;
    public:
        void setup();
        void update();
        void draw();
        void exit();

        ofxEdsdk::Camera mCamera;
        GifScene mScene;
        ofImage mCanvas;

        ofxUDPManager mUDP;
        char frameBuffer[PHOTO_DIM*PHOTO_DIM*3];
};
