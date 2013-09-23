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
    
    string      soundPath;
    ofImage     background;
    ofImage     interface;
    ofImage     splashImage;
    
public:
    
    void setup(string soundPath, string bgPath, string interfaceImagePath);
    void draw(bool bShowSplash);
    
    vector<ofSoundPlayer> players;
};
