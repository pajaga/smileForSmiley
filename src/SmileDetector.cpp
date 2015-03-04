//
//  SmileDetector.cpp
//  smiles
//
//  Created by Pavle Petrovic on 11/9/14.
//
//

#include "SmileDetector.h"
#include <iostream>

int SmileDetector::smileCount = 0;

SmileDetector::SmileDetector() {
    smileThreshold = 0;
	smileAverageIndex = 0;
    
    for(int i=0; i<5; i++){
		smileAverage[i]=0;
	}
    
    smilesDetected = 0;
    smileTimeStamp = 0;
}

void SmileDetector::start() {
    vidGrabber.setVerbose(true);
    vidGrabber.setDesiredFrameRate(10);
    vidGrabber.initGrabber(1280, 720, false);
    colorImg.setUseTexture(false);
    grayImg.setUseTexture(false);
    saveImg.setUseTexture(false);

    colorImg.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    grayImgFS.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    grayImg.allocate(320, 240);
    saveImg.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_GRAYSCALE);
	
	// Initialize MPSmile class
    pCSmile = new MPSmile();
    if( pCSmile == NULL ){
        cout << " new MPSmile error\n" << endl;
        return NULL;
    }
    
    RImage_pixels = new RImage<float>(grayImg.width, grayImg.height);
    
    startThread();
}

void SmileDetector::stop() {
    stopThread();
    vidGrabber.close();
}

bool SmileDetector::detectSmiles(ofxCvColorImage *img)
{
    int width = grayImg.width;
    int height = grayImg.height;

    grayImgFS = *img;
    grayImg.scaleIntoMe(grayImgFS);

    unsigned char *detectionPixels = grayImg.getPixels();

    int totalDetectionPixels = width * height;
    for (int i = 0; i < totalDetectionPixels; i++){
        RImage_pixels->array[i] = detectionPixels[i];
    }
    
    smileThreshold_prev = smileThreshold;
    
    faces.clear();
    
	// Find Smiles
	if( pCSmile->findSmiles(*RImage_pixels, faces, 1.00, wt_avg ) )
	{
        //printf(" findSmiles error\n");
        return;
	}
    
    smileResponse = 0;
    // The result is in faces(VisualObject).
    if(faces.size() != 0) {
        
        list<VisualObject *>::iterator it = faces.begin();
        //int facenumber=0;
        for( ; it != faces.end(); ++it) {
            face = static_cast<FaceObject*>(*it);
            
            smileResponse = face->activation;
            float mylimit = 5.0;
            if(smileResponse > mylimit) smileResponse = mylimit;
            if(smileResponse < 0) smileResponse = 0;
            smileResponse = (smileResponse / mylimit);
			
        }
    }
    
	smileAverage[smileAverageIndex] = smileResponse;
	smileAverageIndex++;
	if(smileAverageIndex==AVG_COUNT) smileAverageIndex=0;
	
	double smileSum = 0;
	for(int i=0;i<AVG_COUNT;i++)
	{
		smileSum += smileAverage[i];
	}
	
	smileThreshold = smileSum / (float)AVG_COUNT;
    
    int smile = 0;
    if (smileThreshold > SMILE_THR) {
        if (smileThreshold_prev < SMILE_THR) {
            smileTimeStamp = ofGetElapsedTimef();
            smile = 1;
            // if new smile is detected take a picture
            saveImg.setFromPixels(img->getPixels(), img->width, img->height, OF_IMAGE_COLOR);
            char imgFilename[40];
            snprintf(imgFilename, sizeof(imgFilename), "smile_%s.png", ofGetTimestampString().c_str());
            saveImg.saveImage(imgFilename);
        } else {
            if (smileTimeStamp < ofGetElapsedTimef() - SMILE_TIME) {
                smileTimeStamp = ofGetElapsedTimef();
                smile = 1;
            }
        }
    }
    
    return smile;
}

void SmileDetector::threadedFunction() {
    while(isThreadRunning() != 0 ){
        vidGrabber.update();
        if (lock()) {
            if (vidGrabber.isFrameNew()) {
                colorImg.setFromPixels(vidGrabber.getPixelsRef());
                smilesDetected += detectSmiles(&colorImg);
            }
            unlock();
        }
        sleep(33);
    }
}

bool SmileDetector::isDetected() {
    if (smilesDetected > 0) {
        smilesDetected--;
        return true;
    } else {
        return false;
    }
}
