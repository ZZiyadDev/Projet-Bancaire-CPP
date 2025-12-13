#include "Employe.h"
#include <iostream>

Employe::Employe() noexcept : id(0), nom(""), prenom(""), salaire(0.0) {}

Employe::Employe(int id, const std::string& nom, const std::string& prenom, double salaire) noexcept
    : id(id), nom(nom), prenom(prenom), salaire(salaire) {}

int Employe::getId() const noexcept {
    return id;
}

const std::string& Employe::getNom() const noexcept {
    return nom;
}

const std::string& Employe::getPrenom() const noexcept {
    return prenom;
}

double Employe::getSalaire() const noexcept {
    return salaire;
}

void Employe::setSalaire(double salaire) noexcept {
    this->salaire = salaire;
}

void Employe::afficher() const noexcept {
    std::cout << "ID: " << id
              << " | Nom: " << nom
              << " | Prenom: " << prenom
              << " | Salaire: " << salaire << std::endl;
}

std::string Employe::getRole() const noexcept {
    return "Employe";
}
