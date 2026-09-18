#include "Bank.hpp"
#include <algorithm>


Bank::Bank()
	:	_liquidity(_generateLiquidity()),
		_nextAccountId(1000),
		_nextLoanId(5000)
{
}

Bank::~Bank()
{
}

// CUSTOMER METHODS

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

// ACCOUNT METHODS

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

// LOAN METHODS

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

// BUSINESS

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


// OS METHODS

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
