#pragma once

#include "ofxEdsdk.h"

class Scene {
    public:
        virtual ~Scene() {}
        virtual void setup(ofRectangle &posAndSize) = 0;
        virtual void update(ofxEdsdk::Camera &camera) = 0;
        virtual void draw(ofImage &canvas) = 0;
    protected:
        static vector<ofSoundPlayer> shutterSounds;
        static vector<ofSoundPlayer> applauseSounds;
        static void loadSounds();
};
