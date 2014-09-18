#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxEdsdk.h"

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
        ofPath pathLeft, pathCenter, pathRight, newPathRight, newPathLeft;
        ofImage mCanvas, mPanels;

        void toPanels(ofImage &mCanvas, ofImage &mPanels);
        void findSimilarColors(ofColor c, ofImage p);
        bool fadeImage(ofImage p);

        ofxEdsdk::Camera mCamera;

        enum State { WAITING, FLASHING_IN, FLASHING_OUT, FADING_PICTURE_IN, FADING_PICTURE_OUT, CLEARING_PICTURE };

        float flashValue;
        long long nextFlash;
        int stayWhiteCount = 0;

        vector<ofVec2f> pixelsToFade;
        vector<ofColor> colorsToRand;
        ofImage mFoto;

        State mState;
};
