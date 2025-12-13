#include "adminIT.h"
#include <iostream>

AdminIT::AdminIT() noexcept : Employe(), niveauAcces("user") {}

AdminIT::AdminIT(int id, const std::string& nom, const std::string& prenom,
                 double salaire, const std::string& niveauAcces) noexcept
    : Employe(id, nom, prenom, salaire), niveauAcces(niveauAcces) {}

const std::string& AdminIT::getNiveauAcces() const noexcept {
    return niveauAcces;
}

void AdminIT::setNiveauAcces(const std::string& niveau) {
    niveauAcces = niveau;
}

void AdminIT::afficher() const noexcept {
    std::cout << "[Admin IT] ";
    Employe::afficher();
    std::cout << "Niveau d'acces: " << niveauAcces << std::endl;
}

std::string AdminIT::getRole() const noexcept {
    return "Admin IT";
}

void AdminIT::creerUtilisateur() const noexcept {
    std::cout << "Utilisateur cree par Admin IT." << std::endl;
}

void AdminIT::supprimerUtilisateur() const noexcept {
    std::cout << "Utilisateur supprime par Admin IT." << std::endl;
}
