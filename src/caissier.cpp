#include "Caissier.h"
#include <iostream>

Caissier::Caissier() noexcept : Employe() {}

Caissier::Caissier(const std::string& nom, const std::string& prenom, double salaire,
                   const std::string& identifiant, const std::string& motDePasse) noexcept
    : Employe(nom, prenom, salaire, identifiant, motDePasse) {}

void Caissier::afficher() const noexcept {
    std::cout << "[Caissier] ";
    Employe::afficher();
}

std::string Caissier::getRole() const noexcept {
    return "Caissier";
}
