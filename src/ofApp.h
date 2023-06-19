#pragma once


#include "ofMain.h"
#include "../PingPongBuffer.h"
#include <glm/ext/vector_float4.hpp>
#include "../BlobGUI.h"

class ofApp : public ofBaseApp {
private:
	ofShader trailMapShader;
	ofShader trailMapComputeShader;
	ofShader agentComputeShader;

	PingPongBuffer doubleBufferTrailMap;
	vector<float> trailMap;

	ofBufferObject bufferAgents;
	vector<glm::vec4> agents;

	int width;
	int height;

	ofxPanel gui;
	BlobGUI blobGUI;

	void setupBufferTrailMap();
	void setupBufferAgents();
	void setupGUI();
	void updateSettings();

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
		
};

