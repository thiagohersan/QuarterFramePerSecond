#include "FadingGifScene.h"

FadingGifScene::FadingGifScene(){}
FadingGifScene::~FadingGifScene(){}

void FadingGifScene::setup(ofRectangle &posAndSize){
    nextFlashMillis = ofGetElapsedTimeMillis()+500;
    canvasPositionAndSize = ofRectangle(posAndSize);
    tempFbo.allocate(canvasPositionAndSize.width, canvasPositionAndSize.height);
    mState = INITIAL;

    Scene::loadSounds();
}

void FadingGifScene::setVolume(float v){
    maxVolume = v;
    shutterSounds[currentShutterSound].setVolume(Scene::maxVolume);
    applauseSounds[currentApplauseSound].setVolume(Scene::maxVolume);
}

void FadingGifScene::update(ofxEdsdk::Camera &camera){
    // state transitions
    if ((mState == INITIAL) && (ofGetElapsedTimeMillis() > CAMERA_DELAY_MILLIS)) {
        camera.focusFrame();
        numOfFotosLeft = 1;
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

            currentShutterSound = ofRandom(shutterSounds.size());
            shutterSounds[currentShutterSound].setVolume(Scene::maxVolume);
            shutterSounds[currentShutterSound].play();
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
            FadingImage mfi;
            mfi.setup(mFoto);
            mFotos.push_back(mfi);

            numOfFotosLeft = max(numOfFotosLeft-1,0);
            if(numOfFotosLeft <= 0){
                // for looping effect
                for(int i=mFotos.size()-2; i>0; i--){
                    mFotos.push_back(mFotos.at(i));
                }
                currentFotoToDisplay = 0;
                mFoto = mFotos.at(currentFotoToDisplay).getImage();
                mState = FADING_PICTURE_IN;

                currentApplauseSound = ofRandom(applauseSounds.size());
                applauseSounds[currentApplauseSound].setVolume(Scene::maxVolume);
                applauseSounds[currentApplauseSound].setPosition(0.0f);
                applauseSounds[currentApplauseSound].play();
            }
            else{
                flashValue = 1.0;
                stayWhiteCount = 0;
                mState = WAITING_FOR_CAMERA;
                camera.takePhotoNonAF();
                lastFotoChangeMillis = ofGetElapsedTimeMillis();
            }
        }
    }
    else if (mState == WAITING_FOR_CAMERA) {
        if(camera.isButtonPressed()){
            mState = FLASHING_IN;

            shutterSounds[currentShutterSound].setVolume(Scene::maxVolume);
            shutterSounds[currentShutterSound].play();
        }
    }
    else if (mState == FADING_PICTURE_IN) {
        flashValue = min(flashValue+PICTURE_FADE_IN_INCREMENT, 255.0f);
        if ((flashValue >= 255) && (stayWhiteCount>PICTURE_DURATION_COUNT)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            numOfFotosLeft = mFotos.size()*4;
            lastFotoChangeMillis = ofGetElapsedTimeMillis();

            mFotos.at(currentFotoToDisplay).setFadecolor();
            for(vector<FadingImage>::iterator it=mFotos.begin(); it!=mFotos.end(); ++it){
                it->findSimilarColors();
            }
            mState = SHOWING_ANIMATION;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == SHOWING_ANIMATION) {
        bool allFotosFaded = false;
        mFotos.at(currentFotoToDisplay).fadeImage();
        mFoto = mFotos.at(currentFotoToDisplay).getImage();

        if(mFotos.at(currentFotoToDisplay).doneFading()){
            bool somePixelsToFade = false;
            mFotos.at(currentFotoToDisplay).setFadecolor();
            for(vector<FadingImage>::iterator it=mFotos.begin(); it!=mFotos.end(); ++it){
                somePixelsToFade |= it->findSimilarColors();
            }
            allFotosFaded = !somePixelsToFade;
        }

        if((numOfFotosLeft <= 0) || allFotosFaded){
            mFotos.at(currentFotoToDisplay).setFadecolor();
            mFotos.at(currentFotoToDisplay).findSimilarColors();
            camera.focusFrame();
            mState = FADING_PICTURE_OUT;
        }
        else if(ofGetElapsedTimeMillis()-lastFotoChangeMillis > ANIMATION_DELAY_MILLIS){
            numOfFotosLeft = max(numOfFotosLeft-1,0);
            lastFotoChangeMillis = ofGetElapsedTimeMillis();
            mFotos.at(currentFotoToDisplay).forcePixelClear();
            currentFotoToDisplay = (currentFotoToDisplay+1)%mFotos.size();
            mFoto = mFotos.at(currentFotoToDisplay).getImage();
        }
    }
    else if (mState == FADING_PICTURE_OUT) {
        applauseSounds[currentApplauseSound].setVolume(0.95f*applauseSounds[currentApplauseSound].getVolume());
        if (mFotos.at(currentFotoToDisplay).fadeImage() == false) {
            mState = CLEARING_PICTURE;
        }
    }
    else if (mState == CLEARING_PICTURE) {
        applauseSounds[currentApplauseSound].setVolume(0.95f*applauseSounds[currentApplauseSound].getVolume());
        flashValue = min(flashValue+PICTURE_FADE_OUT_INCREMENT, 0.0f);
        if (flashValue >= 0.0 && applauseSounds[currentApplauseSound].getVolume()<0.1) {
            numOfFotosLeft = ofRandom(3, MAX_NUMBER_OF_PICTURES_TO_TAKE);
            mFotos.clear();
            mState = FOCUSING;
            nextFlashMillis = ofGetElapsedTimeMillis()+CAMERA_DELAY_MILLIS;
            applauseSounds[currentApplauseSound].stop();
        }
    }
}

void FadingGifScene::draw(ofImage &canvas){
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
