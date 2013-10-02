//
//  TrackCell.cpp
//  TUIOSequencer
//
//  Created by Serkan Sokmen on 31.07.2013.
//
//

#include "TrackCell.h"


//--------------------------------------------------------------
void TrackCell::setup(const ofRectangle &bb, const ofColor &c){
    
    hue = c.getHue();
    saturation = c.getSaturation();
    brightness = 255;
    
    state = cellOff;
    color.set(hue, saturation, brightness);
    
    setBoundingBox(bb);
}

//--------------------------------------------------------------
void TrackCell::setBoundingBox(const ofRectangle &rect){
    
    float padding = 8.0f;
    
    boundingBox.set(rect);
    outerBox.setFromCenter(boundingBox.getCenter(), boundingBox.getWidth() - padding*.2, boundingBox.getHeight() - padding*.2);
    innerBox.setFromCenter(boundingBox.getCenter(), boundingBox.getWidth() - padding, boundingBox.getHeight() - padding);
}

//--------------------------------------------------------------
ofRectangle &TrackCell::getBoundingBox(){
    return boundingBox;
}

//--------------------------------------------------------------
void TrackCell::update(){
    color.set(hue, saturation, brightness);
}

//--------------------------------------------------------------
void TrackCell::draw(){
    
    ofSetLineWidth(1.0f);
    
    switch (state) {
        case cellOff:
            ofPushStyle();
            ofSetColor(color, 255);
            ofNoFill();
            ofRect(innerBox);
            ofPopStyle();
            break;
        case cellActive:
            ofPushStyle();
            ofSetColor(color, alpha);
            ofRect(innerBox);
            ofPopStyle();
            break;
        case cellOn:
            ofPushStyle();
            ofSetColor(color, alpha);
            ofRect(innerBox);
            ofPopStyle();
            break;
            
        default:
            break;
    }
};

//--------------------------------------------------------------
void TrackCell::setState(TrackCellState s){
    
    state = s;
    
    switch (state) {
        case cellOff:
            Tweener.addTween(alpha, 50, .2);
            break;
        case cellActive:
            Tweener.addTween(alpha, 150, .2);
            break;
        case cellOn:
            Tweener.addTween(alpha, 255, .2);
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
const TrackCellState &TrackCell::getState(){
    return state;
}

//--------------------------------------------------------------
void TrackCell::setColor(ofColor c){
    Tweener.addTween(hue, c.getHue(), .2);
    Tweener.addTween(saturation, c.getSaturation(), .2);
}

//--------------------------------------------------------------
ofColor &TrackCell::getColor(){
    return color;
}
