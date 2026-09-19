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

		void	_sendMenu(int clientFd);
		void	_handleMainMenu(int clientFd, const std::string &request);
		void	_handleClientRequest(int clientFd, const std::string &request);
	public:
		Server(int port);
		~Server();

		void	run();
};
