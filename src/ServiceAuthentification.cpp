#include "ServiceAuthentification.h"

Utilisateur* ServiceAuthentification::authentifier(const std::string& login, const std::string& pass, const std::vector<std::unique_ptr<Utilisateur>>& utilisateurs) {
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == login && u->verifierMotDePasse(pass)) {
            return u.get();
        }
    }
    return nullptr;
}
