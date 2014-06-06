#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup() {
    blobColors[0] = ofColor(255, 0, 0);
    blobColors[1] = ofColor(0, 255, 0);
    blobColors[2] = ofColor(0, 0, 255);
    blobColors[3] = ofColor(255, 255, 0);
    blobColors[4] = ofColor(255, 0, 255);
    blobColors[5] = ofColor(0, 127, 255);
    blobColors[6] = ofColor(0, 255, 127);
    blobColors[7] = ofColor(127, 0, 255);
    blobColors[8] = ofColor(255, 0, 127);
    blobColors[9] = ofColor(127, 255, 0);
    blobColors[10]= ofColor(255, 127, 0);

    kinect.setRegistration(true);
    kinect.init();
	kinect.open();
    
    kpt.loadCalibration("/Users/Gene/Desktop/calibration.xml");
    
    secondWindow.setup("main", ofGetScreenWidth(), 0, PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y, true);
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
    
    fbo.allocate(kinect.width, kinect.height, GL_RGB);
    fbo1.allocate(kinect.width, kinect.height, GL_RGBA);
    img.allocate(kinect.width, kinect.height, OF_IMAGE_COLOR_ALPHA);
    
    shader.load("standard.vert", "edges.frag");
    
    // setup gui
    gui = new ofxUICanvas();
    gui->setHeight(800);
    gui->setName("parameters");
    gui->addLabel("kinect");
    gui->addSpacer();
    gui->addSlider("fade", 0, 255, &fade);
    gui->addSlider("minArea", 0, 5000, &minArea);
    gui->addSlider("maxArea", 5000, 320000, &maxArea);
    gui->addSlider("threshold", 1, 100, &threshold);
    gui->addSlider("persistence", 1, 100, &persistence);
    gui->addSlider("maxDistance", 1, 100, &maxDistance);    
    gui->addSlider("numDilate", 0, 8, &numDilate);
    gui->addSlider("numErode", 0, 8, &numErode);
}

//--------------------------------------------------------------
void ofApp::update() {

    kinect.update();
	
    if(kinect.isFrameNew()) {
		
        // pre-processing depth image
        fbo1.begin();
        ofEnableAlphaBlending();
        ofSetColor(255, fade);
        kinect.drawDepth(0, 0);
        ofDisableAlphaBlending();
        fbo1.end();
        
        fbo.begin();
        shader.begin();
        fbo1.draw(0, 0);
        shader.end();
        fbo.end();
    
        fbo.readToPixels(pixels);
        img.setFromPixels(pixels);
        img.update();
        
        colorImg.setFromPixels(img.getPixelsRef());
        grayImage.setFromColorImage(colorImg);

        // alternative: Canny method
        /*
        convertColor(kinect, gray, CV_RGB2GRAY);
        Canny(gray, edge, mouseX, mouseY, 3);
        edge.update();
        colorImg.setFromPixels(edge.getPixelsRef());
        grayImage.setFromColorImage(colorImg);
        */
        
        // post-processing
        for (int i=0; i<numDilate; i++){
            grayImage.dilate_3x3();
        }
        for (int i=0; i<numErode; i++){
            grayImage.erode_3x3();
        }
        grayImage.invert();
        grayImage.threshold(250);

        contourFinder.setMinArea(minArea);
        contourFinder.setMaxArea(maxArea);
        contourFinder.setThreshold(threshold);
        contourFinder.getTracker().setPersistence(persistence);
        contourFinder.getTracker().setMaximumDistance(maxDistance);
        contourFinder.findContours(grayImage);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    
    ofPushMatrix();
    kinect.draw(0, 0);
    ofTranslate(640, 0);
    kinect.drawDepth(0, 0);
    ofTranslate(-640, 480);
    grayImage.draw(0, 0);
    ofTranslate(640, 0);
    //contourFinder.draw();
    
    RectTracker& tracker = contourFinder.getTracker();
	
    for(int i = 0; i < contourFinder.size(); i++) {
        // get contour, label, center point, and age of contour
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        vector<cv::Point> fitPoints = contourFinder.getFitQuad(i);
        cv::RotatedRect fitQuad = contourFinder.getFitEllipse(i);
        
        // draw contours
        ofFill();
        ofSetColor(blobColors[label % 11]);
        ofBeginShape();
        for (int j=0; j<points.size(); j++) {
            ofVertex(points[j].x, points[j].y);
        }
        ofEndShape();
        
        ofNoFill();
        ofSetLineWidth(3);
        
        // draw approximate contour
        ofBeginShape();
        for (int j=0; j<fitPoints.size(); j++) {
            ofVertex(fitPoints[j].x, fitPoints[j].y);
        }
        ofEndShape();
    }
    ofPopMatrix();
    
    
    secondWindow.begin();
    ofBackground(0);    
	
    for(int i = 0; i < contourFinder.size(); i++) {
        // get contour, label, center point, and age of contour
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        
        // map contour using calibration and draw to main window
        ofBeginShape();
        ofFill();
        ofSetColor(blobColors[label % 11]);
        for (int j=0; j<points.size(); j++) {
            ofVec3f wp = kinect.getWorldCoordinateAt(points[j].x, points[j].y);
            ofVec2f pp = kpt.getProjectedPoint(wp);
            ofVertex(
                     ofMap(pp.x, 0, 1, 0, secondWindow.getWidth()),
                     ofMap(pp.y, 0, 1, 0, secondWindow.getHeight())
            );
        }
        ofEndShape();
    }
    
    secondWindow.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{}
