#include "Account.hpp"

Account::Account(int id, const std::string &name)
	:	_id(id),
		_name(name),
		_balance(0)
{
}

Account::~Account()
{
}


Account::Account(const Account &other)
	:	_id(other._id),
		_name(other._name),
		_balance(0)
{
}

Account	&Account::operator=(const Account &other)
{
	if (this != &other)
	{
		_id = other._id;
		_name = other._name;
		_balance = other._balance;
	}

	return *this;
}

int	Account::getId() const
{
	return _id;
}

const std::string	&Account::getName() const
{
	return _name;
}

double	Account::getBalance() const
{
		return _balance;
}

void	Account::_setName(const std::string &name)
{
	_name = name;
}

void	Account::_addFunds(double amount)
{
	if (amount > 0)
		_balance += amount;
}

bool	Account::_removeFunds(double amount)
{
	if(amount <= 0)
		return false;

	if(amount > _balance)
		return false;

	_balance -= amount;

	return true;
}

/*
std::ostream& operator << (std::ostream& p_os, const Account& p_account) {
	p_os << "[ID: " << p_account.id << "] - [Value: " << p_account.value << "]";
	return (p_os);
}
*/
