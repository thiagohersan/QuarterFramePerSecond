#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);

    syphonServer.setName("Syphon Output");
    mCamera.setup();

    mPanelPositionAndSize = ofRectangle(37,259, 215, 168);

    mCanvas.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mPanels.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mPanels.setColor(ofColor(0));

    panelsMask.loadImage("SP_Urban_MASK_025.png");
    panelsMask.crop(mPanelPositionAndSize.x, mPanelPositionAndSize.y, mPanelPositionAndSize.width, mPanelPositionAndSize.height);

    nextFlash = ofGetElapsedTimeMillis()+500;
    mState = INITIAL;

    fiespMask.loadImage("SP_Urban_MASK_025.png");

    ofDirectory fDir("");
    fDir.listDir();
    for(int i=0; i<fDir.numFiles(); i++){
        if(!(fDir.getPath(i).compare(0, 4, string("foto")) || fDir.getPath(i).compare(fDir.getPath(i).size()-4, 4, string(".jpg")))){
            fotoFileNames.push_back(fDir.getPath(i));
        }
    }
    currentFoto = 0;
}

void ofApp::update__(){
    mCamera.update();

    if(!mFoto.isAllocated()){
        mFoto.allocate(200,200, OF_IMAGE_COLOR);
        mFoto.setColor(ofColor(0,0,128));
        mFoto.reloadTexture();
    }

    // state transitions
    if ((mState == INITIAL) && (ofGetElapsedTimeMillis() > 1500)) {
        mCamera.focusFrame();
        mState = WAITING;
    }
    else if (mState == WAITING) {
        if(mCamera.isFrameFocused()){
            mCamera.takeFocusedPhoto();
            mState = WAITING_FOR_PICTURE;
        }
    }
    else if (mState == WAITING_FOR_PICTURE) {
        if(mCamera.isPhotoNew()){
            mFoto.allocate(mCamera.getWidth(), mCamera.getHeight(), OF_IMAGE_COLOR);
            mFoto = mCamera.getPhotoPixels();
            float sFactor = min((float)(ofGetWidth()-40)/mFoto.width, (float)(ofGetHeight()-40)/mFoto.height);
            mFoto.resize(sFactor*mFoto.width, sFactor*mFoto.height);
            mFoto.reloadTexture();
            mCamera.focusFrame();
            mState = WAITING;
        }
    }
    
}
void ofApp::draw__(){
    ofBackground(0);
    ofSetColor(255);
    mFoto.draw(20,20);
}

