#ifndef SYSTEMEBANCAIRE_H
#define SYSTEMEBANCAIRE_H

#include <vector>
#include <iostream>
#include "Client.h"   
#include "Compte.h"   

class SystemeBancaire {
private:
    // Liste globale des clients (Agrégation)
    std::vector<Client> listeClients; 
    
    // Liste globale des comptes (Agrégation - pour la performance)
    std::vector<Compte*> listeComptes; 

public:
    // Constructeur
    SystemeBancaire(); 

    // Méthodes de gestion de haut niveau
    void ajouterNouveauClient(std::string nom, std::string prenom, std::string dateN);
    bool ouvrirNouveauCompte(std::string idClient, std::string typeCompte, double depotInitial);
    Compte* trouverCompte(std::string identifiant);
    Client* trouverClient(std::string nom, std::string prenom);

    // La méthode qui lance l'interface utilisateur
    void operations(); 
};

#endif // SYSTEMEBANCAIRE_H