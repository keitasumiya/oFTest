#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	ofEnableSmoothing();
	ofEnableDepthTest();
	ofSetSmoothLighting(true);
	ofSetSphereResolution(200);
    
    setCamera();

	setParticles();

	setVertexes();
	setBasisCircles();
	setEdges();
}


//--------------------------------------------------------------
void ofApp::update(){
	updateParticles();
   
}


//--------------------------------------------------------------
void ofApp::draw(){

	ofPushStyle();
	// draw side viewports
	for(int i = 0; i < N_CAMERAS; i++){
		cameras[i]->begin(viewGrid[i]);
		drawScene(i);
		cameras[i]->end();
	}
	ofPopStyle();
    
	if (ofGetFrameNum() < lastFlameNum) {
		saveFrame();
	}
	else {
		ofExit();
	}
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::drawScene(int cameraIndex){
	light.enable();
	light.setPosition(0, 0, 0);
	drawSurfaceLabels();
	drawBasisCircles();
	drawHorizontalSphere();
	drawVerticalShpere();
	//drawVertexes();
	//drawEdges();

	light.setPosition(50, 50, 50);
	drawParticles();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'f'){
		ofToggleFullscreen();
	}
    
    if(key == ' '){
        saveFrame();
    }
}

//--------------------------------------------------------------
void ofApp::saveFrame(){
	img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());

	int num = ofGetFrameNum();
    string numStr = int2str(num);
    img.save(saveDir+numStr+".bmp");
}

//--------------------------------------------------------------
string ofApp::int2str(int _num) {
	string _numStr;
	if (_num < 10) {
		_numStr = "000" + ofToString(_num);
	}
	else if (_num < 100) {
		_numStr = "00" + ofToString(_num);
	}
	else if (_num < 1000) {
		_numStr = "0" + ofToString(_num);
	}
	else {
		_numStr = ofToString(_num);
	}
	return _numStr;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
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
	setupViewports();
}

//--------------------------------------------------------------
void ofApp::setCamera(){
	float ratioSide = xRatio / yRatio;
	float fovSide = 58.75;
	float ratioFloor = 1;
	float fovFloor = 121.2;

    camCenter.lookAt(ofVec3f(  0,  0,-10));
	camBack.lookAt(  ofVec3f(  0,  0, 10));
	camLeft.lookAt(  ofVec3f(-10,  0,  0));
    camRight.lookAt( ofVec3f( 10,  0,  0));
    camFloor.lookAt( ofVec3f(  0,-10,  0));

    cameras[0] = &camCenter;
    cameras[1] = &camLeft;
    cameras[2] = &camRight;
    cameras[3] = &camBack;
    cameras[4] = &camFloor;

    for (size_t i = 0; i != N_CAMERAS; ++i) {
		cameras[i]->setPosition(0, 0, 0);
		//cameras[i]->enableOrtho();
		cameras[i]->setNearClip(0.1);
		cameras[i]->setFarClip(100000);
		if (i == N_CAMERAS - 1) {
			cameras[i]->setAspectRatio(ratioFloor);
			cameras[i]->setFov(fovFloor);
		} else {
			cameras[i]->setAspectRatio(ratioSide);
			cameras[i]->setFov(fovSide);

		}
    }

	setupViewports();
	//setupViewportsAbs();
}

//--------------------------------------------------------------
void ofApp::setupViewportsAbs() {
	float sizeRatio = 5;
	viewGrid[1].x = 0;
	viewGrid[1].y = 0;
	viewGrid[1].width = sizeRatio * xRatio;
	viewGrid[1].height = sizeRatio * yRatio;

	viewGrid[0].x = sizeRatio * xRatio;
	viewGrid[0].y = 0;
	viewGrid[0].width = sizeRatio * xRatio;
	viewGrid[0].height = sizeRatio * yRatio;

	viewGrid[2].x = 2 * sizeRatio * xRatio;
	viewGrid[2].y = 0;
	viewGrid[2].width = sizeRatio * xRatio;
	viewGrid[2].height = sizeRatio * yRatio;

	viewGrid[3].x = 3 * sizeRatio * xRatio;
	viewGrid[3].y = 0;
	viewGrid[3].width = sizeRatio * xRatio;
	viewGrid[3].height = sizeRatio * yRatio;

	viewGrid[4].x = sizeRatio * xRatio;
	viewGrid[4].y = sizeRatio * yRatio;
	viewGrid[4].width = sizeRatio * xRatio;
	viewGrid[4].height = sizeRatio * xRatio;
}


