#pragma once

#include "Scene.h"

class PixelFadeScene: public Scene{
    protected:
        long long const CAMERA_DELAY_MILLIS = 2000;
        float const FLASH_IN_INCREMENT = 50.0f;
        float const FLASH_OUT_INCREMENT = 60.0f;
        int const FLASH_DURATION_COUNT = 4;
        int const PICTURE_DURATION_COUNT = 50;
        float const PICTURE_FADE_IN_INCREMENT = 1.0f;
        float const PICTURE_FADE_OUT_INCREMENT = 4.0f;
    public:
        PixelFadeScene();
        ~PixelFadeScene();
        void setup(ofRectangle &posAndSize);
        void update(ofxEdsdk::Camera &camera);
        void draw(ofImage &canvas);
    protected:
        enum State { INITIAL, WAITING, WAITING_FOR_PICTURE, FLASHING_IN,
            FLASHING_OUT, FADING_PICTURE_IN, FADING_PICTURE_OUT, CLEARING_PICTURE };

        State mState;
        float flashValue;
        long long nextFlashMillis;
        int stayWhiteCount = 0;

        vector<ofVec2f> pixelsToFade;
        vector<ofColor> colorsToRand;
        ofImage mFoto;
        ofFbo tempFbo;
        ofRectangle canvasPositionAndSize;

        void findSimilarColors(ofColor &c, ofImage &p);
        bool fadeImage(ofImage &p);
};
