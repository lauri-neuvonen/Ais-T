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
  sf::Vector2f flockF;
  sf::Vector2f diffVector;
  float distance;
  float angle;
  float r;
  float maxforce;
  float maxspeed;
  float range;
  sf::Sprite boid;
    float w_separate;
    float w_align;
    float w_cohesion;
    sf::Vector2f sep;
    sf::Vector2f ali;
    sf::Vector2f coh;
  
public:
  Boid(sf::Sprite bs, float x, float y);
  void run();
  void draw(sf::RenderTexture &renderTexture);
  sf::Vector2f getPosition();
  sf::Vector2f getVelocity();
  void setSlope(sf::Vector2f s);
  void flock(std::vector<Boid> boids);
  sf::Vector2f separate(Boid other);
  sf::Vector2f align(Boid other);
  sf::Vector2f cohesion(Boid other);
};

#endif //BOID_HPP
