#pragma once

#include "PixelFadeScene.h"

class GifScene: public PixelFadeScene{
    public:
        GifScene();
        ~GifScene();
        void setup(ofRectangle &posAndSize);
        void update(ofxEdsdk::Camera &camera);
        void draw(ofImage &canvas);
    protected:
        enum State { INITIAL, WAITING, WAITING_FOR_PICTURE, FLASHING_IN,
            FLASHING_OUT, FADING_PICTURE_IN, SHOW_ANIMATION, FADING_PICTURE_OUT, CLEARING_PICTURE };

        State mState;

        int numOfPicturesLeft = 0;
        vector<ofImage> mFotos;
        int currentImageToDisplay = 0;
};
