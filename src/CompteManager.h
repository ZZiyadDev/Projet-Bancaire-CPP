#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Compte.h"
#include "UtilisateurManager.h"

class CompteManager {
private:
    std::vector<std::unique_ptr<Compte>> listeComptes;

public:
    CompteManager();
    void chargerDonnees(UtilisateurManager& utilisateurManager);
    Compte* trouverCompte(std::string numCompte);
    bool ouvrirNouveauCompte(std::string idClient, std::string typeCompte, double depotInitial, UtilisateurManager& utilisateurManager);
    const std::vector<std::unique_ptr<Compte>>& getAllComptes() const;
};
