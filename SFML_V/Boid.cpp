#include "Boid.hpp"
#include <stdlib.h>
#include <math.h>

Boid::Boid(sf::Sprite bs, float x, float y)
{
  acceleration = sf::Vector2f(0, 0);
  slope = sf::Vector2f(0, 0);

  angle = M_PI*(rand() % 360)/180.0;
  float init_vel = 1.5 + (rand() % 30)/60.0;
  velocity = sf::Vector2f(cos(angle), sin(angle));
  velocity *= init_vel;

  position = sf::Vector2f(x, y);
  r = 2.0;
  maxspeed = 2;
  maxforce = 0.03;

  boid = bs;
}

void Boid::run()
{
  velocity += slope;
  position += velocity;
  angle = atan2(velocity.y, velocity.x);

}

void Boid::draw(sf::RenderTexture &renderTexture)
{
  sf::Sprite bs = boid;
  bs.setPosition(position);
  bs.setRotation(angle*180.0/M_PI);
  renderTexture.draw(bs);
}

sf::Vector2f Boid::getPosition()
{
  return position;
}

void Boid::setSlope(sf::Vector2f s)
{
  slope = s;
}
