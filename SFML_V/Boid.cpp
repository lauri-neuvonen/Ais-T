#include "Boid.hpp"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>    // std::min

Boid::Boid(sf::Sprite bs, float x, float y)
{
  acceleration = sf::Vector2f(0, 0);
  slope = sf::Vector2f(0, 0);
  flockF = sf::Vector2f(0, 0);


  //diffVector = sf::Vector2f(0, 0);
  //distance = 0.0f; // used to keep track of distance to other boids

  /* SETTINGS for INTERACTION BETWEEN BOIDS */
    // OK to modify these

    // Interaction ranges:
    range = 1000.0f; // range of perception for boids (ignores boids outside of this) REDUNDANT!
    sepRange = 100.0f; // desired separation: no interaction beyond this
    cohRange = 100.0f; // boids within this range are used for cohesion steering
    aliRange = 200.0f; // boids within this range are used for alignment steering

    // Force & speed limits:
    maxspeed = 3;
    maxforce = 0.05;

    // Weights for flocking behavior - These are normalized, so absolute values do not matter - only relations
    w_separate = 8.0f;
    w_align = 3.0f;
    w_cohesion = 1.0f;

    // Initial velocity for new boids:
    float init_vel = 1.5 + (rand() % 30)/60.0;

    /* Other setup - DO NOT CHANGE (unless sure of what you're doing...) */
    angle = M_PI*(rand() % 360)/180.0;

    velocity = sf::Vector2f(cos(angle), sin(angle));
    velocity *= init_vel;

    position = sf::Vector2f(x, y);
    r = 2.0;

    float sumW = w_separate+w_align+w_cohesion;
    w_separate /=sumW;
    w_align /=sumW;
    w_cohesion /=sumW;

    boid = bs;
}

/* BOID FUNCTIONS */
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
  bs.setRotation(90+angle*180.0/M_PI);
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
    float sepCount = 0;
    float aliCount = 0;
    float cohCount = 0;

    // first reset all effects
    force = sf::Vector2f(0, 0);
    sf::Vector2f sep = sf::Vector2f(0, 0);
    sf::Vector2f ali = sf::Vector2f(0, 0);
    sf::Vector2f coh = sf::Vector2f(0, 0);
    for(auto other = boids.begin(); other != boids.end();)
    {

        sf::Vector2f otherPos = other->getPosition();
        sf::Vector2f diffVector = position - otherPos; // difference between the position vecs
        float dx = diffVector.x;
        float dy = diffVector.y;
        float distance = sqrt(dx*dx + dy*dy); // sets distance for this boid pair
        minDist = std::min(distance, minDist);

        if(distance > 0 && distance <= range)
        {
            //then add effect for each other boid
            if(distance <= sepRange){
                sep += diffVector*(sepRange-distance)/((distance*distance)); // tries to get away from closest Boid
                sepCount++;
            }
            if(distance <= aliRange){
                ali += other->getVelocity()-velocity;
                aliCount++;
            }
            if(distance <= cohRange){
               coh += -diffVector;
                cohCount++;
            }

            count++;
        }

        other++;

    }

    if(count>0)
        // This averages the effect from different boids. It doesn't really make sense for separation
        // One option would be to just pick the min of individual separations and use that
    {
        // velocity steering
        sep *= w_separate;
        if(aliCount >0){
            ali *= w_align/aliCount; // alignment taken on average
        }
        if(cohCount >0){
            coh *= w_cohesion/cohCount; // cohesion taken on average position
        }
    }

    force = sep+ali+coh; // adds steers together & substracts current velocity to get wanted change

    /* Following scales the resultant force to max force */
    float fnorm = sqrt(force.x*force.x + force.y*force.y);
    float fRatio = fnorm/maxforce;
    fRatio = std::min(fRatio, 1.0f);
    if(fRatio != 0.0)
      force = force/fnorm; // normalize
    force = force*fRatio*maxforce; //scale to ratio
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
