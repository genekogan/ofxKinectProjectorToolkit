#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;

    settings.width = 1024;
    settings.height = 768;
    settings.setPosition(ofVec2f(100, 100));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    settings.width = PROJECTOR_RESOLUTION_X;
    settings.height = PROJECTOR_RESOLUTION_Y;
    settings.setPosition(ofVec2f(ofGetScreenWidth(), 0));
    settings.resizable = false;
    settings.decorated = false;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(settings);
    secondWindow->setVerticalSync(false);

    shared_ptr<ofApp> mainApp(new ofApp);
    ofAddListener(secondWindow->events().draw, mainApp.get(), &ofApp::drawSecondWindow);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