//--------------------------------------------------------------
void ofApp::setupViewports(){
    viewGrid[0].x = 0;
    viewGrid[0].y = 0;
    viewGrid[0].width = ofGetWidth()/2.0f;
    viewGrid[0].height = ofGetHeight()/2.0f;

    viewGrid[1].x = ofGetWidth()/2.0f;
    viewGrid[1].y = 0;
    viewGrid[1].width = ofGetWidth()/2.0f;
    viewGrid[1].height = ofGetHeight()/2.0f;

    viewGrid[2].x = 0;
    viewGrid[2].y = ofGetHeight()/2.0f;
    viewGrid[2].width = ofGetWidth()/2.0f;
    viewGrid[2].height = ofGetHeight()/2.0f;

    viewGrid[3].x = ofGetWidth()/2.0f;
    viewGrid[3].y = ofGetHeight()/2.0f;
    viewGrid[3].width = ofGetWidth()/4.0f;
    viewGrid[3].height = ofGetHeight()/4.0f;

    viewGrid[4].x = ofGetWidth()*3.0f/4.0f;
    viewGrid[4].y = ofGetHeight()/2.0f;
    viewGrid[4].width = ofGetWidth()/4.0f;
    viewGrid[4].height = ofGetWidth()/4.0f;
}


void ofApp::setBasisCircles() {
	float r0 = sqrt(192 * 192 + 192 * 192);
	//float r0 = 100;
	for (float _theta0 = 0; _theta0 < 180; _theta0 += 10) {
		for (float _phi0 = 0; _phi0 < 360; _phi0 += 10) {
			float _theta = _theta0 * DEG_TO_RAD;
			float _phi = _phi0 * DEG_TO_RAD;
			float z0 = r0 * sin(_theta) * cos(_phi);
			float x0 = r0 * sin(_theta) * sin(_phi);
			float y0 = r0 * cos(_theta);
			//std::cout << theta0 << " x" << x0 << " y" << y0 << " z" << z0 << endl;
			ofVec3f _pos = ofVec3f(x0, y0, z0);
			basisCircles.push_back(_pos);
			float val = 255 * _theta0 / 180;
			//std::cout << val << endl;
			circleColors.push_back(ofColor::fromHsb(val, 255, 255));
		}
	}
}

void ofApp::drawBasisCircles() {
	for (int i = 0; i < basisCircles.size(); i++) {
		//float c = 255 * i / basisCircles.size();
		ofSetColor(circleColors[i]);
		ofDrawSphere(basisCircles[i], 1);
	}
}

void ofApp::drawSurfaceLabels() {
	ofSetColor(255);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);

	float xAdj = 7.5;
	float yAdj = 4;
	int frameNum = ofGetFrameNum();
	string frameNumStr = int2str(frameNum);

	//center
	ofDrawBitmapString("center", ofVec3f(-3*xAdj,yAdj,-xRatio));
	ofDrawBitmapString(frameNumStr, ofVec3f(-3*xAdj,-4*yAdj,-xRatio));

	//back
	ofPushMatrix();
	ofRotateDeg(180, 0, 1, 0);
	ofTranslate(ofVec3f(-2*xAdj,yAdj,-xRatio));
	ofDrawBitmapString("back", ofVec3f(0,0,0));
	ofTranslate(ofVec3f(0,-4*yAdj,0));
	ofDrawBitmapString(frameNumStr, ofVec3f(0,0,0));
	ofPopMatrix();

	//left
	ofPushMatrix();
	ofRotateDeg(90, 0, 1, 0);
	ofTranslate(ofVec3f(-2*xAdj,yAdj,-xRatio));
	ofDrawBitmapString("left", ofVec3f(0,0,0));
	ofTranslate(ofVec3f(0,-4*yAdj,0));
	ofDrawBitmapString(frameNumStr, ofVec3f(0,0,0));
	ofPopMatrix();

	//right
	ofPushMatrix();
	ofRotateDeg(-90, 0, 1, 0);
	ofTranslate(ofVec3f(-2.5*xAdj,yAdj,-xRatio));
	ofDrawBitmapString("right", ofVec3f(0,0,0));
	ofTranslate(ofVec3f(0,-4*yAdj,0));
	ofDrawBitmapString(frameNumStr, ofVec3f(0,0,0));
	ofPopMatrix();

	//floor
	ofPushMatrix();
	ofRotateDeg(-90, 1, 0, 0);
	ofTranslate(ofVec3f(-2.5*xAdj,yAdj,-yRatio));
	ofDrawBitmapString("floor", ofVec3f(0,0,0));
	ofTranslate(ofVec3f(0,-4*yAdj,0));
	ofDrawBitmapString(frameNumStr, ofVec3f(0,0,0));
	ofPopMatrix();
}

void ofApp::setVertexes() {
	BUL = ofVec3f( 192,  108,  192); //back UL
	BUR = ofVec3f(-192,  108,  192); //back UR
	CUR = ofVec3f( 192,  108, -192); //center UR
	CUL = ofVec3f(-192,  108, -192); //center UL
	BLL = ofVec3f( 192, -108,  192); //back LL
	BLR = ofVec3f(-192, -108,  192); //back LR
	CLR = ofVec3f( 192, -108, -192); //center LR
	CLL = ofVec3f(-192, -108, -192); //center LL
}

