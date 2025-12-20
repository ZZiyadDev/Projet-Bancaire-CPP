#include "CompteCourant.h"
#include "BDManager.h"
#include <iostream>

    void CompteCourant::afficherInfo() const  {
    std::cout << "Compte Courant - Titulaire: " << titulaire 
              << ", Solde: " << solde 
              << ", Découvert Autorisé: " << decouvertAutorise 
              << std::endl;
    }
    
    bool CompteCourant::retirer(double montant) {
        if(solde+decouvertAutorise >=montant){
            solde-=montant;
            enregistrerOperation("Retrait", montant);
            //modifier base de donnee
            std::string query = "UPDATE Comptes SET solde = " + std::to_string(solde) + " WHERE numCompte = '" + numCompte + "';";
            BDManager::getInstance()->executeQuery(query);

            std::cout << "Retrait de " << montant << " effectue. Nouveau solde: " << solde << std::endl;
            return true;
        }
        else{
            std::cout << "Erreur: Solde insuffisant (depasse le decouvert)." << std::endl;
            return false;
        }

    }


