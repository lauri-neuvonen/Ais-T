#include "Boid.hpp"
#include <stdlib.h>
#include <math.h>

Boid::Boid(sf::Sprite bs, float x, float y)
{
  acceleration = sf::Vector2f(0, 0);
  slope = sf::Vector2f(0, 0);
  flockF = sf::Vector2f(0, 0);
    

  diffVector = sf::Vector2f(0, 0);
  float distance = 0.0f; // used to keep track of distance to other boids
  float range = 25.0f; // range of perception for boids (ignores boids outside of this)

  angle = M_PI*(rand() % 360)/180.0;
  float init_vel = 1.5 + (rand() % 30)/60.0;
  velocity = sf::Vector2f(cos(angle), sin(angle));
  velocity *= init_vel;

  position = sf::Vector2f(x, y);
  r = 2.0;
  maxspeed = 2;
  maxforce = 0.03;
    
  // Weights for flocking behavior
  w_separate = 3.0f;
  w_align = 1.0f;
  w_cohesion = 1.0f;

  boid = bs;
}

void Boid::run()
{
  acceleration = slope + flockF;
  velocity += acceleration;
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
    
    for(auto other = boids.begin(); other != boids.end();)
    {

        // first reset all effects
        sf::Vector2f sep = sf::Vector2f(0, 0);
        sf::Vector2f ali = sf::Vector2f(0, 0);
        sf::Vector2f coh = sf::Vector2f(0, 0);
        sf::Vector2f otherPos = other->getPosition();
        diffVector = position - otherPos; // difference between the position vecs
        float dx = diffVector.x;
        float dy = diffVector.y;
        distance = sqrt(dx*dx + dy*dy); // sets distance for this boid pair
        
        if(distance > 0 && distance <= range)
        {
            //then add effect for each other boid
            sep += separate(*other) * w_separate;
            ali += align(*other) * w_align;
            coh += cohesion(*other) * w_cohesion;
            
            count++;
        }
        other++;
        
    }
    if(count>0)
        // This averages the effect from different boids. It doesn't really make sense for separation
        // One option would be to just pick the max of individual separations and use that
    {
        sep = sep/count;
        ali = ali/count;
        coh = coh/count;
    }
    
    force = sep+ali+coh;
    force = force/sqrt(force.x*force.x + force.y*force.y); // normalize
    force = force*maxforce; //scale to max force
    flockF = force;

}
                                
sf::Vector2f Boid::separate(Boid other)
{
    sf::Vector2f separate = sf::Vector2f(0, 0);

    separate = diffVector/(distance*distance);
    
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
