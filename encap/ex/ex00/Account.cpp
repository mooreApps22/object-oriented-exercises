#include "Account.hpp"

Account::Account() :
	id(-1),
	value(0)
{

}

std::ostream& operator << (std::ostream& p_os, const Account& p_account) {
	p_os << "[ID: " << p_account.id << "] - [Value: " << p_account.value << "]";
	return (p_os);
}
