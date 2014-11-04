#include "GifScene.h"

GifScene::GifScene(){}
GifScene::~GifScene(){}

void GifScene::setup(ofRectangle &posAndSize){
    nextFlashMillis = ofGetElapsedTimeMillis()+500;
    canvasPositionAndSize = ofRectangle(posAndSize);
    tempFbo.allocate(canvasPositionAndSize.width, canvasPositionAndSize.height);
    mState = INITIAL;

    Scene::loadSounds();
}

void GifScene::update(ofxEdsdk::Camera &camera){
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
            shutterSounds[currentShutterSound].setVolume(1.0f);
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
            mFotos.push_back(mFoto);

            numOfFotosLeft = max(numOfFotosLeft-1,0);
            if(numOfFotosLeft <= 0){
                // for looping effect
                for(int i=mFotos.size()-2; i>0; i--){
                    mFotos.push_back(mFotos.at(i));
                }
                currentFotoToDisplay = 0;
                mFoto = mFotos.at(currentFotoToDisplay);
                mState = FADING_PICTURE_IN;

                currentApplauseSound = ofRandom(applauseSounds.size());
                applauseSounds[currentApplauseSound].setVolume(1.0f);
                applauseSounds[currentApplauseSound].setPosition(0.0f);
                applauseSounds[currentApplauseSound].play();
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
        if(ofGetElapsedTimeMillis()-lastFotoChangeMillis > DELAY_BETWEEN_PICTURES_MILLIS){
            camera.takePhotoNonAF();
            mState = FLASHING_IN;

            shutterSounds[currentShutterSound].setVolume(1.0f);
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
        applauseSounds[currentApplauseSound].setVolume(0.95f*applauseSounds[currentApplauseSound].getVolume());
        if (fadeImage(mFoto) == false) {
            mState = CLEARING_PICTURE;
        }
    }
    else if (mState == CLEARING_PICTURE) {
        flashValue = min(flashValue+PICTURE_FADE_OUT_INCREMENT, 0.0f);
        if (flashValue >= 0.0) {
            numOfFotosLeft = ofRandom(1, MAX_NUMBER_OF_PICTURES_TO_TAKE);
            mFotos.clear();
            mState = FOCUSING;
            nextFlashMillis = ofGetElapsedTimeMillis()+CAMERA_DELAY_MILLIS;
            applauseSounds[currentApplauseSound].stop();
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

void GifScene::findSimilarColors(ofColor &c, ofImage &p) {
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

bool GifScene::fadeImage(ofImage &p) {
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
