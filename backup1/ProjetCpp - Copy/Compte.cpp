#include "Compte.h"

void Compte::deposer(double montant) {
    if (montant > 0) {
        solde += montant;
        std::cout << "Déposé: " << montant << ", Nouveau solde: " << solde << std::endl;
    } else {
        std::cout << "Montant de dépôt invalide." << std::endl;
    }
}

bool Compte::virementVers(Compte& destinataire, double montant) {
    if (this->retirer(montant) == true) {
        destinataire.deposer(montant);
        std::cout << "Virement effectue avec succes." << std::endl;
        return true;
    }
    else {
        std::cout << "Virement echoue." << std::endl;
        return false;
    }
}