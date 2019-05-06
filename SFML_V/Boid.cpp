#include "Boid.hpp"
#include <stdlib.h>
#include <math.h>
#include <iostream>

Boid::Boid(sf::Sprite bs, float x, float y)
{
  acceleration = sf::Vector2f(0, 0);
  slope = sf::Vector2f(0, 0);
  flockF = sf::Vector2f(0, 0);


  //diffVector = sf::Vector2f(0, 0);
  //distance = 0.0f; // used to keep track of distance to other boids
  range = 150.0f; // range of perception for boids (ignores boids outside of this)

  angle = M_PI*(rand() % 360)/180.0;
  float init_vel = 1.5 + (rand() % 30)/60.0;
  velocity = sf::Vector2f(cos(angle), sin(angle));
  velocity *= init_vel;

  position = sf::Vector2f(x, y);
  r = 2.0;
  maxspeed = 2;
  maxforce = 0.03;

  // Weights for flocking behavior
  w_separate = 900.0f;
  w_align = 3.0f;
  w_cohesion = 1.0f;

  boid = bs;
}

void Boid::run()
{
  acceleration = slope + flockF;
  velocity += acceleration;
  if(abs(velocity.x) > maxspeed)
    velocity.x *= maxspeed/abs(velocity.x);

    if(abs(velocity.y) > maxspeed)
      velocity.y *= maxspeed/abs(velocity.y);

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

void Boid::setPosition(sf::Vector2f pos)
{
  position = pos;
}

sf::Vector2f Boid::getVelocity()
{
    return velocity;
}

void Boid::setSlope(sf::Vector2f s)
{
  slope = s;
}

void Boid::flock(std::vector<Boid> boids)
{
    /*
    Here we loop through the other boids and for each one
    calculate separation, alignment and cohesion effects using
    purpose built functions.
     */
    float count = 0;
    sf::Vector2f force = sf::Vector2f(0, 0);
    sf::Vector2f sep = sf::Vector2f(0, 0);
    sf::Vector2f ali = sf::Vector2f(0, 0);
    sf::Vector2f coh = sf::Vector2f(0, 0);

    for(auto other = boids.begin(); other != boids.end();)
    {
        // first reset all effects
        sf::Vector2f otherPos = other->getPosition();
        sf::Vector2f diffVector = position - otherPos; // difference between the position vecs
        float dx = diffVector.x;
        float dy = diffVector.y;
        float distance = sqrt(dx*dx + dy*dy); // sets distance for this boid pair

        if(distance > 0 && distance <= range)
        {
            //then add effect for each other boid
            sep += diffVector/(distance*distance) * w_separate;
            ali += other->getVelocity() * w_align;
            coh += -diffVector * w_cohesion;

            count++;
        }
        
        other++;

    }

    if(count>0)
        // This averages the effect from different boids. It doesn't really make sense for separation
        // One option would be to just pick the max of individual separations and use that
    {
        sep /= count;
        ali /= count;
        coh /= count;
    }

    force = sep+ali+coh;
    float fnorm = sqrt(force.x*force.x + force.y*force.y);
    if(fnorm != 0.0)
      force = force/fnorm; // normalize
    force = force*maxforce; //scale to max force
    flockF = force;

}

sf::Vector2f Boid::separate(Boid other)
{
    sf::Vector2f separate = sf::Vector2f(0, 0);

    //separate = diffVector/(distance*distance);

    return separate;
}

sf::Vector2f Boid::align(Boid other)
{
    sf::Vector2f veloSum = sf::Vector2f(0, 0);

    veloSum += other.getVelocity();

    return veloSum;
}

sf::Vector2f Boid::cohesion(Boid other)
{
    sf::Vector2f coh = sf::Vector2f(0, 0);

    coh += other.getPosition();

    return coh;
}
