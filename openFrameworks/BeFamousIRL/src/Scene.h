#pragma once

#include "ofxEdsdk.h"

class Scene {
    public:
        virtual ~Scene() {}
        virtual void setup(const ofVec2f &canvasSize) = 0;
        virtual void update(ofxEdsdk::Camera &camera) = 0;
        virtual void draw(ofImage &canvas) = 0;
};
