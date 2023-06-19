#include "ofMain.h"
#include "ofApp.h"


//========================================================================
int main( ) {
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofGLFWWindowSettings settings;

	settings.setGLVersion(4, 4); //we define the OpenGL version we want to use
	settings.setSize(1280, 800);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());

}
