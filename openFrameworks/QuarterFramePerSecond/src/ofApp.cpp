#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);

    syphonServer.setName("Syphon Output");
    mCamera.setup();

    mPanelPositionAndSize = ofRectangle(37,259, 215, 168);

    mCanvas.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mPanels.allocate(mPanelPositionAndSize.width, mPanelPositionAndSize.height, OF_IMAGE_COLOR);
    mPanels.setColor(ofColor(0));

    panelsMask.loadImage("SP_Urban_MASK_025.png");
    panelsMask.crop(mPanelPositionAndSize.x, mPanelPositionAndSize.y, mPanelPositionAndSize.width, mPanelPositionAndSize.height);

    fiespMask.loadImage("SP_Urban_MASK_025.png");

    ////////////////////
    gifScene = new GifScene();
    gifScene->setup(mPanelPositionAndSize);

    fadingGifScene = new FadingGifScene();
    fadingGifScene->setup(mPanelPositionAndSize);

    blankScene = new BlankScene();
    blankScene->setup(mPanelPositionAndSize);

    mScene = fadingGifScene;
    mSceneString = STRING_FADING_GIF_SCENE;

    bPlayAudio = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    mCamera.update();

    mScene->update(mCamera);
    mScene->draw(mCanvas);

    mCanvas.reloadTexture();
    toPanels(mCanvas, mPanels);
    syphonServer.publishScreen();
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);

    mCanvas.draw(400,14);
    mPanels.draw(400,259);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10,10, ofColor(255,0,255), ofColor(255,255,0));

    fiespMask.draw(0,0);
    mPanels.draw(mPanelPositionAndSize.x,mPanelPositionAndSize.y);

    if(bPlayAudio){
        ofDrawBitmapStringHighlight("Audio LIGADO. Para desligar tecle '0'", 10,600, ofColor(0,200,0), ofColor(255,255,255));
    }
    else{
        ofDrawBitmapStringHighlight("Audio DESLIGADO. Para ligar tecle '0'", 10,600, ofColor(200,0,0), ofColor(255,255,255));
    }

    // which scene
    ofDrawBitmapStringHighlight("playing: "+mSceneString, 10,700, ofColor(200), ofColor(0));
}

void ofApp::toPanels(ofImage &canvas, ofImage &panels){
    if((canvas.getWidth() < panels.getWidth()) || (canvas.getHeight() < panels.getHeight()))
        return;

    for(int y=0; y<panels.getHeight(); y++){
        int leftOffset=0, rightOffset=0;
        int gapSize = max(0, (int)((0.0-74.0)/mPanels.getHeight()*y+72.0));

        for(int x=0; x<=panels.getWidth()/2; x++){
            // left
            if(panelsMask.getColor(panels.width/2-x, y) == ofColor::white){
                panels.setColor(panels.getWidth()/2-x, y, canvas.getColor(canvas.getWidth()/2-x+(leftOffset*gapSize), y));
            }
            else{
                leftOffset = 1;
            }

            // right
            if(panelsMask.getColor(panels.width/2+x, y) == ofColor::white){
                panels.setColor(panels.getWidth()/2+x, y, canvas.getColor(canvas.getWidth()/2+x-(rightOffset*gapSize), y));
            }
            else{
                rightOffset = 1;
            }
        }
    }
    panels.reloadTexture();
}

void ofApp::drawChessboard(ofImage& canvas){
    for(int y=0; y<canvas.getHeight(); y++) {
        int cy = (y/16)%2;
        for(int x=0; x<canvas.getWidth(); x++) {
            int cx = (x/16+cy)%2;
            canvas.setColor(x, y, ofColor(cx*255));
        }
    }
}

void ofApp::exit(){
    cout << "exit\n";
    delete mScene;
    ofBaseApp::exit();
}


void ofApp::keyPressed(int key){
    if(key == 'a' || key == 'A' || key == '0'){
        bPlayAudio = !bPlayAudio;
        mScene->setVolume(bPlayAudio*1.0f);
    }
    else if(key == '!'){
        mScene = fadingGifScene;
        mSceneString = STRING_FADING_GIF_SCENE;
    }
    else if(key == '@'){
        mScene = blankScene;
        mSceneString = STRING_BLANK_SCENE;
    }
    else if(key == '#'){
        mScene = gifScene;
        mSceneString = STRING_GIF_SCENE;
    }
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
