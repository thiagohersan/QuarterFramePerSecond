#include "BlankScene.h"

BlankScene::BlankScene(){}
BlankScene::~BlankScene(){}

void BlankScene::setup(ofRectangle &posAndSize){
    nextFlashMillis = ofGetElapsedTimeMillis()+500;
    canvasPositionAndSize = ofRectangle(posAndSize);
    tempFbo.allocate(canvasPositionAndSize.width, canvasPositionAndSize.height);
    mState = INITIAL;
}

void BlankScene::update(ofxEdsdk::Camera &camera){
    // state transitions
    if ((mState == INITIAL) && (ofGetElapsedTimeMillis() > CAMERA_DELAY_MILLIS)) {
        camera.focusFrame();
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

            // save image
            mFoto.saveImage(ofToString("025FPS_")+
                            ofToString(ofGetYear())+
                            ofToString(ofGetMonth())+
                            ofToString(ofGetDay())+
                            ofToString("__")+
                            ofToString(ofGetHours())+
                            ofToString(ofGetMinutes())+
                            ofToString(ofGetSeconds())+
                            ofToString(".jpg"));

            flashValue = 1.0;
            stayWhiteCount = 0;
            mState = WAITING_FOR_CAMERA;
            lastFotoChangeMillis = ofGetElapsedTimeMillis();
        }
    }
    else if (mState == WAITING_FOR_CAMERA) {
        if(ofGetElapsedTimeMillis()-lastFotoChangeMillis > DELAY_BETWEEN_PICTURES_MILLIS){
            camera.takePhotoNonAF();
            mState = FLASHING_IN;
        }
    }
}

void BlankScene::draw(ofImage &canvas){
    // update images, drawings, graphics, etc...
    if ((mState == INITIAL) || (mState == FOCUSING) || (mState == FLASHING_IN) ||
        (mState == FLASHING_OUT) || (mState == WAITING_FOR_PICTURE)) {
        tempFbo.begin();
        ofBackground(ofColor(abs(flashValue)));
        tempFbo.end();
    }
    tempFbo.readToPixels(canvas.getPixelsRef());
}

void BlankScene::audioOut(float* output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount){
    for(int i=0; i<bufferSize; i++){
        float overallVolume = 1.0f;
		output[i] = sin(2*PI*440*tickCount*bufferSize/44100)*overallVolume;
	}
}

void BlankScene::setVolume(float v){
    maxVolume = v;
}
