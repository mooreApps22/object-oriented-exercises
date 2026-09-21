#include "Server.hpp"
#include "Bank.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <stdexcept>

Server::Server(int port)
	:_port(port),
	_listenFd(-1),
	_pollFdCount(1)
{
	_setupSocket();
	_initializePollFds();
}

Server::~Server()
{
	for (int i = 0; i < _pollFdCount; ++i)
	{
		if (_pollFds[i].fd != -1)
			close(_pollFds[i].fd);
	}
}

void	Server::_setupSocket()
{
	struct	sockaddr_in	serverAddress;

	_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenFd == -1)
		throw	std::runtime_error("socket() failed");

	std::memset(&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serverAddress.sin_port = htons(_port);

	if (bind(
		_listenFd, 
		reinterpret_cast<const struct sockaddr *>(&serverAddress),
		sizeof(serverAddress)
	) == -1)
	{
		close(_listenFd);
		_listenFd = -1;
		throw std::runtime_error("bind() failed");
	}

	if (listen(_listenFd, 10) == -1)
	{
		close(_listenFd);
		_listenFd = -1;
		throw std::runtime_error("listen() failed");
	}
}

void	Server::_initializePollFds()
{
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		_pollFds[i].fd = -1;
		_pollFds[i].events = POLLIN;
		_pollFds[i].revents = 0;
	}

	_pollFds[0].fd = _listenFd;
	_pollFds[0].events = POLLIN;
}

void	Server::run()
{
	std::cout
		<< "Bank server listening on port "
		<< _port
		<< std::endl;

	for (;;)
	{
		if (poll(_pollFds, _pollFdCount, -1) == -1)
			throw std::runtime_error("poll() failed");
		
		for(int i = 0; i < _pollFdCount; ++i)
		{
			if(!(_pollFds[i].revents & POLLIN))
				continue;
			
			if(_pollFds[i].fd == _listenFd)
				_acceptClient();
			else
				_handleClient(i);
		}
	}
}
