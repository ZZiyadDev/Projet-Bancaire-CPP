#include "SystemeBancaire.h"
#include "Client.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include "CompteProfessionnel.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

void clearScreen() {
    std::cout << std::string(50, '\n');
}

// Constructor
SystemeBancaire::SystemeBancaire() {
    std::cout << "Systeme Bancaire initialisé.\n";
}

// Generate unique client ID
std::string SystemeBancaire::genererIdClient() {
    return "CLI" + std::to_string(listeClients.size() + 1);
}

// Add a new client
void SystemeBancaire::ajouterNouveauClient(std::string nom, std::string prenom, std::string dateN) {
    std::string idClient = genererIdClient();

    // Create client
    Client* client = new Client(idClient, "1234", nom, prenom, dateN);
    listeClients.push_back(*client);

    // Insert into DB
    std::string query = "INSERT INTO Client(id, nom, prenom, dateNaissance) VALUES('"
                        + idClient + "', '" + nom + "', '" + prenom + "', '" + dateN + "');";
    BDManager::getInstance()->executeQuery(query);

    std::cout << "Nouveau client ajouté: " << nom << " " << prenom << std::endl;
}

// Find client by ID
Client* SystemeBancaire::trouverClientParId(const std::string& idClient) {
    for (Client& client : listeClients) {
        if (client.getIdentifiant() == idClient) {
            return &client;
        }
    }
    return nullptr;
}

// Open new account
bool SystemeBancaire::ouvrirNouveauCompte(std::string idClient, std::string typeCompte, double depotInitial) {
    Client* client = trouverClientParId(idClient);
    if (!client) {
        std::cout << "Client introuvable !\n";
        return false;
    }

    Compte* compte = nullptr;
    if (typeCompte == "Epargne") {
        compte = new CompteEpargne("Livret A", depotInitial, 0.05);
    } else if (typeCompte == "Courant") {
        compte = new CompteCourant("Compte Chèque", depotInitial, 200.0);
    } else if (typeCompte == "Professionnel") {
        compte = new CompteProfessionnel("Compte Pro", depotInitial, 500.0);
    } else {
        std::cout << "Type de compte invalide !\n";
        return false;
    }

    client->ajouterCompte(compte);
    listeComptes.push_back(compte);  // Add to global account list

    // Insert into database
    std::string query = "INSERT INTO Compte(numCompte, solde, type, clientId) VALUES('"
                        + compte->getNumCompte() + "', " + std::to_string(depotInitial) + ", '"
                        + typeCompte + "', '" + idClient + "');";
    BDManager::getInstance()->executeQuery(query);

    std::cout << "Compte " << typeCompte << " ouvert pour le client " << idClient << std::endl;
    return true;
}

// Find account by numCompte
Compte* SystemeBancaire::trouverCompte(std::string numCompte) {
    for (Compte* compte : listeComptes) {
        if (compte->getNumCompte() == numCompte) {
            return compte;
        }
    }
    return nullptr;
}

// Find client by name
Client* SystemeBancaire::trouverClient(std::string nom, std::string prenom) {
    for (Client& client : listeClients) {
        if (client.getNom() == nom && client.getPrenom() == prenom) {
            return &client;
        }
    }
    return nullptr;
}

// User interface
void SystemeBancaire::operations() {
    int choix = -1;
    do {
        clearScreen();
        std::cout << "=== Système Bancaire ===\n";
        std::cout << "1. Ajouter un nouveau client\n";
        std::cout << "2. Ouvrir un nouveau compte\n";
        std::cout << "0. Quitter\n";
        std::cout << "Votre choix: ";
        
        if (!(std::cin >> choix)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choix) {
            case 1: {
                std::string nom, prenom, dateNaissance;
                std::cout << "Nom du client: "; std::cin >> nom;
                std::cout << "Prenom du client: "; std::cin >> prenom;
                std::cout << "Date de naissance (YYYY-MM-DD): "; std::cin >> dateNaissance;
                ajouterNouveauClient(nom, prenom, dateNaissance);
                break;
            }
            case 2: {
                std::string idClient, typeCompte;
                double depotInitial;
                std::cout << "Identifiant du client: "; std::cin >> idClient;
                std::cout << "Type de compte (Epargne, Courant, Professionnel): "; std::cin >> typeCompte;
                std::cout << "Depot initial: "; std::cin >> depotInitial;
                ouvrirNouveauCompte(idClient, typeCompte, depotInitial);
                break;
            }
            case 0:
                std::cout << "Au revoir.\n";
                break;
            default:
                std::cout << "Choix invalide.\n";
        }
    } while (choix != 0);
}
