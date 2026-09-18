#include "Bank.hpp"
#include <algorithm>

// BANK ACCOUNT METHODS

Account	*Bank::_findAccount(int accountId)
{
	std::vector<Account>::iterator	accountIterator;

	for (
		accountIterator = _accounts.begin();
		accountIterator != _accounts.end();
		++accountIterator
	)
	{
		if (accountIterator->getId() == accountId)
			return &(*accountIterator);
	}

	return NULL;
}

const Account	*Bank::getAccount(int accountId) const 
{
	std::vector<Account>::const_iterator	accountIterator;

	for (
		accountIterator = _accounts.begin();
		accountIterator != _accounts.end();
		++accountIterator
	)
	{
		if (accountIterator->getId() == accountId)
			return &(*accountIterator);
	}

	return NULL;
	
}

int	Bank::createAccount(int customerId, const std::string &accountName)
{
	Customer	*customer;

	customer = _findCustomer(customerId);

	if (customer == NULL)
		return -1;

	int accountId = _nextAccountId++;

	_accounts.push_back(
		Account(accountId, accountName)
	);

	customer->_addAccount(accountId);

	return accountId;
}

bool	Bank::deleteAccount(int customerId, int accountId)
{
	Customer	*customer;

	customer = _findCustomer(customerId);

	if (customer == NULL)
		return false;

	const std::vector<int>	&accountIds = customer->getAccountIds();

	if (
		std::find(accountIds.begin(), accountIds.end(), accountId)
		== accountIds.end()
	)
		return false;

	std::vector<Account>::iterator	accountIterator;

	for (
		accountIterator = _accounts.begin();
		accountIterator != _accounts.end();
		++accountIterator
	)
	{
		if (accountIterator->getId() == accountId)
		{
			_accounts.erase(accountIterator);
			customer->_removeAccount(accountId);
			return true;
		}
	}

	return false;
}

bool	Bank::modifyAccountName(int customerId, int accountId, const std::string &accountName)
{
	Customer	*customer;

	customer = _findCustomer(customerId);

	if (customer == NULL)
		return false;

	const std::vector<int> &accountIds = customer->getAccountIds();

	if (
		std::find(accountIds.begin(), accountIds.end(), accountId)
		== accountIds.end()
	)
		return false;

	Account	*account = _findAccount(accountId);

	if (account == NULL)
		return false;

	account->_setName(accountName);

	return true;
}
