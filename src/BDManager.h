#pragma once

#include <string>
#include <iostream>
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

    // Get the raw connection (useful for advanced fetching later)
    sqlite3* getConnection() {
        return db;
    }

    // Destructeur 
    ~BDManager() {
        sqlite3_close(db);
    }
};

// Initializer le membre statique
BDManager* BDManager::instance = 0;
