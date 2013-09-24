#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowTitle("Body Sequencer");
    ofSetCoordHandedness(OF_RIGHT_HANDED);
    ofSetCircleResolution(100);
    
    font.loadFont("type/verdana.ttf", 120);
    
    // Setup themes
    currentTheme = new SequencerTheme();
    
    // Initialize themes
    themes.clear();
    
    SequencerTheme theme0, theme1;
    
    theme0.setup("themes/pack_1/sounds/", "themes/pack_1/images/background.png", "themes/pack_1/images/interface.png");
    theme1.setup("themes/pack_2/sounds/", "themes/pack_2/images/background.png", "themes/pack_2/images/interface.png");
    
    themes.push_back(theme0);
    themes.push_back(theme1);
    
    currentThemeId = 0;
    
    
    gui.setup("Body Sequencer");
    gui.add(bpm.set("Speed", 192, 40, 255));
    gui.add(bDrawBpmTapper.set("Draw BPM Tapper", true));
    gui.add(startCountdownButton.setup("Start Game"));
    gui.add(endGameButton.setup("End Game"));
    gui.add(seqPos.set("Sequencer Position",
                       ofVec2f((ofGetWidth() - SEQUENCER_MAX_WIDTH)*.5,
                               (ofGetHeight() - SEQUENCER_MAX_HEIGHT)*.5),
                       ofVec2f(0, 0),
                       ofVec2f(ofGetWidth(), ofGetHeight())));
    gui.add(seqWidth.set("Sequencer Width", SEQUENCER_MAX_WIDTH, 0, SEQUENCER_MAX_WIDTH));
    gui.add(seqHeight.set("Sequencer Height", SEQUENCER_MAX_HEIGHT, 0, SEQUENCER_MAX_HEIGHT));
    gui.add(currentThemeId.set("Current Theme", 0, 0, themes.size() - 1));
    gui.add(level_0_rounds.set("LEVEL 0 Rounds", 2, 1, 50));
    gui.add(level_1_rounds.set("LEVEL 1 Rounds", 2, 1, 20));
    gui.add(level_2_rounds.set("LEVEL 2 Rounds", 2, 1, 10));
    gui.add(level_3_rounds.set("LEVEL 3 Rounds", 2, 1, 5));
    
    startCountdownButton.addListener(this, &ofApp::startCountdown);
    endGameButton.addListener(this, &ofApp::endGame);
    bpm.addListener(this, &ofApp::bpmChanged);
    seqPos.addListener(this, &ofApp::sequencerPositionChanged);
    seqWidth.addListener(this, &ofApp::sequencerWidthChanged);
    seqHeight.addListener(this, &ofApp::sequencerHeightChanged);
    currentThemeId.addListener(this, &ofApp::currentThemeIdChanged);
    
    
    gui.loadFromFile("settings.xml");
    bHideGui = false;
    
    bpmTapper.setBpm(bpm);
    
    bCountdownRunning = false;
    bGameRunning = false;
    startTimer.stop();
    currentStep = 0;
    lastStep = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    Tweener.update();
    
    if (bCountdownRunning && !bGameRunning && startTimer.getSeconds() >= COUNTDOWN){
        startGame();
    }
    
    if (bGameRunning){
        
        int totalSteps = (int)bpmTapper.beatTime();
        currentStep = totalSteps % COLUMNS;
        
        if (lastStep != currentStep && currentStep == 0) {
            if (totalSteps < level_0_rounds*COLUMNS){
                ofLog(OF_LOG_NOTICE, "LEVEL 0");
                bpm = 80.0;
            }
            
            else if (totalSteps >= level_0_rounds*COLUMNS &&
                     totalSteps < (level_0_rounds+level_1_rounds)*COLUMNS){
                ofLog(OF_LOG_NOTICE, "LEVEL 1");
                bpm = 90.0;
            }
            
            else if (totalSteps >= (level_0_rounds+level_1_rounds)*COLUMNS &&
                     totalSteps < (level_0_rounds+level_1_rounds+level_2_rounds)*COLUMNS){
                ofLog(OF_LOG_NOTICE, "LEVEL 2");
                bpm = 120.0;
            }
            
            else if (totalSteps >= (level_0_rounds+level_1_rounds+level_2_rounds)*COLUMNS &&
                     totalSteps < (level_0_rounds+level_1_rounds+level_2_rounds+level_3_rounds)*COLUMNS){
                ofLog(OF_LOG_NOTICE, "LEVEL 3");
                bpm = 160.0;
            }
            
            else { endGame(); }
        }
        
        lastStep = currentStep;
        
        bpmTapper.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Draw background with or without splash
    currentTheme->draw(!bGameRunning && !bCountdownRunning);
    
    if (bGameRunning && !bCountdownRunning){
        
        // Draw scrubbers
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(ofColor::greenYellow);
        ofTranslate(sequencerArea.getTopLeft());
        ofRect(currentStep*sequencerArea.getWidth()/COLUMNS,
               -SCRUBBER_HEIGHT,
               sequencerArea.getWidth()/COLUMNS,
               SCRUBBER_HEIGHT);
        ofRect(currentStep*sequencerArea.getWidth()/COLUMNS,
               sequencerArea.getHeight(),
               sequencerArea.getWidth()/COLUMNS,
               SCRUBBER_HEIGHT);
        ofPopStyle();
        ofPopMatrix();
        
        // Draw sqeuencer
        ofPushMatrix();
        ofTranslate(sequencerArea.getTopLeft());
        for (int j=0; j<COLUMNS; j++) {
            for (int i=0; i<ROWS; i++) {
                int gridIndex = i + j * COLUMNS;
                
                float cw = sequencerArea.getWidth()/COLUMNS;
                float ch = sequencerArea.getHeight()/ROWS;
                float cx = i * cw;
                float cy = j * ch;
                
                ofRectangle cellRect(cx, cy, cw, ch);
                
                ofPushStyle();
                ofSetColor(ofColor::blueSteel);
                ofNoFill();
                ofRect(cellRect);
                ofPopStyle();
                
            }
        }
        ofPopMatrix();
        
        if (bDrawBpmTapper) {
            bpmTapper.draw(ofGetWidth() - 120, 20, 5);
        }
        
    } else {
        
        if (bCountdownRunning){
            // Draw Countdown
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5);
            ofSetColor(ofColor::greenYellow);
            ofCircle(0, 0, COUNTDOWN_RADIUS * 1.2);
            ofSetColor(ofColor::whiteSmoke);
            ofCircle(0, 0, COUNTDOWN_RADIUS);
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
            startCountdown();
            break;
        case 's':
            bHideGui = !bHideGui;
            break;
        case 'b':
            bpmTapper.tap();
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
    
    seqPos.removeListener(this, &ofApp::sequencerPositionChanged);
    seqWidth.removeListener(this, &ofApp::sequencerWidthChanged);
    seqHeight.removeListener(this, &ofApp::sequencerHeightChanged);
    
    currentThemeId.removeListener(this, &ofApp::currentThemeIdChanged);
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
    lastStep = 0;
    
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
//    bpmTapper.startFresh();
}

//--------------------------------------------------------------
void ofApp::sequencerPositionChanged(ofVec2f &newPos){
    sequencerArea.setPosition(newPos);
}

//--------------------------------------------------------------
void ofApp::sequencerWidthChanged(float &newWidth){
    sequencerArea.setWidth(newWidth);
}

//--------------------------------------------------------------
void ofApp::sequencerHeightChanged(float &newHeight){
    sequencerArea.setHeight(newHeight);
}

//--------------------------------------------------------------
void ofApp::currentThemeIdChanged(int &newId){
    
    currentTheme = &themes[newId];

}
