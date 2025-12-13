#include "Caissier.h"
#include <iostream>

Caissier::Caissier() : Employe() {}

Caissier::Caissier(int id, const std::string& nom, const std::string& prenom, double salaire)
    : Employe(id, nom, prenom, salaire) {}

void Caissier::afficher() const {
    std::cout << "[Caissier] ";
    Employe::afficher();
}

std::string Caissier::getRole() const {
    return "Caissier";
}
