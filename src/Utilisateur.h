#pragma once
#include <string>
#include <iostream>

class Utilisateur {
protected:
    std::string identifiant;
    std::string motDePasse;
    std::string nom;
    std::string prenom;

public:
    Utilisateur(const std::string& id, const std::string& mdp,
                const std::string& n, const std::string& p)
        : identifiant(id), motDePasse(mdp), nom(n), prenom(p) {}

    virtual ~Utilisateur() = default;

    bool verifierMotDePasse(const std::string& mdp) const {
        return mdp == motDePasse;
    }

    const std::string& getIdentifiant() const {
        return identifiant;
    }

    const std::string& getNom() const noexcept {
        return nom;
    }

    const std::string& getPrenom() const noexcept {
        return prenom;
    }

    virtual void afficherProfil() const = 0;
    virtual std::string getTypeUtilisateur() const = 0;
};
