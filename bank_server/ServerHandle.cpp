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

void	Server::_endResponse(int clientFd)
{
	const char	*delimiter = "<END_RESPONSE>";

	send(clientFd, delimiter, std::strlen(delimiter), 0);
}

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
		_endResponse(clientFd);
		return ;
	}

	_clientSessions[clientFd].state = MAIN_MENU;
	std::ostringstream	response;
	std::string			message;
	response
		<< "Newly Opened Account Name: "
		<< request
		<< " & Account ID: "
		<< accountId
		<< ".\n";

	message = response.str();
	_sendSimplePrompt(clientFd, message.c_str());
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
		_endResponse(clientFd);
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
		_endResponse(clientFd);
		return ;
	}

	session.selectedAccountId = accountId;
	session.state = WAITING_FOR_DEPOSIT_AMOUNT;

	_sendSimplePrompt(
		clientFd,
		"Enter Deposit Amount: "
	);
	_endResponse(clientFd);
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
		_endResponse(clientFd);
		return ;
	}

	if (amount <= 0)
	{
		_sendSimplePrompt(
			clientFd,
			"Deposit must be greater than zero.\n"
			"Enter Deposit Amount: "
		);
		_endResponse(clientFd);
		return;
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
		_endResponse(clientFd);
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
		_endResponse(clientFd);
		return ;
	}

	session.selectedAccountId = accountId;
	session.state = WAITING_FOR_WITHDRAW_AMOUNT;

	_sendSimplePrompt(
		clientFd,
		"Enter Withdraw Amount: "
	);
	_endResponse(clientFd);
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
		_endResponse(clientFd);
		return ;
	}

	if (amount <= 0)
	{
		_sendSimplePrompt(
			clientFd,
			"Withdrawal must be greater than zero.\n"
			"Enter Withdraw Amount: "
		);
		_endResponse(clientFd);
		return;
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
	double				accountId;
	char				extra;

	if (!(input >> accountId) || (input >> extra))
	{
		_sendSimplePrompt(
			clientFd,
			"Invalid Account ID.\n"
			"Enter Account ID to DELETE: "
		);
		_endResponse(clientFd);
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
			"Enter Account ID to Modify: "
		);
		_endResponse(clientFd);
		return ;
	}

	account = _bank.getAccount(accountId);

	if (account == NULL)
	{
		_sendSimplePrompt(
			clientFd,
			"Account not found.\n"
			"Enter Account ID to Modify: "
		);
		_endResponse(clientFd);
		return ;
	}

	session.selectedAccountId = accountId;
	session.state = WAITING_FOR_MODIFY_ACCOUNT_NAME;

	_sendSimplePrompt(
		clientFd,
		"Enter New Account Name: "
	);
	_endResponse(clientFd);
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
			"Account name cannot be empty.\n"
			"Enter New Account Name: "
		);
		_endResponse(clientFd);
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
