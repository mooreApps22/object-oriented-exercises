#include "Bank.hpp"
#include <algorithm>

// BANK CUSTOMER METHODS

Customer	*Bank::_findCustomer(int customerId)
{
	std::vector<Customer>::iterator customerIterator;

	for (
		customerIterator = _customers.begin();
		customerIterator != _customers.end();
		++customerIterator
	)
	{
		if (customerIterator->getId() == customerId)
			return &(*customerIterator);
	}
	return NULL;
}

double	Bank::_generateCustomerCash()
{
	return 100 + (std::rand() % 901);
}

void	Bank::addCustomer(int customerId)
{
	if (_findCustomer(customerId) != NULL)
		return;

	double startingCash = _generateCustomerCash();

	_customers.push_back(Customer(customerId, startingCash));
}

void	Bank::removeCustomer(int customerId)
{
	std::vector<Customer>::iterator customerIterator;

	for(
		customerIterator = _customers.begin();
		customerIterator != _customers.end();
		++customerIterator
	)
	{
		if (customerIterator->getId() == customerId)
		{
			_customers.erase(customerIterator);
			return;
		}
	}
}

const Customer	*Bank::getCustomer(int customerId) const
{
	std::vector<Customer>::const_iterator	customerIterator;

	for(
		customerIterator = _customers.begin();
		customerIterator != _customers.end();
		++customerIterator
	)
	{
		if (customerIterator->getId() == customerId)
			return &(*customerIterator);
	}

	return NULL;
}

bool	Bank::_customerOwnsAccount(const Customer &customer, int accountId) const
{
	const std::vector<int>	&accountIds = customer.getAccountIds();

	return (
		std::find(accountIds.begin(), accountIds.end(), accountId)
		!= accountIds.end()
	);
}
