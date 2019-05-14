#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Grid.hpp"
#include "Boid.hpp"
#include "Interaction.hpp"



// GRID
// See list in the beginning of "Grid.cpp"

// BOID INTERACTIONS:
// See list in the beginning of "Boid.cpp"

/* VISUAL SETTINGS */
// BOID VISUALS:


// This is the main program:
int main(int argc, char *argv[])
{
    // resolution width and height in px
  uint16_t res_w = 640;
  uint16_t res_h = 480;

    // how many boids to begin with
  uint16_t n_boids = 50;

    // width and height of grid in nodes(?)
  uint16_t grid_w = 50;
  uint16_t grid_h = 25;

  // address for input device(?)
  std::string addr = "192.168.1.105";
  //std::string addr = "127.0.0.1";

  srand (time(NULL));

    // This creates the link to the input devide
  Interaction inter = Interaction(1899, addr);
  inter.startReceiver();

    // Creates a new window for display
  sf::RenderWindow window(sf::VideoMode(res_w, res_h), "Ais-T");
  window.setFramerateLimit(30.0);

  sf::RenderTexture renderTexture;
  if (!renderTexture.create(res_w, res_h))
  {
    std::cout << "Render texture creation failed\n";
  }

  sf::Texture b_texture;
  b_texture.loadFromFile("boid.png");
  //sf::Texture *b_texture = boidTexture();

  // creates a Sprite which can be used to draw the boids
  sf::Sprite bsh = sf::Sprite(b_texture);
  bsh.setScale(0.3, 0.3);
  sf::Vector2u s = b_texture.getSize();
  bsh.setOrigin(s.x/2, s.y/2);

  // creates a new grid
  Grid grid = Grid(grid_w+1, grid_h+2, res_w/grid_w, res_h/grid_h);

  // Creates the boids in the beginning of simulation
  std::vector<Boid> boids;
  for (int i = 0; i < n_boids; i++)
  {
      boids.push_back(Boid(bsh, 600, 400)); // bsh sets the boid figure, last 2 params are coordinates
  }


  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      window.close();
    }

    // Grid is inflated on mouse position. Could be based on the interaction definition
    //grid.inflate(sf::Mouse::getPosition(window).x,
    //             sf::Mouse::getPosition(window).y);
    std::vector<sf::Vector2f> pts = inter.getPoints();
    for(auto p: pts)
    {
      grid.inflate(p.x*res_w/640, p.y*res_h/480);
    }

    window.clear();
    renderTexture.clear();
    renderTexture.setSmooth(true);
    grid.draw(renderTexture, boids);


    // Here the boids interact with each other and the grid
    for(auto boid = boids.begin(); boid != boids.end();)
    {
      sf::Vector2f pos = boid->getPosition();
      if(pos.x < 0)
      {
        boid->setPosition(pos + sf::Vector2f(res_w, 0));
      }
      if(pos.y < 0)
      {
        boid->setPosition(pos + sf::Vector2f(0, res_h));
      }
      if(pos.x > res_w)
      {
        boid->setPosition(pos + sf::Vector2f(-res_w, 0));
      }
      if(pos.y > res_h)
      {
        boid->setPosition(pos + sf::Vector2f(0, -res_h));
      }
      sf::Vector2f slope = grid.getSlope(boid->getPosition());
      boid->setSlope(slope); // sets avoidance of obstacles for this boid
      boid->flock(boids); // sets flocking parameters for this boid
      boid->run(); // calculates new position based on flocking parameters for this boid

      boid->draw(renderTexture);
      boid++;
    }

    const sf::Texture& texture = renderTexture.getTexture();
    // draw it to the window
    sf::Sprite sprite(texture);
    sprite.setScale(1, -1);
    sprite.setPosition(0, res_h);
    window.draw(sprite);
    sf::CircleShape cursor(4.f);
    cursor.setPosition(sf::Mouse::getPosition(window).x,
                       sf::Mouse::getPosition(window).y);
    window.draw(cursor);
    window.display();
  }

  return 0;
}
