#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "sqlite3.h"

class BDManager {
private:
    sqlite3* db;
    static BDManager* instance;

    //Constructor privee (Pattern Singleton)
    BDManager() {
        int rc = sqlite3_open("banque.db", &db);
        if (rc) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Connected to SQLite database successfully." << std::endl;
        }
    }

public:
    // Getter de l'instance de la classe
    static BDManager* getInstance() {
        if (!instance) {
            instance = new BDManager();
        }
        return instance;
    }

    // Helper to execute simple queries (INSERT, UPDATE, DELETE, CREATE)
    bool executeQuery(const std::string& query) {
        char* zErrMsg = 0;
        int rc = sqlite3_exec(db, query.c_str(), 0, 0, &zErrMsg);
        
        if (rc != SQLITE_OK) {
            std::cerr << "SQL Error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }

    // Executes a prepared statement with bound parameters to prevent SQL injection
    bool executePrepared(const std::string& query, const std::vector<std::string>& params) {
        sqlite3_stmt* stmt = nullptr;
        // 1. Prepare the statement
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL Error (prepare): " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        // 2. Bind parameters
        for (size_t i = 0; i < params.size(); ++i) {
            // Bind value at i-th position to the (i+1)-th '?' in the query
            if (sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
                std::cerr << "SQL Error (bind): " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }

        // 3. Execute the statement
        int rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            // This is expected for SELECT, but for INSERT/UPDATE/DELETE it's an error
            std::cerr << "SQL Error (step): " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }

        // 4. Finalize
        sqlite3_finalize(stmt);
        return true;
    }

    // Get the raw connection (useful for advanced fetching later)
    sqlite3* getConnection() {
        return db;
    }

    // Creates the necessary tables if they don't exist
    bool setupTables() {
        // 1. Table USERS (Stores Clients, Employees, and Admins)
        // We merge fields from all child classes into one table for simplicity.
        // 'type' will be "Client", "Employe", etc.
        std::string createUsers = 
            "CREATE TABLE IF NOT EXISTS Users ("
            "id TEXT PRIMARY KEY, "
            "mdp TEXT NOT NULL, "
            "nom TEXT, "
            "prenom TEXT, "
            "type TEXT, "           // Discriminiator: 'Client', 'Employe'
            "dateNaissance TEXT, "  // Specific to Client
            "salaire REAL, "        // Specific to Employe
            "role TEXT"             // Specific to Employe (Manager, Caissier)
            ");";

        // 2. Table COMPTES
        // Links to a User via userId.
        std::string createComptes = 
            "CREATE TABLE IF NOT EXISTS Comptes ("
            "numCompte TEXT PRIMARY KEY, "
            "userId TEXT, "
            "typeCompte TEXT, "     // 'Epargne', 'Courant'
            "solde REAL, "
            "taux REAL, "           // Specific to CompteEpargne
            "decouvert REAL, "      // Specific to CompteCourant
            "FOREIGN KEY(userId) REFERENCES Users(id)"
            ");";

        // 3. Table TRANSACTIONS
        // Links to a Compte via numCompte
        std::string createTransactions = 
            "CREATE TABLE IF NOT EXISTS Transactions ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "numCompte TEXT, "
            "typeOperation TEXT, "  // 'Depot', 'Retrait', 'Virement'
            "montant REAL, "
            "date TEXT, "
            "FOREIGN KEY(numCompte) REFERENCES Comptes(numCompte)"
            ");";

        // Execute all queries
        bool success = true;
        if (!executeQuery(createUsers)) success = false;
        if (!executeQuery(createComptes)) success = false;
        if (!executeQuery(createTransactions)) success = false;

        if (success) {
            std::cout << "[DB] Tables verified/created successfully." << std::endl;
        } else {
            std::cerr << "[DB] Error creating tables." << std::endl;
        }
        
        return success;
    }    

    // Destructeur 
    ~BDManager() {
        sqlite3_close(db);
    }
};

