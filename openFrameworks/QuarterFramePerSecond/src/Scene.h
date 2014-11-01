#pragma once

#include "ofxEdsdk.h"

class Scene: public ofBaseSoundOutput {
    public:
        virtual ~Scene() {}
        virtual void setup(ofRectangle &posAndSize) = 0;
        virtual void update(ofxEdsdk::Camera &camera) = 0;
        virtual void draw(ofImage &canvas) = 0;
    protected:
        float flickerPeriod;
        virtual void audioOut(float* output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) = 0;
};
