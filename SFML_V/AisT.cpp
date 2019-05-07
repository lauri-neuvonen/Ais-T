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

// How the boids look like:
sf::Texture* boidTexture() // This creates a set of three circles and combines them for the boid
{
  sf::RenderTexture rtexture;
  rtexture.create(60, 30); // sets dimensions of the boid "picture box"
    // 1st circle
  sf::CircleShape circ(10); // size of first circle, used as a reference for others
  circ.setFillColor(sf::Color::Transparent); // boid circle fill
  circ.setOutlineThickness(3); // thickness of boid outline
  circ.setOutlineColor(sf::Color(255, 150, 120));
  circ.setPosition(2, 2);
  rtexture.draw(circ);
    // 2nd circle
  circ.setScale(0.8, 0.8); // scale of middle circle
  circ.setPosition(22, 3);
  circ.setOutlineThickness(4);
  rtexture.draw(circ);
    // 3rd circle
  circ.setScale(0.5, 0.5); // scale of last circle
  circ.setPosition(38, 5);
  circ.setOutlineThickness(5);
  rtexture.draw(circ);
    // drawing:
  rtexture.setSmooth(true);
  rtexture.display();

  return new sf::Texture(rtexture.getTexture());
}

// This is the main program:
int main(int argc, char *argv[])
{
    // resolution width and height in px
  uint16_t res_w = 1200;
  uint16_t res_h = 800;

    // how many boids to begin with
  uint16_t n_boids = 50;

    // width and height of grid in nodes(?)
  uint16_t grid_w = 60;
  uint16_t grid_h = 40;

  // address for input device(?)
  //std::string addr = "192.168.1.106";
  std::string addr = "127.0.0.1";

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

  sf::Texture *b_texture = boidTexture();

  // creates a Sprite which can be used to draw the boids
  sf::Sprite bsh = sf::Sprite(*b_texture);

  // creates a new grid
  Grid grid = Grid(grid_w+1, grid_h+1, res_w/grid_w, res_h/grid_h);

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
    grid.inflate(sf::Mouse::getPosition(window).x,
                 sf::Mouse::getPosition(window).y);
    //std::vector<sf::Vector2f> pts = inter.getPoints();
    //grid.inflate(pts[0].x, pts[0].y);


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
      else if(pos.y < 0)
      {
        boid->setPosition(pos + sf::Vector2f(0, res_h));
      }
      else if(pos.x > res_w)
      {
        boid->setPosition(pos + sf::Vector2f(-res_w, 0));
      }
      else if(pos.y > res_h)
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
