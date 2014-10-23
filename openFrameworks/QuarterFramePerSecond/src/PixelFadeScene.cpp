#include "PixelFadeScene.h"

PixelFadeScene::PixelFadeScene(){}
PixelFadeScene::~PixelFadeScene(){}

void PixelFadeScene::setup(ofRectangle &posAndSize){
    nextFlash = ofGetElapsedTimeMillis()+500;
    mState = INITIAL;
    canvasPositionAndSize = ofRectangle(posAndSize);
    tempFbo.allocate(canvasPositionAndSize.width, canvasPositionAndSize.height);
}

void PixelFadeScene::update(ofxEdsdk::Camera &camera){
    // state transitions
    if ((mState == INITIAL) && (ofGetElapsedTimeMillis() > 1500)) {
        camera.focusFrame();
        mState = WAITING;
    }
    else if (mState == WAITING) {
        if((camera.getFocusState() == ofxEdsdk::Camera::OFX_EDSDK_FOCUS_OK) ||
           (camera.getFocusState() == ofxEdsdk::Camera::OFX_EDSDK_FOCUS_FAIL)) {
            mState = FLASHING_IN;
            flashValue = 1.0;
            stayWhiteCount = 0;
            camera.takePhotoAF();
        }
    }
    else if (mState == FLASHING_IN) {
        flashValue = min(flashValue+50.0, 255.0);
        if ((flashValue >= 255) && (stayWhiteCount>4)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            mState = FLASHING_OUT;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == FLASHING_OUT) {
        flashValue = min(flashValue+60.0, 0.0);
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
            float sFactor = max((float)(canvasPositionAndSize.width)/mFoto.width, (float)(canvasPositionAndSize.height)/mFoto.height);
            mFoto.resize(sFactor*mFoto.width, sFactor*mFoto.height);
            // first color to Fade
            ofColor rColor = mFoto.getColor(ofRandom(mFoto.width), ofRandom(mFoto.height));
            //find a color to fade
            findSimilarColors(rColor, mFoto);
            mState = FADING_PICTURE_IN;
            camera.focusFrame();
        }
    }
    else if (mState == FADING_PICTURE_IN) {
        flashValue = min(flashValue+1.0, 255.0);
        if ((flashValue >= 255) && (stayWhiteCount>50)) {
            stayWhiteCount = 0;
            flashValue = -255.0;
            mState = FADING_PICTURE_OUT;
        }
        else if (flashValue >= 255) {
            stayWhiteCount++;
        }
    }
    else if (mState == FADING_PICTURE_OUT) {
        if (fadeImage(mFoto) == false) {
            mState = CLEARING_PICTURE;
        }
    }
    else if (mState == CLEARING_PICTURE) {
        flashValue = min(flashValue+4.0, 0.0);
        if (flashValue >= 0.0) {
            mState = WAITING;
            nextFlash = ofGetElapsedTimeMillis()+2000;
        }
    }

}

void PixelFadeScene::draw(ofImage &canvas){
    // update images, drawings, graphics, etc...
    if ((mState == INITIAL) || (mState == WAITING) || (mState == FLASHING_IN) ||
        (mState == FLASHING_OUT) || (mState == WAITING_FOR_PICTURE)) {
        tempFbo.begin();
        ofBackground(ofColor(abs(flashValue)));
        tempFbo.end();
    }
    else if ((mState == FADING_PICTURE_IN) || (mState == FADING_PICTURE_OUT) || (mState == CLEARING_PICTURE)) {
        // draw to temp fbo with a tint
        tempFbo.begin();
        ofBackground(0);
        ofSetColor(ofColor(abs(flashValue)));
        mFoto.draw(0,0);
        tempFbo.end();
    }
    tempFbo.readToPixels(canvas.getPixelsRef());
}

void PixelFadeScene::findSimilarColors(ofColor &c, ofImage &p) {
    pixelsToFade.clear();
    colorsToRand.clear();

    for (int y=0; y<p.height; y++) {
        for (int x=0; x<p.width; x++) {
            ofColor pixelColor = p.getColor(x, y);
            ofVec3f rgb(pixelColor.r, pixelColor.g, pixelColor.b);
            ofVec3f rgb2(c.r, c.g, c.b);

            if(rgb.distanceSquared(rgb2) < 10000){
                pixelsToFade.push_back(ofVec2f(x,y));
            }
            else if (colorsToRand.size() < 255) {
                colorsToRand.push_back(p.getColor(x,y));
            }
        }
    }
}

bool PixelFadeScene::fadeImage(ofImage &p) {
    bool randColor = false;

    for(vector<ofVec2f>::iterator it=pixelsToFade.begin(); it!=pixelsToFade.end(); ++it){
        ofColor pixelColor = p.getColor(it->x, it->y);

        pixelColor = ofColor(min(pixelColor.r+6, 255), min(pixelColor.g+6, 255), min(pixelColor.b+6, 255));

        randColor = (pixelColor.r >= 255 && pixelColor.g >= 255 && pixelColor.b >= 255);
        p.setColor(it->x, it->y, pixelColor);
    }
    p.reloadTexture();

    if (randColor && colorsToRand.size() > 0) {
        ofColor rColor = colorsToRand.at(ofRandom(colorsToRand.size()));
        findSimilarColors(rColor, p);
    }

    return (colorsToRand.size() > 200);
}