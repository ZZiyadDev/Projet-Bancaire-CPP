#include<iostream>
#include "CompteEpargne.h"


void CompteEpargne::afficherInfo() const {
    std::cout << "Compte Epargne - Titulaire: " << titulaire 
              << ", Solde: " << solde 
              << ", Taux d'Interet: " << tauxInteret * 100 << "%" 
              << std::endl;
}

bool CompteEpargne::retirer(double montant) {
    if (montant <= solde) {
        solde -= montant;
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