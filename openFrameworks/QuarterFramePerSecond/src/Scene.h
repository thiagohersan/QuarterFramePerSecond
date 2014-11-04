#pragma once

#include "ofxEdsdk.h"

class Scene {
    public:
        virtual ~Scene() {}
        virtual void setup(ofRectangle &posAndSize) = 0;
        virtual void update(ofxEdsdk::Camera &camera) = 0;
        virtual void draw(ofImage &canvas) = 0;
    protected:
        vector<ofSoundPlayer> shutterSounds;
        vector<ofSoundPlayer> applauseSounds;
        void loadSounds(){
            ofDirectory soundsDir("sounds/shutter");
            soundsDir.allowExt("wav");
            soundsDir.listDir();
            for(int i=0; i < soundsDir.size(); i++){
                ofSoundPlayer s;
                s.loadSound(ofToDataPath(soundsDir.getPath(i)));
                s.setMultiPlay(true);
                shutterSounds.push_back(s);
            }

            soundsDir.open("sounds/applause");
            soundsDir.allowExt("wav");
            soundsDir.listDir();
            for(int i=0; i < soundsDir.size(); i++){
                ofSoundPlayer s;
                s.loadSound(ofToDataPath(soundsDir.getPath(i)));
                s.setMultiPlay(true);
                applauseSounds.push_back(s);
            }
        }
};
