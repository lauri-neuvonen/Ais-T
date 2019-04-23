#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Grid.hpp"
#include "Boid.hpp"
#include "Interaction.hpp"

sf::Texture* boidTexture()
{
  sf::RenderTexture rtexture;
  rtexture.create(60, 30);
  sf::CircleShape circ(10);
  circ.setFillColor(sf::Color::Transparent);
  circ.setOutlineThickness(3);
  circ.setOutlineColor(sf::Color(255, 150, 120));
  circ.setPosition(2, 2);
  rtexture.draw(circ);
  circ.setScale(0.8, 0.8);
  circ.setPosition(22, 3);
  circ.setOutlineThickness(4);
  rtexture.draw(circ);
  circ.setScale(0.5, 0.5);
  circ.setPosition(38, 5);
  circ.setOutlineThickness(5);
  rtexture.draw(circ);
  rtexture.setSmooth(true);
  rtexture.display();

  return new sf::Texture(rtexture.getTexture());
}

int main(int argc, char *argv[])
{
  uint16_t res_w = 1200;
  uint16_t res_h = 800;

  uint16_t n_boids = 30;

  uint16_t grid_w = 60;
  uint16_t grid_h = 30;


  //std::string addr = "192.168.1.106";
  std::string addr = "127.0.0.1";

  srand (time(NULL));

  Interaction inter = Interaction(1899, addr);
  inter.startReceiver();

  sf::RenderWindow window(sf::VideoMode(res_w, res_h), "Ais-T");
  window.setFramerateLimit(30.0);

  sf::RenderTexture renderTexture;
  if (!renderTexture.create(res_w, res_h))
  {
    std::cout << "Render texture creation failed\n";
  }

  sf::Texture *b_texture = boidTexture();
  sf::Sprite bsh = sf::Sprite(*b_texture);

  Grid grid = Grid(grid_w, grid_h, res_w/grid_w, res_h/grid_h);

  std::vector<Boid> boids;
  for (int i = 0; i < n_boids; i++)
  {
    boids.push_back(Boid(bsh, 600, 400));
  }

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      window.close();
    }

    grid.inflate(sf::Mouse::getPosition(window).x,
                 sf::Mouse::getPosition(window).y);
    //std::vector<sf::Vector2f> pts = inter.getPoints();
    //grid.inflate(pts[0].x, pts[0].y);


    window.clear();
    renderTexture.clear();
    renderTexture.setSmooth(true);
    grid.draw(renderTexture);

    for(auto boid = boids.begin(); boid != boids.end();)
    {
      sf::Vector2f pos = boid->getPosition();
      if(pos.x < 0 || pos.y < 0 || pos.x > res_w || pos.y > res_h)
      {
        boid = boids.erase(boid);
        boids.push_back(Boid(bsh, res_w/2, res_h/2));
      }
      sf::Vector2f slope = grid.getSlope(pos);
      boid->setSlope(slope);
      boid->run();
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
