#pragma once

#include <netinet/in.h>
#include <poll.h>

class Server {
	private:
		static const int	BUF_SIZE = 1024;
		static const int	MAX_CLIENTS = 1024;
		int					_port;
		int					_listenFd;
		int					_pollFdCount;
		struct pollfd		_pollFds[MAX_CLIENTS];

		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		void	setupSocket();
		void	initializePollFds();

		void	acceptClient();
		void	handleClient(int pollIndex);
		void	disconnectClient(int pollIndex);

		void	sendMenu(int clientFd);
	public:
		Server(int port);
		~Server();

		void	run();
};
