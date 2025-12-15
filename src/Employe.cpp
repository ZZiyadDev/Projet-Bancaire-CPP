#include "Employe.h"
#include <iostream>

Employe::Employe() noexcept 
    : Utilisateur("", "", "", ""), salaire(0.0) {}

Employe::Employe(const std::string& nom, const std::string& prenom, double salaire,
                 const std::string& identifiant, const std::string& motDePasse) noexcept
    : Utilisateur(identifiant, motDePasse, nom, prenom), salaire(salaire) {}

double Employe::getSalaire() const noexcept {
    return salaire;
}

void Employe::setSalaire(double salaire) noexcept {
    this->salaire = salaire;
}

void Employe::afficher() const noexcept {
    std::cout << "Identifiant: " << identifiant
              << " | Nom: " << nom
              << " | Prenom: " << prenom
              << " | Salaire: " << salaire << std::endl;
}

std::string Employe::getRole() const noexcept {
    return "Employe";
}

void Employe::afficherProfil() const noexcept {
    std::cout << "\n=== Profil Employe ===\n";
    std::cout << "Identifiant: " << identifiant
              << "\nNom: " << nom
              << "\nPrenom: " << prenom
              << "\nRole: " << getRole()
              << "\nSalaire: " << salaire << "\n";
}

std::string Employe::getTypeUtilisateur() const noexcept {
    return "Employe";
}
