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
    
    countdownFont.loadFont("type/Comfortaa-Light.ttf", COUNTDOWN_FONT_SIZE);
    textFont.loadFont("type/Comfortaa-Light.ttf", DEFAULT_FONT_SIZE);
    
    initTrackCells();
    setupThemes();
    setupGUI();
    
    bCountdownRunning = false;
    bGameRunning = false;
    clock.notesPerPhrase = COLUMNS;
    tempo = level_1_tempo;
    
    // Setup OpenTSPS
    tspsReceiver.connect(12000);
    currentLevelStr = "";
}

//--------------------------------------------------------------
void ofApp::update(){
    
    Tweener.update();
    
    if (bCountdownRunning && !bGameRunning && countdownTimer.getSeconds() >= COUNTDOWN){
        startGame();
    }
    
    if (bGameRunning){
        for (int i=0; i<cells.size(); i++) {
            cells[i].update();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Draw background with or without splash
    currentTheme->draw(!bGameRunning && !bCountdownRunning);
    
    if (bCountdownRunning) drawCountdown();
    
    if (bGameRunning){
        
        drawSequencer();
        drawScrubbers();
            
        textFont.drawStringCentered(currentLevelStr,
                                    ofGetWidth()/2,
                                    sequencerArea.getY() - DEFAULT_FONT_SIZE - 20);
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
            generateNewPattern(true);
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
    gui.add(playerCount.set("Player Count", 1, 1, MAX_PLAYER_COUNT));
    gui.add(startCountdownButton.setup("Start Game"));
    gui.add(endGameButton.setup("End Game"));
    
    sequencerParameters.setName("Sequencer");
    sequencerParameters.add(seqPos.set("Position",
                                       ofVec2f((ofGetWidth() - SEQUENCER_MAX_WIDTH)*.5,
                                               (ofGetHeight() - SEQUENCER_MAX_HEIGHT)*.5),
                                       ofVec2f(0, 0),
                                       ofVec2f(ofGetWidth(), ofGetHeight())));
    sequencerParameters.add(seqWidth.set("Width", SEQUENCER_MAX_WIDTH, 0, SEQUENCER_MAX_WIDTH));
    sequencerParameters.add(seqHeight.set("Height", SEQUENCER_MAX_HEIGHT, 0, SEQUENCER_MAX_HEIGHT));
    sequencerParameters.add(currentThemeId.set("Current Theme", 0, 0, themes.size() - 1));
    gui.add(sequencerParameters);
    
    level_1_parameters.setName("Level 1");
    level_1_parameters.add(level_1_tempo.set("Tempo", LEVEL_1_MIN_TEMPO, LEVEL_1_MIN_TEMPO, LEVEL_2_MIN_TEMPO));
    level_1_parameters.add(level_1_rounds.set("Rounds", 2, 1, 50));
    gui.add(level_1_parameters);
    
    level_2_parameters.setName("Level 2");
    level_2_parameters.add(level_2_tempo.set("Tempo", LEVEL_2_MIN_TEMPO, LEVEL_2_MIN_TEMPO, LEVEL_3_MIN_TEMPO));
    level_2_parameters.add(level_2_rounds.set("Rounds", 2, 1, 20));
    gui.add(level_2_parameters);
    
    level_3_parameters.setName("Level 3");
    level_3_parameters.add(level_3_tempo.set("Tempo", LEVEL_3_MIN_TEMPO, LEVEL_3_MIN_TEMPO, LEVEL_4_MIN_TEMPO));
    level_3_parameters.add(level_3_rounds.set("Rounds", 2, 1, 10));
    gui.add(level_3_parameters);
    
    level_4_parameters.setName("Level 4");
    level_4_parameters.add(level_4_tempo.set("Tempo", LEVEL_4_MIN_TEMPO, LEVEL_4_MIN_TEMPO, 200.f));
    level_4_parameters.add(level_4_rounds.set("Rounds", 2, 1, 5));
    gui.add(level_4_parameters);
    
    startCountdownButton.addListener(this, &ofApp::startCountdown);
    endGameButton.addListener(this, &ofApp::endGame);
    seqPos.addListener(this, &ofApp::sequencerPositionChanged);
    seqWidth.addListener(this, &ofApp::sequencerWidthChanged);
    seqHeight.addListener(this, &ofApp::sequencerHeightChanged);
    currentThemeId.addListener(this, &ofApp::currentThemeIdChanged);
    
    gui.setBackgroundColor(ofColor::black);
    gui.setBorderColor(ofColor::white);
    
    gui.loadFromFile("settings.xml");
    bHideGui = false;
}

//--------------------------------------------------------------
void ofApp::initTrackCells(){
    
    cells.assign(COLUMNS*ROWS, TrackCell());
    
    for (int j=0; j<ROWS; j++) {
        for (int i=0; i<COLUMNS; i++) {
            int gridIndex = i + j * COLUMNS;
            
            float cw = sequencerArea.getWidth()/COLUMNS;
            float ch = sequencerArea.getHeight()/ROWS;
            float cx = i * cw;
            float cy = j * ch;
            
            ofRectangle cellRect(cx, cy, cw, ch);
            ofColor cellColor(ofColor::blueSteel);
            cellColor.setHue(ofMap(j, 0, ROWS, 0, 255));
            cells[gridIndex].setup(cellRect, cellColor);
        }
    }
}

//--------------------------------------------------------------
void ofApp::startCountdown(){
    
    bGameRunning = false;
    bCountdownRunning = true;
    tempo = level_1_tempo;
    currentLevelStr = "LEVEL 1";
    countdownTimer.start();
    
    ofLog(OF_LOG_NOTICE,
          "Starting game in " + ofToString(COUNTDOWN) + " seconds");
}

//--------------------------------------------------------------
void ofApp::startGame(){
    
    bGameRunning = true;
    bCountdownRunning = false;
    
    countdownTimer.stop();
    generateNewPattern(true);
    if (clock.isThreadRunning()){
        clock.stop();
    }
    clock.start(this);
    
    ofxAddTSPSListeners(this);
    
    // listen to any of the events for the game
    ofAddListener(GameEvent::events, this, &ofApp::gameEvent);
    
    ofLog(OF_LOG_NOTICE, "Game started");
}

//--------------------------------------------------------------
void ofApp::endGame(){
    
    bGameRunning = false;
    resetPatterns();
    
    if (clock.isThreadRunning()){
        clock.stop();
    }
    
    ofxRemoveTSPSListeners(this);
    
    // listen to any of the events for the game
    ofRemoveListener(GameEvent::events, this, &ofApp::gameEvent);
    
    ofLog(OF_LOG_NOTICE, "Game ended");
}

//--------------------------------------------------------------
void ofApp::gameEvent(GameEvent &e) {
    
    e.trackCell->setState(e.cellState);
}

//--------------------------------------------------------------
void ofApp::generateNewPattern(bool initial){
    
    // Temporary values vector
    vector<int> vals;
    vals.assign(COLUMNS*ROWS, 0);
    
    if (initial){
        // Generate initial pattern
        for (int i=0; i<playerCount; i++){
            int randIndex = 3 + i * COLUMNS;
            vals[randIndex] = 1;
        }
    } else {
        // Assign current pattern to previous
        for (int i=0; i<COLUMNS*ROWS; i++) {
            previousPattern[i] = currentPattern[i];
        }
        // Randomize pattern
        for (int i=0; i<playerCount; i++){
            vals[i] = 1;
        }
        ofRandomize(vals);
        // FIXME: Check if previous and current overlaps
    }
    
    // Assign pattern to current pattern values
    for (int j=0; j<vals.size(); j++){
        currentPattern[j] = vals[j];
    }
}

//--------------------------------------------------------------
void ofApp::resetPatterns(){
    
    // Generate zero pattern
    vector<int> vals;
    vals.assign(COLUMNS*ROWS, 0);
    
    // Assign zero pattern to pattern values
    for (int j=0; j<vals.size(); j++){
        currentPattern[j] = vals[j];
        previousPattern[j] = vals[j];
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
    string msg = ofToString(COUNTDOWN - (int)countdownTimer.getSeconds());
    countdownFont.drawStringCentered(msg, 0, 0);
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
    for (int j=0; j<ROWS; j++) {
        for (int i=0; i<COLUMNS; i++) {
            int gridIndex = i + j * COLUMNS;
            
            cells[gridIndex].draw();
            
//            ofPushStyle();
//            ofSetColor(ofColor::blueSteel);
//            if (currentPattern[gridIndex] == 1 || previousPattern[gridIndex] == 1){
//                
//                if (previousPattern[gridIndex] == 1){
//                    ofSetColor(ofColor::grey, 150);
//                }
//                if (currentPattern[gridIndex] == 1){
//                    ofSetColor(ofColor::grey);
//                }
//                
//            } else {
//                ofNoFill();
//            }
//            ofRect(cellRect);
//            ofPopStyle();
            
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
    
    for (int j=0; j<ROWS; j++) {
        for (int i=0; i<COLUMNS; i++) {
            int gridIndex = i + j * COLUMNS;
            
            float cw = sequencerArea.getWidth()/COLUMNS;
            float ch = sequencerArea.getHeight()/ROWS;
            float cx = i * cw;
            float cy = j * ch;
            
            cells[gridIndex].setBoundingBox(ofRectangle(cx, cy, cw, ch));
        }
    }
}

//--------------------------------------------------------------
void ofApp::sequencerHeightChanged(float &newHeight){
    sequencerArea.setHeight(newHeight);
    
    for (int j=0; j<ROWS; j++) {
        for (int i=0; i<COLUMNS; i++) {
            int gridIndex = i + j * COLUMNS;
            
            float cw = sequencerArea.getWidth()/COLUMNS;
            float ch = sequencerArea.getHeight()/ROWS;
            float cx = i * cw;
            float cy = j * ch;
            
            cells[gridIndex].setBoundingBox(ofRectangle(cx, cy, cw, ch));
        }
    }
}

//--------------------------------------------------------------
void ofApp::currentThemeIdChanged(int &newId){
    currentTheme = &themes[newId];
}

#pragma mark - Threaded Clock
//--------------------------------------------------------------
void ofApp::phraseComplete(){
    
    ofLog(OF_LOG_NOTICE, "Phrase Complete");
    
    int totalSteps = clock.totalNotes;
    int currentStep = totalSteps % COLUMNS;
    
    if (totalSteps < (level_1_rounds+level_2_rounds+level_3_rounds+level_4_rounds)*COLUMNS)
        generateNewPattern(false);
    
    if (totalSteps < level_1_rounds*COLUMNS){
        currentLevelStr = "LEVEL 1";
        ofLog(OF_LOG_NOTICE, currentLevelStr);
        tempo = level_1_tempo;
    }
    
    else if (totalSteps >= level_1_rounds*COLUMNS &&
             totalSteps < (level_1_rounds+level_2_rounds)*COLUMNS){
        currentLevelStr = "LEVEL 2";
        ofLog(OF_LOG_NOTICE, currentLevelStr);
        tempo = level_2_tempo;
    }
    
    else if (totalSteps >= (level_1_rounds+level_2_rounds)*COLUMNS &&
             totalSteps < (level_1_rounds+level_2_rounds+level_3_rounds)*COLUMNS){
        currentLevelStr = "LEVEL 3";
        ofLog(OF_LOG_NOTICE, currentLevelStr);
        tempo = level_3_tempo;
    }
    
    else if (totalSteps >= (level_1_rounds+level_2_rounds+level_3_rounds)*COLUMNS &&
             totalSteps < (level_1_rounds+level_2_rounds+level_3_rounds+level_4_rounds)*COLUMNS){
        currentLevelStr = "LEVEL 4";
        ofLog(OF_LOG_NOTICE, currentLevelStr);
        tempo = level_4_tempo;
    }
    
    else if (totalSteps >= (level_1_rounds+level_2_rounds+level_3_rounds+level_4_rounds)*COLUMNS){
        endGame();
    }
}

//--------------------------------------------------------------
int ofApp::calculateNoteDuration(){
    // Translate tempo to milliseconds
    return (int)floor(60000.0000f / tempo);
}


#pragma mark - OpenTSPS Listeners
//--------------------------------------------------------------
void ofApp::onPersonEntered(ofxTSPS::EventArgs &tspsEvent){
    checkPersonInsideTrackCell(tspsEvent.person);
//    ofLog(OF_LOG_NOTICE, "Person entered");
}

//--------------------------------------------------------------
void ofApp::onPersonUpdated(ofxTSPS::EventArgs &tspsEvent){
    checkPersonInsideTrackCell(tspsEvent.person);
//    ofLog(OF_LOG_NOTICE, "Person updated!");
}

//--------------------------------------------------------------
void ofApp::onPersonWillLeave(ofxTSPS::EventArgs &tspsEvent){
    checkPersonInsideTrackCell(tspsEvent.person);
//    ofLog(OF_LOG_NOTICE, "Person left");
}

//--------------------------------------------------------------
void ofApp::checkPersonInsideTrackCell(ofxTSPS::Person *person){
    
    ofPoint loc = ofPoint(person->centroid.x * sequencerArea.getWidth(),
                          person->centroid.y * sequencerArea.getHeight());
    
    int currentStep = clock.notes;
    
    for (int rowIndex=0; rowIndex<ROWS; rowIndex++) {
        for (int columnIndex=0; columnIndex<ROWS; columnIndex++) {
            
            int cellIndex = columnIndex + rowIndex * COLUMNS;
            
            ofPoint loc = ofPoint(person->centroid.x * sequencerArea.getWidth(),
                                  person->centroid.y * sequencerArea.getHeight());
            
            if (cells[cellIndex].getBoundingBox().inside(loc)){
                
                if (rowIndex == currentStep){
                    static GameEvent cellCatchEvent;
                    cellCatchEvent.trackCell = &cells[cellIndex];
                    cellCatchEvent.person = person;
                    cellCatchEvent.cellState = cellOn;
                    ofNotifyEvent(GameEvent::events, cellCatchEvent);
                } else {
                    static GameEvent cellAwaitsEvent;
                    cellAwaitsEvent.trackCell = &cells[cellIndex];
                    cellAwaitsEvent.person = person;
                    cellAwaitsEvent.cellState = cellActive;
                    ofNotifyEvent(GameEvent::events, cellAwaitsEvent);
                }
                
            } else {
                static GameEvent cellMissedEvent;
                cellMissedEvent.trackCell = &cells[cellIndex];
                cellMissedEvent.person = person;
                cellMissedEvent.cellState = cellOff;
                ofNotifyEvent(GameEvent::events, cellMissedEvent);
            }
        }
    }
}
