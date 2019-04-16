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


//import org.openkinect.freenect.*;
//import org.openkinect.processing.*; // imports the OpenKinect library used in image processing
//int localw = 10;
//int localh = 10;

int dh = 10;
int dw = 10;

ArrayList<PVector> grid_points;
ArrayList<PVector> grid_points_orig;

  int localw = 25;
  int localh = 25;
  
float px;
float py;

Flock flock;
//Kinect kinect;
//KinectTracker tracker;

void setup() {
  
  //kinect = new Kinect(this);
  //kinect.initDepth();
  //kinect.initVideo();
  //kinect.enableIR(ir);
  //kinect.enableColorDepth(colorDepth);
  //deg = kinect.getTilt();
    size(640, 360);
    textSize(16);
  
  grid_points = new ArrayList<PVector>();
  grid_points_orig = new ArrayList<PVector>();
  {
    for(int w = 0; w < localw; w++)
    {
      for(int h = 0; h <localh; h++ /* h=h+1*/)
      {   
         grid_points.add(new PVector(w*dw, h*dh+dh/2*(w%2))); //modulo 2: even numbers =1 odd numbers=0
         grid_points_orig.add(new PVector(w*dw, h*dh+dh/2*(w%2)));
      }
    }
  }

  flock = new Flock();
  //tracker = new KinectTracker();
  // Add an initial set of boids into the system, no obstacles initially
  for (int i = 0; i < 150; i++) {
    flock.addBoid(new Boid(width/2, height/2));
  }
}

void draw() {
  background(50);
  //image(kinect.getVideoImage(), 0, 0);
  //image(kinect.getDepthImage(), 640, 0);
  //tracker.display();
  //tracker.track();
  grid();
  flock.liveObs.pos = new PVector(mouseX, mouseY);
  flock.renderObs();
  flock.run();
  stroke(255);
}
// Add a new boid into the System
void mousePressed() {
  flock.addObs(new Obstacle(mouseX, mouseY));
}
