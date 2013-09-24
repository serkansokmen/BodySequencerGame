#include "ClockThread.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ClockThread::start(ofApp* p)
{
    
    // Get parent app
    parent = p;
    
    // Initialize note count
    notes = 0;
    totalNotes = 0;
    
    // Start thread -- blocking, venbose
    startThread(true, false);
    
}

//--------------------------------------------------------------
void ClockThread::stop()
{
    
    // Stop thread
    stopThread();
    
}

//--------------------------------------------------------------
void ClockThread::threadedFunction()
{
    // Thread is running
    while (isThreadRunning() != 0){
        // Lock thread
        if (lock()){
            // Increment count and unlock
            notes++;
            totalNotes++;
            unlock();
            
            // Phrase complete
            if (notes >= notesPerPhrase){
                // Call function on main app
                parent->phraseComplete();
                
                // Reset count
                notes = 0;
            }
            // Sleep for duration of one note
            ofSleepMillis(parent->calculateNoteDuration());
        }
    }
}
