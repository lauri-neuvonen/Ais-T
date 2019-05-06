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

  light = sf::Vector2f(3.0, 0.0);

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
  // light.x = (cos(0.1)*light.x + sin(0.1)*light.y);
  // light.y = (-sin(0.1)*light.x + cos(0.1)*light.y);
  // float n = sqrt(light.x*light.x + light.y*light.y);
  // light.x /= n;
  // light.y /= n;
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
      //float color1 = dv1.x*light.x + dv1.y*light.y;
      //float color2 = dv2.x*light.x + dv2.y*light.y;

      float color1 = 0.0;
      float color2 = 0.0;

      for(auto light = lights.begin(); light != lights.end(); light++)
      {
        sf::Vector2f l = light->getPosition();
        float c1 = dv1.x/l.x + dv1.y/l.y;
        float c2 = dv2.x/l.x + dv2.y/l.y;

        color1 += c1 > 0.0 ? c1 : 0.0;
        color2 += c2 > 0.0 ? c2 : 0.0;

      }

      triangles[2*h].color = sf::Color(150+color1, 100+color1, 110+color1);
      triangles[2*h+1].color = sf::Color(150+color2, 100+color2, 110+color2);

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

      //sf::Vector2f dv = start_pos(w, h) - points[i];
      //float color = dv.x*light.x + dv.y*light.y;
      lines[h].color = sf::Color(75, 55, 45);
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
      //sf::Vector2f dv = start_pos(W, H) - points[i];
      //float color = dv.x*light.x + dv.y*light.y;
      lines[h].color = sf::Color(75, 55, 45);
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
//  std::cout << "slope: " << W << " " << H << " " << i << " " << pos.x << " " << pos.y << "\n";
//  std::cout << points[i].x << " " << points[i].y << "\n";
  sf::Vector2f diff = points[i] - start_pos(W, H);

  float d = std::max<float>(sqrt(diff.x*diff.x + diff.y*diff.y)/4000.0, 500.0);

  return sf::Vector2f(diff.x/d, diff.y/d);
}
