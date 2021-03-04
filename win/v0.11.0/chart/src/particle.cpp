#include "particle.h"



void particle::init()
{

	float r = rDif * ofRandomf() + rCenter;
	float _theta = (60 + ofRandom(120)) * DEG_TO_RAD;
	float _phi = ofRandom(360) * DEG_TO_RAD;
	float z0 = r * sin(_theta) * cos(_phi);
	float x0 = r * sin(_theta) * sin(_phi);
	float y0 = r * cos(_theta);
	pos = ofVec3f(x0, y0, z0);
	vec = vRatio * ofVec3f(ofRandomf(), ofRandomf(), ofRandomf());
	size = sizeBasis * ofRandomf() + sizeBasis + 5;
	color = ofColor::fromHsb(ofRandom(255), ofRandom(200, 255), ofRandom(200, 255), ofRandom(150)+10);
}

void particle::update()
{
	float r = ofDist(pos.x, pos.y, pos.z, 0, 0, 0);
	if (r < rInner) {
		vec = - vec;
		pos = (rInner / r) * pos;
		//color = ofColor::fromHsb(ofRandom(255), ofRandom(200, 255), ofRandom(200, 255), color.a);
	}
	else if (r > rOuter) {
		vec = -vec;
		pos = (rOuter / r) * pos;
		//color = ofColor::fromHsb(ofRandom(255), ofRandom(200, 255), ofRandom(200, 255), color.a);
	}

	int changeFrameNum = 300;
	int val = ofGetFrameNum() % changeFrameNum;
	if (val == 0) {
		//std::cout << ofGetFrameNum() << endl;
		vec = vRatio * ofVec3f(ofRandomf(), ofRandomf(), ofRandomf());
		color = ofColor::fromHsb(ofRandom(255), ofRandom(200, 255), ofRandom(200, 255), color.a);
	}
	else if (val < 0.4*changeFrameNum ) {
		vec *= 0.99;
	}
	pos += vec;
}

void particle::draw()
{
	ofPushStyle();
	ofSetColor(color);
	ofDrawSphere(pos, size);
	ofPopStyle();
}
