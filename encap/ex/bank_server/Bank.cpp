#include "Bank.hpp"
#include <algorithm>

// BANK CONSTRUCTOR & DECONSTRUCTOR 

Bank::Bank()
	:	_liquidity(_generateLiquidity()),
		_nextAccountId(1000),
		_nextLoanId(5000)
{
}

Bank::~Bank()
{
}

// BANK BUSINESS METHODS

bool	Bank::deposit(int customerId, int accountId, double amount)
{
	Customer	*customer;
	Account		*account;

	customer = _findCustomer(customerId);
	account = _findAccount(accountId);

	if (customer == NULL || account == NULL)
		return false;

	if (amount <= 0)
		return false;

	if (_customerOwnsAccount(*customer, accountId) == false)
		return false;

	if (customer->_removeCash(amount) == false)
		return false;

	double	bankFee = amount * 0.05;
	double	accountDeposit = amount - bankFee;

	_liquidity += bankFee;
	account->_addFunds(accountDeposit);

	return true;
}

bool	Bank::withdraw(int customerId, int accountId, double amount)
{
	Customer	*customer;
	Account		*account;

	customer = _findCustomer(customerId);
	account = _findAccount(accountId);

	if (customer == NULL || account == NULL)
		return false;

	if (amount <= 0)
		return false;
	
	if (_customerOwnsAccount(*customer, accountId) == false)
		return false;

	if (account->_removeFunds(amount) == false)
		return false;

	customer->_addCash(amount);

	return true;
}

int	Bank::applyForLoan(int customerId, int accountId, double principal)
{
	Customer	*customer;
	Account		*account;
	double		interest;
	int			loanId;

	customer = _findCustomer(customerId);
	account = _findAccount(accountId);

	if (customer == NULL || account == NULL)
		return -1;

	if (principal <= 0)
		return -1;

	if (_customerOwnsAccount(*customer, accountId) == false)
		return  -1;

	if (principal > _liquidity)
		return -1;

	interest = principal * 0.10;
	loanId = _nextLoanId++;

	_loans.push_back(
		Loan(
			loanId,
			customerId,
			principal,
			interest
		)
	);

	_liquidity -= principal;
	account->_addFunds(principal);

	return loanId;
}



bool	Bank::makeLoanPayment(int customerId, int loanId, double amount)
{
	Customer	*customer;
	Loan		*loan;

	customer = _findCustomer(customerId);
	loan = _findLoan(loanId);

	if (customer == NULL || loan == NULL)
		return false;

	if (loan->getBorrower() != customerId)
		return false;

	if (amount <= 0)
		return false;

	if (customer->getCash() < amount)
		return false;

	if (amount > loan->getDebtBalance())
		return false;

	if (customer->_removeCash(amount) == false)
		return false;

	if (loan->_makePayment(amount) == false)
	{
		customer->_addCash(amount);
		return false;
	}

	_liquidity += amount;
	return true;

}


// BANK OUT STREAM METHODS

void Bank::_getAllCustomers(std::ostream &p_os) const
{
	std::vector<Customer>::const_iterator	customerIterator;

	for (
		customerIterator = _customers.begin();
		customerIterator != _customers.end();
		++customerIterator
	)
	{
		p_os << "Customer " << customerIterator->getId()
			<< "| Cash: " << customerIterator->getCash()
			<< std::endl;
	}
}

void Bank::_getAllAccounts(std::ostream &p_os) const
{
	std::vector<Account>::const_iterator	accountIterator;

	for (
		accountIterator = _accounts.begin();
		accountIterator != _accounts.end();
		++accountIterator
	)
	{
		p_os << " Account " << accountIterator->getId()
			<< " |  " << accountIterator->getName()
			<< " | Balance  " << accountIterator->getBalance()
			<< std::endl;
	}
}

void Bank::_getAllLoans(std::ostream &p_os) const
{
	std::vector<Loan>::const_iterator		loanIterator;

	for (
		loanIterator = _loans.begin();
		loanIterator != _loans.end();
		++loanIterator
	)
	{
		p_os << " Loan " << loanIterator->getId()
			<< " | Borrower: " << loanIterator->getBorrower()
			<< " | Principal: " << loanIterator->getPrincipal()
			<< " | Interest: " << loanIterator->getInterest()
			<< " | Debt: " << loanIterator->getDebtBalance()
			<< std::endl;
	}
}

double Bank::_generateLiquidity()
{
	return 10000 + (std::rand() % 40001);
}

std::ostream& operator<<(std::ostream& p_os, const Bank& p_bank)
{
	p_os << "Bank informations : " << std::endl;
	p_os << "Liquidity : " << p_bank._liquidity << std::endl;
	p_os << std::endl;

	p_os << " Customers [" << p_bank._customers.size() << "]:" << std::endl;
	p_bank._getAllCustomers(p_os);

	p_os << " Accounts [" << p_bank._accounts.size() << "]:" << std::endl;
	p_bank._getAllAccounts(p_os);

	p_os << " Loans [" << p_bank._loans.size() << "]:" << std::endl;
	p_bank._getAllLoans(p_os);

	return (p_os);
}
