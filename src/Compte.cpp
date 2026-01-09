#include "Compte.h"
#include "BDManager.h"
#include "Exceptions.h"
#include <ctime>
#include <sstream>




void Compte::enregistrerOperation(std::string type, double montant) {
        // 1. Récupérer la date actuelle automatiquement
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        
        std::stringstream dateStream;
        dateStream << (now->tm_year + 1900) << '-' 
                   << (now->tm_mon + 1) << '-'
                   <<  now->tm_mday;
        
        std::string dateStr = dateStream.str();           
        // 2. Créer et ajouter la transaction
        historique.push_back(Transaction(type, dateStream.str(), montant));
        
        //3. modifier la base de donnee
        std::string query = "INSERT INTO Transactions (numCompte, typeOperation, montant, date) VALUES ('"
                        + numCompte + "', '" + type + "', " + std::to_string(montant) + ", '" + dateStr + "');";
    
    BDManager::getInstance()->executeQuery(query);
    }

void Compte::deposer(double montant) {
    if (montant > 0) {
        solde += montant;
        enregistrerOperation("Depot", montant);
        std::cout << "Depose: " << montant << ", Nouveau solde: " << solde << std::endl;
        std::string query = "UPDATE Comptes SET solde = " + std::to_string(solde) 
                          + " WHERE numCompte = '" + numCompte + "';";
        
        BDManager::getInstance()->executeQuery(query);
    } else {
        throw MontantInvalide("Le montant du depot doit etre positif.");
    }
}

void Compte::virementVers(Compte& destinataire, double montant) {
    if (montant <= 0) {
        throw MontantInvalide("Le montant du virement doit etre positif.");
    }
    retirer(montant);
    destinataire.deposer(montant);
    std::cout << "Virement effectue avec succes." << std::endl;
}    

void Compte::afficherHistorique() const {
        std::cout << "\n--- Historique du compte de " << titulaire << " ---" << std::endl;
        for (const auto& transaction : historique) {
            transaction.afficherDetails();
        }
        std::cout << "-------------------------------------------" << std::endl;
    }

// Implementations for operator overloading

// Surcharge operator<<
std::ostream& operator<<(std::ostream& os, const Compte& c) {
    c.afficher(os);
    return os;
}

// Surcharge operator+=
Compte& Compte::operator+=(double montant) {
    deposer(montant);
    return *this;
}

// Surcharge operator-=
Compte& Compte::operator-=(double montant) {
    retirer(montant);
    return *this;
}

// Surcharge operator==
bool Compte::operator==(const Compte& autre) const {
    return this->numCompte == autre.numCompte;
}              