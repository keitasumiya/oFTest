#pragma once
#include "ofMain.h"

class particle : ofBaseApp {
	public:
		void init();
		void update();
		void draw();

	private:
		ofColor color;
		ofVec3f pos;
		ofVec3f vec;
		float size;
		float rCenter = 600; //sqrt(192*192);
		float rDif = 200;
		float rOuter = rCenter + rDif;
		float rInner = rCenter - rDif;
		float vRatio = 2;
		float sizeBasis = 50;
};

