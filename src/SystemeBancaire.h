#ifndef SYSTEMEBANCAIRE_H
#define SYSTEMEBANCAIRE_H

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory> // Include for std::unique_ptr

// Include your headers
#include "Utilisateur.h"
#include "Client.h"
#include "Employe.h"
#include "Compte.h"
#include "Manager.h"
#include "BDManager.h"

class AdminIT; // Forward declaration

class SystemeBancaire {
private:
    // POLYMORPHISM: Stores Clients, Employees, and Admins using smart pointers
    std::vector<std::unique_ptr<Utilisateur>> utilisateurs; 
    
    // Global list of accounts (for easy searching), managed by smart pointers
    std::vector<std::unique_ptr<Compte>> listeComptes; 

    // Internal Helper Methods
    void chargerDonnees(); // Load from DB on startup
    
    // Authentication & Menus
    Utilisateur* authentifier(const std::string& login, const std::string& pass);
    void sessionClient(Client* client);
    void sessionEmploye(Employe* employe);
    void sessionManager(Manager* manager);
    void sessionAdmin(AdminIT* admin);
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

    // Finder methods now return raw pointers for observation (non-owning)
    Compte* trouverCompte(std::string numCompte);
    Client* trouverClient(std::string nom, std::string prenom);
    Client* trouverClientParId(const std::string& idClient);
};

#endif