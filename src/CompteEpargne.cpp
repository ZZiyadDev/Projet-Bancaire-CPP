#include<iostream>
#include "CompteEpargne.h"
#include "BDManager.h"


void CompteEpargne::afficherInfo() const {
    std::cout << "Compte Epargne - Titulaire: " << titulaire 
              << ", Solde: " << solde 
              << ", Taux d'Interet: " << tauxInteret * 100 << "%" 
              << std::endl;
}

bool CompteEpargne::retirer(double montant) {
    if (montant <= solde) {
        solde -= montant;
        enregistrerOperation("Retrait", montant);
        //modifier la base de donnee
        std::string query = "UPDATE Comptes SET solde = " + std::to_string(solde) + " WHERE numCompte = '" + numCompte + "';";
        BDManager::getInstance()->executeQuery(query);

        
        std::cout << "Retrait de " << montant << " effectue. Nouveau solde: " << solde << std::endl;
        return true;
    } else {
        std::cout << "Erreur: Solde insuffisant." << std::endl;
        return false;
    }
}

void CompteEpargne::appliquerInteret() {
    double interet = solde * tauxInteret;
    solde += interet;
    std::cout << "Interet de " << interet << "'%' applique. Nouveau solde: " << solde << std::endl;
}