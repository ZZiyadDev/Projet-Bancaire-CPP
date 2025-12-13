#include "adminIT.h"
#include <iostream>

AdminIT::AdminIT() : Employe(), niveauAcces("user") {}

AdminIT::AdminIT(int id, const std::string& nom, const std::string& prenom,
                 double salaire, const std::string& niveauAcces)
    : Employe(id, nom, prenom, salaire), niveauAcces(niveauAcces) {}

std::string AdminIT::getNiveauAcces() const {
    return niveauAcces;
}

void AdminIT::setNiveauAcces(const std::string& niveau) {
    niveauAcces = niveau;
}

void AdminIT::afficher() const {
    std::cout << "[Admin IT] ";
    Employe::afficher();
    std::cout << "Niveau d'acces: " << niveauAcces << std::endl;
}

std::string AdminIT::getRole() const {
    return "Admin IT";
}

void AdminIT::creerUtilisateur() const {
    std::cout << "Utilisateur cree par Admin IT." << std::endl;
}

void AdminIT::supprimerUtilisateur() const {
    std::cout << "Utilisateur supprime par Admin IT." << std::endl;
}
