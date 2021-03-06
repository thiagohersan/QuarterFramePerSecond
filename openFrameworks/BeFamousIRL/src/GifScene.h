#pragma once

#include "ofxEdsdk.h"

class GifScene {
    protected:
        long long const CAMERA_DELAY_MILLIS = 2000;
        float const FLASH_IN_INCREMENT = 60.0f;
        float const FLASH_OUT_INCREMENT = 60.0f;
        int const FLASH_DURATION_COUNT = 3;
        int const PICTURE_DURATION_COUNT = 50;
        float const PICTURE_FADE_IN_INCREMENT = 4.0f;
        float const PICTURE_FADE_OUT_INCREMENT = 4.0f;
        long long const ANIMATION_DELAY_MILLIS = 250;
        int const ANIMATION_LOOP_COUNT = 5;
        int const NUMBER_OF_PICTURES_TO_TAKE = 5;
        int const CLICK_DELAY_MILLIS = 800;
    public:
        GifScene();
        ~GifScene();
        void setup(const ofVec2f &canvasSize);
        void update(ofxEdsdk::Camera &camera);
        void draw(ofImage &canvas);
        static void staticCallbackWrapper(void* p);
        void onCameraClick();
    protected:
        enum State { INITIAL, WAITING_FOR_CLICK, WAITING_FOR_PICTURE, FLASHING_IN, FLASHING_OUT,
            FADING_PICTURE_IN, SHOWING_ANIMATION, CLEARING_PICTURE };

        State mState;

        int numOfPhotosLeft = 0;
        vector<ofImage> mPhotos;
        int currentPhotoToDisplay = 0;
        long long lastPhotoChangeMillis;

        float flashValue;
        int stayWhiteCount = 0;

        ofVec2f photoSize;
        ofImage mPhoto;
        ofFbo tempFbo;
};
