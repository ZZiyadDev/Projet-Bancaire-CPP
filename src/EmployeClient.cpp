#include "EmployeClient.h"

EmployeClient::EmployeClient(const std::string& nom, const std::string& prenom, double salaire,
                             const std::string& identifiant, const std::string& motDePasse,
                             const std::string& role, const std::string& dateNaissance)
    : Utilisateur(identifiant, motDePasse, nom, prenom), // Initialize virtual base
      Employe(nom, prenom, salaire, identifiant, motDePasse),
      Client(identifiant, motDePasse, nom, prenom, dateNaissance),
      roleSpecifique(role) {}

std::string EmployeClient::getRoleSpecifique() const {
    return roleSpecifique;
}

void EmployeClient::afficherProfil() const {
    std::cout << "\n=== Profil Employe-Client ===\n";
    std::cout << "Nom: " << getNom() << "\n";
    std::cout << "Prenom: " << getPrenom() << "\n";
    std::cout << "Identifiant: " << getIdentifiant() << "\n";
    std::cout << "Salaire: " << getSalaire() << " MAD\n";
    // Now uses getComptes() inherited from Client
    std::cout << "Nombre de comptes personnels: " << getComptes().size() << "\n";
}

std::string EmployeClient::getTypeUtilisateur() const {
    return "EmployeClient";
}
