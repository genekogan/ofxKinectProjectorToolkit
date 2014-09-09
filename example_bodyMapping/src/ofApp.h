#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxSecondWindow.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxUI.h"


// this must match the display resolution of your projector
#define PROJECTOR_RESOLUTION_X 1280
#define PROJECTOR_RESOLUTION_Y 800


using namespace ofxCv;
using namespace cv;


class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
    ofxSecondWindow             secondWindow;
    
    ofxUICanvas                 *gui;
    ofColor                     blobColors[12];
    
	ofxCv::ContourFinder        contourFinder;
	ofxKinectProjectorToolkit   kpt;
    ofxKinect                   kinect;
    
    ofxCvGrayscaleImage         bgImage;
    ofxCvGrayscaleImage         grayImage;
	ofxCvGrayscaleImage         grayThreshNear;
	ofxCvGrayscaleImage         grayThreshFar;
	
	float                       nearThreshold;
    float                       farThreshold;
    float                       minArea;
    float                       maxArea;
    float                       threshold;
    float                       persistence;
    float                       maxDistance;
};
