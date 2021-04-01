#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//executeCommands();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (ofGetFrameNum() == 1) {
		executeCommands();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(ofToString(ofGetFrameNum()), 20, 20);
}

//--------------------------------------------------------------
void ofApp::executeCommands() {
	sysCom.callCommand("start data\\test.bat");
	ofSleepMillis(5000);
	sysCom2.callCommand("start data\\test.bat");
	sysCom3.callCommand("dir");
	//ofSleepMillis(10000);
	//sysCom4.callCommand("data\\test.bat");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
