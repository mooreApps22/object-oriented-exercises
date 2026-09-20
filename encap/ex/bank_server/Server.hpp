#pragma once

#include <netinet/in.h>
#include <poll.h>
#include <string>
#include "Bank.hpp"
#include <map>



class Server {
	private:
		static const int	BUF_SIZE = 1024;
		static const int	MAX_CLIENTS = 1024;
		int					_port;
		int					_listenFd;
		int					_pollFdCount;
		struct pollfd		_pollFds[MAX_CLIENTS];
		Bank				_bank;

		enum ClientState
		{
			MAIN_MENU,
			WAITING_FOR_ACCOUNT_NAME,
			WAITING_FOR_DEPOSIT_ACCOUNT,
			WAITING_FOR_DEPOSIT_AMOUNT,
			WAITING_FOR_WITHDRAW_ACCOUNT,
			WAITING_FOR_WITHDRAW_AMOUNT,
			WAITING_FOR_DELETE_ACCOUNT,
			WAITING_FOR_MODIFY_ACCOUNT,
			WAITING_FOR_MODIFY_ACCOUNT_NAME,
			WAITING_FOR_LOAN_ACCOUNT,
			WAITING_FOR_LOAN_AMOUNT
		};

		struct ClientSession
		{
			ClientState	state;
			std::string	buffer;
			int			selectedAccountId;
		};

		std::map<int, ClientSession>	_clientSessions;

		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		void	_setupSocket();
		void	_initializePollFds();

		void	_acceptClient();
		void	_handleClient(int pollIndex);
		void	_disconnectClient(int pollIndex);

		void	_sendMainMenu(int clientFd);
		void 	_sendSimplePrompt(int clientFd, const char *response);
		void 	_sendInvalidRequestMessage(int clientFd);
		void 	_sendAccountNamePrompt(int clientFd);
		void 	_sendDepositAccountNamePrompt(int clientFd);

		void	_sendCustomerDetails(int clientFd);
		void 	_sendAccountsDetails(int clientFd);
		void 	_sendLoansDetails(int clientFd);

		void	_handleMainMenu(int clientFd, const std::string &request);
		void	_handleClientRequest(int clientFd, const std::string &request);

		void	_handleAccountName(int clientFd, const std::string &request);
		void	_handleDepositAccount(int clientFd, const std::string &request);
		void	_handleDepositAmount(int clientFd, const std::string &request);
		void	_handleWithdrawAccount(int clientFd, const std::string &request);
		void	_handleWithdrawAmount(int clientFd, const std::string &request);
		void	_handleLoanAccount(int clientFd, const std::string &request);
		void	_handleLoanAmount(int clientFd, const std::string &request);
		void	_handleDeleteAccount(int clientFd, const std::string &request);
		void	_handleModifyAccount(int clientFd, const std::string &request);
		void	_handleModifyAccountName(int clientFd, const std::string &request);

		void	_endResponse(int clientFd);
	public:
		Server(int port);
		~Server();

		void	run();
};
