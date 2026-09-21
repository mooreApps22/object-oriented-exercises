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

void	Server::_handleLoanPaymentId(int clientFd, const std::string &request)
{
	ClientSession			&session = _clientSessions[clientFd];
	std::istringstream		input(request);
	int						loanId;
	char					extra;

	if (!(input >> loanId) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid Loan ID.\n"
			"Enter Loan ID: "
		);
		_endResponse(clientFd);
		return ;
	}

	session.selectedLoanId = loanId;
	session.state = WAITING_FOR_LOAN_PAYMENT_AMOUNT;

	_sendSimplePrompt(
		clientFd,
		"Enter Payment Amount: "
	);
	_endResponse(clientFd);
}

void	Server::_handleLoanPaymentAmount(int clientFd, const std::string &request)
{
	ClientSession			&session = _clientSessions[clientFd];
	std::istringstream		input(request);
	double					amount;
	char					extra;

	if (!(input >> amount) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid Loan payment amount.\n"
			"Enter Payment Amount: "
		);
		_endResponse(clientFd);
		return ;
	}

	if (amount <= 0)
	{
		_sendSimplePrompt(clientFd,
			"Payment must be greater than zero.\n"
			"Enter Payment Amount: "
		);
		_endResponse(clientFd);
		return ;
	}

	if (_bank.makeLoanPayment(
		clientFd,
		session.selectedLoanId,
		amount) == false)
	{
		_sendSimplePrompt(
			clientFd, 
			"Loan payment failed.\n"
		);

		session.selectedLoanId = -1;
		session.state = MAIN_MENU;
		_sendMainMenu(clientFd);
		return ;
	}

	_sendSimplePrompt(
		clientFd, 
		"Loan payment successful.\n"
	);

	session.selectedLoanId = -1;
	session.state = MAIN_MENU;
	_sendMainMenu(clientFd);
}
