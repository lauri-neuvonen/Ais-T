void grid () {
  //noStroke();
  colorMode(HSB);
  fill(50,200);
  rect(0,0,1000,1000);
  int localw = 10;
  int localh = 10;
  
  fill(150,200);
   for(int h = 0; h <localh; h++ /* h=h+1*/)
  {
    for(int w = 0; w < localw; w++)
    {
      PVector p= grid_points.get(w*localh+h);
      PVector po= grid_points_orig.get(w*localh+h);
      
      float dx = p.x-mouseX;
      float dy = p.y-mouseY;
      float d = (dx*dx+dy*dy);
   
      PVector dp = new PVector(dx/d*30,dy/d*30);
      
      PVector dpo = new PVector((po.x-p.x)/150, (po.y-p.y)/150);
      
      p.add(dp); 
      p.add(dpo);
    }
  }
  
  
   for(int h = 0; h <localh; h++ /* h=h+1*/)
  {
    for(int w = 0; w < localw; w++)
    {
      PVector p = grid_points.get(w*localh+h);
      ellipse(p.x, p.y, 5, 5);
      int lx, ly;
      
      ly = h+1;
      lx = w;
      if(ly < localh)
      {
        PVector p2 = grid_points.get(lx*localh+ly);
        line(p.x, p.y, p2.x, p2.y);
      }
       
      ly = h-1 + w%2;
      lx = w+1;
      if(ly > -1 && lx < localw) // && logic and
      {
        PVector p2 = grid_points.get(lx*localh+ly);
        line(p.x, p.y, p2.x, p2.y);
      }
      
      ly = h + w%2;
      lx = w+1;
      if(ly < localh && lx < localw)
      {
        PVector p2 = grid_points.get(lx*localh+ly);
        line(p.x, p.y, p2.x, p2.y);
      }
    }
  }
}
