#pragma once
#include <iostream>
#include <string>

class Bank;

class Account
{
	private:
		int			_id;
		std::string	_name;
		int			_balance;

		void	_setName(const std::string &name);
		void	_addFunds(double amount);
		bool	_removeFunds(double amount);

		friend class Bank;

	public:
		Account(int id, const std::string &name);
		Account(const Account &other);
		Account &operator=(const Account &other);
		~Account();

		int					getId() const;
		const std::string	&getName() const;
		double				getBalance() const;

	friend std::ostream& operator<<(std::ostream& p_os, const Account& p_account);
};
