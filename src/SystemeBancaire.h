#ifndef SYSTEMEBANCAIRE_H
#define SYSTEMEBANCAIRE_H

#include <vector>
#include <iostream>
#include <string>
#include "Client.h"
#include "Compte.h"
#include "BDManager.h"

class SystemeBancaire {
private:
    // Liste globale des clients (Agrégation)
    std::vector<Client> listeClients; 
    
    // Liste globale des comptes (Agrégation)
    std::vector<Compte*> listeComptes; 

    // Helper to generate unique IDs
    std::string genererIdClient();

public:
    // Constructeur
    SystemeBancaire(); 

    // Méthodes de gestion de haut niveau
    void ajouterNouveauClient(std::string nom, std::string prenom, std::string dateN);
    bool ouvrirNouveauCompte(std::string idClient, std::string typeCompte, double depotInitial);
    Compte* trouverCompte(std::string numCompte);
    Client* trouverClient(std::string nom, std::string prenom);
 Client* trouverClientParId(const std::string& idClient);

    // La méthode qui lance l'interface utilisateur
    void operations(); 
};

#endif // SYSTEMEBANCAIRE_H
