#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxEdsdk.h"
#include "GifScene.h"
#include "BlankScene.h"

class ofApp : public ofBaseApp{
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

        Scene* mScene;
        bool bPlayAudio;

        ofImage mCanvas, mPanels, panelsMask;
        ofImage fiespMask;

        ofRectangle mPanelPositionAndSize;

        void toPanels(ofImage &mCanvas, ofImage &mPanels);
        void drawChessboard(ofImage &mCanvas);
};
