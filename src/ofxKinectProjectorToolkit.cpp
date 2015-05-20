#include "ofxKinectProjectorToolkit.h"


ofxKinectProjectorToolkit::ofxKinectProjectorToolkit() {
    calibrated = false;
}

void ofxKinectProjectorToolkit::calibrate(vector<ofVec3f> pairsKinect,
                                          vector<ofVec2f> pairsProjector) {
    int nPairs = pairsKinect.size();
    A.set_size(nPairs*2, 11);
    y.set_size(nPairs*2, 1);
    
    for (int i=0; i<nPairs; i++) {
        A(2*i, 0) = pairsKinect[i].x;
        A(2*i, 1) = pairsKinect[i].y;
        A(2*i, 2) = pairsKinect[i].z;
        A(2*i, 3) = 1;
        A(2*i, 4) = 0;
        A(2*i, 5) = 0;
        A(2*i, 6) = 0;
        A(2*i, 7) = 0;
        A(2*i, 8) = -pairsKinect[i].x * pairsProjector[i].x;
        A(2*i, 9) = -pairsKinect[i].y * pairsProjector[i].x;
        A(2*i, 10) = -pairsKinect[i].z * pairsProjector[i].x;
        
        A(2*i+1, 0) = 0;
        A(2*i+1, 1) = 0;
        A(2*i+1, 2) = 0;
        A(2*i+1, 3) = 0;
        A(2*i+1, 4) = pairsKinect[i].x;
        A(2*i+1, 5) = pairsKinect[i].y;
        A(2*i+1, 6) = pairsKinect[i].z;
        A(2*i+1, 7) = 1;
        A(2*i+1, 8) = -pairsKinect[i].x * pairsProjector[i].y;
        A(2*i+1, 9) = -pairsKinect[i].y * pairsProjector[i].y;
        A(2*i+1, 10) = -pairsKinect[i].z * pairsProjector[i].y;
        
        y(2*i, 0) = pairsProjector[i].x;
        y(2*i+1, 0) = pairsProjector[i].y;
    }
    
    dlib::qr_decomposition<dlib::matrix<double, 0, 11> > qrd(A);
    x = qrd.solve(y);
    calibrated = true;
}

ofVec2f ofxKinectProjectorToolkit::getProjectedPoint(ofVec3f worldPoint) {
    float a = x(0, 0)*worldPoint.x + x(1, 0)*worldPoint.y + x(2, 0)*worldPoint.z + x(3,0);
    float b = x(4, 0)*worldPoint.x + x(5, 0)*worldPoint.y + x(6, 0)*worldPoint.z + x(7,0);
    float c = x(8, 0)*worldPoint.x + x(9, 0)*worldPoint.y + x(10, 0)*worldPoint.z + 1;
    ofVec2f projectedPoint(a/c, b/c);
    return projectedPoint;
}

vector<double> ofxKinectProjectorToolkit::getCalibration()
{
    vector<double> coefficients;
    for (int i=0; i<11; i++) {
        coefficients.push_back(x(i, 0));
    }
    return coefficients;
}

void ofxKinectProjectorToolkit::loadCalibration(string path){
    ofXml xml;
    xml.load(path);    
    xml.setTo("CALIBRATION");
    for (int i=0; i<11; i++) {
        x(i, 0) = xml.getValue<float>("COEFF"+ofToString(i));
    }
    calibrated = true;
}

void ofxKinectProjectorToolkit::saveCalibration(string path){
    ofXml xml;
    xml.addChild("CALIBRATION");
    xml.setTo("CALIBRATION");
    for (int i=0; i<11; i++) {
        ofXml coeff;
        coeff.addValue("COEFF"+ofToString(i), x(i, 0));
        xml.addXml(coeff);
    }
    xml.setToParent();
    xml.save(path);
}


