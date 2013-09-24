#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCenteredTrueTypeFont.h"
#include "ClockThread.h"
#include "MSATimer.h"
#include "ofxTweener.h"
#include "SequencerTheme.h"


#define COUNTDOWN               3
#define COUNTDOWN_RADIUS        180

#define COLUMNS                 4
#define ROWS                    4

#define SEQUENCER_MAX_WIDTH     800
#define SEQUENCER_MAX_HEIGHT    800

#define SCRUBBER_HEIGHT         8

#define LEVEL_0_TEMPO           80.0
#define LEVEL_1_TEMPO           100.0
#define LEVEL_2_TEMPO           120.0
#define LEVEL_3_TEMPO           140.0



class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    // Game
    void startCountdown();
    void startGame();
    void endGame();
    
    void setupThemes();
    void setupGUI();
    void drawCountdown();
    void drawScrubbers();
    void drawSequencer();
    
    // ofParameter handlers
    void sequencerPositionChanged(ofVec2f &newPos);
    void sequencerWidthChanged(float &newWidth);
    void sequencerHeightChanged(float &newHeight);
    void currentThemeIdChanged(int &newId);
    
    // Threaded Clock
    void phraseComplete();
    int calculateNoteDuration();
    
    void generateNewPattern();
    
    
    ofxPanel                gui;
    ofxButton               startCountdownButton, endGameButton;
    
    ofxCenteredTrueTypeFont font;
    
    ClockThread             clock;
    msa::Timer              countdownTimer;
    
    ofRectangle             sequencerArea;
    SequencerTheme          *currentTheme;
    vector<SequencerTheme>  themes;
    
    ofParameter<ofVec2f>    seqPos;
    ofParameter<float>      seqWidth;
    ofParameter<float>      seqHeight;
    ofParameter<int>        currentThemeId;
    ofParameter<int>        level_0_rounds;
    ofParameter<int>        level_1_rounds;
    ofParameter<int>        level_2_rounds;
    ofParameter<int>        level_3_rounds;
    
    int                     currentPattern[COLUMNS*ROWS];
    int                     previousPattern[COLUMNS*ROWS];
    unsigned int            playerCount;
    
    float                   tempo;
    bool                    bCountdownRunning;
    bool                    bGameRunning;
    bool                    bHideGui;
};
