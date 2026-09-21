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

void	Server::_acceptClient()
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

	_bank.addCustomer(clientFd);
	_clientSessions[clientFd] = ClientSession();

	std::cout
		<< "Customer connected. fd = "
		<< clientFd
		<< std::endl;

//	std::count << _bank << std::endl;

	_sendMainMenu(clientFd);
}

void	Server::_handleClient(int pollIndex)
{
	char					buffer[BUF_SIZE + 1];
	int						clientFd;
	ssize_t					bytesRead;
	std::string::size_type	newlinePosition;
	std::string				request;

	clientFd = _pollFds[pollIndex].fd;

	bytesRead = recv(
		clientFd,
		buffer,
		BUF_SIZE,
		0
	);

	if (bytesRead <= 0)
	{
		_disconnectClient(pollIndex);
		return;
	}
	_clientSessions[clientFd].buffer.append(buffer, bytesRead);
	newlinePosition = _clientSessions[clientFd].buffer.find('\n');
	if (newlinePosition == std::string::npos)
		return;

	request = _clientSessions[clientFd].buffer.substr(0, newlinePosition);
	_clientSessions[clientFd].buffer.erase(0, newlinePosition + 1);
	//buffer[bytesRead] = '\0';


	std::cout
		<< "Received from customer fd "
		<< clientFd
		<< ": "
		<< request
		<< std::endl;

	if (_handleClientRequest(clientFd, request))
		_disconnectClient(pollIndex);
}

void	Server::_disconnectClient(int pollIndex)
{
	int	clientFd = _pollFds[pollIndex].fd;

	std::cout
		<< "Customer disconnected. fd = "
		<< clientFd
		<< std::endl;

	_bank.removeCustomer(clientFd);
	_clientSessions.erase(clientFd);

	close(clientFd);

	std::cout << _bank << std::endl;

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
