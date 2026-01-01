#include "AdminIT.h"
#include "Manager.h"
#include "BDManager.h"
#include "Utilisateur.h"
#include "EmployeClient.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <ctime>

AdminIT::AdminIT() : Employe(), niveauAcces("user") {}

AdminIT::AdminIT(const std::string& nom, const std::string& prenom,
                 double salaire, const std::string& niveauAcces,
                 const std::string& identifiant, const std::string& motDePasse)
    : Utilisateur(identifiant, motDePasse, nom, prenom),
      Employe(nom, prenom, salaire, identifiant, motDePasse),
      niveauAcces(niveauAcces) {}

const std::string& AdminIT::getNiveauAcces() const {
    return niveauAcces;
}

void AdminIT::setNiveauAcces(const std::string& niveau) {
    niveauAcces = niveau;
}

void AdminIT::afficher() const {
    std::cout << "[Admin IT] ";
    Employe::afficher();
    std::cout << "   Niveau d'acces: " << niveauAcces << std::endl;
}

std::string AdminIT::getRole() const {
    return "Admin IT";
}

#include "EmployeClient.h"

// ... (rest of the includes)

std::unique_ptr<Utilisateur> AdminIT::creerUtilisateur() {
    std::string nom, prenom, role, dateNaissance;
    double salaire;

    std::cout << "\n--- Creation d'un Nouvel Utilisateur ---\n";
    std::cout << "Nom de famille: ";
    std::cin >> nom;
    std::cout << "Prenom: ";
    std::cin >> prenom;
    std::cout << "Role (Employe/Manager/EmployeClient): ";
    std::cin >> role;

    if (role != "Employe" && role != "Manager" && role != "EmployeClient") {
        std::cerr << "Erreur: Role invalide specifie." << std::endl;
        return nullptr;
    }

    std::cout << "Salaire: ";
    std::cin >> salaire;

    if (role == "EmployeClient") {
        std::cout << "Date de Naissance (YYYY-MM-DD): ";
        std::cin >> dateNaissance;
    }

    std::string id = (role == "Manager" ? "MNG" : "EMP") + std::to_string(time(nullptr) % 1000);
    std::string mdp = "password123";

    std::unique_ptr<Utilisateur> nouvelUtilisateur = nullptr;
    std::string query;
    std::vector<std::string> params;

    if (role == "EmployeClient") {
        nouvelUtilisateur = std::make_unique<EmployeClient>(nom, prenom, salaire, id, mdp, role, dateNaissance);
        query = "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role, dateNaissance) "
                "VALUES (?, ?, ?, ?, 'EmployeClient', ?, ?, ?);";
        params = {id, mdp, nom, prenom, std::to_string(salaire), role, dateNaissance};
    } else {
        if (role == "Manager") {
            nouvelUtilisateur = std::make_unique<Manager>(nom, prenom, salaire, 0, id, mdp);
        } else { // Employe
            nouvelUtilisateur = std::make_unique<Employe>(nom, prenom, salaire, id, mdp);
        }
        query = "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
                "VALUES (?, ?, ?, ?, 'Employe', ?, ?);";
        params = {id, mdp, nom, prenom, std::to_string(salaire), role};
    }

    std::cout << "Utilisateur cree en memoire. ID: " << id << ", MDP (defaut): " << mdp << std::endl;
    std::cout << "Ajout a la base de donnees..." << std::endl;

    if (BDManager::getInstance()->executePrepared(query, params)) {
        std::cout << "Utilisateur ajoute a la DB avec succes." << std::endl;
        return nouvelUtilisateur;
    } else {
        std::cerr << "Erreur: Impossible d'ajouter l'utilisateur a la DB." << std::endl;
        return nullptr;
    }
}

std::string AdminIT::supprimerUtilisateur() const {
    std::string id;
    std::cout << "\n--- Suppression d'un Utilisateur ---\n";
    std::cout << "ID de l'utilisateur a supprimer: ";
    std::cin >> id;

    if (id.empty()) {
        std::cerr << "Erreur: ID invalide." << std::endl;
        return "";
    }
    
    if (id == this->getIdentifiant()) {
        std::cerr << "Erreur: Vous ne pouvez pas vous supprimer vous-meme." << std::endl;
        return "";
    }

    std::string query = "DELETE FROM Users WHERE id = ?;";
    std::vector<std::string> params = {id};

    if (BDManager::getInstance()->executePrepared(query, params)) {
        std::cout << "Utilisateur " << id << " supprime de la base de donnees." << std::endl;
        return id; // Return the ID on success
    } else {
        std::cerr << "Erreur: Impossible de supprimer l'utilisateur de la DB." << std::endl;
        return ""; // Return empty string on failure
    }
}