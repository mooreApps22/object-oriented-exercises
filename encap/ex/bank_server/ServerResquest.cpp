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

void	Server::_handleClientRequest(int clientFd, const std::string &request)
{
	if (request.empty())
		return;

	if (_clientSessions[clientFd].state == WAITING_FOR_ACCOUNT_NAME)
	{
		_bank.createAccount(clientFd, request);
		_clientSessions[clientFd].state = MAIN_MENU;
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
