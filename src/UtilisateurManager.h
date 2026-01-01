#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Utilisateur.h"
#include "Client.h"

class UtilisateurManager {
private:
    std::vector<std::unique_ptr<Utilisateur>> utilisateurs;
    std::string genererIdClient();

public:
    UtilisateurManager();
    void chargerDonnees();
    Utilisateur* getUtilisateur(const std::string& id);
    Client* getClient(const std::string& nom, const std::string& prenom);
    Client* getClientById(const std::string& id);
    const std::vector<std::unique_ptr<Utilisateur>>& getAllUtilisateurs() const;
    void ajouterNouveauClient(std::string nom, std::string prenom, std::string dateN);
    bool supprimerUtilisateur(const std::string& id);
    void ajouterUtilisateur(std::unique_ptr<Utilisateur> utilisateur);
};
