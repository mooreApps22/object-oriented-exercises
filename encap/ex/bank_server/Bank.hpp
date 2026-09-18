#pragma once
#include <vector>
#include <iostream>
#include "Account.hpp"
#include "Customer.hpp"
#include "Loan.hpp"
#include <string>
#include <cstdlib>
#include <algorithm>

class Bank
{
	private:
		double					_liquidity;
		std::vector<Customer> 	_customers;
		std::vector<Account> 	_accounts;
		std::vector<Loan>		_loans;

		int						_nextAccountId;
		int						_nextLoanId;

		Customer	*_findCustomer(int customerId);
		Account		*_findAccount(int accountId);
		Loan		*_findLoan(int loanId);

		double		_generateCustomerCash();
		void		_removeLoan(int loanId);
		bool		_customerOwnsAccount(
			const Customer &customer,
			int accountId
		) const;
		double		_generateLiquidity();

		void _getAllCustomers(std::ostream &p_os) const;
		void _getAllAccounts(std::ostream &p_os) const;
		void _getAllLoans(std::ostream &p_os) const;

	public:
		Bank();
		~Bank();

		// Customer
		void	addCustomer(int customerId);
		void	removeCustomer(int customerId);

		const Customer	*getCustomer(int customerId) const;

		//Account
		int				createAccount(int customerId,
							const std::string &accountName);
		bool			deleteAccount(int customerId, int accountId);
		bool			modifyAccountName(int customerId,
							int accountId,
							const std::string &accountName);
		const Account	*getAccount(int accountId) const;

		//Loan
		bool	makeLoanPayment(int loanId, double amount);

		//Business
		bool	deposit(
			int customerId,
			int accountId,
			double amount
		);

		bool	withdraw(
			int customerId,
			int accountId,
			double amount
		);

		int	applyForLoan(
			int customerId,
			int accountId,
			double principal
		);

	friend std::ostream& operator<<(std::ostream& p_os, const Bank& p_bank);
};
