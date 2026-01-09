#include "CompteProfessionnel.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>

// Constructor
CompteProfessionnel::CompteProfessionnel(const std::string& titulaire, double soldeInitial, double plafond)
    : Compte(titulaire, soldeInitial), plafondDecouvert(plafond) {}

// Withdraw method
void CompteProfessionnel::retirer(double montant) {
    if (montant <= 0) {
        throw MontantInvalide("Le montant du retrait doit être positif.");
    }
    if (montant <= solde + plafondDecouvert) {
        solde -= montant;
        enregistrerOperation("Retrait", montant);
        std::cout << "Retrait de " << montant << " effectué. Nouveau solde: " << solde << std::endl;
    } else {
        throw SoldeInsuffisant("Retrait impossible : dépasse le plafond autorisé !");
    }
}

// Display account info
void CompteProfessionnel::afficherInfo() const {
    afficher(std::cout);
}

void CompteProfessionnel::afficher(std::ostream& os) const {
    os << "[Compte Professionnel] Num: " << numCompte
       << " | Titulaire: " << titulaire
       << " | Solde: " << solde
       << " | Plafond Découvert: " << plafondDecouvert << std::endl;
}

// Destructor
CompteProfessionnel::~CompteProfessionnel() {}


