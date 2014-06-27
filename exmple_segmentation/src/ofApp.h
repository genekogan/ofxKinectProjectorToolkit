#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxSecondWindow.h"

// this must match the display resolution of your projector
#define PROJECTOR_RESOLUTION_X 1280
#define PROJECTOR_RESOLUTION_Y 800


class ofApp : public ofBaseApp
{
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
    ofxKinectProjectorToolkit kpt;
    ofxSecondWindow         secondWindow;
    ofxUICanvas             *gui;

    ofColor                 blobColors[11];
    
	ofxKinect               kinect;

	ofxCvColorImage         colorImg;
	ofxCvGrayscaleImage     grayImage;
    
    ofxCv::ContourFinder    contourFinder;
	
    ofShader                shader;
    ofFbo                   fbo, fbo1;
    ofImage                 img;
    
	ofPixels                gray;
	ofImage                 edge;


    ofPixels                pixels;
    
    float                   fade;
    float                   minArea;
    float                   maxArea;
    float                   threshold;
    float                   persistence;
    float                   maxDistance;
    float                   numDilate;
    float                   numErode;

};
