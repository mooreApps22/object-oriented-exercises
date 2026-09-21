#pragma once

#include <vector>

class Bank;

class Customer
{
	private:
		int					_id;
		double				_cash;
		std::vector<int>	_accountIds;

		void				_addCash(double amount);
		bool				_removeCash(double amount);

		void				_addAccount(int accountId);
		void				_removeAccount(int accountId);

		friend class Bank;
	
	public:
		Customer(int id, double startingCash);
		Customer(const Customer &other);
		Customer	&operator=(const Customer &other);
		~Customer();

		int						getId() const;
		double					getCash() const;
		const std::vector<int>	&getAccountIds() const;

};
