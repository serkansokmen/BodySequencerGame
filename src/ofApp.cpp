#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowTitle("Body Race");
    
    gui.setup("Body Race");
    gui.add(bpm.set("Speed", 192, 40, 255));
    gui.add(startGameButton.setup("Start Game"));
    gui.add(endGameButton.setup("End Game"));
    gui.loadFromFile("settings.xml");
    bHideGui = false;
    
    startGameButton.addListener(this, &ofApp::startGame);
    endGameButton.addListener(this, &ofApp::endGame);
    bpm.addListener(this, &ofApp::bpmChanged);
    
    bGameRunning = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (bGameRunning) bpmTapper.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (bGameRunning){
        ofSetColor(ofColor::blueSteel);
        bpmTapper.draw(40, ofGetHeight() - 40, 10);
    }
    if (!bHideGui) gui.draw();
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
void ofApp::windowResized(int w, int h){

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
    
    startGameButton.removeListener(this, &ofApp::startGame);
    endGameButton.removeListener(this, &ofApp::endGame);
    bpm.removeListener(this, &ofApp::bpmChanged);
}

//--------------------------------------------------------------
void ofApp::startGame(){
    ofLog(OF_LOG_NOTICE, "Start game");
    bpmTapper.startFresh();
    bGameRunning = true;
}

//--------------------------------------------------------------
void ofApp::endGame(){
    ofLog(OF_LOG_NOTICE, "End game");
    bGameRunning = false;
}

//--------------------------------------------------------------
void ofApp::bpmChanged(float &newVal){
    bpmTapper.setBpm(newVal);
}
