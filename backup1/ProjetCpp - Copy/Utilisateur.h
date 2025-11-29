#pragma once
#include <string>
#include <iostream>

class Utilisateur {
protected:
    std::string identifiant;
    std::string motDePasse;   // Stockage local uniquement / simulation

public:
    Utilisateur(const std::string& id,const std::string& mdp): identifiant(id), motDePasse(mdp) {}

    virtual ~Utilisateur() = default;

    
    bool verifierMotDePasse(const std::string& mdp) const {
        return mdp == motDePasse;
    }

    const std::string& getIdentifiant() const {
        return identifiant;
    }

    virtual void afficherProfil() const = 0;

    
    virtual std::string getTypeUtilisateur() const = 0;
};
