#include "Transaction.h"

std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    os << "Transaction: Type=" << t.type 
       << ", Date=" << t.date 
       << ", Montant=" << t.montant;
    return os;
}
