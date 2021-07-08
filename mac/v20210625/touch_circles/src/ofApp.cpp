#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    loadSettings();
    ofBackground(0);
    setVideoGrabber();
    allocateOfxCvImages();
    setBox2D();
    setCircles();
    loadCropInfo();
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    
    bool bNewFrame = false;
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    if(bNewFrame){
        makeImageFromVideo();
        findContours();
        makeContourCircles();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    float ratioX = (float)ofGetWidth()/(float)grayDiff.width;
    float ratioY = (float)ofGetHeight()/(float)grayDiff.height;

    ofPushMatrix();
    ofScale(ratioX,ratioY); //scale!!
    ofSetColor(255);
    colorImg.draw(0,0);
    drawCircles();
    drawContours();
    drawCropRect();
    ofPopMatrix();

    drawInformation();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key==' '){
        bLearnBackground = true;
        std::cout << "Background learned" << endl;
    } else if(key=='c'){
        setCircles();
        std::cout << "circles are reset" << endl;
    } else if(key=='+'){
        threshold++;
        if(threshold>255){threshold=255;}
        std::cout << "threshold = " << threshold << endl;
        saveSettings();
    } else if(key=='-'){
        threshold--;
        if(threshold<0){threshold=0;}
        std::cout << "threshold = " << threshold << endl;
        saveSettings();
    } else if(key=='d'){
        if (isDebugMode) {
            isDebugMode = false;
        } else {
            isDebugMode = true;
        }
        saveSettings();
    } else if(key=='p'){
        if (isCropMode) {
            isCropMode = false;
        } else {
            isCropMode = true;
        }
        bLearnBackground = true;
    } else if(key=='o'){
        if (isCalibrationMode) {
            isCalibrationMode = false;
            std::cout << "calibration canceled" << endl;
        } else {
            isCalibrationMode = true;
            std::cout << "calibration started" << endl;
        }
        calibrationPointCount = 0;

        isCropMode = false;
        bLearnBackground = true;
    }

}


//--------------------------------------------------------------
void ofApp::setCircles(){
    circles.clear();
    for(int i=0; i<numOfCircles; i++){
        auto circle = std::make_shared<CustomCircle>();
        float density = 1.0;
        float bounce = 0.8;
        float friction = 0.0;
        circle.get() -> setPhysics(density, bounce, friction);
        float xPos = ofRandom(colorImg.width);
        float yPos = ofRandom(colorImg.height);
        float radius = circleRadius;
        circle.get() -> setup(box2d.getWorld(), xPos, yPos, radius);
        circles.push_back(circle);
    }
}

//--------------------------------------------------------------
void ofApp::setBox2D(){
    box2d.init();
    float gravityX = 0.0;
    float gravityY = 5.0;
    box2d.setGravity(gravityX, gravityY);
    box2d.createBounds(0,0,colorImg.width,colorImg.height);
    box2d.setFPS(60);
    box2d.checkBounds(true);
}

//--------------------------------------------------------------
void ofApp::setVideoGrabber(){
    int _numOfCameras = vidGrabber.listDevices().size();
    
    std::cout << "num of cameras = " << _numOfCameras << endl;
    if(_numOfCameras >= cameraId + 1){
        vidGrabber.setDeviceID(cameraId);
    }
    vidGrabber.initGrabber(imgW, imgH);
}

//--------------------------------------------------------------
void ofApp::allocateOfxCvImages(){
    colorImg.allocate(imgW, imgH);
    grayImg.allocate(imgW, imgH);
    grayBg.allocate(imgW, imgH);
    grayDiff.allocate(imgW, imgH);
}

//--------------------------------------------------------------
void ofApp::makeImageFromVideo(){
    ofImage img;
    img.setFromPixels(vidGrabber.getPixels());
    int _w0 = vidGrabber.getWidth();
    int _h0 = vidGrabber.getHeight();
    int _x, _y, _w, _h;
    if (isCropMode){
        _x = cropRatioX * _w0;
        _y = cropRatioY * _h0;
        _w = cropRatioW * _w0;
        _h = cropRatioH * _h0;
    }else{
        _x = 0;
        _y = 0;
        _w = _w0;
        _h = _h0;
    }
    img.crop(_x,_y,_w,_h);
    ofxCvColorImage cvImg;
    cvImg.allocate(_w,_h);
    cvImg.setFromPixels(img.getPixels());
    cvImg.resize(imgW, imgH);
    colorImg.setFromPixels(cvImg.getPixels());
}

//--------------------------------------------------------------
void ofApp::findContours(){
    grayImg = colorImg;
    if(bLearnBackground){
        grayBg = grayImg;
        bLearnBackground = false;
    }
    grayDiff.absDiff(grayBg,grayImg);
    grayDiff.threshold(threshold);
    
    contourFinder.findContours(grayDiff,20,imgW*imgH/3,10,true);
}

