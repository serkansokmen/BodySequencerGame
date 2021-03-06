#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCenteredTrueTypeFont.h"
#include "ClockThread.h"
#include "MSATimer.h"
#include "ofxTSPSReceiver.h"
#include "ofxTweener.h"
#include "SequencerTheme.h"
#include "GameEvent.h"
#include "TrackCell.h"


#define COUNTDOWN               3
#define COUNTDOWN_RADIUS        180
#define COUNTDOWN_FONT_SIZE     80
#define DEFAULT_FONT_SIZE       50

#define MAX_PLAYER_COUNT        4

#define COLUMNS                 4
#define ROWS                    4

#define SEQUENCER_MAX_WIDTH     800
#define SEQUENCER_MAX_HEIGHT    800

#define SCRUBBER_HEIGHT         8

#define LEVEL_1_MIN_TEMPO       80.0
#define LEVEL_2_MIN_TEMPO       100.0
#define LEVEL_3_MIN_TEMPO       120.0
#define LEVEL_4_MIN_TEMPO       140.0



class ofApp : public ofBaseApp {

public:
    void    setup();
    void    update();
    void 	draw();
    
    void    keyPressed(int key);
    void    keyReleased(int key);
    void    mouseMoved(int x, int y );
    void    mouseDragged(int x, int y, int button);
    void    mousePressed(int x, int y, int button);
    void    mouseReleased(int x, int y, int button);
    void    windowResized(int w, int h);
    void    dragEvent(ofDragInfo dragInfo);
    void    gotMessage(ofMessage msg);
    void    exit();
    
    // Game
    void    initTrackCells();
    void    startCountdown();
    void    startGame();
    void    endGame();
    void    generateNewPattern(bool initial);
    void    resetPatterns();
    
    void    setupThemes();
    void    setupGUI();
    void    drawCountdown();
    void    drawScrubbers();
    void    drawSequencer();
    
    // ofParameter handlers
    void    sequencerPositionChanged(ofVec2f &newPos);
    void    sequencerWidthChanged(float &newWidth);
    void    sequencerHeightChanged(float &newHeight);
    void    currentThemeIdChanged(int &newId);
    
    // Threaded Clock
    void    phraseComplete();
    int     calculateNoteDuration();
    
    // OpenTSPS Event Listeners
    void    onPersonEntered(ofxTSPS::EventArgs &tspsEvent);
    void    onPersonUpdated(ofxTSPS::EventArgs &tspsEvent);
    void    onPersonWillLeave(ofxTSPS::EventArgs &tspsEvent);
    void    checkPersonInsideTrackCell(ofxTSPS::Person *person);
    
    // Game Event
    void    gameEvent(GameEvent &event);
    
    ofxTSPS::Receiver       tspsReceiver;
    
    ofxPanel                gui;
    ofxButton               startCountdownButton, endGameButton;
    
    ofxCenteredTrueTypeFont countdownFont, textFont;
    
    ClockThread             clock;
    msa::Timer              countdownTimer;
    
    ofRectangle             sequencerArea;
    SequencerTheme          *currentTheme;
    vector<SequencerTheme>  themes;
    vector<TrackCell>       cells;
    
    ofParameterGroup        sequencerParameters;
    ofParameter<int>        playerCount;
    ofParameter<ofVec2f>    seqPos;
    ofParameter<float>      seqWidth;
    ofParameter<float>      seqHeight;
    ofParameter<int>        currentThemeId;
    
    ofParameterGroup        level_1_parameters;
    ofParameterGroup        level_2_parameters;
    ofParameterGroup        level_3_parameters;
    ofParameterGroup        level_4_parameters;
    ofParameter<float>      level_1_tempo;
    ofParameter<float>      level_2_tempo;
    ofParameter<float>      level_3_tempo;
    ofParameter<float>      level_4_tempo;
    ofParameter<int>        level_1_rounds;
    ofParameter<int>        level_2_rounds;
    ofParameter<int>        level_3_rounds;
    ofParameter<int>        level_4_rounds;
    string                  currentLevelStr;
    
    int                     currentPattern[COLUMNS*ROWS];
    int                     previousPattern[COLUMNS*ROWS];
    
    float                   tempo;
    bool                    bCountdownRunning;
    bool                    bGameRunning;
    bool                    bHideGui;
};
