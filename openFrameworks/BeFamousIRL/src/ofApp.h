#pragma once

#include "ofMain.h"
#include "ofxEdsdk.h"
#include "GifScene.h"

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
};
