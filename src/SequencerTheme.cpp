//
//  SequencerTheme.cpp
//  BodySequencer
//
//  Created by Serkan Sökmen on 17.09.2013.
//
//

#include "SequencerTheme.h"


void SequencerTheme::setup(string soundPath, string bgPath, string interfaceImagePath){
    
    this->soundPath = soundPath;
    
    background.loadImage(bgPath);
    interface.loadImage(interfaceImagePath);
    splashImage.loadImage("images/splash.png");
    
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

void SequencerTheme::draw(bool bShowSplash){
    
    if (bShowSplash){
        background.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofPushMatrix();
        ofTranslate((background.getWidth()-splashImage.getWidth())*.5, (background.getHeight()-splashImage.getHeight())*.5 + 40);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(ofColor::white);
        splashImage.draw(0, 0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofPopMatrix();
    } else {
        interface.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
}
