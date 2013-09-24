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
#define START_TEMPO             80.0

#define SCRUBBER_HEIGHT         8



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
    
    void startCountdown();
    void startGame();
    void endGame();
    
    void tempoChanged(float &newVal);
    
    void sequencerPositionChanged(ofVec2f &newPos);
    void sequencerWidthChanged(float &newWidth);
    void sequencerHeightChanged(float &newHeight);
    
    void currentThemeIdChanged(int &newId);
    
    // Events
    void phraseComplete();
    
    // Gets note duration
    int calculateNoteDuration();
    
    
    ofxPanel                gui;
    ofxButton               startCountdownButton, endGameButton;
    
    ofxCenteredTrueTypeFont font;
    
    // Clock
    ClockThread             clock;
    ofParameter<float>      tempo;
    
    msa::Timer              startTimer;
    
    ofRectangle             sequencerArea;
    ofParameter<ofVec2f>    seqPos;
    ofParameter<float>      seqWidth;
    ofParameter<float>      seqHeight;
    
    ofParameter<int>        currentThemeId;
    SequencerTheme          *currentTheme;
    vector<SequencerTheme>  themes;

    bool                    bCountdownRunning;
    bool                    bGameRunning;
    bool                    bHideGui;
    
    ofParameterGroup parameters;
    ofXml settings;
    
    ofParameter<int>        level_0_rounds;
    ofParameter<int>        level_1_rounds;
    ofParameter<int>        level_2_rounds;
    ofParameter<int>        level_3_rounds;
};
