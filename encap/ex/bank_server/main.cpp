#include <iostream>
#include "Account.hpp"
#include "Bank.hpp"
#include "Server.hpp"
#include <exception>
#include <cstdlib>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./encap <port>" << std::endl;
		return 1;
	}

	try
	{
		Server server(std::atoi(av[1]));

		server.run();
	}
	catch (const std::exception &exception)
	{
		std::cerr
			<< "Server error: "
			<< exception.what()
			<< std::endl;
	}
	return (0);
}
