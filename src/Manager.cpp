#include "Manager.h"
#include <iostream>

Manager::Manager() : Employe(), nombreEmployes(0) {}

Manager::Manager(const std::string& nom, const std::string& prenom,
                 double salaire, int nombreEmployes,
                 const std::string& identifiant, const std::string& motDePasse)
    : Utilisateur(identifiant, motDePasse, nom, prenom),
      Employe(nom, prenom, salaire, identifiant, motDePasse),
      nombreEmployes(nombreEmployes) {}

int Manager::getNombreEmployes() const {
    return nombreEmployes;
}

void Manager::setNombreEmployes(int nombre) {
    nombreEmployes = nombre;
}

void Manager::afficher() const {
    std::cout << "[Manager] ";
    Employe::afficher();
    std::cout << "Employes geres: " << nombreEmployes << std::endl;
}

std::string Manager::getRole() const {
    return "Manager";
}
