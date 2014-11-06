#include "Scene.h"

vector<ofSoundPlayer> Scene::shutterSounds;
vector<ofSoundPlayer> Scene::applauseSounds;

void Scene::loadSounds(){
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
