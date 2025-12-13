#include "Employe.h"
#include <iostream>

Employe::Employe() : id(0), nom(""), prenom(""), salaire(0.0) {}

Employe::Employe(int id, const std::string& nom, const std::string& prenom, double salaire)
    : id(id), nom(nom), prenom(prenom), salaire(salaire) {}

int Employe::getId() const {
    return id;
}

std::string Employe::getNom() const {
    return nom;
}

std::string Employe::getPrenom() const {
    return prenom;
}

double Employe::getSalaire() const {
    return salaire;
}

void Employe::setSalaire(double salaire) {
    this->salaire = salaire;
}

void Employe::afficher() const {
    std::cout << "ID: " << id
              << " | Nom: " << nom
              << " | Prenom: " << prenom
              << " | Salaire: " << salaire << std::endl;
}

std::string Employe::getRole() const {
    return "Employe";
}

Employe::~Employe() {}
