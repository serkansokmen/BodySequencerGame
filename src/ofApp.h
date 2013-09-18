#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCenteredTrueTypeFont.h"
#include "MSABPMTapper.h"
#include "MSATimer.h"


#define COUNTDOWN           3

#define COLUMNS             4
#define ROWS                4

#define SEQUENCER_WIDTH     600
#define SEQUENCER_HEIGHT    600

#define SCRUBBER_HEIGHT     8


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
    
    void bpmChanged(float &newVal);
    
    ofxPanel    gui;
    ofxButton   startCountdownButton, endGameButton;
    
    ofxCenteredTrueTypeFont font;
    
    ofParameter<float>  bpm;
    ofParameter<bool>   bDrawBpmTapper;
    ofParameter<int>    currentStep;
    
    msa::BPMTapper      bpmTapper;
    msa::Timer          startTimer;
    
    ofRectangle         sequencerArea;
    ofPlanePrimitive    sequencerPlane;

    bool        bCountdownRunning;
    bool        bGameRunning;
    bool        bHideGui;
};
