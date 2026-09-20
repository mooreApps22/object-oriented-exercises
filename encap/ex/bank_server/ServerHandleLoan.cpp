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

void	Server::_handleLoanAccount(
	int clientFd,
	const std::string &request)
{
	ClientSession		&session = _clientSessions[clientFd];
	std::istringstream	input(request);
	const Account		*account;
	int					accountId;
	char				extra;

	if (!(input >> accountId) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid Account ID.\n"
			"Enter Account ID For Loan: "
		);
		return ;
	}

	account = _bank.getAccount(accountId);
	
	if (account == NULL)
	{
		_sendSimplePrompt(
			clientFd,
			"Account not found.\n"
			"Enter Account ID for Loan: "
		);
		return;
	}

	session.selectedAccountId = accountId;
	session.state = WAITING_FOR_LOAN_AMOUNT;

	_sendSimplePrompt(
		clientFd,
		"Enter Loan Amount: "
	);
}

void	Server::_handleLoanAmount(int clientFd, const std::string &request)
{
	ClientSession		&session = _clientSessions[clientFd];
	std::istringstream	input(request);
	double				amount;
	int					loanId;
	char				extra;

	if (!(input >> amount) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid loan amount.\n"
			"Enter Loan Amount: "
		);
		return ;
	}

	if (amount <= 0)
	{
		_sendSimplePrompt(
			clientFd,
			"Loan amount must be greater than zero.\n"
			"Enter Loan Amount: "
		);
		return ;
	}

	loanId = _bank.applyForLoan(
		clientFd,
		session.selectedAccountId,
		amount
	);

	if (loanId == -1)
	{
		_sendSimplePrompt(
			clientFd,
			"Loan application failed.\n"
		);

		session.selectedAccountId = -1;
		session.state = MAIN_MENU;
		_sendMainMenu(clientFd);
		return;
	}

	_sendSimplePrompt(
		clientFd,
		"Loan approved.\n"
	);

	session.selectedAccountId = -1;
	session.state = MAIN_MENU;
	_sendMainMenu(clientFd);
}
