#include <iostream>
#include <cstdlib>
#include <ctime>
#include "SystemeBancaire.h"
#include "BDManager.h"

using namespace std;

// Initializer le membre statique

BDManager* BDManager::instance = nullptr;

int main() {
    
    // Initialise le générateur aléatoire avec l'heure actuelle
    std::srand(std::time(nullptr));
    
    // 1. Setup Database Tables (Create them if they don't exist)
    if (!BDManager::getInstance()->setupTables()) {
        cerr << "ERREUR CRITIQUE : Impossible d'initialiser la base de données." << endl;
        return -1;
    }

    // 2. Instantiate the System (The Controller)
    SystemeBancaire banque;

    // 3. Start the Application
    // This method will handle the Login Loop and menus internally
    banque.lancer(); 

    return 0;
}