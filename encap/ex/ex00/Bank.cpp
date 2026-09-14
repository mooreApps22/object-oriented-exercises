#include "Bank.hpp"


Bank::Bank() : liquidity(0) {}

std::ostream& operator<<(std::ostream& p_os, const Bank& p_bank)
{
	p_os << "Bank informations : " << std::endl;
	p_os << "Liquidity : " << p_bank.liquidity << std::endl;
	for (std::vector<Account *>::const_iterator it =  p_bank.clientAccounts.begin();
		it != p_bank.clientAccounts.end(); 
		it++) {
		p_os << (*it)->id << std::endl;
	}
	return (p_os);
}
