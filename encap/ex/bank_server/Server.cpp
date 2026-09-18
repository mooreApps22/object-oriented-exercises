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

	std::cout
		<< "Customer connected. fd = "
		<< clientFd
		<< std::endl;

//	std::count << _bank << std::endl;

	_sendMenu(clientFd);
}

void	Server::_handleClient(int pollIndex)
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
		_disconnectClient(pollIndex);
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
	 * handleClientRequest(clientFd, buffer);
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

	 _handleClientRequest(clientFd, buffer);

	_sendMenu(clientFd);
}

void	Server::_disconnectClient(int pollIndex)
{
	int	clientFd = _pollFds[pollIndex].fd;

	std::cout
		<< "Customer disconnected. fd = "
		<< clientFd
		<< std::endl;

	_bank.removeCustomer(clientFd);

	std::cout << _bank << std::endl;

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

void Server::_sendMenu(int clientFd)
{
	const char	*menu =
		"\n"
		"============================\n"
		"          BANK SERVER       \n"
		"============================\n"
		"A. View Customer Details\n"
		"B. View Accounts\n"
		"C. Open New Account\n"
		"D. Deposit\n"
		"E. Withdraw\n"
		"F. Modify Acount\n"
		"G. Delete Account\n"
		"H. Apply for Loan\n"
		"I. View Loans\n"
		"J. Make Loan Payment\n"
		"K. Disconnect\n"
		"============================\n"
		"Select an option[A-K]: ";

	send(clientFd, menu, std::strlen(menu), 0);
}

void	Server::_handleClientRequest(int clientFd, const std::string &request)
{
	if (request.empty())
		return;
	switch (request[0])
	{
		case 'A':
		case 'a':
			// View Customer Details
			
			break;
		case 'B':
		case 'b':
			// View Accounts

			break;
		case 'C':
		case 'c':
			// Open New Account

			_bank.createAccount(clientFd, "TMP ACCOUNT NAME");
			break;
		case 'D':
		case 'd':
			// Deposit
			
			break;
		case 'E':
		case 'e':
			// Withdraw

			break;
		case 'F':
		case 'f':
			// Modify Account

			break;
		case 'G':
		case 'g':
			// Delete Account

			break;
		case 'H':
		case 'h':
			// Apply for Loan

			break;
		case 'I':
		case 'i':
			// View Loans

			break;
		case 'J':
		case 'j':
			//  Make Loan Payment

			break;
		case 'K':
		case 'k':
			// Disconnect

			break;
		default:
			// Invalid request
			const char* res =
			"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
			"\n      Invalid request      \n"
			"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			send(clientFd, res, std::strlen(res), 0);
			break;
	}
}
