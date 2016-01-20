#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

struct Packet {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

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
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);        

        ofVec2f camDim, sendDim;
        ofVideoGrabber vidGrabber;
        ofImage imgToSend;

        ofxUDPManager udpConnection;
        struct Packet frameBuffer [50*50];
};
