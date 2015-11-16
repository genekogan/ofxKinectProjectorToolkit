#pragma once

#include "ofMain.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"


// this must match the display resolution of your projector
#define PROJECTOR_RESOLUTION_X 1280
#define PROJECTOR_RESOLUTION_Y 800


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void drawSecondWindow(ofEventArgs& args);
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);

    void drawChessboard(int x, int y, int chessboardSize);
    void drawTestingPoint(ofVec2f projectedPoint);
    void addPointPair();
    
    ofxKinect                   kinect;
    ofxKinectProjectorToolkit   kpt;

    ofFbo                       fboChessboard;
    ofxCvColorImage             rgbImage;
    cv::Mat                     cvRgbImage;

    vector<ofVec2f>             currentProjectorPoints;
    vector<cv::Point2f>         cvPoints;
    vector<ofVec3f>             pairsKinect;
    vector<ofVec2f>             pairsProjector;

    string                      resultMessage;
    ofColor                     resultMessageColor;
    ofVec2f                     testPoint;

    int                         chessboardSize;
    int                         chessboardX;
    int                         chessboardY;
    bool                        testing;
    bool                        saved;
};


