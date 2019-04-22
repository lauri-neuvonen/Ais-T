#ifndef BOID_HPP
#define BOID_HPP

#include <SFML/Graphics.hpp>

class Boid
{
private:
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Vector2f acceleration;
  sf::Vector2f slope;
  float angle;
  float r;
  float maxforce;
  float maxspeed;
  sf::Sprite boid;
  BoidShape boidsh;

public:
  Boid(BoidShape bsh, float x, float y);
  Boid(sf::Sprite bs, float x, float y);
  void run();
  void draw(sf::RenderTexture &renderTexture);
  sf::Vector2f getPosition();
  void setSlope(sf::Vector2f s);
};

#endif //BOID_HPP
