#include "Client.hpp"
#include <iostream> 
#include <cstdlib> 

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./client <port>" << std::endl;
		return 1;
	}

	try
	{
		Client client(std::atoi(av[1]));

		client.run();
	}
	catch (const std::exception &exception)
	{
		std::cerr
			<< "Client error: "
			<< exception.what()
			<< std::endl;
	}

	return (0);
}