//--------------------------------------------------------------
void ofApp::update(){
    mCamera.update();

    // state transitions
    if ((mState == INITIAL) && (ofGetElapsedTimeMillis() > 1500)) {
        mCamera.focusFrame();
        mState = WAITING;
    }
    else if (mState == WAITING) {
        /*
        if (ofGetElapsedTimeMillis() > nextFlash) {
            //reload a pic
            mFoto.loadImage(fotoFileNames.at(currentFoto));
            currentFoto = (currentFoto+1)%fotoFileNames.size();
            float sFactor = max((float)(mCanvas.width)/mFoto.width, (float)(mCanvas.height)/mFoto.height);
            mFoto.resize(sFactor*mFoto.width, sFactor*mFoto.height);

            // first color to Fade
            ofColor rColor = mFoto.getColor(ofRandom(mFoto.width), ofRandom(mFoto.height));
            //find a color to fade
            findSimilarColors(rColor, mFoto);

            mState = FLASHING_IN;
            flashValue = 1.0;
            stayWhiteCount = 0;
        }
        */
        if(mCamera.isFrameFocused()){
            mState = FLASHING_IN;
            flashValue = 1.0;
            stayWhiteCount = 0;
            mCamera.takeFocusedPhoto();
        }
    }
    else if (mState == FLASHING_IN) {
        flashValue = min(flashValue+50.0, 255.0);
        if ((flashValue >= 255) && (stayWhiteCount>4)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            mState = FLASHING_OUT;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == FLASHING_OUT) {
        flashValue = min(flashValue+60.0, 0.0);
        if (flashValue >= 0.0) {
            flashValue = 0.0;
            mState = WAITING_FOR_PICTURE;
        }
    }
    else if (mState == WAITING_FOR_PICTURE) {
        if(mCamera.isPhotoNew()){
            mFoto.allocate(mCamera.getWidth(), mCamera.getHeight(), OF_IMAGE_COLOR);
            mFoto = mCamera.getPhotoPixels();
            mFoto.reloadTexture();
            float sFactor = max((float)(mCanvas.width)/mFoto.width, (float)(mCanvas.height)/mFoto.height);
            mFoto.resize(sFactor*mFoto.width, sFactor*mFoto.height);
            // first color to Fade
            ofColor rColor = mFoto.getColor(ofRandom(mFoto.width), ofRandom(mFoto.height));
            //find a color to fade
            findSimilarColors(rColor, mFoto);
            mState = FADING_PICTURE_IN;
            mCamera.focusFrame();
        }
    }
    else if (mState == FADING_PICTURE_IN) {
        flashValue = min(flashValue+1.0, 255.0);
        if ((flashValue >= 255) && (stayWhiteCount>50)) {
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
        flashValue = min(flashValue+4.0, 0.0);
        if (flashValue >= 0.0) {
            mState = WAITING;
            nextFlash = ofGetElapsedTimeMillis()+2000;
        }
    }

    // update images, drawings, graphics, etc...
    if ((mState == INITIAL) || (mState == WAITING) || (mState == FLASHING_IN) ||
        (mState == FLASHING_OUT) || (mState == WAITING_FOR_PICTURE)) {
        mCanvas.setColor(ofColor(abs(flashValue)));
    }
    else if ((mState == FADING_PICTURE_IN) || (mState == FADING_PICTURE_OUT) || (mState == CLEARING_PICTURE)) {
        // draw to temp fbo with a tint
        ofFbo tempFbo;
        tempFbo.allocate(mCanvas.width, mCanvas.height);
        tempFbo.begin();
        ofBackground(0);
        ofSetColor(ofColor(abs(flashValue)));
        mFoto.draw(0,0);
        tempFbo.end();

        tempFbo.readToPixels(mCanvas.getPixelsRef());
    }
    mCanvas.reloadTexture();

    toPanels(mCanvas, mPanels);
    syphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);

    mCanvas.draw(400,14);
    mPanels.draw(400,259);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10,10, ofColor(255,0,255), ofColor(255,255,0));

    fiespMask.draw(0,0);
    mPanels.draw(mPanelPositionAndSize.x,mPanelPositionAndSize.y);
}

void ofApp::toPanels(ofImage &canvas, ofImage &panels){
    if((canvas.getWidth() < panels.getWidth()) || (canvas.getHeight() < panels.getHeight()))
        return;

    for(int y=0; y<panels.getHeight(); y++){
        int leftOffset=0, rightOffset=0;
        for(int x=0; x<=panels.getWidth()/2; x++){
            // left
            if(panelsMask.getColor(panels.width/2-x, y) == ofColor::white){
                panels.setColor(panels.getWidth()/2-x, y, canvas.getColor(canvas.getWidth()/2-leftOffset, y));
                leftOffset++;
            }
            // right
            if(panelsMask.getColor(panels.width/2+x, y) == ofColor::white){
                panels.setColor(panels.getWidth()/2+x, y, canvas.getColor(canvas.getWidth()/2+rightOffset, y));
                rightOffset++;
            }
        }
    }
    panels.reloadTexture();
}

void ofApp::drawChessboard(ofImage& canvas){
    for(int y=0; y<canvas.getHeight(); y++) {
        int cy = (y/16)%2;
        for(int x=0; x<canvas.getWidth(); x++) {
            int cx = (x/16+cy)%2;
            canvas.setColor(x, y, ofColor(cx*255));
        }
    }
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