//
//  Smiley.h
//  smiles
//
//  Created by Pavle Petrovic on 11/8/14.
//
//

#pragma once

#include "ofMain.h"

#define MAX_SMILIES      10
#define MAX_TRAIL_LENGTH 10
#define FADE_LENGTH      1
#define MOVE_STEP        10
#define SMILEY_ORIGINAL_WIDTH  1920
#define SMILEY_ORIGINAL_HEIGHT 1080
#define SMILEY_ORIGINAL_OFFSET_X 827
#define SMILEY_ORIGINAL_OFFSET_Y 353

class Smiley {
    
    public:
    
        Smiley();
        
        void update();
        void draw();
        void move();
        Smiley* getNext();
        bool isDead();
        void setNext(Smiley *s);
        static void resize(int w, int h);
        static void setImage(string s);
        static void setMode(int m);
        void windowResized();
    
    private:
    
        static ofImage smileyImgOriginal;
        static ofImage smileyImg;
        static int smileysId;
        int id;
        static int mode;
    
        float transparency;
        ofPolyline trail;
        int trailPos;
        ofPoint currentPos;
        int fading; // fade in = 1; stay = 0; fade out = -1;
        bool dead;
        
        Smiley *next;
};