#pragma once
#include "Utilisateur.h"

class Admin : public Utilisateur {
private:
    std::string nom;
    std::string prenom;

public:
    Admin(const std::string& id,
          const std::string& mdp,
          const std::string& n,
          const std::string& p)
        : Utilisateur(id, mdp), nom(n), prenom(p) {}

    void afficherProfil() const override {
        std::cout << "\n=== Profil Administrateur ===\n";
        std::cout << "Nom: " << nom << "\nPrenom: " << prenom
                  << "\nIdentifiant: " << identifiant << "\n";
    }
};
