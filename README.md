# Derived from videoPlayerExample

![Screenshot of videoPlayer](videoPlayer.png)

### Learning Objectives

This example is designed to demonstrate how to play video in openFrameworks.

* How to display a FileOpen dialog for selection of the video file to be played.
* How to play a video
* How to change the video playback speed
* How to use the mouse speed to control the video playback speed
* How to go to the previous/next frames using arrow keys
* How to iterate through a movie frame's pixels and use the pixel values to control the sizes of circles

Notice how ``movie.setSpeed()``, ``movie.setPaused()``, ``movie.setPosition()``, ``movie.nextFrame()``, ``movie.prevFrame()`` can be used to control the movie.



### Expected Behavior

Instructions for use:

* Try pressing the left and right arrow keys to go to the next/previous frame.
* Drag across the window to seek to a new location in the video.

### Classes used in this example

This example uses the following class: 

* ofVideoPlayer
* ofxMidi
