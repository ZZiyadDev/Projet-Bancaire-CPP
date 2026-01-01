#include "UtilisateurManager.h"
#include "BDManager.h"
#include "AdminIT.h"
#include "Manager.h"
#include "Caissier.h"
#include "EmployeClient.h"
#include <iostream>
#include <algorithm>

using namespace std;

UtilisateurManager::UtilisateurManager() {
    cout << "[UtilisateurManager] Initialisation..." << endl;
    chargerDonnees();
}

void UtilisateurManager::chargerDonnees() {
    cout << "[UtilisateurManager] Chargement des utilisateurs..." << endl;
    sqlite3* db = BDManager::getInstance()->getConnection();
    sqlite3_stmt* stmt = nullptr;

    const char* qUsers = "SELECT id, nom, prenom, type, mdp, dateNaissance, salaire, role FROM Users";
    if (sqlite3_prepare_v2(db, qUsers, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string id     = (const char*)sqlite3_column_text(stmt, 0);
            string nom    = (const char*)sqlite3_column_text(stmt, 1);
            string prenom = (const char*)sqlite3_column_text(stmt, 2);
            string type   = (const char*)sqlite3_column_text(stmt, 3);
            string mdp    = (const char*)sqlite3_column_text(stmt, 4);

            string role = "";
            if (type != "Client") {
                const char* roleTxt = (const char*)sqlite3_column_text(stmt, 7);
                role = roleTxt ? roleTxt : "";
            }

            cout << "[LOAD] Processing user ID: " << id << ", Type: " << type << ", Role: " << role << endl;

            if (type == "Client") {
                const char* dN = (const char*)sqlite3_column_text(stmt, 5);
                string dateN = dN ? dN : "Inconnue";
                utilisateurs.push_back(make_unique<Client>(id, mdp, nom, prenom, dateN));
                cout << "  -> Created Client" << endl;
                continue;
            }
            
            if (type == "EmployeClient") {
                double salaire = sqlite3_column_double(stmt, 6);
                const char* dN = (const char*)sqlite3_column_text(stmt, 5);
                string dateN = dN ? dN : "Inconnue";
                utilisateurs.push_back(std::make_unique<EmployeClient>(nom, prenom, salaire, id, mdp, role, dateN));
                cout << "  -> Created EmployeClient" << endl;
                continue;
            }

            if (type == "Employe") {
                double salaire = sqlite3_column_double(stmt, 6);
                if (role == "Admin") {
                    utilisateurs.push_back(make_unique<AdminIT>(nom, prenom, salaire, "SuperUser", id, mdp));
                    cout << "  -> Created AdminIT" << endl;
                } else if (role == "Manager") {
                    utilisateurs.push_back(make_unique<Manager>(nom, prenom, salaire, 0, id, mdp));
                    cout << "  -> Created Manager" << endl;
                } else if (role == "Caissier") {
                    utilisateurs.push_back(make_unique<Caissier>(nom, prenom, salaire, id, mdp));
                    cout << "  -> Created Caissier" << endl;
                } else {
                    utilisateurs.push_back(make_unique<Employe>(nom, prenom, salaire, id, mdp));
                    cout << "  -> Created generic Employe" << endl;
                }
                continue;
            }
            
            cout << "[ERROR] Unhandled user type in chargerDonnees: " << type << endl;
        }
        sqlite3_finalize(stmt);
    } else {
        cout << "[UtilisateurManager] Erreur lors de la lecture des utilisateurs." << endl;
    }

    cout << "[DEBUG] Utilisateurs loaded from DB (count: " << utilisateurs.size() << "):" << endl;
    for (const auto& u : utilisateurs) {
        cout << "[DEBUG]   - ID: " << u->getIdentifiant() << ", Type: " << u->getTypeUtilisateur() << endl;
    }

    bool adminExists = false;
    bool managerExists = false;
    bool caissierExists = false;

    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == "EMP001") adminExists = true;
        if (u->getIdentifiant() == "MNG001") managerExists = true;
        if (u->getIdentifiant() == "CAIS001") caissierExists = true;
    }

    if (!adminExists) {
        cout << "[UtilisateurManager] Admin par defaut non trouve. Creation..." << endl;
        auto admin = make_unique<AdminIT>("Admin", "Root", 20000.0, "RootAccess", "EMP001", "admin");
        BDManager::getInstance()->executeQuery("INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) VALUES ('EMP001', 'admin', 'Admin', 'Root', 'Employe', 20000, 'Admin');");
        utilisateurs.push_back(move(admin));
    }

    if (!managerExists) {
        cout << "[UtilisateurManager] Manager par defaut non trouve. Creation..." << endl;
        auto manager = make_unique<Manager>("Martin", "Sophie", 15000.0, 0, "MNG001", "manager");
        BDManager::getInstance()->executeQuery("INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) VALUES ('MNG001', 'manager', 'Martin', 'Sophie', 'Employe', 15000, 'Manager');");
        utilisateurs.push_back(move(manager));
    }

    if (!caissierExists) {
        cout << "[UtilisateurManager] Caissier par defaut non trouve. Creation..." << endl;
        auto caissier = make_unique<Caissier>("Dupont", "Jean", 2500.0, "CAIS001", "caissier");
        BDManager::getInstance()->executeQuery("INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) VALUES ('CAIS001', 'caissier', 'Dupont', 'Jean', 'Employe', 2500, 'Caissier');");
        utilisateurs.push_back(move(caissier));
    }
}

