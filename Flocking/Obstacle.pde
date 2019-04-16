class Obstacle {
   PVector pos;
   float r;
   
   Obstacle (float xx, float yy) {
     pos = new PVector(xx,yy);
     r = 10.0;
   }

// this stuf below probably to be removed   
//   void draw () {
//     fill(0, 0, 0);
//     ellipse(pos.x, pos.y, 15, 15);
//   }


}
