//
//  SequencerTheme.cpp
//  BodySequencer
//
//  Created by Serkan Sökmen on 17.09.2013.
//
//

#include "SequencerTheme.h"


void SequencerTheme::setup(string soundPath, string bgPath){
    
    this->soundPath = soundPath;
    
    background.loadImage(bgPath);
    
    // Sound
    ofDirectory dir;
    dir.listDir(soundPath);
    
    if (dir.size()){
        dir.sort();
        
        for (int i=0; i<dir.size(); i++){
            ofSoundPlayer player;
            player.loadSound(dir.getPath(i));
            players.push_back(player);
        }
    }
}