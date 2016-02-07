#include "GifScene.h"

GifScene::GifScene(){}
GifScene::~GifScene(){}

void GifScene::staticCallbackWrapper(void* p){
    ((GifScene*)p)->onCameraClick();
}

void GifScene::setup(const ofVec2f &canvasSize){
    photoSize = canvasSize;
    mState = INITIAL;
    tempFbo.allocate(photoSize.x, photoSize.y);
}

void GifScene::onCameraClick(){
    if ((mState == WAITING_FOR_CLICK) && (ofGetElapsedTimeMillis()-lastFotoChangeMillis > CLICK_DELAY_MILLIS)) {
        flashValue = 1.0;
        stayWhiteCount = 0;
        lastFotoChangeMillis = ofGetElapsedTimeMillis();
        mState = FLASHING_IN;
    }
}

void GifScene::update(ofxEdsdk::Camera &camera){
    // state transitions
    if ((mState == INITIAL) && (ofGetElapsedTimeMillis() > CAMERA_DELAY_MILLIS)) {
        numOfPhotosLeft = NUMBER_OF_PICTURES_TO_TAKE;
        mPhotos.clear();
        mState = WAITING_FOR_CLICK;
    }
    else if (mState == WAITING_FOR_CLICK) {
        // this happens in callback
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
            mPhoto.allocate(camera.getWidth(), camera.getHeight(), OF_IMAGE_COLOR);
            mPhoto = camera.getPhotoPixels();

            float sFactor = max((float)(photoSize.x)/mPhoto.getWidth(), (float)(photoSize.y)/mPhoto.getHeight());
            mPhoto.resize(sFactor*mPhoto.getWidth(), sFactor*mPhoto.getHeight());
            mPhoto.crop((mPhoto.getWidth()-photoSize.x)/2.0, (mPhoto.getHeight()-photoSize.y)/2.0, photoSize.x, photoSize.y);
            mPhotos.push_back(mPhoto);
            
            numOfPhotosLeft = max(numOfPhotosLeft-1,0);
            if(numOfPhotosLeft <= 0){
                // for looping effect
                for(int i=mPhotos.size()-2; i>0; i--){
                    mPhotos.push_back(mPhotos.at(i));
                }
                currentPhotoToDisplay = 0;
                mPhoto = mPhotos.at(currentPhotoToDisplay);
                mState = FADING_PICTURE_IN;
            }
            else{
                flashValue = 1.0;
                stayWhiteCount = 0;
                mState = WAITING_FOR_CLICK;
                lastFotoChangeMillis = ofGetElapsedTimeMillis();
            }
        }
    }
    else if (mState == FADING_PICTURE_IN) {
        flashValue = min(flashValue+PICTURE_FADE_IN_INCREMENT, 255.0f);
        if ((flashValue >= 255) && (stayWhiteCount>PICTURE_DURATION_COUNT)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            numOfPhotosLeft = mPhotos.size()*ANIMATION_LOOP_COUNT;
            lastFotoChangeMillis = ofGetElapsedTimeMillis();
            mState = SHOWING_ANIMATION;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == SHOWING_ANIMATION) {
        if(numOfPhotosLeft <= 0){
            mState = CLEARING_PICTURE;
        }
        else if(ofGetElapsedTimeMillis()-lastFotoChangeMillis > ANIMATION_DELAY_MILLIS){
            numOfPhotosLeft = max(numOfPhotosLeft-1,0);
            lastFotoChangeMillis = ofGetElapsedTimeMillis();
            currentPhotoToDisplay = (currentPhotoToDisplay+1)%mPhotos.size();
            mPhoto = mPhotos.at(currentPhotoToDisplay);
        }
    }
    else if (mState == CLEARING_PICTURE) {
        flashValue = min(flashValue+PICTURE_FADE_OUT_INCREMENT, 0.0f);
        if (flashValue >= 0.0) {
            numOfPhotosLeft = NUMBER_OF_PICTURES_TO_TAKE;
            mPhotos.clear();
            mState = WAITING_FOR_CLICK;
        }
    }
}

void GifScene::draw(ofImage &canvas){
    // update images, drawings, graphics, etc...
    if ((mState == INITIAL) || (mState == WAITING_FOR_CLICK) || (mState == FLASHING_IN) ||
        (mState == FLASHING_OUT) || (mState == WAITING_FOR_PICTURE)) {
        tempFbo.begin();
        ofBackground(ofColor(abs(flashValue)));
        tempFbo.end();
    }
    else if ((mState == FADING_PICTURE_IN) || (mState == SHOWING_ANIMATION) || (mState == CLEARING_PICTURE)) {
        // draw to temp fbo with a tint
        tempFbo.begin();
        ofBackground(0);
        ofSetColor(ofColor(abs(flashValue)));
        mPhoto.draw(0,0);
        tempFbo.end();
    }

    tempFbo.readToPixels(canvas.getPixels());
    canvas.update();
}
