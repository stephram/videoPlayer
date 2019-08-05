#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

	public:

		void setup();
		void update();
		void draw();

    virtual void exit(void);

    int loadVideo(void);
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		ofVideoPlayer 		video;
		bool frameByframe;
    bool adjustAspect;
    float aspectFactor;

    ofxMidiIn midiInp;

    //ofTrueTypeFont ttFont;
    ofTrueTypeFont ovFont;

    void newMidiMessage(ofxMidiMessage& eventArgs);

    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 20;
};

