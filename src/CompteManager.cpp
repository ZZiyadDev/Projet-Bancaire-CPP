#include "CompteManager.h"
#include "BDManager.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include "EmployeClient.h"
#include <iostream>
#include <ctime>

using namespace std;

CompteManager::CompteManager() {
    cout << "[CompteManager] Initialisation..." << endl;
}

void CompteManager::chargerDonnees(UtilisateurManager& utilisateurManager) {
    cout << "[CompteManager] Chargement des comptes..." << endl;
    sqlite3* db = BDManager::getInstance()->getConnection();
    sqlite3_stmt* stmt = nullptr;

    const char* qComptes = "SELECT numCompte, userId, typeCompte, solde, taux, decouvert FROM Comptes";
    if (sqlite3_prepare_v2(db, qComptes, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string num = (const char*)sqlite3_column_text(stmt, 0);
            string userId = (const char*)sqlite3_column_text(stmt, 1);
            string type = (const char*)sqlite3_column_text(stmt, 2);
            double solde = sqlite3_column_double(stmt, 3);

            Client* proprietaire = utilisateurManager.getClientById(userId);
            if (!proprietaire) {
                // Maybe it's an EmployeClient
                Utilisateur* user = utilisateurManager.getUtilisateur(userId);
                if (auto* ec = dynamic_cast<EmployeClient*>(user)) {
                    proprietaire = ec;
                } else {
                    continue; // Skip if no owner found
                }
            }

            string nomTitulaire = proprietaire->getPrenom() + " " + proprietaire->getNom();
            unique_ptr<Compte> compte = nullptr;

            if (type == "Epargne") {
                double taux = sqlite3_column_double(stmt, 4);
                compte = make_unique<CompteEpargne>(nomTitulaire, solde, taux);
            } else {
                double dec = sqlite3_column_double(stmt, 5);
                compte = make_unique<CompteCourant>(nomTitulaire, solde, dec);
            }

            if (compte) {
                compte->setNumCompte(num);
                proprietaire->ajouterCompte(compte.get());
                listeComptes.push_back(move(compte));
            }
        }
        sqlite3_finalize(stmt);
    } else {
        cout << "[CompteManager] Erreur lors de la lecture des comptes." << endl;
    }
}

Compte* CompteManager::trouverCompte(string numCompte) {
    for (const auto& compte : listeComptes) {
        if (compte->getNumCompte() == numCompte) {
            return compte.get();
        }
    }
    return nullptr;
}

bool CompteManager::ouvrirNouveauCompte(string idClient, string typeCompte, double depotInitial, UtilisateurManager& utilisateurManager) {
    Client* target = utilisateurManager.getClientById(idClient);
    if (!target) {
        cout << "Client introuvable.\n";
        return false;
    }

    string nomTitulaire = target->getPrenom() + " " + target->getNom();
    unique_ptr<Compte> compte = nullptr;

    if (typeCompte == "Epargne") 
        compte = make_unique<CompteEpargne>(nomTitulaire, depotInitial, 0.05);
    else if (typeCompte == "Courant")
        compte = make_unique<CompteCourant>(nomTitulaire, depotInitial, 200.0);
    else {
        cout << "Type de compte invalide.\n";
        return false;
    }

    if (compte) {
        string uniqueID = "CPT" + to_string(time(nullptr)) + to_string(rand() % 1000);
        compte->setNumCompte(uniqueID);

        string q = "INSERT INTO Comptes (numCompte, userId, typeCompte, solde, taux, decouvert) VALUES (?, ?, ?, ?, ?, ?);";
        
        double taux = (typeCompte == "Epargne") ? 0.05 : 0.0;
        double decouvert = (typeCompte == "Courant") ? 200.0 : 0.0;

        vector<string> params = {
            uniqueID,
            idClient,
            typeCompte,
            to_string(depotInitial),
            to_string(taux),
            to_string(decouvert)
        };
        
        if (BDManager::getInstance()->executePrepared(q, params)) {
            target->ajouterCompte(compte.get());
            listeComptes.push_back(move(compte));
            cout << "Compte cree avec succes (ID: " << uniqueID << ")\n";
            return true;
        } else {
            cout << "Erreur SQL : Impossible de creer le compte.\n";
            return false;
        }
    }
    return false;
}

const std::vector<std::unique_ptr<Compte>>& CompteManager::getAllComptes() const {
    return listeComptes;
}
