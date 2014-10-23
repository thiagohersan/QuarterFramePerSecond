#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxEdsdk.h"
#include "PixelFadeScene.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        ofxSyphonServer syphonServer;
        ofxEdsdk::Camera mCamera;

        PixelFadeScene mPFS;

        ofImage mCanvas, mPanels, panelsMask;
        ofImage fiespMask;

        ofRectangle mPanelPositionAndSize;

        void toPanels(ofImage &mCanvas, ofImage &mPanels);
        void drawChessboard(ofImage &mCanvas);
};
