#include "ofMain.h"
#include "ofApp.h"
#include "ofGlProgrammableRenderer.h"


//========================================================================
int main(){
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
