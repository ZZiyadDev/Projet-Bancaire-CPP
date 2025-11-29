#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Utilisateur.h"
#include "Compte.h"

class Client : public Utilisateur {
private:
    std::string nom;
    std::string prenom;
    std::string dateNaissance;
    std::vector<Compte*> comptes;

public:
    Client(const std::string& id,
           const std::string& mdp,
           const std::string& n,
           const std::string& p,
           const std::string& dn)
        : Utilisateur(id, mdp), nom(n), prenom(p), dateNaissance(dn) {}
    
    std::string getTypeUtilisateur() const override {
        return "Client";
    }

    // Gestion des comptes
    void ajouterCompte(Compte* c) {
        comptes.push_back(c);
    }

    const std::vector<Compte*>& getComptes() const {
        return comptes;
    }

    // Affichage du profil client
    void afficherProfil() const override {
        std::cout << "\n=== Profil Client ===\n";
        std::cout << "Nom: " << nom
                  << "\nPrenom: " << prenom
                  << "\nDate de naissance: " << dateNaissance
                  << "\nIdentifiant: " << identifiant
                  << "\nNombre de comptes: " << comptes.size()
                  << "\n";
    }

    ~Client() {
        for (Compte* c : comptes)
            delete c;
    }
};
