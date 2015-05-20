#include "ofApp.h"


void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetWindowShape(1280, 960);
    
    blobColors[0] = ofColor(255, 0, 0);
    blobColors[1] = ofColor(0, 255, 0);
    blobColors[2] = ofColor(0, 0, 255);
    blobColors[3] = ofColor(255, 255, 0);
    blobColors[4] = ofColor(255, 0, 255);
    blobColors[5] = ofColor(0, 255, 255);
    blobColors[6] = ofColor(255, 127, 0);
    blobColors[7] = ofColor(127, 0, 255);
    blobColors[8] = ofColor(0, 255, 127);
    blobColors[9] = ofColor(127, 255, 0);
    blobColors[10]= ofColor(255, 0, 127);
    blobColors[11]= ofColor(0, 127, 255);
    
    // set up kinect
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    kpt.loadCalibration("/Users/Gene/Desktop/calibration.xml");
    
    projector.setup("main", ofGetScreenWidth(), 0, PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y, true);
    
    // setup gui
    gui.setup("parameters");
    gui.add(nearThreshold.set("nearThresh", 230, 0, 255));
    gui.add(farThreshold.set("farThresh", 10, 0, 255));
    gui.add(minArea.set("minArea", 1000, 0, 5000));
    gui.add(maxArea.set("maxArea", 70000, 15000, 150000));
    gui.add(threshold.set("threshold", 15, 1, 100));
    gui.add(persistence.set("persistence", 15, 1, 100));
    gui.add(maxDistance.set("maxDistance", 32, 1, 100));
}

void ofApp::update() {
    kinect.update();
    
    if(kinect.isFrameNew()) {
        // process kinect depth image
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        grayImage.flagImageChanged();
        
        // set contour tracker parameters
        contourFinder.setMinArea(minArea);
        contourFinder.setMaxArea(maxArea);
        contourFinder.setThreshold(threshold);
        contourFinder.getTracker().setPersistence(persistence);
        contourFinder.getTracker().setMaximumDistance(maxDistance);
        
        // determine found contours
        contourFinder.findContours(grayImage);
    }
}

void ofApp::draw() {
    // GUI
    ofBackground(0);
    ofSetColor(255);
    ofPushMatrix();
    kinect.draw(0, 0);
    ofTranslate(640, 0);
    grayImage.draw(0, 0);
    ofTranslate(-640, 480);
    contourFinder.draw();
    ofTranslate(640, 0);
    ofPopMatrix();
    
    gui.draw();
    
    // MAIN WINDOW
    projector.begin();
    
    ofBackground(0);
    
    RectTracker& tracker = contourFinder.getTracker();
    
    for(int i = 0; i < contourFinder.size(); i++) {
        // get contour, label, center point, and age of contour
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        
        // map contour using calibration and draw to main window
        ofBeginShape();
        ofFill();
        ofSetColor(blobColors[label % 12]);
        for (int j=0; j<points.size(); j++) {
            ofVec3f worldPoint = kinect.getWorldCoordinateAt(points[j].x, points[j].y);
            ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
            ofVertex(projector.getWidth() * projectedPoint.x, projector.getHeight() * projectedPoint.y);
        }
        ofEndShape();
    }
    
    projector.end();
}

void ofApp::keyPressed(int key) {
    
}