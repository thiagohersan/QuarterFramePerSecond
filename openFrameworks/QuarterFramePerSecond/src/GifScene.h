#pragma once

#include "PixelFadeScene.h"

class GifScene: public PixelFadeScene{
    protected:
        long long const ANIMATION_DELAY_MILLIS = 200;
    public:
        GifScene();
        ~GifScene();
        void setup(ofRectangle &posAndSize);
        void update(ofxEdsdk::Camera &camera);
        void draw(ofImage &canvas);
    protected:
        enum State { INITIAL, FOCUSING, WAITING_FOR_PICTURE, FLASHING_IN,
            FLASHING_OUT, FADING_PICTURE_IN, SHOWING_ANIMATION, FADING_PICTURE_OUT, CLEARING_PICTURE };

        State mState;

        int numOfFotosLeft = 0;
        vector<ofImage> mFotos;
        int currentFotoToDisplay = 0;
        long long lastFotoChangeMillis;
};