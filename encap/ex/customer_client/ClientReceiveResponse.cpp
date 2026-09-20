#include "Client.hpp"
#include <netinet/in.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

bool	Client::_receiveResponse()
{
	std::string	response;
	char		buffer[1024];
	ssize_t		bytesRead;

	while (response.find("<END_RESPONSE>") == std::string::npos)
	{
		bytesRead = recv(_socketFd, buffer, sizeof(buffer), 0);
		if (bytesRead <= 0)
			return false;

		response.append(buffer, bytesRead);
	}

	std::string::size_type	delimiterPosition = response.find("<END_RESPONSE>");

	response.erase(delimiterPosition);

	std::cout << response;
	std::cout.flush();

	return true;
}

