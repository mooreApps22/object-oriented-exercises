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

void Server::_sendSimplePrompt(int clientFd, const char *response)
{
	send(clientFd, response, std::strlen(response), 0);
}

void Server::_sendAccountsDetails(int clientFd)
{
	const Customer				*customer;
	const Account				*account;
	std::ostringstream			response;
	std::string					message;

	customer = _bank.getCustomer(clientFd);

	if (customer == NULL)
	{
		_sendSimplePrompt(
			clientFd,
			"Unable to find customer.\n"
		);
		return;
	}

	const std::vector<int>	&accountIds = customer->getAccountIds();
	
	response
		<< "\n"
		<< "================================\n"
		<< "=           ACCOUNTS           =\n"
		<< "================================\n";

	if (accountIds.empty())
	{
		response
			<< "No accounts found.\n";
	}
	else
	{
		for (std::vector<int>::const_iterator it = accountIds.begin();
				it != accountIds.end();
				++it)
		{
			account = _bank.getAccount(*it);

			if (account != NULL)
			{
				response
					<< "Account ID: "
					<< account->getId()
					<< " | "
					<< "Name: "
					<< account->getName()
					<< " | "
					<< "Balance: "
					<< account->getBalance()
					<< "\n";
			}
		}
	}

	response
		<< "================================\n";

	message = response.str();

	_sendSimplePrompt(clientFd, message.c_str());
}

void	Server::_sendCustomerDetails(int clientFd)
{
	const Customer		*customer;
	std::ostringstream	response;

	customer = _bank.getCustomer(clientFd);

	if (customer == NULL)
	{
		_sendSimplePrompt(
			clientFd,
			"Unable to find customer.\n"
		);
		return;
	}

	response
		<< "\n"
		<< "================================\n"
		<< "=      CUSTOMER DETAILS        =\n"
		<< "================================\n"
		<< "Customer ID: "
		<< customer->getId()
		<< "\n"
		<< "Cash: $"
		<< customer->getCash()	
		<< "\n"
		<< "Accounts: "
		<< customer->getAccountIds().size()	
		<< "\n"
		<< "================================\n";

	_sendSimplePrompt(
		clientFd,
		response.str().c_str()
	);
}

void Server::_sendMainMenu(int clientFd)
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
