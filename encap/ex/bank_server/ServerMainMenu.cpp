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
#include <sstream>
#include <stdexcept>

void	Server::_handleMainMenu(int clientFd, const std::string &request)
{
	switch (request[0])
	{
		case 'A':
		case 'a':
		{
			// View Customer Details
			_sendSimplePrompt(clientFd, "You Wish to See your Customer Details: \n");
			_sendCustomerDetails(clientFd);
			_sendMainMenu(clientFd);
			break;
		}
		case 'B':
		case 'b':
		{
			// View Accounts
			_sendSimplePrompt(clientFd, "This is a List of All your Accounts: \n");
			_sendAccountsDetails(clientFd);
			_sendMainMenu(clientFd);
			break;
		}
		case 'C':
		case 'c':
		{
			// Open New Account: Prompt Account Name
			_clientSessions[clientFd].state = WAITING_FOR_ACCOUNT_NAME;
			_sendSimplePrompt(clientFd, "Enter the New Account Name: ");
			_endResponse(clientFd);
			break;
		}
		case 'D':
		case 'd':
		{
			// Deposit
			_clientSessions[clientFd].state = WAITING_FOR_DEPOSIT_ACCOUNT;
			_sendSimplePrompt(clientFd, "Enter an Account ID for Your Deposit: ");
			_endResponse(clientFd);
			break;
		}
		case 'E':
		case 'e':
		{
			// Withdraw
			_clientSessions[clientFd].state = WAITING_FOR_WITHDRAW_ACCOUNT;
			_sendSimplePrompt(clientFd, "Enter an Account ID for Your Withdrawal: ");
			_endResponse(clientFd);
			break;
		}
		case 'F':
		case 'f':
		{
			// Modify Account
			_clientSessions[clientFd].state = WAITING_FOR_MODIFY_ACCOUNT;
			_sendSimplePrompt(clientFd, "Enter Account ID You Want to Modify: ");
			_endResponse(clientFd);
			break;
		}
		case 'G':
		case 'g':
		{
			// Delete Account
			_clientSessions[clientFd].state = WAITING_FOR_DELETE_ACCOUNT;
			_sendSimplePrompt(clientFd, "You Wish to Delete an Account: \n");
			_endResponse(clientFd);
			break;
		}
		case 'H':
		case 'h':
		{
			// Apply for Loan
			_clientSessions[clientFd].state = WAITING_FOR_LOAN_AMOUNT;
			_sendSimplePrompt(clientFd, "Enter loan amount: ");
			_endResponse(clientFd);
			break;
		}
		case 'I':
		case 'i':
		{
			// View Loans
			_sendLoansDetails(clientFd);
			_sendMainMenu(clientFd);
			break;
		}
		case 'J':
		case 'j':
		{
			//  Make Loan Payment
			_clientSessions[clientFd].state = WAITING_FOR_LOAN_PAYMENT_ID;
			_sendSimplePrompt(clientFd, "Enter Account ID for Loan: ");
			_endResponse(clientFd);
			break;
		}
		case 'K':
		case 'k':
		{
			// Disconnect
			_sendSimplePrompt(clientFd, "You have chosen to Disconnect. Goodbye.\n");
			break;
		}
		default:
		{
			// Invalid request
			_sendSimplePrompt(clientFd, "That is an Invalid Request. Please try again.\n");
			_sendMainMenu(clientFd);
			break;
		}
	}
}
