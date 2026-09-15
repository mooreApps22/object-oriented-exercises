#pragma once
#include <sys/socket.h>
#include <netinet/in.h>

class Client
{
	private:
		int					_port;
		int 				_socketFd;

		Client();
		Client(const Client &other);
		Client	&operator=(const Client &other);

		void	setupSocket();
		void	connectToServer();

	public:
		Client(int port);
		~Client();

		void run();
};
