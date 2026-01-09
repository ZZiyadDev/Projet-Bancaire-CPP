#include "Utilisateur.h"

std::ostream& operator<<(std::ostream& os, const Utilisateur& u) {
    u.afficher(os);
    return os;
}
