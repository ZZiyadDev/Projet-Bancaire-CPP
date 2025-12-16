#include "CompteProfessionnel.h"
#include <iostream>
#include <iomanip>

// Constructor
CompteProfessionnel::CompteProfessionnel(const std::string& titulaire, double soldeInitial, double plafond)
    : Compte(titulaire, soldeInitial), plafondDecouvert(plafond) {}

// Withdraw method
bool CompteProfessionnel::retirer(double montant) {
    if (montant <= solde + plafondDecouvert) {
        solde -= montant;
        enregistrerOperation("Retrait", montant);
        std::cout << "Retrait de " << montant << " effectué. Nouveau solde: " << solde << std::endl;
        return true;
    } else {
        std::cout << "Retrait impossible : dépasse le plafond autorisé !" << std::endl;
        return false;
    }
}

// Display account info
void CompteProfessionnel::afficherInfo() const {
    std::cout << "[Compte Professionnel] Num: " << numCompte
              << " | Titulaire: " << titulaire
              << " | Solde: " << solde
              << " | Plafond Découvert: " << plafondDecouvert << std::endl;
}

// Destructor
CompteProfessionnel::~CompteProfessionnel() {}


