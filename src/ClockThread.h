//
//  ClockThread.h
//  BodySequencer
//
//  Created by Serkan Sökmen on 24.09.2013.
//
//

#pragma once

#include "ofMain.h"

class ofApp;

class ClockThread : public ofThread {
    
    ofApp *parent;
    
public:
    
    // Timing properties
    int totalNotes;
    int notes;
    int notesPerPhrase;
    
    // Methods
    void start(ofApp *app);
    void stop();
    void threadedFunction();
    
};
