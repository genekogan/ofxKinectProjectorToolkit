#pragma once

#include "ofMain.h"
#include "matrix.h"
#include "matrix_qr.h"


class ofxKinectProjectorToolkit
{
public:
    ofxKinectProjectorToolkit();
    
    void calibrate(vector<ofVec3f> pairsKinect,
                   vector<ofVec2f> pairsProjector);
    
    ofVec2f getProjectedPoint(ofVec3f worldPoint);
    vector<ofVec2f> getProjectedContour(vector<ofVec3f> *worldPoints);
    
    vector<double> getCalibration();

    void loadCalibration(string path);
    void saveCalibration(string path);
    
    bool isCalibrated() {return calibrated;}
    
private:
    
    dlib::matrix<double, 0, 11> A;
    dlib::matrix<double, 0, 1> y;
    dlib::matrix<double, 11, 1> x;
    
    bool calibrated;
};