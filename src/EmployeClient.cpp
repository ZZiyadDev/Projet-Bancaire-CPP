#include "EmployeClient.h"

EmployeClient::EmployeClient() : Employe() {}

EmployeClient::EmployeClient(const std::string& nom, const std::string& prenom, double salaire,
                             const std::string& identifiant, const std::string& motDePasse,
                             const std::string& role)
    : Employe(nom, prenom, salaire, identifiant, motDePasse), roleSpecifique(role) {}

void EmployeClient::ajouterCompte(Compte* c) {
    comptes.push_back(c);
}

const std::vector<Compte*>& EmployeClient::getComptes() const {
    return comptes;
}

std::string EmployeClient::getRoleSpecifique() const {
    return roleSpecifique;
}

void EmployeClient::afficherProfil() const {
    std::cout << "\n=== Profil Employe-Client ===\n";
    std::cout << "Nom: " << nom << "\n";
    std::cout << "Prenom: " << prenom << "\n";
    std::cout << "Identifiant: " << identifiant << "\n";
    std::cout << "Salaire: " << salaire << " MAD\n";
    std::cout << "Nombre de comptes personnels: " << comptes.size() << "\n";
}

std::string EmployeClient::getTypeUtilisateur() const {
    return "EmployeClient";
}
