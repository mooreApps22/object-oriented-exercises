#pragma once
#include <sys/socket.h>
#include <netinet/in.h>

enum ResponseStatus
{
	RESPONSE_COMPLETE,
	SERVER_DISCONNECTED
};

class Client
{
	private:
		int					_port;
		int 				_socketFd;

		Client();
		Client(const Client &other);
		Client	&operator=(const Client &other);

		void			_setupSocket();
		void			_connectToServer();
		ResponseStatus	_receiveResponse();

	public:
		Client(int port);
		~Client();

		void run();
};
