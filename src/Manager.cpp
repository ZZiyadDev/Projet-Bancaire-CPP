#include "Manager.h"
#include <iostream>

Manager::Manager() : Employe(), nombreEmployes(0) {}

Manager::Manager(int id, const std::string& nom, const std::string& prenom,
                 double salaire, int nombreEmployes)
    : Employe(id, nom, prenom, salaire), nombreEmployes(nombreEmployes) {}

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
