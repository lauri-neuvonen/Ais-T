#include "Interaction.hpp"
#include <iostream>
#include <string.h>
#include <unistd.h>

Interaction::Interaction(uint16_t port, std::string address)
{
  if(!address.empty())
	{
		if (inet_aton(address.c_str(), &addr) == 0)
		{
			std::cout << "Invalid address\n";
 		}
	}
	else
	{
		addr.s_addr = INADDR_ANY;
	}

  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
	}

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr = addr;

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 200000;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv,sizeof(tv)) < 0) {
      perror("Error");
  }
}

std::vector<sf::Vector2f> Interaction::getPoints()
{
  return std::vector<sf::Vector2f> {sf::Vector2f(coord[0], coord[1])};
}

void Interaction::startReceiver()
{
  const char sub[] =  "SUBSCRIBE";
  int n, len;
  n = sendto(sockfd, (const char *)sub, strlen(sub),
    0, (const struct sockaddr *) &servaddr,
      sizeof(servaddr));

      if(n > 0)
    	{

    		n = recvfrom(sockfd, (char *)&ID, sizeof(uint16_t),
    			0, (struct sockaddr *) &servaddr, (socklen_t *)&len);
    		if(n > 0)
    		{
    			printf("Subscribed succsefully. ID: %d\n", ID);
    		}
    	}

    	if(n < 0)
    	{
    		printf("Server not responding!\n");
    		close(sockfd);
        return;
    	}


      th = std::thread([=]{
        while(1){
          int n, len;
        	n = recvfrom(sockfd, (char *)coord, sizeof(uint16_t)*2,
        				0, (struct sockaddr *) &servaddr, (socklen_t *)&len);
        }
      });
}
