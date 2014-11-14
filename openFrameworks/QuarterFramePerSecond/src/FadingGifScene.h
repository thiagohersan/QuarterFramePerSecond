#pragma once

#include "Scene.h"
#include "FadingImage.h"

class FadingGifScene: public Scene{
    protected:
        long long const CAMERA_DELAY_MILLIS = 2000;
        float const FLASH_IN_INCREMENT = 50.0f;
        float const FLASH_OUT_INCREMENT = 60.0f;
        int const FLASH_DURATION_COUNT = 4;
        int const PICTURE_DURATION_COUNT = 50;
        float const PICTURE_FADE_IN_INCREMENT = 4.0f;
        float const PICTURE_FADE_OUT_INCREMENT = 4.0f;
        long long const ANIMATION_DELAY_MILLIS = 400;
        int const MAX_NUMBER_OF_PICTURES_TO_TAKE = 10;
    public:
        FadingGifScene();
        ~FadingGifScene();
        void setup(ofRectangle &posAndSize);
        void update(ofxEdsdk::Camera &camera);
        void draw(ofImage &canvas);
        void setVolume(float v);
    protected:
        enum State { INITIAL, FOCUSING, WAITING_FOR_PICTURE, WAITING_FOR_CAMERA, FLASHING_IN,
            FLASHING_OUT, FADING_PICTURE_IN, SHOWING_ANIMATION, FADING_PICTURE_OUT, CLEARING_PICTURE };

        State mState;

        int numOfFotosLeft = 0;
        //vector<ofImage> mFotos;
        vector<FadingImage> mFotos;
        int currentFotoToDisplay = 0;
        long long lastFotoChangeMillis;

        float flashValue;
        long long nextFlashMillis;
        int stayWhiteCount = 0;

        int currentShutterSound;
        int currentApplauseSound;

        vector<ofVec2f> pixelsToFade;
        vector<ofColor> colorsToRand;
        ofImage mFoto;
        ofFbo tempFbo;
        ofRectangle canvasPositionAndSize;
};