//--------------------------------------------------------------
void ofApp::makeContourCircles(){
    for(int i=0; i<contourCircles.size(); i++){
        contourCircles[i] -> destroy();
    }
    contourCircles.clear();
    
    for(int i=0; i<contourFinder.nBlobs; i++){
        for(int j=0; j<contourFinder.blobs[i].pts.size(); j+=4){
            ofPoint pos = contourFinder.blobs[i].pts[j];
            auto circle = std::make_shared<ofxBox2dCircle>();
            circle.get() -> setup(box2d.getWorld(), pos.x, pos.y, 10);
            contourCircles.push_back(circle);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawContours(){
    if (isDebugMode){
        contourFinder.draw(0,0);
        ofNoFill();
        ofSetColor(255,0,0);
        for(int i=0; i<contourCircles.size(); i++){
            contourCircles[i] -> draw();
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawCropRect(){
    if (isDebugMode && !isCropMode){
        ofPushStyle();
        ofNoFill();
        ofSetColor(255,0,0);
//        int _w0 = vidGrabber.getWidth();
//        int _h0 = vidGrabber.getHeight();
        int _w0 = imgW;
        int _h0 = imgH;
        ofDrawRectangle(cropRatioX * _w0, cropRatioY * _h0, cropRatioW * _w0, cropRatioH * _h0);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::drawInformation(){
    if(isDebugMode){
        ofPushStyle();
        ofSetColor(255);
        int initX = 30;
        int initY = 30;
        int inclX = 15;
        int inclY = 15;
        ofDrawBitmapString("framerate = " + ofToString(ofGetFrameRate()), initX, initY + inclX * 0);
        ofDrawBitmapString("space key => learn background", initX, initY + inclX * 1);
        ofDrawBitmapString("c key => reset circles", initX, initY + inclX * 2);
        ofDrawBitmapString("+/- key => increase/decrease a threshold : " + ofToString(threshold), initX, initY + inclX * 3);
        ofDrawBitmapString("d key => debug mode on/off", initX, initY + inclX * 4);
        ofDrawBitmapString("p key => crop mode on/off : " + ofToString(isCropMode), initX, initY + inclX * 5);
        ofDrawBitmapString("o key => crop calibration mode on/off : " + ofToString(isCalibrationMode), initX, initY + inclX * 6);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::drawCircles(){
    for(int i=0; i<circles.size(); i++){
        circles[i] -> draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    int w = ofGetWidth();
    int h = ofGetHeight();
    std::cout << "mouse x=" << x << "/" << w << " y=" << y << "/" << h << endl;
    if (isCalibrationMode){
        if(calibrationPointCount == 0){
            cropRatioTempX = float(x) / float(w);
            cropRatioTempY = float(y) / float(h);
            calibrationPointCount++;
            std::cout << "calibration upper left was done!!" << endl;
        } else {
            cropRatioTempW = float(x) / float(w) - cropRatioTempX;
            cropRatioTempH = float(y) / float(h) - cropRatioTempY;
            
//            if (0 < cropRatioTempW < 1 && 0 < cropRatioTempH < 1 && 0 < cropRatioTempX + cropRatioTempW < 1 && 0 < cropRatioTempY + cropRatioTempH < 1){
            float cropRatioTempXW = cropRatioTempX + cropRatioTempW;
            float cropRatioTempYH = cropRatioTempY + cropRatioTempH;
            if (0 < cropRatioTempW && cropRatioTempW < 1 && 0 < cropRatioTempH && cropRatioTempH < 1 && 0 < cropRatioTempXW && cropRatioTempXW < 1 && 0 < cropRatioTempYH && cropRatioTempYH < 1){
                cropRatioX = cropRatioTempX;
                cropRatioY = cropRatioTempY;
                cropRatioW = cropRatioTempW;
                cropRatioH = cropRatioTempH;
                std::cout << "calibration done!!" << endl;
                saveCropInfo();
            } else {
                std::cout << "calibration failed..." << endl;
                std::cout << "x=" << cropRatioTempX << " y=" << cropRatioTempY << " w=" << cropRatioTempW << " h=" << cropRatioTempH << endl;
            }

            calibrationPointCount = 0;
            isCalibrationMode = false;
        }
        
    }

}

//--------------------------------------------------------------
void ofApp::loadCropInfo(){
    ofFile file(cropJsFilename);
    if(file.exists()){
        file >> cropJs;
        std::cout << " x=" << cropJs["x"] << " y=" << cropJs["y"] << " w=" << cropJs["w"] << " h=" << cropJs["h"] << endl;
        float _x = float(cropJs["x"]);
        float _y = float(cropJs["y"]);
        float _w = float(cropJs["w"]);
        float _h = float(cropJs["h"]);

        if (0 < _w && _w < 1 && 0 < _h && _h < 1 && 0 < _x+_w && _x+_w < 1 && 0 < _y+_h && _y+_h < 1){
            cropRatioX = _x;
            cropRatioY = _y;
            cropRatioW = _w;
            cropRatioH = _h;
            std::cout << "crop data load success!!" << endl;
        } else {
            std::cout << "crop data load failed..." << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::saveCropInfo(){
    cropJs["x"] = cropRatioX;
    cropJs["y"] = cropRatioY;
    cropJs["w"] = cropRatioW;
    cropJs["h"] = cropRatioH;
    ofSaveJson(cropJsFilename, cropJs);
    std::cout << "crop data saved!!" << endl;
}

//--------------------------------------------------------------
void ofApp::loadSettings(){
    ofFile file(settingsJsFilename);
    if(file.exists()){
        file >> settingsJs;
        threshold = int(settingsJs["threshold"]);
        cameraId = int(settingsJs["cameraId"]);
        numOfCircles = int(settingsJs["numOfCircles"]);
        if( int(settingsJs["isDebugMode"]) == 1){
            isDebugMode = true;
        }else{
            isDebugMode = false;
        }
        imgW = int(settingsJs["imgW"]);
        imgH = int(settingsJs["imgH"]);
        circleRadius = int(settingsJs["circleRadius"]);
        std::cout << "settings data load success!!" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::saveSettings(){
    settingsJs["threshold"] = threshold;
    settingsJs["cameraId"] = cameraId;
    settingsJs["numOfCircles"] = numOfCircles;
    if (isDebugMode){
        settingsJs["isDebugMode"] = 1;
    }else{
        settingsJs["isDebugMode"] = 0;
    }
    settingsJs["imgW"] = imgW;
    settingsJs["imgH"] = imgH;
    settingsJs["circleRadius"] = circleRadius;
    ofSaveJson(settingsJsFilename, settingsJs);
    std::cout << "settings data saved!!" << endl;
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
