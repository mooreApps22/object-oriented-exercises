#pragma once

class Bank;

class Loan
{
	private:
		int			_id;
		int			_borrower;
		double		_principal;
		double		_interest;
		double		_debtBalance;

		bool	_makePayment(double amount);

		friend class Bank;
	
	public:
		Loan(
				int id,
				int	borrower,
				double principal,
				double interest
		);
		Loan(const Loan &other);
		Loan &operator=(const Loan &other);
		~Loan();

		int	getId() const;
		int	getBorrower() const;
		double getPrincipal() const;
		double getInterest() const;
		double getDebtBalance() const;
};
