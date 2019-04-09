/**
 * An extended and modified version of 
 * Flocking 
 * by Daniel Shiffman.  
 * 
 * An implementation of Craig Reynold's Boids program to simulate
 * the flocking behavior of birds. Each boid steers itself based on 
 * rules of avoidance, alignment, and coherence.
 * 
 * Click the mouse to add a new boid.
 */

import org.openkinect.freenect.*;
import org.openkinect.processing.*; // imports the OpenKinect library used in image processing

float px;
float py;

Flock flock;
Kinect kinect;
KinectTracker tracker;

void setup() {
  
  kinect = new Kinect(this);
  kinect.initDepth();
  kinect.initVideo();
  //kinect.enableIR(ir);
  //kinect.enableColorDepth(colorDepth);
  //deg = kinect.getTilt();
  
  size(640, 360);
  flock = new Flock();
  tracker = new KinectTracker();
  // Add an initial set of boids into the system
  for (int i = 0; i < 150; i++) {
    flock.addBoid(new Boid(width/2,height/2));
  }
}

void draw() {
  background(50);
  //image(kinect.getVideoImage(), 0, 0);
  //image(kinect.getDepthImage(), 640, 0);
  tracker.display();
  tracker.track();
  flock.run();
  stroke(255);
  if (mousePressed == true) {
    line(tracker.lerpedLoc.x, tracker.lerpedLoc.y, px, py);
    px = tracker.lerpedLoc.x;
    py = tracker.lerpedLoc.y;
  }
}

// Add a new boid into the System
void mousePressed() {
  flock.addBoid(new Boid(tracker.lerpedLoc.x, tracker.lerpedLoc.y));
}
