#include "Server.hpp"

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
	setupSocket();
	initializePollFds();
}

Server::~Server()
{
	for (int i = 0; i < _pollFdCount; ++i)
	{
		if (_pollFds[i].fd != -1)
			close(_pollFds[i].fd);
	}
}

void	Server::setupSocket()
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

void	Server::initializePollFds()
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
				acceptClient();
			else
				handleClient(i);
		}
	}
}

void	Server::acceptClient()
{
	struct sockaddr_in	clientAddress;
	socklen_t			clientAddressSize;

	clientAddressSize = sizeof(clientAddress);

	int clientFd = accept(
		_listenFd,
		reinterpret_cast<struct sockaddr *>(&clientAddress),
		&clientAddressSize
	);

	if (clientFd == -1)
		throw std::runtime_error("accept() failed");

	if (_pollFdCount >= MAX_CLIENTS)
	{
		std::cerr << "Server full. Rejecting client." << std::endl;
		close(clientFd);
		return;
	}

	_pollFds[_pollFdCount].fd = clientFd;
	_pollFds[_pollFdCount].events = POLLIN;
	_pollFds[_pollFdCount].revents = 0;

	++_pollFdCount;

	std::cout
		<< "Customer connected. fd = "
		<< clientFd
		<< std::endl;

	sendMenu(clientFd);
}

void	Server::handleClient(int pollIndex)
{
	char	buffer[BUF_SIZE + 1];
	int		clientFd;
	ssize_t	bytesRead;

	clientFd = _pollFds[pollIndex].fd;

	bytesRead = recv(
		clientFd,
		buffer,
		BUF_SIZE,
		0
	);

	if (bytesRead <= 0)
	{
		disconnectClient(pollIndex);
		return;
	}

	buffer[bytesRead] = '\0';

	std::cout
		<< "Received from customer fd "
		<< clientFd
		<< ": "
		<< buffer;

	/*
	 * To implement next:
	 *
	 * handleCustomerRequest(clientFd, buffer);
	 *
	 * Menu:
	 * A: View Account Details
	 * B: Deposit
	 * C: Withdraw
	 * D: Apply for Loan
	 * E: Edit Account info
	 * G: Disconnect
	 * 
	 */

	sendMenu(clientFd);
}

void	Server::disconnectClient(int pollIndex)
{
	int	clientFd = _pollFds[pollIndex].fd;

	std::cout
		<< "Customer disconnected. fd = "
		<< clientFd
		<< std::endl;

	close(clientFd);

	/*
	 * If a client is removed all indexes have to be shifted.
	 */
	for (int i = pollIndex; i < _pollFdCount - 1; ++i)
		_pollFds[i] = _pollFds[i + 1];

	_pollFdCount--;

	_pollFds[_pollFdCount].fd = -1;
	_pollFds[_pollFdCount].events = POLLIN;
	_pollFds[_pollFdCount].revents = 0;
}

void Server::sendMenu(int clientFd)
{
	const char	*menu =
		"\n"
		"============================\n"
		"          BANK SERVER       \n"
		"============================\n"
		"A. View Account Details\n"
		"B. Deposit\n"
		"C. Withdraw Funds\n"
		"D. Apply for Loan\n"
		"E. Edit Account Details\n"
		"F. Disconnect\n"
		"============================\n"
		"Select an option[A-F]: ";

	send(clientFd, menu, std::strlen(menu), 0);
}

void	Server::handleClientRequest(int clientFd, const std::string &request)
{
	if (request.empty())
		return;
	switch (request[0])
	{
		case 'A':
		case 'a':
			// View Account Details
			break;
		case 'B':
		case 'b':
			// Deposit
			break;
		case 'C':
		case 'c':
			// Withdrawal
			break;
		case 'D':
		case 'd':
			// Apply for Loan
			break;
		case 'E':
		case 'e':
			// Edit Account Details
			break;
		case 'F':
		case 'f':
			// Disconnect
			break;
		default:
			// Invalid request
			break;
	}
}
