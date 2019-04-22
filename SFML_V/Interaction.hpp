#ifndef INTERACTION_HPP
#define INTERACTION_HPP

#define BUFFER_SIZE 128

#include <thread>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <SFML/Graphics.hpp>

class Interaction
{
public:
  Interaction(uint16_t port, std::string address);
  std::vector<sf::Vector2f> getPoints();
  void startReceiver();

private:
  int sockfd;
  uint8_t port;
  struct sockaddr_in servaddr;
  struct in_addr addr;
  uint8_t buffer[BUFFER_SIZE];
  std::thread th;
  uint16_t ID;
  uint16_t coord[2];
};

#endif //INTERACTION_HPP
