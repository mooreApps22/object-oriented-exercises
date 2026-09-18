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
	_clientStates[clientFd] = MAIN_MENU;

	std::cout
		<< "Customer connected. fd = "
		<< clientFd
		<< std::endl;

//	std::count << _bank << std::endl;

	_sendMenu(clientFd);
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
	_clientBuffers[clientFd].append(buffer, bytesRead);
	newlinePosition = _clientBuffers[clientFd].find('\n');
	if (newlinePosition == std::string::npos)
		return;

	request = _clientBuffers[clientFd].substr(0, newlinePosition);
	_clientBuffers[clientFd].erase(0, newlinePosition + 1);
	//buffer[bytesRead] = '\0';


	std::cout
		<< "Received from customer fd "
		<< clientFd
		<< ": "
		<< request
		<< std::endl;

	_handleClientRequest(clientFd, request);

	//_sendMenu(clientFd);
}

void	Server::_disconnectClient(int pollIndex)
{
	int	clientFd = _pollFds[pollIndex].fd;

	std::cout
		<< "Customer disconnected. fd = "
		<< clientFd
		<< std::endl;

	_bank.removeCustomer(clientFd);
	_clientStates.erase(clientFd);
	_clientBuffers.erase(clientFd);

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

void Server::_sendMenu(int clientFd)
{
	const char	*mainMenu =
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

	send(clientFd, mainMenu, std::strlen(mainMenu), 0);
}

void	Server::_handleClientRequest(int clientFd, const std::string &request)
{
	if (request.empty())
		return;

	if (_clientStates[clientFd] == WAITING_FOR_ACCOUNT_NAME)
	{
		_bank.createAccount(clientFd, request);
		_clientStates[clientFd] = MAIN_MENU;
		_sendMenu(clientFd);

		return;
	}

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
		{
			// Open New Account: Prompt Account Name
			const char *response = "Enter Account Name: ";
			
			_clientStates[clientFd] = WAITING_FOR_ACCOUNT_NAME;
			send(clientFd, response, std::strlen(response), 0);

			break;
		}
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
