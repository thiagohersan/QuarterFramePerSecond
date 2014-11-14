#include "FadingImage.h"


ofColor FadingImage::currentFadeColor = ofColor(255,255,255);

FadingImage::FadingImage(){}
FadingImage::~FadingImage(){}

void FadingImage::setup(ofImage &img){
    mFoto.clone(img);
}

ofImage& FadingImage::getImage(){
    return mFoto;
}

void FadingImage::setFadecolor(){
    if(colorsToRand.size() > 0){
        currentFadeColor = colorsToRand.at(ofRandom(colorsToRand.size()));
    }
}

bool FadingImage::findSimilarColors() {
    pixelsToFade.clear();
    colorsToRand.clear();

    for (int y=0; y<mFoto.height; y++) {
        for (int x=0; x<mFoto.width; x++) {
            ofColor pixelColor = mFoto.getColor(x, y);
            ofVec3f rgb(pixelColor.r, pixelColor.g, pixelColor.b);
            ofVec3f rgb2(currentFadeColor.r, currentFadeColor.g, currentFadeColor.b);

            if(rgb.distanceSquared(rgb2) < 4000){
                pixelsToFade.push_back(ofVec2f(x,y));
            }
            else if (colorsToRand.size() < 255) {
                colorsToRand.push_back(mFoto.getColor(x,y));
            }
        }
    }
    return (colorsToRand.size() > 200);
}

bool FadingImage::fadeImage() {
    bool doneFading = false;

    for(vector<ofVec2f>::iterator it=pixelsToFade.begin(); it!=pixelsToFade.end(); ++it){
        ofColor pixelColor = mFoto.getColor(it->x, it->y);

        pixelColor = ofColor(min(pixelColor.r+16, 255), min(pixelColor.g+16, 255), min(pixelColor.b+16, 255));

        doneFading = (pixelColor.r >= 255 && pixelColor.g >= 255 && pixelColor.b >= 255);
        mFoto.setColor(it->x, it->y, pixelColor);
    }
    mFoto.reloadTexture();
    return !doneFading;
}

bool FadingImage::doneFading(){
    return (pixelsToFade.size() == 0);
}

void FadingImage::forcePixelClear() {
    for(vector<ofVec2f>::iterator it=pixelsToFade.begin(); it!=pixelsToFade.end(); ++it){
        mFoto.setColor(it->x, it->y, ofColor(255,255,255));
    }
    mFoto.reloadTexture();
    pixelsToFade.clear();
}
