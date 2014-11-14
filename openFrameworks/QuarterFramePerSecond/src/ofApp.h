#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxEdsdk.h"
#include "GifScene.h"
#include "FadingGifScene.h"
#include "BlankScene.h"

class ofApp : public ofBaseApp{
    private:
        const string STRING_GIF_SCENE = "GifScene";
        const string STRING_FADING_GIF_SCENE = "FadingGifScene";
        const string STRING_BLANK_SCENE = "BlankScene";
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

        ofxSyphonServer syphonServer;
        ofxEdsdk::Camera mCamera;

        Scene* mScene, *gifScene, *blankScene, *fadingGifScene;
        string mSceneString;
        bool bPlayAudio;

        ofImage mCanvas, mPanels, panelsMask;
        ofImage fiespMask;

        ofRectangle mPanelPositionAndSize;

        void toPanels(ofImage &mCanvas, ofImage &mPanels);
        void drawChessboard(ofImage &mCanvas);
};
