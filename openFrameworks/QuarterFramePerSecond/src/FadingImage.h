#pragma once

#include "ofImage.h"

class FadingImage {
    public:
        FadingImage();
        ~FadingImage();
        void setup(ofImage &img);
        ofImage& getImage();
        void setFadecolor();
        bool findSimilarColors();
        bool fadeImage();
        bool doneFading();
        void forcePixelClear();
    protected:
        static ofColor currentFadeColor;
        vector<ofColor> colorsToRand;
        vector<ofVec2f> pixelsToFade;
        ofImage mFoto;
};
