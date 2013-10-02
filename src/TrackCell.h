//
//  TrackCell.h
//  TUIOSequencer
//
//  Created by Serkan Sokmen on 31.07.2013.
//
//

#pragma once

#include "ofMain.h"
#include "ofxTweener.h"


enum TrackCellState {
    cellOff,
    cellActive,
    cellOn,
};


class TrackCell {
    
    ofRectangle     outerBox;
    ofRectangle     innerBox;
    ofRectangle     boundingBox;
    
    TrackCellState  state;
    float           hue, saturation, brightness;
    ofColor         color;
    float           alpha;
    
public:
    
    void    setup(const ofRectangle &boundingBox, const ofColor &color);
    void    update();
    void    draw();
    
    void    setState(TrackCellState s);
    const   TrackCellState &getState();
    
    void    setColor(ofColor c);
    ofColor &getColor();
    
    void    setBoundingBox(const ofRectangle &rect);
    ofRectangle &getBoundingBox();
};
