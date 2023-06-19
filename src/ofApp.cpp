#include "ofApp.h"
#include "../Settings.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofEnableGLDebugLog();

	ofDisableArbTex();
	ofSeedRandom();
	ofBackground(128, 0, 128);
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	width = ofGetWidth();
	height = ofGetHeight();

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

	setupGUI();
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	updateSettings();

	doubleBufferTrailMap.swap();
	doubleBufferTrailMap.src->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	doubleBufferTrailMap.dst->bindBase(GL_SHADER_STORAGE_BUFFER, 2);

	agentComputeShader.begin();
	agentComputeShader.setUniform1i("width", width);
	agentComputeShader.setUniform1i("height", height);
	agentComputeShader.setUniform1f("speed", Settings::agentsSpeed);
	agentComputeShader.setUniform1f("maxTrailDensity", Settings::maxTrailDensity);
	agentComputeShader.setUniform1f("senseDistance", Settings::agentsSenseDistance);
	agentComputeShader.setUniform1i("sensorSize", Settings::agentsSensorSize);
	agentComputeShader.setUniform1f("senseAngle", Settings::agentSenseAngle);
	agentComputeShader.setUniform1f("steerStrength", Settings::agentSteerStrength);
	agentComputeShader.setUniform1f("rotateAngle", Settings::agentRotateAngle);
	agentComputeShader.setUniform1f("pheromoneStrength", Settings::agentPheromoneStrength);
	agentComputeShader.dispatchCompute((Settings::agentsAmount + 1024 - 1) / 1024, 1, 1);

	agentComputeShader.end();

	trailMapComputeShader.begin();
	trailMapComputeShader.setUniform1i("width", width);
	trailMapComputeShader.setUniform1i("height", height);
	trailMapComputeShader.setUniform1f("diffuseRate", Settings::diffuseRate);
	trailMapComputeShader.setUniform1f("decayRate", Settings::decayRate);
	trailMapComputeShader.dispatchCompute(height * width / 1000, 1, 1);
	trailMapComputeShader.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	trailMapShader.begin();
	trailMapShader.setUniform1i("width", width);
	trailMapShader.setUniform1i("height", height);
	trailMapShader.setUniform1f("decay", Settings::decayRate);
	trailMapShader.setUniform1f("maxTrailDensity", Settings::maxTrailDensity);
	trailMapShader.setUniform1f("xRatio", float(ofGetWidth()) / float(width));
	trailMapShader.setUniform1f("yRatio", float(ofGetHeight()) / float(height));
	trailMapShader.setUniform4f("baseColor", ofColor::black);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	trailMapShader.end();

	gui.draw();
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
	agents.resize(Settings::agentsAmount);
	ofVec2f spawnPoint = ofVec2f(width / 2, height / 2);
	for (auto& agent : agents) {
		agent.x = spawnPoint.x;
		agent.y = spawnPoint.y;
		agent.z = ofRandom(0, 359);
	}
	bufferAgents.allocate(agents, GL_DYNAMIC_DRAW);
}

void ofApp::setupGUI() {
	gui.setup();

	gui.add(blobGUI.steerSlider.setup("Steer", 6, 0, 20));
	gui.add(blobGUI.angleSlider.setup("Angle", 6, 0, 60));
	gui.add(blobGUI.senseDistanceSlider.setup("SenseDistance", 20, 1, 100));
	gui.add(blobGUI.decayRateSlider.setup("Decay", 0.05, 0.0, 3.0));
	gui.add(blobGUI.diffuseRateSlider.setup("Diffuse", 0.06, 0, 3));
	gui.add(blobGUI.speedSlider.setup("Speed", 1.0, 0, 5));
	gui.add(blobGUI.maxTrailDensitySlider.setup("Max Trail Density", 200, 0, 300));
	gui.add(blobGUI.sensorSizeSlider.setup("Sensor Size", 1, 0, 4));
	gui.add(blobGUI.rotateAngleSlider.setup("Rotate angle", 0, -2, 2));
	gui.add(blobGUI.pheromoneStrengthSlider.setup("Pheromone strength", 3, 0, 10));
}

void ofApp::updateSettings() {
	Settings::agentSteerStrength = blobGUI.steerSlider;
	Settings::agentSenseAngle = blobGUI.angleSlider;
	Settings::agentsSenseDistance = blobGUI.senseDistanceSlider;
	Settings::diffuseRate = blobGUI.diffuseRateSlider;
	Settings::decayRate = blobGUI.decayRateSlider;
	Settings::agentsSpeed = blobGUI.speedSlider;
	Settings::maxTrailDensity = blobGUI.maxTrailDensitySlider;
	Settings::agentsSensorSize = blobGUI.sensorSizeSlider;
	Settings::agentRotateAngle = blobGUI.rotateAngleSlider;

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
