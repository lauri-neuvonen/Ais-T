#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>

class Grid
{
public:
  Grid(int w, int h, float dw, float dh);
  void inflate(float x, float y);
  void draw(sf::RenderTexture &renderTexture);
  sf::Vector2f getSlope(sf::Vector2f pos);

private:
  int height;
  int width;
  float dh;
  float dw;
  sf::Vector2f *points;

  sf::Vector2f start_pos(int w, int h);
};

#endif //GRID_HPP
