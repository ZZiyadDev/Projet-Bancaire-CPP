#include "CompteCourant.h"
#include "BDManager.h"
#include "Exceptions.h"
#include <iostream>

    void CompteCourant::afficherInfo() const  {
    std::cout << "Compte Courant - Titulaire: " << titulaire 
              << ", Solde: " << solde 
              << ", Decouvert Autorise: " << decouvertAutorise 
              << std::endl;
    }
    
    void CompteCourant::retirer(double montant) {
        if (montant <= 0) {
            throw MontantInvalide("Le montant du retrait doit être positif.");
        }
        if(solde + decouvertAutorise >= montant){
            solde -= montant;
            enregistrerOperation("Retrait", montant);
            //modifier base de donnee
            std::string query = "UPDATE Comptes SET solde = " + std::to_string(solde) + " WHERE numCompte = '" + numCompte + "';";
            BDManager::getInstance()->executeQuery(query);

            std::cout << "Retrait de " << montant << " effectue. Nouveau solde: " << solde << std::endl;
        }
        else{
            throw SoldeInsuffisant("Erreur: Solde insuffisant (depasse le decouvert).");
        }
    }


