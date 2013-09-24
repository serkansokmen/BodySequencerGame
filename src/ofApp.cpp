#include "ofApp.h"


#pragma mark - ofApp
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
    
    setupThemes();
    setupGUI();
    
    bCountdownRunning = false;
    bGameRunning = false;
    clock.notesPerPhrase = COLUMNS;
    tempo = LEVEL_0_TEMPO;
    playerCount = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    Tweener.update();
    
    if (bCountdownRunning && !bGameRunning && countdownTimer.getSeconds() >= COUNTDOWN){
        startGame();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Draw background with or without splash
    currentTheme->draw(!bGameRunning && !bCountdownRunning);
    
    if (bCountdownRunning) drawCountdown();
    
    if (bGameRunning){
        drawScrubbers();
        drawSequencer();
    }
    
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
        case 'g':
            generateNewPattern();
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
    seqPos.removeListener(this, &ofApp::sequencerPositionChanged);
    seqWidth.removeListener(this, &ofApp::sequencerWidthChanged);
    seqHeight.removeListener(this, &ofApp::sequencerHeightChanged);
    
    currentThemeId.removeListener(this, &ofApp::currentThemeIdChanged);
}

#pragma mark - Game
//--------------------------------------------------------------
void ofApp::setupThemes(){
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
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    
    gui.setup("Body Sequencer");
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
    seqPos.addListener(this, &ofApp::sequencerPositionChanged);
    seqWidth.addListener(this, &ofApp::sequencerWidthChanged);
    seqHeight.addListener(this, &ofApp::sequencerHeightChanged);
    currentThemeId.addListener(this, &ofApp::currentThemeIdChanged);
    
    gui.loadFromFile("settings.xml");
    bHideGui = false;
}

//--------------------------------------------------------------
void ofApp::startCountdown(){
    
    bGameRunning = false;
    bCountdownRunning = true;
    
    countdownTimer.start();
    
    ofLog(OF_LOG_NOTICE, "Starting game in " + ofToString(COUNTDOWN) + " seconds");
}

//--------------------------------------------------------------
void ofApp::startGame(){
    
    bGameRunning = true;
    bCountdownRunning = false;
    
    countdownTimer.stop();
    generateNewPattern();
    clock.start(this);
    
    ofLog(OF_LOG_NOTICE, "Game started");
}

//--------------------------------------------------------------
void ofApp::endGame(){
    bGameRunning = false;
    
    if (clock.isThreadRunning()){
        clock.stop();
    }
    
    ofLog(OF_LOG_NOTICE, "Game ended");
}

//--------------------------------------------------------------
void ofApp::generateNewPattern(){
    
    // Generate random pattern
    vector<int> vals;
    vals.assign(COLUMNS*ROWS, 0);
    for (int i=0; i<playerCount; i++){
        vals[i] = 1;
    }
    ofRandomize(vals);
    
    // Assign random pattern to current pattern values
    for (int j=0; j<vals.size(); j++){
        currentPattern[j] = vals[j];
    }
}

#pragma mark - Draw methods
//--------------------------------------------------------------
void ofApp::drawCountdown(){
    // Draw Countdown
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5);
    ofSetColor(ofColor::greenYellow, 20);
    ofCircle(0, 0, COUNTDOWN_RADIUS * 1.05);
    ofSetColor(ofColor::whiteSmoke, 20);
    ofCircle(0, 0, COUNTDOWN_RADIUS);
    ofSetColor(ofColor::white);
    font.drawStringCentered(ofToString(COUNTDOWN - (int)countdownTimer.getSeconds()), 0, 0);
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawScrubbers(){
    
    int currentStep = clock.notes;
    
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
}

//--------------------------------------------------------------
void ofApp::drawSequencer(){
    
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
            if (currentPattern[gridIndex] == 1 || previousPattern[gridIndex] == 1){
                
                if (previousPattern[gridIndex] == 1){
                    ofSetColor(ofColor::greenYellow);
                }
                if (currentPattern[gridIndex] == 1){
                    ofSetColor(ofColor::blueSteel);
                }
                
            } else {
                ofNoFill();
            }
            ofRect(cellRect);
            ofPopStyle();
            
        }
    }
    ofPopMatrix();
}

#pragma mark - ofParameter Handlers
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

#pragma mark - Threaded Clock
//--------------------------------------------------------------
void ofApp::phraseComplete(){
    
    cout << "Phrase complete" << endl;
    
    for (int i=0; i<COLUMNS*ROWS; i++) {
        previousPattern[i] = currentPattern[i];
    }
    generateNewPattern();
    
    
    int totalSteps = clock.totalNotes;
    int currentStep = totalSteps % COLUMNS;
    
    if (totalSteps < level_0_rounds*COLUMNS){
        ofLog(OF_LOG_NOTICE, "LEVEL 0");
        tempo = LEVEL_0_TEMPO;
    }
    
    else if (totalSteps >= level_0_rounds*COLUMNS &&
             totalSteps < (level_0_rounds+level_1_rounds)*COLUMNS){
        ofLog(OF_LOG_NOTICE, "LEVEL 1");
        tempo = LEVEL_1_TEMPO;
    }
    
    else if (totalSteps >= (level_0_rounds+level_1_rounds)*COLUMNS &&
             totalSteps < (level_0_rounds+level_1_rounds+level_2_rounds)*COLUMNS){
        ofLog(OF_LOG_NOTICE, "LEVEL 2");
        tempo = LEVEL_2_TEMPO;
    }
    
    else if (totalSteps >= (level_0_rounds+level_1_rounds+level_2_rounds)*COLUMNS &&
             totalSteps < (level_0_rounds+level_1_rounds+level_2_rounds+level_3_rounds)*COLUMNS){
        ofLog(OF_LOG_NOTICE, "LEVEL 3");
        tempo = LEVEL_3_TEMPO;
    }
    
    else if (totalSteps > (level_0_rounds+level_1_rounds+level_2_rounds+level_3_rounds)*COLUMNS){
        endGame();
    }
}

//--------------------------------------------------------------
int ofApp::calculateNoteDuration(){
    
    // Translate tempo to milliseconds
    return (int)floor(60000.0000f / tempo);
}
