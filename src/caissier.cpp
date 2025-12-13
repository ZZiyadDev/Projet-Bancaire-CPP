#include "Caissier.h"
#include <iostream>

Caissier::Caissier() noexcept : Employe() {}

Caissier::Caissier(int id, const std::string& nom, const std::string& prenom, double salaire) noexcept
    : Employe(id, nom, prenom, salaire) {}

void Caissier::afficher() const noexcept {
    std::cout << "[Caissier] ";
    Employe::afficher();
}

std::string Caissier::getRole() const noexcept {
    return "Caissier";
}
