#include "Grid.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>


Grid::Grid(int w, int h, float dw, float dh)
{
  height = h;
  width = w;
  this->dh = dh;
  this->dw = dw;

  inflation_range = 50.0;

  light = sf::Vector2f(2.0, 0.0);

  points = (sf::Vector2f *)malloc(sizeof(sf::Vector2f)*height*width);

  for(int w = 0; w < width; w++)
  {
    for(int h = 0; h < height; h++)
    {
      points[w*height+h] = start_pos(w, h);
    }
  }
}

void Grid::inflate(float x, float y)
{
   /*light.x = (cos(0.1)*light.x + sin(0.1)*light.y);
   light.y = (-sin(0.1)*light.x + cos(0.1)*light.y);
   float n = sqrt(light.x*light.x + light.y*light.y);
   light.x /= n;
   light.y /= n; */
  for(int w = 0; w < width; w++)
  {
    for(int h = 0; h < height; h++)
    {
      int i = w*height+h;
      float dx = points[i].x - x;
      float dy = points[i].y - y;
      //float d = pow(sqrt(dx*dx + dy*dy), 0.05)*150.0;

      float d = sqrt(dx*dx + dy*dy);

      //if(d > 0.0001 && d < 250.0)
      if(d > 0.01 && d < 3.14*inflation_range)
      {
        float k = (cos(d/inflation_range)+1.0)/5.0;
        sf::Vector2f dp = sf::Vector2f(dx/d*k, dy/d*k);
        points[i] += dp;
      }
      sf::Vector2f sp = start_pos(w, h);
      sf::Vector2f dpo = sf::Vector2f((sp.x-points[i].x)/1000, (sp.y-points[i].y)/1000);
      points[i] += dpo;
    }
  }
}

void Grid::draw(sf::RenderTexture &renderTexture, std::vector<Boid> lights)
{
  // draw the triangles
  for(int w = 0; w < width-1; w++)
  {
    sf::VertexArray triangles(sf::TriangleStrip, height*2);
    for(int h = 0; h < height; h++)
    {
      int H = (int)(h);
      int W1 = w+(w%2);
      int W2 = 1+w-(w%2);
      int i1 = H+W1*height;
      int i2 = H+W2*height;

      triangles[2*h].position.x = points[i1].x;
      triangles[2*h].position.y = points[i1].y;
      triangles[2*h+1].position.x = points[i2].x;
      triangles[2*h+1].position.y = points[i2].y;

      sf::Vector2f dv1 = start_pos(W1, H) - points[i1];
      sf::Vector2f dv2 = start_pos(W2, H) - points[i2];
      float colors1 = (dv1.x*light.x + dv1.y*light.y)/200.0;
      float colors2 = (dv2.x*light.x + dv2.y*light.y)/200.0;

      float color1 = 0.0;
      float color2 = 0.0;

      /*for(auto light2 = lights.begin(); light2 != lights.end(); light2++)
      {
        sf::Vector2f l1 = points[i1]-light2->getPosition();
        sf::Vector2f l2 = points[i2]-light2->getPosition();
        float ml1 = l1.x*l1.x + l1.y*l1.y;
        float ml2 = l2.x*l2.x + l2.y*l2.y;
        float c1 = (dv1.x*l1.x + dv1.y*l1.y)/ml1;
        float c2 = (dv2.x*l2.x + dv2.y*l2.y)/ml2;

        c1 = c1 > 0.0 ? c1 : 0.0;
        c2 = c2 > 0.0 ? c2 : 0.0;
        color1 += c1/15.0;// > 0.0 ? c1 : 0.0;
        color2 += c2/15.0;// > 0.0 ? c2 : 0.0;

      }*/
      color1 += colors1;
      color2 += colors2;
      // 55 20 64
      // 237 209 33

      color1 = color1 < 0 ? 0 : color1;
      color1 = color1 > 1.0 ? 1.0 : color1;
      color2 = color2 < 0 ? 0 : color2;
      color2 = color2 > 1.0 ? 1.0 : color2;

      //237
      triangles[2*h].color = sf::Color(55+(251-55)*color1, 20+(227-20)*color1, 64+(145-64)*color1);
      triangles[2*h+1].color = sf::Color(55+(251-55)*color2, 20+(227-20)*color2, 64+(145-64)*color2);

    }
    renderTexture.draw(triangles);
  }

  // draw the lines
  sf::VertexArray lines(sf::LineStrip, height);
  for(int w = 0; w < width-1; w++)
  {
    for(int h = 0; h < height; h++)
    {
      int i = h+w*height;
      lines[h].position.x = points[i].x;
      lines[h].position.y = points[i].y;

      sf::Vector2f dv = start_pos(w, h) - points[i];
      float color = 0;//dv.x*light.x + dv.y*light.y;
      //141
      lines[h].color = sf::Color(111+color, 111+color, 111+color);
    }
    renderTexture.draw(lines);
  }
  for(int w = 0; w < width-1; w++)
  {
    sf::VertexArray lines(sf::LineStrip, height*2);
    int w_toggle = w%2;

    for(int h = 0; h < height*2; h++)
    {
      int hodd = h%2;
      int H = (h-hodd)/2;
      int W = w + w_toggle;
      w_toggle = 1-w_toggle;

      int i = H+W*height;

      lines[h].position.x = points[i].x;
      lines[h].position.y = points[i].y;
      sf::Vector2f dv = start_pos(W, H) - points[i];
      float color = 0;//dv.x*light.x + dv.y*light.y;
      lines[h].color = sf::Color(111+color, 111+color, 111+color);
      //lines[h].color = sf::Color(75, 55, 45);
    }
    renderTexture.draw(lines);
  }
}

sf::Vector2f Grid::start_pos(int w, int h)
{
  return sf::Vector2f(w*dw, (h-1)*dh+dh/2*(w%2));
}

sf::Vector2f Grid::getSlope(sf::Vector2f pos)
{
  if(pos.x <= 0 || pos.x >= width*dw || pos.y <= 0 || pos.y >= width*dh)
  {
       return sf::Vector2f(0, 0);
  }
  int W = (int)round(pos.x/dw);
  int H = (int)round(pos.y/dh);
  int i = H+W*height;
  sf::Vector2f sp = start_pos(W, H);
  sf::Vector2f diff = points[i] - start_pos(W, H);

  float d = std::max<float>(sqrt(diff.x*diff.x + diff.y*diff.y)/1000.0, 500.0);

  return sf::Vector2f(diff.x/d, diff.y/d);
}
