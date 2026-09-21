#include "Bank.hpp"
#include <algorithm>

// BANK LOAN METHODS

Loan	*Bank::_findLoan(int loanId)
{
	std::vector<Loan>::iterator loanIterator;

	for (
		loanIterator = _loans.begin();
		loanIterator != _loans.end();
		++loanIterator
	)
	{
		if (loanIterator->getId() == loanId)
			return &(*loanIterator);
	}

	return NULL;
}

bool	Bank::makeLoanPayment(int loanId, double amount)
{
	Loan	*loan = _findLoan(loanId);

	if (loan == NULL)
		return false;

	if (loan->_makePayment(amount) == false)
		return false;
	_liquidity += amount;
	if (loan->getDebtBalance() == 0)
		_removeLoan(loan->getId());	

	return true;
}

void	Bank::_removeLoan(int loanId)
{
	std::vector<Loan>::iterator loanIterator;

	for (
		loanIterator = _loans.begin();
		loanIterator != _loans.end();
		++loanIterator
	)
	{
		if (loanIterator->getId() == loanId)
		{
			_loans.erase(loanIterator);
			return ;
		}
	}
}

std::vector<const Loan *>	Bank::getCustomerLoans(int customerId) const
{
	std::vector<const Loan *>	customerLoans;

	for (std::vector<Loan>::const_iterator it = _loans.begin();
		it != _loans.end();
		++it)
	{
		if (it->getBorrower() == customerId)
			customerLoans.push_back(&(*it));
	}
	
	return customerLoans;
}
