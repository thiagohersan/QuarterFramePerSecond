#include "GifScene.h"

GifScene::GifScene(){}
GifScene::~GifScene(){}

void GifScene::setup(ofRectangle &posAndSize){
    PixelFadeScene::setup(posAndSize);
    mState = INITIAL;
}
