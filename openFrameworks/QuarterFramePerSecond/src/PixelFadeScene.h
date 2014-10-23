#pragma once

#include "Scene.h"

class PixelFadeScene: public Scene{
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
        long long nextFlash;
        int stayWhiteCount = 0;

        vector<ofVec2f> pixelsToFade;
        vector<ofColor> colorsToRand;
        ofImage mFoto;
        ofFbo tempFbo;
        ofRectangle canvasPositionAndSize;

        void findSimilarColors(ofColor &c, ofImage &p);
        bool fadeImage(ofImage &p);
};
