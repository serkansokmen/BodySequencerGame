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
    gui.loadFromFile("settings.xml");
    bHideGui = false;
    
    startCountdownButton.addListener(this, &ofApp::startCountdown);
    endGameButton.addListener(this, &ofApp::endGame);
    bpm.addListener(this, &ofApp::bpmChanged);
    
    bCountdownRunning = false;
    bGameRunning = false;
    startTimer.stop();
    
    
    
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
        
        // Draw sqeuencer
        ofPushMatrix();
        ofPopMatrix();
        
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
