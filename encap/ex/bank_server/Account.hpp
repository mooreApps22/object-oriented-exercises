#pragma once
#include <iostream>

struct Account
{
	int id;
	int	value;

	Account();

	friend std::ostream& operator<<(std::ostream& p_os, const Account& p_account);
};
