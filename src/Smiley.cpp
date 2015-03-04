//
//  Smiley.cpp
//  smiles
//
//  Created by Pavle Petrovic on 11/8/14.
//
//

#include "Smiley.h"

ofImage Smiley::smileyImgOriginal;
ofImage Smiley::smileyImg;
int Smiley::smileysId = 0;
int Smiley::mode = 1;

// Smiley class constructor
Smiley::Smiley() {
    id = smileysId++;
    
    transparency = -0.5;
    trailPos = 0;
    fading = 1;
    next = NULL;
    dead = false;
    
    // set default position according to design
    currentPos.x = (float)SMILEY_ORIGINAL_OFFSET_X / SMILEY_ORIGINAL_WIDTH * ofGetWindowWidth();
    currentPos.y = (float)SMILEY_ORIGINAL_OFFSET_Y / SMILEY_ORIGINAL_HEIGHT * ofGetWindowHeight();
}

// Static function, set image for the Smiley
void Smiley::setImage(string s) {
    smileyImgOriginal.loadImage(s);
}

// Static function, resize all Smileys
void Smiley::resize(int w, int h) {
    smileyImg = smileyImgOriginal;
    smileyImg.resize((float)smileyImg.width * w / SMILEY_ORIGINAL_WIDTH,
                     (float)smileyImg.height * h / SMILEY_ORIGINAL_HEIGHT);
}

// Adapt current position of a Smiley if window is resized
void Smiley::windowResized() {
    if (trail.size() == 0) {
        currentPos.x = (float)SMILEY_ORIGINAL_OFFSET_X / SMILEY_ORIGINAL_WIDTH * ofGetWindowWidth();
        currentPos.y = (float)SMILEY_ORIGINAL_OFFSET_Y / SMILEY_ORIGINAL_HEIGHT * ofGetWindowHeight();
    } else if (trail.size() - trailPos > 1) {
        trail.clear();
        trailPos = 0;
        this->move();
    }
}

void Smiley::setMode(int m) {
    mode = m;
}

void Smiley::update() {
    // set transparency
    transparency += ofLerp(0, fading, 1/ofGetFrameRate());
    // if transaprency close to 1 and fading in set to 1
    if (transparency > 0.95 && fading == 1) {
        transparency = 1;
        fading = 0;
    }
    // if transparency close to 0 while fading out set to 0
    if (transparency < 0.05 && fading == -1) {
        transparency = 0;
        fading = 0;
        dead = true;
    }
    // set new position for the smiley
    if (trail.size()) {
        int endX = trail.getVertices()[trailPos+1].x;
        int endY = trail.getVertices()[trailPos+1].y;
        int moveX = abs(endX - currentPos.x) > MOVE_STEP ? MOVE_STEP : abs(endX - currentPos.x);
        int moveY = abs(endY - currentPos.y) > MOVE_STEP ? MOVE_STEP : abs(endY - currentPos.y);
        currentPos.x += ofSign(endX - currentPos.x) * moveX;
        currentPos.y += ofSign(endY - currentPos.y) * moveY;
        // if new position is achieved aim for the next position
        if (moveX < MOVE_STEP && moveY < MOVE_STEP) trailPos++;
        // if all positions are done clear trail and fade out
        if (trailPos == trail.size() - 1) {
            fading = -1;
            trail.clear();
        }
    }
}

void Smiley::draw() {
    //draw the smile
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, transparency * 255);
    smileyImg.draw(currentPos.x, currentPos.y);
    ofDisableAlphaBlending();
}

void Smiley::move() {
    int width = ofGetWindowWidth();
    int height = ofGetWindowHeight();
    
    if (mode == 1) {
        int length = (int)ofRandom(2, MAX_TRAIL_LENGTH - 1);
        int chooseFirstMove = (ofRandomuf() > 0.5);
        trail.addVertex(currentPos);
        for (int i = 0; i < length; i++) {
            if ((i + chooseFirstMove) % 2) {
                trail.addVertex(ofRandom(0, ofGetWindowWidth() - smileyImg.width), trail.getVertices()[trail.size()-1].y);
            } else {
                trail.addVertex(trail.getVertices()[trail.size()-1].x, ofRandom(0, ofGetWindowHeight() - smileyImg.height));
            }
        }
    } else if (mode == 2) {
        int chooseDirection = (ofRandomuf() * 80);
        ofPoint zeroPos;
        zeroPos.x = (float)SMILEY_ORIGINAL_OFFSET_X / SMILEY_ORIGINAL_WIDTH * ofGetWindowWidth();
        zeroPos.y = (float)SMILEY_ORIGINAL_OFFSET_Y / SMILEY_ORIGINAL_HEIGHT * ofGetWindowHeight();
        trail.addVertex(currentPos);
        if (chooseDirection < 10) {
            trail.addVertex(0, 0);
        } else if (chooseDirection < 20) {
            trail.addVertex(zeroPos.x, 0);
        } else if (chooseDirection < 30) {
            trail.addVertex(ofGetWindowWidth() - smileyImg.width, 0);
        } else if (chooseDirection < 40) {
            trail.addVertex(ofGetWindowWidth() - smileyImg.width, zeroPos.y);
        } else if (chooseDirection < 50) {
            trail.addVertex(ofGetWindowWidth() - smileyImg.width, ofGetWindowHeight() - smileyImg.height);
        } else if (chooseDirection < 60) {
            trail.addVertex(zeroPos.x, ofGetWindowHeight() - smileyImg.height);
        } else if (chooseDirection < 70) {
            trail.addVertex(0, ofGetWindowHeight() - smileyImg.height);
        } else if (chooseDirection < 80) {
            trail.addVertex(0, zeroPos.y);
        } else {
            cout << "Wrong direction!" << endl;
        }
    }
}

Smiley* Smiley::getNext() {
    return next;
}

void Smiley::setNext(Smiley *s) {
    next = s;
}

bool Smiley::isDead() {
    return dead;
}