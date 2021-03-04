#pragma once

#include "ofMain.h"

// Custom objects for this example
//#include "Swarm.h"
#include "Grid.h"
#include "particle.h"

#define N_CAMERAS 5
#define NUM 1000


class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void setupViewports();
		void setupViewportsAbs();
		void drawScene(int iCameraDraw);
		//void updateMouseRay();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
        void saveFrame();
        void setCamera();
		string int2str(int _num);

		void setBasisCircles();
		void drawBasisCircles();
		void drawSurfaceLabels();
		void drawHorizontalSphere();
		void drawVerticalShpere();
		void setVertexes();
		void drawVertexes();
		void setEdges();
		void drawEdges();
		void setParticles();
		void updateParticles();
		void drawParticles();

		//cameras (all these inherit from ofCamera)
//		ofEasyCam camEasyCam;
		ofCamera camCenter;
		ofCamera camLeft;
        ofCamera camRight;
        ofCamera camBack;
        ofCamera camFloor;

		//cameras have parent?
//		bool bCamParent;

		//camera pointers
		ofCamera * cameras[N_CAMERAS];
//		int iMainCamera;

		//viewports
//		ofRectangle viewMain;
		ofRectangle viewGrid[N_CAMERAS];

		//my custom node
//		swarm nodeSwarm;
		//grid nodeGrid;

		//ray drawn under mouse cursor [start,end]
//		glm::vec3 ray[2];

		float xRatio = 192.0f;
		float yRatio = 108.0f;
    
        float velocity = -0.1f;
        float theta = 180;
        ofLight light;
        ofMaterial material;
    
        ofImage img;

		particle particles[NUM];
		ofPolyline edges[NUM];
		
		vector <ofVec3f> pos0s;
		vector <ofVec3f> basisCircles;
		vector <ofColor> circleColors;

		ofVec3f BUL;
		ofVec3f BUR;
		ofVec3f CUR;
		ofVec3f CUL;
		ofVec3f BLL;
		ofVec3f BLR;
		ofVec3f CLR;
		ofVec3f CLL;

		int lastFlameNum = 3600;
		string saveDir = "E:/chart/5/3600/";
};
