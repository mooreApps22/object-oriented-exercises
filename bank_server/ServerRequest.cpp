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

bool	Server::_handleClientRequest(int clientFd, const std::string &request)
{
	ClientSession &session = _clientSessions[clientFd];

	switch(session.state)
	{
		case MAIN_MENU:
			return _handleMainMenu(clientFd, request);
		case WAITING_FOR_ACCOUNT_NAME:
			_handleAccountName(clientFd, request);
			break;
		case WAITING_FOR_DEPOSIT_ACCOUNT:
			_handleDepositAccount(clientFd, request);
			break;
		case WAITING_FOR_DEPOSIT_AMOUNT:
			_handleDepositAmount(clientFd, request);
			break;
		case WAITING_FOR_WITHDRAW_ACCOUNT:
			_handleWithdrawAccount(clientFd, request);
			break;
		case WAITING_FOR_WITHDRAW_AMOUNT:
			_handleWithdrawAmount(clientFd, request);
			break;
		case WAITING_FOR_MODIFY_ACCOUNT:
			_handleModifyAccount(clientFd, request);
			break;
		case WAITING_FOR_MODIFY_ACCOUNT_NAME:
			_handleModifyAccountName(clientFd, request);
			break;
		case WAITING_FOR_DELETE_ACCOUNT:
			_handleDeleteAccount(clientFd, request);
			break;
		case WAITING_FOR_LOAN_AMOUNT:
			_handleLoanAmount(clientFd, request);
			break;
		case WAITING_FOR_LOAN_PAYMENT_ID:
			_handleLoanPaymentId(clientFd, request);
			break;
		case WAITING_FOR_LOAN_PAYMENT_AMOUNT:
			_handleLoanPaymentAmount(clientFd, request);
			break;
	}

	return false;
}
