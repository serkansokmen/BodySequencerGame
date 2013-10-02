//
//  GameEvent.h
//  BodySequencerGame
//
//  Created by Serkan Sökmen on 01.10.2013.
//
//

#pragma pnce

#include "ofMain.h"
#include "TrackCell.h"
#include "ofxTSPSPerson.h"


class GameEvent : public ofEventArgs {
    
public:
    
    static ofEvent <GameEvent> events;
    
    GameEvent() {
        person = NULL;
        trackCell = NULL;
    }
    
    ofxTSPS::Person *person;
    TrackCell       *trackCell;
    string          message;
    TrackCellState  cellState;
    
};
