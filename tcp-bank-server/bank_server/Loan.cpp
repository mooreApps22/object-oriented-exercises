#include "Loan.hpp"

Loan::Loan(
	int id,
	int borrower,
	double principal,
	double interest,
	double debtBalance)
	:	_id(id),
		_borrower(borrower),
		_principal(principal),
		_interest(interest),
		_debtBalance(debtBalance)
{
}

Loan::Loan(const Loan &other)
	:	_id(other._id),
		_borrower(other._borrower),
		_principal(other._principal),
		_interest(other._interest),
		_debtBalance(other._debtBalance)
{
}


Loan	&Loan::operator=(const Loan &other)
{
	if (this != &other)
	{
		_id = other._id;
		_borrower = other._borrower;
		_principal = other._principal;
		_interest = other._interest;
		_debtBalance = other._debtBalance;
	}

	return *this;
}

Loan::~Loan()
{
}

int	Loan::getId() const
{
		return _id;
}

int	Loan::getBorrower() const
{
		return _borrower;
}

double	Loan::getPrincipal() const
{
		return _principal;
}

double	Loan::getInterest() const
{
		return _interest;
}

double	Loan::getDebtBalance() const
{
		return _debtBalance;
}

bool	Loan::_makePayment(double amount)
{
	if (amount <= 0)
		return false;

	if (amount > _debtBalance)
		return false;

	_debtBalance -= amount;

	return true;
}
