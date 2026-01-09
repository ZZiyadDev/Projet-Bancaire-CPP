#include<iostream>
#include "CompteEpargne.h"
#include "BDManager.h"
#include "Exceptions.h"


void CompteEpargne::afficherInfo() const {
    afficher(std::cout);
}

void CompteEpargne::afficher(std::ostream& os) const {
    os << "Compte Epargne - Titulaire: " << titulaire 
       << ", Solde: " << solde 
       << ", Taux d'Interet: " << tauxInteret * 100 << "%" 
       << std::endl;
}

void CompteEpargne::retirer(double montant) {
    if (montant <= 0) {
        throw MontantInvalide("Le montant du retrait doit etre positif.");
    }
    if (montant <= solde) {
        solde -= montant;
        enregistrerOperation("Retrait", montant);
        //modifier la base de donnee
        std::string query = "UPDATE Comptes SET solde = " + std::to_string(solde) + " WHERE numCompte = '" + numCompte + "';";
        BDManager::getInstance()->executeQuery(query);

        
        std::cout << "Retrait de " << montant << " effectue. Nouveau solde: " << solde << std::endl;
    } else {
        throw SoldeInsuffisant("Erreur: Solde insuffisant.");
    }
}

void CompteEpargne::appliquerInteret() {
    double interet = solde * tauxInteret;
    solde += interet;
    std::cout << "Interet de " << interet << "'%' applique. Nouveau solde: " << solde << std::endl;
}