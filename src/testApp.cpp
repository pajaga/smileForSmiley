#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(30);
    
    titleImgOriginal.loadImage("smile_text.png");
    titleImg = titleImgOriginal;
    titleImg.resize((float)titleImg.width * 1280 / TITLE_ORIGINAL_WIDTH,
                    (float)titleImg.height * 720 / TITLE_ORIGINAL_HEIGHT);
    titleImg.setAnchorPoint(-(float)TITLE_ORIGINAL_OFFSET_X / TITLE_ORIGINAL_WIDTH * ofGetWindowWidth(),
                            -(float)TITLE_ORIGINAL_OFFSET_Y / TITLE_ORIGINAL_HEIGHT * ofGetWindowHeight());
    
    smileys = new Smiley();
    Smiley::setImage("smile_smiley.png");
    Smiley::resize(1280, 720);
    
    smileDetector.start();
}

//--------------------------------------------------------------
void testApp::update(){

    ofBackgroundHex(0xf7bf00);

    if (smileDetector.isDetected()) {
        if (smileys != NULL) {
            Smiley *s;
            for (s = smileys; s->getNext() != NULL; s = s->getNext());
            s->setNext(new Smiley());
            s->move();
        }
    }
    
    for (Smiley *s = smileys; s != NULL; s = s->getNext()) {
        s->update();
    }
    
    if (smileys != NULL) {
        if (smileys->isDead()) smileys = smileys->getNext();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    //draw the smile
    for (Smiley *s = smileys; s != NULL; s = s->getNext()) {
        s->draw();
    }

    ofSetColor(255);
    ofEnableAlphaBlending();
    titleImg.draw(0, 0);
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    float desiredAspect = (float)TITLE_ORIGINAL_WIDTH / TITLE_ORIGINAL_HEIGHT;
    int newHeight = w / desiredAspect;
    titleImg = titleImgOriginal;

    titleImg.resize((float)titleImg.width * w / TITLE_ORIGINAL_WIDTH,
                    (float)titleImg.height * newHeight / TITLE_ORIGINAL_HEIGHT);
    Smiley::resize(w, newHeight);

    // If not fullscreen adjust window size to constrain proportions
    if (!(w == ofGetScreenWidth() && h == ofGetScreenHeight())) {
        ofSetWindowShape(w, newHeight);
    }

    titleImg.setAnchorPoint(-(float)TITLE_ORIGINAL_OFFSET_X / TITLE_ORIGINAL_WIDTH * ofGetWindowWidth(),
                            -(float)TITLE_ORIGINAL_OFFSET_Y / TITLE_ORIGINAL_HEIGHT * ofGetWindowHeight());
    for (Smiley *s = smileys; s != NULL; s = s->getNext()) {
        s->windowResized();
    }
}

void testApp::keyPressed(int key){
    switch (key) {
        case 'F': case 'f': ofToggleFullscreen(); break;
        case 'Q': case 'q': smileDetector.stop(); ofExit(); break;
        case '1': Smiley::setMode(1); break;
        case '2': Smiley::setMode(2); break;
        default: break;
    }
}
