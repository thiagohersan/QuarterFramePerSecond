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

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);

        ofxEdsdk::Camera mCamera;
        GifScene mScene;
        ofImage mCanvas;
};
