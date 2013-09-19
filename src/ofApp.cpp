#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowTitle("Body Race");
    ofSetCircleResolution(100);
    
    font.loadFont("type/verdana.ttf", 120);
    
    gui.setup("Body Race");
    gui.add(bpm.set("Speed", 192, 40, 255));
    gui.add(bDrawBpmTapper.set("Draw BPM Tapper", true));
    gui.add(startCountdownButton.setup("Start Game"));
    gui.add(endGameButton.setup("End Game"));
    
    gui.add(bDrawFaces.set("Draw Faces", true));
    gui.add(bDrawWireframe.set("Draw Wireframe", true));
    gui.add(bDrawVertices.set("Draw Vertices", true));
    
    gui.loadFromFile("settings.xml");
    bHideGui = false;
    
    startCountdownButton.addListener(this, &ofApp::startCountdown);
    endGameButton.addListener(this, &ofApp::endGame);
    bpm.addListener(this, &ofApp::bpmChanged);
    
    bCountdownRunning = false;
    bGameRunning = false;
    startTimer.stop();
    
    gridAlphaMask.loadImage("images/grid_mask.png");
    
    sequencerArea.setFromCenter(ofGetWidth()*.5, ofGetHeight()*.5, SEQUENCER_WIDTH, SEQUENCER_HEIGHT);
    sequencerPlane.set(SEQUENCER_WIDTH, SEQUENCER_HEIGHT, COLUMNS * 8, ROWS * 8, OF_PRIMITIVE_TRIANGLES);
    sequencerPlane.mapTexCoords(0, 0, gridAlphaMask.getWidth(), gridAlphaMask.getHeight());
    ofVec4f tcoords = sequencerPlane.getTexCoords();
    sequencerPlane.mapTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
    
#ifdef TARGET_OPENGLES
	sequencerShader.load("shaders/ES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		sequencerShader.load("shaders/GL3/shader");
	}else{
		sequencerShader.load("shaders/GL2/shader");
	}
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (bCountdownRunning && !bGameRunning && startTimer.getSeconds() >= COUNTDOWN){
        startGame();
    }
    
    if (bGameRunning){
        bpmTapper.update();
        
        currentStep = (int)bpmTapper.beatTime() % COLUMNS;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (bGameRunning && !bCountdownRunning){
        
//        for (int j=0; j<COLUMNS; j++) {
//            for (int i=0; i<ROWS; i++) {
//                int gridIndex = i + j * COLUMNS;
//            }
//        }
        
        // Draw scrubbers
        ofSetColor(ofColor::greenYellow);
        ofPushMatrix();
        ofTranslate(sequencerArea.getTopLeft());
        ofRect(currentStep*sequencerArea.getWidth()/COLUMNS,
               -SCRUBBER_HEIGHT*2,
               sequencerArea.getWidth()/COLUMNS,
               SCRUBBER_HEIGHT);
        ofRect(currentStep*sequencerArea.getWidth()/COLUMNS,
               sequencerArea.getHeight()+SCRUBBER_HEIGHT,
               sequencerArea.getWidth()/COLUMNS,
               SCRUBBER_HEIGHT);
        ofPopMatrix();
        
        
        gridAlphaMask.getTextureReference().bind();
        sequencerShader.begin();
        
        ofPushMatrix();
//        sequencerShader.setUniform2f("scrubberPosition", ofNormalize(currentStep, 0, COLUMNS), ofNormalize(currentStep+1, 0, COLUMNS));
        float mousePosition = ofMap(mouseX, 0, ofGetWidth(), 1.0, -1.0, true);
#ifndef TARGET_OPENGLES
        mousePosition *= sequencerPlane.getWidth();
#endif

        sequencerShader.setUniform1f("mouseX", mousePosition);
        
        ofTranslate(sequencerArea.getCenter());
        
        if (bDrawFaces)
            sequencerPlane.drawFaces();
        if (bDrawWireframe)
            sequencerPlane.drawWireframe();
        if (bDrawVertices)
            sequencerPlane.drawVertices();
        
        sequencerShader.end();
        ofPopMatrix();
        gridAlphaMask.getTextureReference().unbind();
        
        if (bDrawBpmTapper) {
            bpmTapper.draw(40, ofGetHeight() - 40, 10);
        }
        
    } else {
        if (bCountdownRunning){
            // Draw Countdown
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5);
            ofSetColor(ofColor::greenYellow);
            ofCircle(0, 0, 240);
            ofSetColor(ofColor::whiteSmoke);
            ofCircle(0, 0, 200);
            ofSetColor(ofColor::grey);
            font.drawStringCentered(ofToString(COUNTDOWN - (int)startTimer.getSeconds()), 0, 0);
            ofPopStyle();
            ofPopMatrix();
        }
    }
    
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);
    
    if (!bHideGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            startGame();
            break;
        case 's':
            bHideGui = !bHideGui;
            break;
        default:
        break;
    }
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
void ofApp::windowResized(int w, int h){
    sequencerArea.setFromCenter(ofGetWidth()*.5, ofGetHeight()*.5, SEQUENCER_WIDTH, SEQUENCER_HEIGHT);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile("settings.xml");
    
    startCountdownButton.removeListener(this, &ofApp::startCountdown);
    endGameButton.removeListener(this, &ofApp::endGame);
    bpm.removeListener(this, &ofApp::bpmChanged);
}

//--------------------------------------------------------------
void ofApp::startCountdown(){
    
    bGameRunning = false;
    bCountdownRunning = true;
    
    startTimer.start();
    
    ofLog(OF_LOG_NOTICE, "Starting game in " + ofToString(COUNTDOWN) + " seconds");
}

//--------------------------------------------------------------
void ofApp::startGame(){
    
    bGameRunning = true;
    bCountdownRunning = false;
    
    startTimer.stop();
    bpmTapper.startFresh();
    
    ofLog(OF_LOG_NOTICE, "Game started");
}

//--------------------------------------------------------------
void ofApp::endGame(){
    bGameRunning = false;
    
    ofLog(OF_LOG_NOTICE, "Game ended");
}

//--------------------------------------------------------------
void ofApp::bpmChanged(float &newVal){
    bpmTapper.setBpm(newVal);
}
