#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofEnableGLDebugLog();


	ofDisableArbTex();
	ofSeedRandom();
	ofBackground(128, 0, 128);
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	width = ofGetWidth();
	height = ofGetHeight();
	agentsAmount = 10000;

	trailMapShader.load("shadersGL3/shader");
	//trailMapShader.linkProgram();
	
	trailMapComputeShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shadersGL3/trailMapShader.glsl");
	trailMapComputeShader.linkProgram();

	agentComputeShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shadersGL3/agentShader.glsl");
	agentComputeShader.linkProgram();

	setupBufferTrailMap();
	setupBufferAgents();

	bufferAgents.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	doubleBufferTrailMap.src->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	doubleBufferTrailMap.dst->bindBase(GL_SHADER_STORAGE_BUFFER, 2);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	doubleBufferTrailMap.swap();
	doubleBufferTrailMap.src->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	doubleBufferTrailMap.dst->bindBase(GL_SHADER_STORAGE_BUFFER, 2);

	agentComputeShader.begin();
	agentComputeShader.setUniform1i("width", width);
	agentComputeShader.setUniform1i("height", height);
	agentComputeShader.setUniform1f("speed", 1);
	agentComputeShader.setUniform1f("maxTrailDensity", 200);
	agentComputeShader.dispatchCompute((agentsAmount + 1024 - 1) / 1024, 1, 1);
	agentComputeShader.end();

	trailMapComputeShader.begin();
	trailMapComputeShader.setUniform1i("width", width);
	trailMapComputeShader.setUniform1i("height", height);
	trailMapComputeShader.setUniform1f("diffuseRate", 0.06);
	trailMapComputeShader.setUniform1f("decayRate", 0.01);
	trailMapComputeShader.dispatchCompute(height * width / 1000, 1, 1);
	trailMapComputeShader.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	trailMapShader.begin();
	trailMapShader.setUniform1i("width", width);
	trailMapShader.setUniform1i("height", height);
	trailMapShader.setUniform1f("decay", 0.01);
	trailMapShader.setUniform1f("maxTrailDensity", 200);
	trailMapShader.setUniform1f("xRatio", float(ofGetWidth()) / float(width));
	trailMapShader.setUniform1f("yRatio", float(ofGetHeight()) / float(height));
	trailMapShader.setUniform4f("baseColor", ofColor::black);
	ofDrawRectangle(0, 0, width, height);
	trailMapShader.end();
}

//--------------------------------------------------------------
void ofApp::setupBufferTrailMap() {
	trailMap.clear();
	int size = width * height;
	trailMap = vector<float>(size);
	for (int i = 0; i < size; i++) {
		trailMap[i] = 0;
	}
	doubleBufferTrailMap.allocate(trailMap);
}

//--------------------------------------------------------------
void ofApp::setupBufferAgents() {
	agents.clear();
	agents.resize(agentsAmount);
	ofVec2f spawnPoint = ofVec2f(width / 2, height / 2);
	for (auto& agent : agents) {
		agent.x = spawnPoint.x;
		agent.y = spawnPoint.y;
		agent.z = ofRandom(60, 359);
	}
	bufferAgents.allocate(agents, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
