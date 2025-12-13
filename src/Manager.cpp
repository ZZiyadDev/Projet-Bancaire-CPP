#include "Manager.h"
#include <iostream>

Manager::Manager() noexcept : Employe(), nombreEmployes(0) {}

Manager::Manager(int id, const std::string& nom, const std::string& prenom,
                 double salaire, int nombreEmployes) noexcept
    : Employe(id, nom, prenom, salaire), nombreEmployes(nombreEmployes) {}

int Manager::getNombreEmployes() const noexcept {
    return nombreEmployes;
}

void Manager::setNombreEmployes(int nombre) noexcept {
    nombreEmployes = nombre;
}

void Manager::afficher() const noexcept {
    std::cout << "[Manager] ";
    Employe::afficher();
    std::cout << "Employes geres: " << nombreEmployes << std::endl;
}

std::string Manager::getRole() const noexcept {
    return "Manager";
}
