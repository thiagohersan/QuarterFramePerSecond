#pragma once

#include "Scene.h"

class BlankScene: public Scene{
    protected:
        long long const CAMERA_DELAY_MILLIS = 2000;
        float const FLASH_IN_INCREMENT = 50.0f;
        float const FLASH_OUT_INCREMENT = 60.0f;
        int const FLASH_DURATION_COUNT = 4;
        long long const DELAY_BETWEEN_PICTURES_MILLIS = 500;
    public:
        BlankScene();
        ~BlankScene();
        void setup(ofRectangle &posAndSize);
        void update(ofxEdsdk::Camera &camera);
        void draw(ofImage &canvas);
    protected:
        enum State { INITIAL, FOCUSING, WAITING_FOR_PICTURE, WAITING_FOR_CAMERA,
            FLASHING_IN, FLASHING_OUT };

        State mState;
        long long lastFotoChangeMillis;

        float flashValue;
        long long nextFlashMillis;
        int stayWhiteCount = 0;

        ofImage mFoto;
        ofFbo tempFbo;
        ofRectangle canvasPositionAndSize;

        void audioOut(float* output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
};
