#include "ofApp.h"
#include "ofxMidi.h"

const int vidLeft = 0; // 20;
const int vidTop = 0; // 170;

const float minAspectFactor = -1.0f;
const float maxAspectFactor = 1.0f;

int vidHeight = 0;
int vidWidth = 0;
int nChannels = 0;

//--------------------------------------------------------------
void ofApp::setup() {
  ofSetLogLevel(OF_LOG_VERBOSE);

  //old OF default is 96 - but this results in fonts looking larger than in other programs.
  ofTrueTypeFont::setGlobalDpi(72);

  ovFont.load("verdana.ttf", 32, true);
  ovFont.setLineHeight(18.0f);
  ovFont.setLetterSpacing(1.037);

  ofBackground(0, 0, 0);
  //ofSetVerticalSync(true);
  ofSetVerticalSync(false); // disable to fix performance problems.
  frameByframe = false;
  adjustAspect = false;
  aspectFactor = 0.4;

  midiInp.listInPorts();
  midiInp.openPort(4);
  midiInp.ignoreTypes(false, false, false);
  midiInp.addListener(this);
  midiInp.setVerbose(true);

  // Uncomment this to show movies with alpha channels
  //    video.setPixelFormat(OF_PIXELS_RGBA);

  loadVideo();
//  ofSetWindowPosition(0, 0);
  ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
  video.play();
}

void ofApp::exit(void) {
  if (midiInp.isOpen()) {
    midiInp.closePort();
    midiInp.removeListener(this);
  }
  ofBaseApp::exit();
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
  midiMessages.push_back(msg);
  ofLog(OF_LOG_NOTICE, msg.toString());
  
  while (midiMessages.size() > maxMessages) {
    midiMessages.erase(midiMessages.begin());
  }
}

//--------------------------------------------------------------
void ofApp::update() {
  video.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
  ofSetHexColor(0xcccccc);
  //ofDrawBitmapString("press f to change", 20, 20);

  if (frameByframe)
    ofSetHexColor(0xCCCCCC);

  if (video.getIsMovieDone()) {
    ofSetHexColor(0xFF0000);
    ofDrawBitmapString("end of movie", 20, 170);
  }

  ofSetHexColor(0xFFFFFF);

  int vWidth = vidWidth;
  int vTop = vidTop;
  if (adjustAspect) {
    vTop = ofGetScreenHeight() / 2 - video.getHeight() / 2;
    //vWidth = vidWidth + (vidWidth * aspectFactor);
    //vWidth = ofGetScreenWidth();
    vWidth = vidWidth * aspectFactor;
  }
  //video.draw(vidLeft, vidTop);
  //video.draw(vidLeft, vidTop, vWidth, vidHeight);
  video.draw(vidLeft, vTop, vWidth, vidHeight);

  ofSetHexColor(0xffffff);
  ofDrawLine(vidLeft, vidTop, vidLeft + vWidth + 1, vidTop);
  ofDrawLine(vidLeft + vWidth + 1, vidTop, vidLeft + vWidth + 1, vidTop + vidHeight + 1);


  //ofSetColor(3, 3, 3, 127);
  ofSetColor(0, 0, 0, 127);

  const float rcMargin = 10.0;
  const float rcRound = 9.0;

  ofRectangle metaRect(vidLeft + rcMargin, vidTop + rcMargin, vWidth - rcMargin * 2, 100);
  ////ofDrawRectRounded(vidLeft + 20, vidTop + 20, vWidth - 20 - 20, 100, 6);
  ofDrawRectRounded(metaRect, rcRound);

  ofRectangle nameRect(vidLeft + rcMargin, (vidTop + vidHeight) - 210.0, vWidth / 3.0, 200);
  ofDrawRectRounded(nameRect, rcRound);
  ofSetHexColor(0xaaaaaa7f);
  
  float lineSpacing = ovFont.getAscenderHeight() + ovFont.getLineHeight() + ovFont.getDescenderHeight();

  ofSetColor(200); // , 255, 255, 255);
  ovFont.drawString(ofToString(video.getCurrentFrame()) + "/" + ofToString(video.getTotalNumFrames()) + " : " + ofToString(video.getSpeed()), 
    nameRect.getLeft() + 4, 
    nameRect.getTop() + 1 * lineSpacing);
  ovFont.drawString(ofToString(video.getPosition() * video.getDuration(), 2) + "/" + ofToString(video.getDuration(), 2), 
    nameRect.getLeft() + 4,
    nameRect.getTop() + 2 * lineSpacing);
  ovFont.drawString(ofToString(vidWidth) + "x" + ofToString(vidHeight)
    + ", " + ofToString(video.getWidth()) + "x" + ofToString(video.getHeight())
    + ", " + ofToString(vidWidth * aspectFactor) + "x" + ofToString(video.getHeight()) 
    + ", " + ofToString(aspectFactor)
    + ", channels: " + ofToString(nChannels),
    nameRect.getLeft() + 4,
    nameRect.getTop() + 3 * lineSpacing);
  ovFont.drawString(ofToString(video.getPixelFormat()),
    nameRect.getLeft() + 4,
    nameRect.getTop() + 4 * lineSpacing);

  // x, y, w, h
  ofRectangle subtRect(vidLeft + vWidth - (vWidth / 3.0) - rcMargin, (vidTop + vidHeight) - 210.0, vWidth / 3.0, 200.0);
  ofSetColor(0, 0, 0, 0x3f);
  ofDrawRectRounded(subtRect, rcRound); 
}

int ofApp::loadVideo() {
  ofFileDialogResult result = ofSystemLoadDialog("Select a video file");
  if (!result.bSuccess) {
    ofExit(1);
  }
  if (video.isLoaded()) {
    video.close();
  }

  video.setLoopState(OF_LOOP_NORMAL);
  video.setPixelFormat(OF_PIXELS_RGBA);
  //video.setPixelFormat(OF_PIXELS_RGB);
  video.setUseTexture(true);
  //video.setUseTexture(false);

  if (!video.load(result.getPath())) {
    ofExit(2);
  }

  ofPixels &pixels = video.getPixels();
  vidHeight = pixels.getHeight();
  vidWidth = pixels.getWidth();
  nChannels = pixels.getNumChannels();

  aspectFactor = (float)vidWidth / (float)vidHeight;
  ofLog(OF_LOG_NOTICE, ofToString(aspectFactor));

  return 0;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  switch (key) {
  case 'o':
    loadVideo();
    video.play();
    break;
  case 'f':
    frameByframe = !frameByframe;
    video.setPaused(frameByframe);
    break;
  case 'a':
    adjustAspect = !adjustAspect;
    break;
  case OF_KEY_LEFT:
    if (OF_KEY_SHIFT) {
      aspectFactor = ofClamp(aspectFactor - 0.1, minAspectFactor, maxAspectFactor);
      break;
    }
    video.previousFrame();
    break;
  case OF_KEY_RIGHT:
    if (OF_KEY_SHIFT) {
      aspectFactor = ofClamp(aspectFactor + 0.1, minAspectFactor, maxAspectFactor);
    }
    video.nextFrame();
    break;
  case '0':
    video.firstFrame();
    break;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
  if (!frameByframe) {
    int width = ofGetWidth();
    float pct = (float)x / (float)width;
    float speed = (2 * pct - 1) * 5.0f;
    //        video.setSpeed(speed);
  }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
  if (!frameByframe) {
    int width = ofGetWidth();
    float pct = (float)x / (float)width;
    video.setPosition(pct);
  }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  if (!frameByframe) {
    video.setPaused(true);
  }
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
  if (!frameByframe) {
    video.setPaused(false);
  }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}