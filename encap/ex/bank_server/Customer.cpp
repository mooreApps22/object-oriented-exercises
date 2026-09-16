#include "Customer.hpp"
#include <algorithm>

Customer::Customer(int id, double startingCash)
	:	_id(id),
		_cash(startingCash)
{
}

Customer::Customer(const Customer &other)
	:	_id(other._id),
		_cash(other._cash),
		_accountIds(other._accountIds)
{
}

Customer	&Customer::operator=(const Customer &other)
{
	if (this != &other)
	{
		_id = other._id;
		_cash = other._cash;
		_accountIds = other._accountIds;
	}
	return *this;
}

Customer::~Customer()
{
}


int	Customer::getId() const
{
	return _id;
}

double	Customer::getCash() const
{
	return _cash;
}

const std::vector<int>	Customer::&getAccountIds() const
{
	return _accountIds;
}

void	Customer::_addCash(double amount)
{
	if (amount > 0)
		_cash += amount;
}

bool	Customer::_removeCash(double amount)
{
	if (amount <= 0)
			return false;

	if (amount > _cash)
		return false;

	_cash -= amount;

	return true;
}

void	Customer::_addAccount(int accountId)
{
	_accountIds.push_back(accountId);
}

void	Customer::_removeAccount(int accountId)
{
	std::vector<int>::iterator	accountIterator;

	accountIterator = std::find(
		_accountIds.begin(),
		_accountIds.end(),
		accountId
	);

	if (accountIterator != _accountIds.end())
		_accountIds.erase(accountIterator);
}
