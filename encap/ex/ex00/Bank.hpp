#pragma once
#include <vector>
#include <iostream>
#include "Account.hpp"

struct Bank
{
	int liquidity;
	std::vector<Account *> clientAccounts;

	Bank();

	friend std::ostream& operator<<(std::ostream& p_os, const Bank& p_bank);
};
