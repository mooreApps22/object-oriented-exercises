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

void	Server::_handleMainMenu(int clientFd, const std::string &request)
{
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
			
			_clientSessions[clientFd].state = WAITING_FOR_ACCOUNT_NAME;
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

void	Server::_handleClientRequest(int clientFd, const std::string &request)
{
	ClientSession &session = _clientSessions[clientFd];

	switch(session.state)
	{
		case MAIN_MENU:
			_handleMainMenu(clientFd, request);
			break;
		case WAITING_FOR_ACCOUNT_NAME:
			break;
		case WAITING_FOR_DEPOSIT_ACCOUNT:
			break;
		case WAITING_FOR_DEPOSIT_AMOUNT:
			break;
	}
}
