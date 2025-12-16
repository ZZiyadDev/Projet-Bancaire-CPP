#include "Caissier.h"
#include <iostream>

Caissier::Caissier() : Employe() {}

Caissier::Caissier(const std::string& nom, const std::string& prenom, double salaire,
                   const std::string& identifiant, const std::string& motDePasse)
    : Employe(nom, prenom, salaire, identifiant, motDePasse) {}

void Caissier::afficher() const {
    std::cout << "[Caissier] ";
    Employe::afficher();
}

std::string Caissier::getRole() const {
    return "Caissier";
}
