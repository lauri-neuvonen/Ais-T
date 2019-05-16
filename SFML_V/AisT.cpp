#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Grid.hpp"
#include "Boid.hpp"
#include "Interaction.hpp"

#include "boost/program_options.hpp"


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
  uint32_t res_w = 1024;
  uint32_t res_h = 768;

    // how many boids to begin with
  uint32_t n_boids = 50;

    // width and height of grid in nodes(?)
  uint32_t grid_w = 50;
  uint32_t grid_h = 25;

  int inflate_markers = 0;
  
  std::string addr = "127.0.0.1";

  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
      ("help,h", "Print help message")
      ("nboids,n", po::value<uint32_t>(&n_boids)->default_value(50), "Number of boids")
      ("res,r", po::value<std::string>(), "Window resolution WxH")
      ("grid,g", po::value<std::string>(), "Grid resolution WxH")
      ("address,a", po::value<std::string>(), "IP address of AisTCam ####.####.####.####")
      ("inflate,i", "Show inflation markers");

  po::variables_map vm;

  try
  {
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if(vm.count("help"))
    {
      std::cout << desc << std::endl;
      return 0;
    }

    if(vm.count("nboids"))
    {
      n_boids = vm["nboids"].as<uint32_t>();
    }
    if(vm.count("res"))
    {
      char *bp;
      char buffer[20];
      strcpy(buffer, vm["res"].as<std::string>().c_str());
      bp = strchr(buffer, 'x')+1;
      std::sscanf(buffer, "%d", &res_w);
      std::sscanf(bp, "%d", &res_h);
    }
    if(vm.count("grid"))
    {
      char *bp;
      char buffer[20];
      strcpy(buffer, vm["grid"].as<std::string>().c_str());
      bp = strchr(buffer, 'x')+1;
      std::sscanf(buffer, "%d", &grid_w);
      std::sscanf(bp, "%d", &grid_h);
    }
    if(vm.count("address"))
    {
      addr = vm["address"].as<std::string>();
    }

    if(vm.count("inflate"))
    {
      inflate_markers = 1;
    }

  }
  catch(std::exception& e)
  {
    std::cerr << "Error passing arguments!" << std::endl;
  }

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
      boids.push_back(Boid(bsh, res_w/2, res_h/2)); // bsh sets the boid figure, last 2 params are coordinates
  }


  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      window.close();
    }

    window.clear();
    renderTexture.clear();
    std::vector<sf::Vector2f> pts = inter.getPoints();
    for(auto p: pts)
    {
      grid.inflate(p.x*res_w/640, p.y*res_h/480);
    }

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
   
    if(inflate_markers)
    { 
      for(auto p: pts)
      {
        sf::CircleShape cursor(4.f);
        cursor.setPosition(p.x*res_w/640,
                         p.y*res_h/480);
        window.draw(cursor);
      }
    }
    window.display();
  }

  return 0;
}