Utilisateur* UtilisateurManager::getUtilisateur(const std::string& id) {
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == id) {
            return u.get();
        }
    }
    return nullptr;
}

Client* UtilisateurManager::getClient(const std::string& nom, const std::string& prenom) {
    for (const auto& u : utilisateurs) {
        if (u->getNom() == nom && u->getPrenom() == prenom) {
            return dynamic_cast<Client*>(u.get());
        }
    }
    return nullptr;
}

Client* UtilisateurManager::getClientById(const std::string& id) {
    Utilisateur* user = getUtilisateur(id);
    return dynamic_cast<Client*>(user);
}

const std::vector<std::unique_ptr<Utilisateur>>& UtilisateurManager::getAllUtilisateurs() const {
    return utilisateurs;
}

std::string UtilisateurManager::genererIdClient() {
    return "CLI" + to_string(utilisateurs.size() + 100); // More robust generation
}

void UtilisateurManager::ajouterNouveauClient(string nom, string prenom, string dateN) {
    string id = genererIdClient();
    string defaultPassword = "1234";
    
    string query = "INSERT INTO Users (id, mdp, nom, prenom, type, dateNaissance) VALUES (?, ?, ?, ?, 'Client', ?);";
    vector<string> params = {id, defaultPassword, nom, prenom, dateN};

    if (BDManager::getInstance()->executePrepared(query, params)) {
        auto newClient = make_unique<Client>(id, defaultPassword, nom, prenom, dateN);
        utilisateurs.push_back(move(newClient));
        cout << "Client cree avec succes. ID : " << id << endl;
    } else {
        cout << "Erreur SQL: Impossible de creer le client." << endl;
    }
}

bool UtilisateurManager::supprimerUtilisateur(const std::string& id) {
    // First, delete the user's accounts from the Comptes table
    string deleteAccountsQuery = "DELETE FROM Comptes WHERE client_id = ?;";
    vector<string> deleteAccountsParams = {id};
    if (!BDManager::getInstance()->executePrepared(deleteAccountsQuery, deleteAccountsParams)) {
        cout << "Erreur SQL: impossible de supprimer les comptes de l'utilisateur de la DB" << endl;
        // We can choose to continue even if the user has no accounts
    }

    // Then, delete the user from the Users table
    string deleteUserQuery = "DELETE FROM Users WHERE id = ?;";
    vector<string> deleteUserParams = {id};
    if (!BDManager::getInstance()->executePrepared(deleteUserQuery, deleteUserParams)) {
        cout << "Erreur SQL: impossible de supprimer l'utilisateur de la DB" << endl;
        return false;
    }

    auto it = std::remove_if(utilisateurs.begin(), utilisateurs.end(),
        [&](const std::unique_ptr<Utilisateur>& u) {
            return u->getIdentifiant() == id;
        });

    if (it != utilisateurs.end()) {
        utilisateurs.erase(it, utilisateurs.end());
        return true;
    }
    return false;
}

void UtilisateurManager::ajouterUtilisateur(std::unique_ptr<Utilisateur> utilisateur) {
    if (utilisateur) {
        utilisateurs.push_back(move(utilisateur));
    }
}
