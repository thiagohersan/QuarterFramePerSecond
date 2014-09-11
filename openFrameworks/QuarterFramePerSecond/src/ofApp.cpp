#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    syphonServer.setName("Syphon Output");
    ofEnableSmoothing();
    ofSetFrameRate(60);

    // lol
    pathLeft.moveTo(37,259);
    pathLeft.lineTo(46,259);
    pathLeft.lineTo(98,426);
    pathLeft.lineTo(37,426);
    pathLeft.close();

    pathCenter.moveTo(120,259);
    pathCenter.lineTo(171,259);
    pathCenter.lineTo(192,426);
    pathCenter.lineTo(99,426);
    pathCenter.close();

    pathRight.moveTo(245,259);
    pathRight.lineTo(251,259);
    pathRight.lineTo(251,426);
    pathRight.lineTo(193,426);
    pathRight.close();

    mCanvas.allocate(214, 167, OF_IMAGE_COLOR);
    mPanels.allocate(214, 167, OF_IMAGE_COLOR);

    for(int y=0; y<mCanvas.getHeight(); y++) {
        int cy = (y/16)%2;
        for(int x=0; x<mCanvas.getWidth(); x++) {
            int cx = (x/16+cy)%2;
            mCanvas.setColor(x, y, ofColor(cx*255));
        }
    }
    mCanvas.reloadTexture();
}

//--------------------------------------------------------------
void ofApp::update(){
    syphonServer.publishScreen();
    toPanels(mCanvas, mPanels);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);

    pathLeft.draw();
    pathCenter.draw();
    pathRight.draw();

    mCanvas.draw(400,14);
    mPanels.draw(400,259);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10,10, ofColor(255,0,255), ofColor(255,255,0));
}

void ofApp::toPanels(ofImage &mCanvas, ofImage &mPanels){
    if(!(mCanvas.getWidth() == 214 && mCanvas.getHeight() == 167))
        return;

    for(int y=0; y<mPanels.getHeight(); y++){
        int rowWidthHalf = (int)((93.0-51.0)/mPanels.getHeight()*y/2.0+25.0);
        int rowCenterPixel = y*mPanels.getWidth()+mPanels.getWidth()/2;
        // center
        for(int x=0; x<=rowWidthHalf; x++){
            mPanels.setColor(mPanels.getWidth()/2+x, y, mCanvas.getColor(mPanels.getWidth()/2+x, y));
            mPanels.setColor(mPanels.getWidth()/2-x, y, mCanvas.getColor(mPanels.getWidth()/2-x, y));
        }

        // left/right
        int gapSize = (int)((0.0-74.0)/mPanels.getHeight()*y+74.0);
        int leftoverPixels = (int)((61.0-9.0)/mPanels.getHeight()*y+9.0);
        for(int x=0; x<leftoverPixels; x++){
            mPanels.setColor(mPanels.getWidth()/2+rowWidthHalf+1+x+gapSize, y, mCanvas.getColor(mPanels.getWidth()/2+rowWidthHalf+1+x, y));
            mPanels.setColor(mPanels.getWidth()/2-rowWidthHalf-1-x-gapSize, y, mCanvas.getColor(mPanels.getWidth()/2-rowWidthHalf-1-x, y));
        }
    }
    mPanels.reloadTexture();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}