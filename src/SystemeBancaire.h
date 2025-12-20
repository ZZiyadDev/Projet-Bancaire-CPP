#ifndef SYSTEMEBANCAIRE_H
#define SYSTEMEBANCAIRE_H

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

// Include your headers
#include "Utilisateur.h"
#include "Client.h"
#include "Employe.h"
#include "Compte.h"
#include "BDManager.h"

class SystemeBancaire {
private:
    // POLYMORPHISM: Stores Clients, Employees, and Admins in one list
    std::vector<Utilisateur*> utilisateurs; 
    
    // Global list of accounts (for easy searching)
    std::vector<Compte*> listeComptes; 

    // Internal Helper Methods
    void chargerDonnees(); // Load from DB on startup
    void nettoyerMemoire();
    
    // Authentication & Menus (Moved from main.cpp)
    Utilisateur* authentifier(const std::string& login, const std::string& pass);
    void sessionClient(Client* client);
    void sessionEmploye(Employe* employe);
    
    // Utilities
    std::string genererIdClient();
    void clearScreen();
    void pause();

public:
    SystemeBancaire(); 
    ~SystemeBancaire();

    // The main entry point (The Login Screen)
    void operations(); 
    
    // Admin/creation methods
    void ajouterNouveauClient(std::string nom, std::string prenom, std::string dateN);
    bool ouvrirNouveauCompte(std::string idClient, std::string typeCompte, double depotInitial);

    Compte* trouverCompte(std::string numCompte);
    Client* trouverClient(std::string nom, std::string prenom);
    Client* trouverClientParId(const std::string& idClient);
};

#endif