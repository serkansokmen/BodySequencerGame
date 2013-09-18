#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "MSABPMTapper.h"
#include "MSATimer.h"


#define COUNTDOWN   3


class ofApp : public ofBaseApp{
    
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
    
    ofParameter<float>  bpm;
    
    msa::BPMTapper      bpmTapper;
    msa::Timer          startTimer;

    bool        bCountdownRunning;
    bool        bGameRunning;
    bool        bHideGui;
};
