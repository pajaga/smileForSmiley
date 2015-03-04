#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "Smiley.h"
#include "SmileDetector.h"
#include <deque>

#define TITLE_ORIGINAL_WIDTH  1920
#define TITLE_ORIGINAL_HEIGHT 1080
#define TITLE_ORIGINAL_OFFSET_X 735
#define TITLE_ORIGINAL_OFFSET_Y 604

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

        void keyPressed(int key);
		void windowResized(int w, int h);

        ofImage             titleImgOriginal;
        ofImage             titleImg;

        Smiley              *smileys;
        SmileDetector       smileDetector;
};

#endif
