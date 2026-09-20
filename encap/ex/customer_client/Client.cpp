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

Client::Client(int port)
	:	_port(port),
		_socketFd(-1)
{
	_setupSocket();
	_connectToServer();
}

Client::~Client()
{
	if (_socketFd != -1)
		close(_socketFd);
}

void	Client::_setupSocket()
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (_socketFd == -1)
		throw std::runtime_error("socket() failed");
}

void	Client::_connectToServer()
{
	struct sockaddr_in	serverAddress;

	std::memset(&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serverAddress.sin_port = htons(_port);

	if (connect(
		_socketFd,
		reinterpret_cast<struct sockaddr *>(&serverAddress),
		sizeof(serverAddress)
		) == -1)
	{
		throw std::runtime_error("connect() failed");
	}
}

void	Client::run()
{
	std::string			input;

	for (;;)
	{
		if (_receiveResponse() == false)
			break;

		std::getline(std::cin, input);

		if(!std::cin)
			break;

		input += '\n';
		
		send(_socketFd, input.c_str(), input.size(), 0);
	}
}
