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

    newPathLeft.moveTo(110,259);
    newPathLeft.lineTo(119,259);
    newPathLeft.lineTo(98,426);
    newPathLeft.lineTo(37,426);
    newPathLeft.close();

    newPathRight.moveTo(172,259);
    newPathRight.lineTo(178,259);
    newPathRight.lineTo(251,426);
    newPathRight.lineTo(193,426);
    newPathRight.close();

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

    nextFlash = ofGetElapsedTimeMillis()+500;
    mState = WAITING;
}

//--------------------------------------------------------------
void ofApp::update(){
    // state transitions
    if (mState == WAITING) {
        if (ofGetElapsedTimeMillis() > nextFlash) {
            //reload a pic
            mFoto.loadImage("foto"+ofToString((int)ofRandom(4))+ ".jpg");
            mFoto.resize(mCanvas.getWidth(), mCanvas.getHeight());

            // first color to Fade
            ofColor rColor = mFoto.getColor(ofRandom(mFoto.width), ofRandom(mFoto.height));
            //find a color to fade
            findSimilarColors(rColor, mFoto);

            mState = FLASHING_IN;
            flashValue = 1.0;
            stayWhiteCount = 0;
        }
    }
    else if (mState == FLASHING_IN) {
        flashValue = min(flashValue+40.0, 255.0);
        if ((flashValue >= 255) && (stayWhiteCount>8)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            mState = FLASHING_OUT;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == FLASHING_OUT) {
        flashValue = min(flashValue+50.0, 0.0);
        if (flashValue >= 0.0) {
            flashValue = 0.0;
            mState = FADING_PICTURE_IN;
        }
    }
    else if (mState == FADING_PICTURE_IN) {
        flashValue = min(flashValue+8.0, 255.0);
        if ((flashValue >= 255) && (stayWhiteCount>10)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            mState = FADING_PICTURE_OUT;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == FADING_PICTURE_OUT) {
        if (fadeImage(mFoto) == false) {
            mState = CLEARING_PICTURE;
        }
    }
    else if (mState == CLEARING_PICTURE) {
        flashValue = min(flashValue+40.0, 0.0);
        if (flashValue >= 0.0) {
            mState = WAITING;
            nextFlash = ofGetElapsedTimeMillis()+2000;
        }
    }

    // update images, drawings, graphics, etc...
    if ((mState == WAITING) || (mState == FLASHING_IN) || (mState == FLASHING_OUT)) {
        mCanvas.setColor(ofColor(abs(flashValue)));
    }
    else if ((mState == FADING_PICTURE_IN) || (mState == FADING_PICTURE_OUT) || (mState == CLEARING_PICTURE)) {
        // scale
        float sFactor = max(mCanvas.width/mFoto.width, mCanvas.height/mFoto.height);
        mFoto.resize(sFactor*mFoto.width, sFactor*mFoto.height);

        // draw to temp fbo with a tint
        ofFbo tempFbo;
        tempFbo.allocate(mCanvas.width, mCanvas.height);
        tempFbo.begin();
        ofSetColor(ofColor(abs(flashValue)));
        mFoto.draw((mFoto.width-tempFbo.getWidth())/2, (mFoto.height-tempFbo.getHeight())/2);
        tempFbo.end();

        tempFbo.readToPixels(mCanvas.getPixelsRef());
    }

    toPanels(mCanvas, mPanels);
    syphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);

    pathLeft.draw();
    pathCenter.draw();
    pathRight.draw();

    ofPushMatrix();
    ofTranslate(0,-200);
    newPathLeft.draw();
    pathCenter.draw();
    newPathRight.draw();
    ofPopMatrix();

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

void ofApp::findSimilarColors(ofColor &c, ofImage &p) {
    pixelsToFade.clear();
    colorsToRand.clear();

    for (int y=0; y<p.height; y++) {
        for (int x=0; x<p.width; x++) {
            ofColor pixelColor = p.getColor(x, y);
            ofVec3f rgb(pixelColor.r, pixelColor.g, pixelColor.b);
            ofVec3f rgb2(c.r, c.g, c.b);

            if(rgb.distanceSquared(rgb2) < 10000){
                pixelsToFade.push_back(ofVec2f(x,y));
            }
            else if (colorsToRand.size() < 255) {
                colorsToRand.push_back(p.getColor(x,y));
            }
        }
    }
}

bool ofApp::fadeImage(ofImage &p) {
    bool randColor = false;

    for(vector<ofVec2f>::iterator it=pixelsToFade.begin(); it!=pixelsToFade.end(); ++it){
        ofColor pixelColor = p.getColor(it->x, it->y);

        pixelColor = ofColor(min(pixelColor.r+6, 255), min(pixelColor.g+6, 255), min(pixelColor.b+6, 255));

        randColor = (pixelColor.r >= 255 && pixelColor.g >= 255 && pixelColor.b >= 255);
        p.setColor(it->x, it->y, pixelColor);
    }
    p.reloadTexture();

    if (randColor && colorsToRand.size() > 0) {
        ofColor rColor = colorsToRand.at(ofRandom(colorsToRand.size()));
        findSimilarColors(rColor, p);
    }

    return (colorsToRand.size() > 200);
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