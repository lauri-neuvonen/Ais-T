#include "Grid.hpp"
#include <math.h>
#include <algorithm>

Grid::Grid(int w, int h, float dw, float dh)
{
  height = h;
  width = w;
  this->dh = dh;
  this->dw = dw;

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
  for(int w = 0; w < width; w++)
  {
    for(int h = 0; h < height; h++)
    {
      int i = w*height+h;
      float dx = points[i].x - x;
      float dy = points[i].y - y;
      //float d = pow(sqrt(dx*dx + dy*dy), 0.05)*150.0;
      float range = 50.0;
      float d = sqrt(dx*dx + dy*dy);
      float k = (cos(d/range)+1.0)/5.0;

      //if(d > 0.0001 && d < 250.0)
      if(d > 0.01 && d < 3.14*range)
      {
        sf::Vector2f dp = sf::Vector2f(dx/d*k, dy/d*k);
        points[i] += dp;
      }
      sf::Vector2f sp = start_pos(w, h);
      sf::Vector2f dpo = sf::Vector2f((sp.x-points[i].x)/1000, (sp.y-points[i].y)/1000);
      points[i] += dpo;
    }
  }
}

void Grid::draw(sf::RenderTexture &renderTexture)
{
  for(int w = 0; w < width; w++)
  {
    sf::VertexArray lines(sf::LineStrip, height);
    for(int h = 0; h < height; h++)
    {
      int i = h+w*height;
      lines[h].position.x = points[i].x;
      lines[h].position.y = points[i].y;
      sf::Vector2f dv = start_pos(w, h) - points[i];
      float dd = (dv.x*dv.x + dv.y*dv.y)*20;
      lines[h].color = sf::Color(205-(int)dd, 205-(int)dd/50, 205-(int)dd);
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
      float dd = (dv.x*dv.x + dv.y*dv.y)*20;
      lines[h].color = sf::Color(155-(int)dd, 155-(int)dd, 155-(int)dd/50);
    }
    renderTexture.draw(lines);
  }
}

sf::Vector2f Grid::start_pos(int w, int h)
{
  return sf::Vector2f(w*dw, h*dh+dh/2*(w%2));
}

sf::Vector2f Grid::getSlope(sf::Vector2f pos)
{
  if(pos.x < 0 || pos.x > width*dw || pos.y < 0 || pos.y > width*dh)
  {
       return sf::Vector2f(0, 0);
  }
  int W = (int)round(pos.x/dw);
  int H = (int)round(pos.y/dh);
  int i = H+W*height;
  sf::Vector2f sp = start_pos(W, H);
  sf::Vector2f diff = points[i] - start_pos(W, H);

  float d = std::max<float>(sqrt(diff.x*diff.x + diff.y*diff.y)/40000.0, 500.0);

  return sf::Vector2f(diff.x/d, diff.y/d);
}
