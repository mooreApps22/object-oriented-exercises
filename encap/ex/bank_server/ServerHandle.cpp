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

void	Server::_handleAccountName(int clientFd, const std::string &request)
{
	int	accountId;

	accountId = _bank.createAccount(clientFd, request);

	if (accountId == -1)
	{
		_sendSimplePrompt(
			clientFd,
			"Unable to create account.\n"
			"Enter New Account Name: "
		);
		return ;
	}

	_clientSessions[clientFd].state = MAIN_MENU;
	// send custom message here
	_sendMainMenu(clientFd);
}

void	Server::_handleDepositAccount(
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
			"Enter Deposit Account ID: "
		);
		return ;
	}
	
	account = _bank.getAccount(accountId);

	if (account == NULL)
	{
		_sendSimplePrompt(
			clientFd,
			"Account not found.\n"
			"Enter Deposit Account ID: "
		);
		return ;
	}

	session.selectedAccountId = accountId;
	session.state = WAITING_FOR_DEPOSIT_AMOUNT;

	_sendSimplePrompt(
		clientFd,
		"Enter Deposit Amount: "
	);
}

void	Server::_handleDepositAmount(int clientFd, const std::string &request)
{
	ClientSession		&session = _clientSessions[clientFd];
	std::istringstream	input(request);
	double				amount;
	char				extra;

	if (!(input >> amount) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid amount.\n"
			"Enter Deposit Amount: "
		);
		return ;
	}

	if (amount <= 0)
	{
		_sendSimplePrompt(
			clientFd,
			"Deposit must be greater than zero.\n"
			"Enter Deposit Amount: "
		);
	}

	if (_bank.deposit(
		clientFd,
		session.selectedAccountId,
		amount) == false)
	{
		_sendSimplePrompt(
			clientFd,
			"Deposit failed.\n"
		);
		session.state = MAIN_MENU;
		session.selectedAccountId = -1;
		_sendMainMenu(clientFd);
		return ;
	}

		_sendSimplePrompt(
			clientFd,
			"Deposit successful.\n"
		);
	
		session.state = MAIN_MENU;
		session.selectedAccountId = -1;

		_sendMainMenu(clientFd);
}

void	Server::_handleWithdrawAccount(
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
			"Enter Withdraw Account ID: "
		);
		return ;
	}
	
	account = _bank.getAccount(accountId);

	if (account == NULL)
	{
		_sendSimplePrompt(
			clientFd,
			"Account not found.\n"
			"Enter Withdraw Account ID: "
		);
		return ;
	}

	session.selectedAccountId = accountId;
	session.state = WAITING_FOR_WITHDRAW_AMOUNT;

	_sendSimplePrompt(
		clientFd,
		"Enter Withdraw Amount: "
	);
}

void	Server::_handleWithdrawAmount(int clientFd, const std::string &request)
{
	ClientSession		&session = _clientSessions[clientFd];
	std::istringstream	input(request);
	double				amount;
	char				extra;

	if (!(input >> amount) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid amount.\n"
			"Enter Withdraw Amount: "
		);
		return ;
	}

	if (amount <= 0)
	{
		_sendSimplePrompt(
			clientFd,
			"Withdrawal must be greater than zero.\n"
			"Enter Withdraw Amount: "
		);
	}

	if (_bank.withdraw(
		clientFd,
		session.selectedAccountId,
		amount) == false)
	{
		_sendSimplePrompt(
			clientFd,
			"Withdrawal failed.\n"
		);
		session.state = MAIN_MENU;
		session.selectedAccountId = -1;
		_sendMainMenu(clientFd);
		return ;
	}

		_sendSimplePrompt(
			clientFd,
			"Withdrawal successful.\n"
		);
	
		session.state = MAIN_MENU;
		session.selectedAccountId = -1;

		_sendMainMenu(clientFd);
}

void	Server::_handleDeleteAccount(int clientFd, const std::string &request)
{
	ClientSession		&session = _clientSessions[clientFd];
	std::istringstream	input(request);
	double				amount;
	char				extra;

	if (!(input >> amount) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid amount.\n"
			"Enter Withdraw Amount: "
		);
		return ;
	}

	if (_bank.deleteAccount(clientFd, session.selectedAccountId) == false)
	{
		_sendSimplePrompt(
			clientFd,
			"Unable to delete account.\n"
		);

		session.state = MAIN_MENU;
		_sendMainMenu(clientFd);
		return;
	}

	_sendSimplePrompt(
		clientFd,
		"Account deleted successfully.\n"
	);
	
	session.state = MAIN_MENU;
	_sendMainMenu(clientFd);
}

void	Server::_handleModifyAccount(
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
			"Enter Deposit Account ID: "
		);
		return ;
	}

	account = _bank.getAccount(accountId);

	if (account == NULL)
	{
		_sendSimplePrompt(
			clientFd,
			"Account not found.\n"
			"Enter Withdraw Account ID: "
		);
		return ;
	}

	session.selectedAccountId = accountId;
	session.state = WAITING_FOR_MODIFY_ACCOUNT_NAME;

	_sendSimplePrompt(
		clientFd,
		"Enter New Account Name: "
	);
}

void	Server::_handleModifyAccountName(
	int clientFd,
	const std::string &request)
{
	ClientSession	&session = _clientSessions[clientFd];

	if (request.empty())
	{
		_sendSimplePrompt(
			clientFd,
			"Account anme cannot be empty.\n"
			"Enter New Account Name: "
		);
		return; 
	}

	if (_bank.modifyAccountName(
			clientFd,
			session.selectedAccountId,
			request) == false)
	{
		_sendSimplePrompt(
			clientFd,
			"Unable to modify account.\n"
		);

		session.selectedAccountId = -1;
		session.state = MAIN_MENU;
		_sendMainMenu(clientFd);
		return;
	}

	_sendSimplePrompt(
		clientFd,
		"Account name changed successfully.\n"
	);

	session.selectedAccountId = -1;
	session.state = MAIN_MENU;
	_sendMainMenu(clientFd);
}

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
