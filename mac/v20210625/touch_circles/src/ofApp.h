#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "CustomCircle.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void setCircles();
    void setBox2D();
    void setVideoGrabber();
    void allocateOfxCvImages();
    void makeImageFromVideo();
    void findContours();
    void makeContourCircles();
    void drawContours();
    void drawCropRect();
    void drawInformation();
    void drawCircles();
    void saveCropInfo();
    void loadCropInfo();
    void saveSettings();
    void loadSettings();

    ofVideoGrabber vidGrabber;
//    ofxCvColorImage colorImg0;
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImg;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
    ofxCvContourFinder contourFinder;
    int imgW = 320;
    int imgH = 240;
    int numOfCircles = 100;
    int cameraId = 0;
    
    int threshold = 30;
    bool bLearnBackground = true;
    bool isDebugMode = true;
    bool isCropMode = false;
    float cropRatioX = 0.25f;
    float cropRatioY = 0.25f;
    float cropRatioW = 0.5f;
    float cropRatioH = 0.5f;
    float cropRatioTempX = 0.f;
    float cropRatioTempY = 0.f;
    float cropRatioTempW = 1.f;
    float cropRatioTempH = 1.f;
    bool isCalibrationMode = false;
    int calibrationPointCount = 0;
    float circleRadius = 10;

    ofJson cropJs;
    string cropJsFilename = "cropInfo.json";
    ofJson settingsJs;
    string settingsJsFilename = "settings.json";

    ofxBox2d box2d;
    vector <shared_ptr <CustomCircle>> circles;
    vector <shared_ptr <ofxBox2dCircle>> contourCircles;
};
