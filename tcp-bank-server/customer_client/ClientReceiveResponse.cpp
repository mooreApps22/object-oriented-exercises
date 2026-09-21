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

ResponseStatus	Client::_receiveResponse()
{
	std::string	response;
	char		buffer[1024];
	ssize_t		bytesRead;

	while (response.find("<END_RESPONSE>") == std::string::npos
			&& response.find("<DISCONNECT>") == std::string::npos)
	{
		bytesRead = recv(_socketFd, buffer, sizeof(buffer), 0);

		if (bytesRead <= 0)
			return SERVER_DISCONNECTED;

		response.append(buffer, bytesRead);
	}

	std::string::size_type	delimiterPosition;

	delimiterPosition = response.find("<DISCONNECT>");

	if (delimiterPosition != std::string::npos)
	{
		response.erase(delimiterPosition);

		std::cout << response;
		std::cout.flush();

		return SERVER_DISCONNECTED;
	}

	delimiterPosition = response.find("<END_RESPONSE>");

	response.erase(delimiterPosition);

	std::cout << response;
	std::cout.flush();

	return RESPONSE_COMPLETE;
}

