//
//  SequencerTheme.h
//  BodySequencer
//
//  Created by Serkan Sokmen on 17.09.2013.
//
//

#pragma once


#include "ofMain.h"


class SequencerTheme {
    
public:
    
    void setup(string soundPath, string bgPath);
    void draw();
    
    string      soundPath;
    ofImage     background;
    
    vector<ofSoundPlayer> players;
};
