// The Flock (a list of Boid objects)

class Flock {
  ArrayList<Boid> boids; // An ArrayList for all the boids
  ArrayList<Obstacle> obstacles;
  Obstacle liveObs;
  

  Flock() {
    boids = new ArrayList<Boid>(); // Initialize the ArrayList
    obstacles = new ArrayList<Obstacle>();
    liveObs = new Obstacle(mouseX, mouseY);
    }

  void run() {
    for (Boid b : boids) {
      b.run(boids, obstacles);  // Passing the entire list of boids to each boid individually
    }
  }

  void addBoid(Boid b) {
    boids.add(b);
  }
  void addObs(Obstacle o) {
    obstacles.add(o);
    liveObs = o;
  }
  
  void renderObs() {
    for (Obstacle o : obstacles) {
      stroke(204, 102, 0);
      fill(203, 102, 0);
      ellipse(o.pos.x, o.pos.y, o.r, o.r);
    }
  }

}
