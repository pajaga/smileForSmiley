//
//  SmileDetector.h
//  smiles
//
//  Created by Pavle Petrovic on 11/9/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "smile.h"

#define SMILE_THR 0.3
#define SMILE_TIME 0.5 //in seconds
#define AVG_COUNT 5

class SmileDetector : public ofThread {
    
    public:
    
        SmileDetector();
        void threadedFunction();
        bool isDetected();
        void start();
        void stop();

    private:
    
        bool detectSmiles(ofxCvColorImage *img);
    
        ofVideoGrabber      vidGrabber;
        ofxCvColorImage     colorImg;
        ofxCvGrayscaleImage grayImgFS;
        ofxCvGrayscaleImage grayImg;
        ofImage             saveImg;

        static int          smileCount;

        RImage<float>*      RImage_pixels;
        unsigned char *     detectionPixels; // detection pixels

        MPSmile*            pCSmile;
        VisualObject        faces; // Smile Value
        float               smileResponse;
        float				smileAverage[AVG_COUNT];
        unsigned			smileAverageIndex;

        double              smileThreshold;
        double              smileThreshold_prev;
        float               smileTimeStamp;

        FaceObject          *face;
    
        int                 smilesDetected;

};