void ofApp::drawVertexes() {
	float radiusSize = 5.0f;
	ofSetColor(255, 0, 0, 50);
	ofDrawSphere(BUL, radiusSize); //back UL
	ofDrawSphere(BUR, radiusSize); //back UR
	ofDrawSphere(CUR, radiusSize); //center UR
	ofDrawSphere(CUL, radiusSize); //center UL
	ofDrawSphere(BLL, radiusSize); //back LL
	ofDrawSphere(BLR, radiusSize); //back LR
	ofDrawSphere(CLR, radiusSize); //center LR
	ofDrawSphere(CLL, radiusSize); //center LL

	radiusSize = 3.0f;
	ofSetColor(0, 0, 255);
	ofDrawSphere(BUL, radiusSize); //back UL
	ofDrawSphere(BUR, radiusSize); //back UR
	ofDrawSphere(CUR, radiusSize); //center UR
	ofDrawSphere(CUL, radiusSize); //center UL
	ofDrawSphere(BLL, radiusSize); //back LL
	ofDrawSphere(BLR, radiusSize); //back LR
	ofDrawSphere(CLR, radiusSize); //center LR
	ofDrawSphere(CLL, radiusSize); //center LL

}

void ofApp::setEdges() {
	//center
	edges[0].addVertex(CUL);
	edges[0].addVertex(CUR);
	edges[0].addVertex(CLR);
	edges[0].addVertex(CLL);
	edges[0].addVertex(CUL);
	edges[0].addVertex(CLR);
	edges[0].addVertex(CLL);
	edges[0].addVertex(CUR);

	//right
	edges[1].addVertex(CUR);
	edges[1].addVertex(CLR);
	edges[1].addVertex(BLL);
	edges[1].addVertex(BUL);
	edges[1].addVertex(CUR);
	edges[1].addVertex(BLL);
	edges[1].addVertex(BUL);
	edges[1].addVertex(CLR);

	//left
	edges[2].addVertex(BUR);
	edges[2].addVertex(BLR);
	edges[2].addVertex(CLL);
	edges[2].addVertex(CUL);
	edges[2].addVertex(BUR);
	edges[2].addVertex(CLL);
	edges[2].addVertex(CUL);
	edges[2].addVertex(BLR);

	//back
	edges[3].addVertex(BUL);
	edges[3].addVertex(BUR);
	edges[3].addVertex(BLR);
	edges[3].addVertex(BLL);
	edges[3].addVertex(BUL);
	edges[3].addVertex(BLR);
	edges[3].addVertex(BLL);
	edges[3].addVertex(BUR);

	//floor
	edges[4].addVertex(CLL);
	edges[4].addVertex(CLR);
	edges[4].addVertex(BLL);
	edges[4].addVertex(BLR);
	edges[4].addVertex(CLL);
	edges[4].addVertex(BLL);
	edges[4].addVertex(BLR);
	edges[4].addVertex(CLR);

}

void ofApp::drawEdges() {
	ofPushStyle();
	ofSetColor(255, 0, 0);
	edges[0].draw();
	ofSetColor(0, 255, 0);
	edges[1].draw();
	ofSetColor(0, 255, 0);
	edges[2].draw();
	ofSetColor(0, 0, 255);
	edges[3].draw();
	ofSetColor(255, 255, 255);
	edges[4].draw();
	ofPopStyle();
}

void ofApp::drawHorizontalSphere() {
	float r0 = sqrt(192 * 192 + 192 * 192);
	float _theta0 = 90;
	float _phi0 = ofGetFrameNum();
	float _theta = _theta0 * DEG_TO_RAD;
	float _phi = _phi0 * DEG_TO_RAD;
	float z0 = r0 * sin(_theta) * cos(_phi);
	float x0 = r0 * sin(_theta) * sin(_phi);
	float y0 = r0 * cos(_theta);
	ofVec3f _pos = ofVec3f(x0, y0, z0);
	ofDrawSphere(x0, y0, z0, 10);
}

void ofApp::drawVerticalShpere() {
	float r0 = sqrt(192 * 192 + 192 * 192);
	float _theta0 = ofGetFrameNum();
	float _phi0 = 0;
	float _theta = _theta0 * DEG_TO_RAD;
	float _phi = _phi0 * DEG_TO_RAD;
	float z0 = r0 * sin(_theta) * cos(_phi);
	float x0 = r0 * sin(_theta) * sin(_phi);
	float y0 = r0 * cos(_theta);
	ofVec3f _pos = ofVec3f(x0, y0, z0);
	ofDrawSphere(x0, y0, z0, 10);
}

void ofApp::setParticles() {
	for (int i = 0; i < NUM; i++) {
		particles[i].init();
	}
}

void ofApp::updateParticles() {
	for (int i = 0; i < NUM; i++) {
		particles[i].update();
	}
}
void ofApp::drawParticles() {
	for (int i = 0; i < NUM; i++) {
		particles[i].draw();
	}
}