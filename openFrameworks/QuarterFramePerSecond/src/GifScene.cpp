#include "GifScene.h"

GifScene::GifScene(){}
GifScene::~GifScene(){}

void GifScene::setup(ofRectangle &posAndSize){
    PixelFadeScene::setup(posAndSize);
    mState = INITIAL;
}

void GifScene::update(ofxEdsdk::Camera &camera){
    // state transitions
    if ((mState == INITIAL) && (ofGetElapsedTimeMillis() > CAMERA_DELAY_MILLIS)) {
        camera.focusFrame();
        numOfFotosLeft = 5;
        mFotos.clear();
        mState = FOCUSING;
    }
    else if (mState == FOCUSING) {
        if((camera.getFocusState() == ofxEdsdk::Camera::OFX_EDSDK_FOCUS_OK) ||
           (camera.getFocusState() == ofxEdsdk::Camera::OFX_EDSDK_FOCUS_FAIL)) {
            flashValue = 1.0;
            stayWhiteCount = 0;
            camera.takePhotoAF();
            mState = FLASHING_IN;
        }
    }
    else if (mState == FLASHING_IN) {
        flashValue = min(flashValue+FLASH_IN_INCREMENT, 255.0f);
        if ((flashValue >= 255) && (stayWhiteCount>FLASH_DURATION_COUNT)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            mState = FLASHING_OUT;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == FLASHING_OUT) {
        flashValue = min(flashValue+FLASH_OUT_INCREMENT, 0.0f);
        if (flashValue >= 0.0) {
            flashValue = 0.0;
            mState = WAITING_FOR_PICTURE;
        }
    }
    else if (mState == WAITING_FOR_PICTURE) {
        if(camera.isPhotoNew()){
            mFoto.allocate(camera.getWidth(), camera.getHeight(), OF_IMAGE_COLOR);
            mFoto = camera.getPhotoPixels();
            mFoto.reloadTexture();
            float sFactor = max((float)(canvasPositionAndSize.width)/mFoto.width, (float)(canvasPositionAndSize.height)/mFoto.height);
            mFoto.resize(sFactor*mFoto.width, sFactor*mFoto.height);
            mFotos.push_back(mFoto);

            numOfFotosLeft = max(numOfFotosLeft-1,0);
            if(numOfFotosLeft <= 0){
                // for looping effect
                for(int i=mFotos.size()-2; i>0; i--){
                    mFotos.push_back(mFotos.at(i));
                }
                currentFotoToDisplay = 0;
                mState = FADING_PICTURE_IN;
            }
            else{
                flashValue = 1.0;
                stayWhiteCount = 0;
                mState = WAITING_FOR_CAMERA;
                lastFotoChangeMillis = ofGetElapsedTimeMillis();
            }
        }
    }
    else if (mState == WAITING_FOR_CAMERA) {
        if(ofGetElapsedTimeMillis()-lastFotoChangeMillis > PICTURE_DELAY_MILLIS){
            camera.takePhotoNonAF();
            mState = FLASHING_IN;
        }
    }
    else if (mState == FADING_PICTURE_IN) {
        flashValue = min(flashValue+PICTURE_FADE_IN_INCREMENT, 255.0f);
        if ((flashValue >= 255) && (stayWhiteCount>PICTURE_DURATION_COUNT)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            numOfFotosLeft = mFotos.size()*4;
            lastFotoChangeMillis = ofGetElapsedTimeMillis();
            mState = SHOWING_ANIMATION;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == SHOWING_ANIMATION) {
        if(numOfFotosLeft <= 0){
            ofColor rColor = mFoto.getColor(ofRandom(mFoto.width), ofRandom(mFoto.height));
            findSimilarColors(rColor, mFoto);
            camera.focusFrame();
            mState = FADING_PICTURE_OUT;
        }
        else if(ofGetElapsedTimeMillis()-lastFotoChangeMillis > ANIMATION_DELAY_MILLIS){
            numOfFotosLeft = max(numOfFotosLeft-1,0);
            lastFotoChangeMillis = ofGetElapsedTimeMillis();
            currentFotoToDisplay = (currentFotoToDisplay+1)%mFotos.size();
            mFoto = mFotos.at(currentFotoToDisplay);
        }
    }
    else if (mState == FADING_PICTURE_OUT) {
        if (fadeImage(mFoto) == false) {
            mState = CLEARING_PICTURE;
        }
    }
    else if (mState == CLEARING_PICTURE) {
        flashValue = min(flashValue+PICTURE_FADE_OUT_INCREMENT, 0.0f);
        if (flashValue >= 0.0) {
            numOfFotosLeft = 5;
            mFotos.clear();
            mState = FOCUSING;
            nextFlashMillis = ofGetElapsedTimeMillis()+CAMERA_DELAY_MILLIS;
        }
    }
}

void GifScene::draw(ofImage &canvas){
    // update images, drawings, graphics, etc...
    if ((mState == INITIAL) || (mState == FOCUSING) || (mState == FLASHING_IN) ||
        (mState == FLASHING_OUT) || (mState == WAITING_FOR_PICTURE)) {
        tempFbo.begin();
        ofBackground(ofColor(abs(flashValue)));
        tempFbo.end();
    }
    else if ((mState == FADING_PICTURE_IN) || (mState == FADING_PICTURE_OUT) ||
             (mState == SHOWING_ANIMATION) || (mState == CLEARING_PICTURE)) {
        // draw to temp fbo with a tint
        tempFbo.begin();
        ofBackground(0);
        ofSetColor(ofColor(abs(flashValue)));
        mFoto.draw(0,0);
        tempFbo.end();
    }
    tempFbo.readToPixels(canvas.getPixelsRef());
}
