#pragma once
#include "Employe.h"
#include "Compte.h"
#include <vector>
#include <iostream>

class EmployeClient : public Employe {
private:
    std::vector<Compte*> comptes;
    std::string roleSpecifique;

public:
    EmployeClient();
    EmployeClient(const std::string& nom, const std::string& prenom, double salaire,
                  const std::string& identifiant, const std::string& motDePasse,
                  const std::string& role);

    // Client-like features implemented manually
    void ajouterCompte(Compte* c);
    const std::vector<Compte*>& getComptes() const;
    std::string getRoleSpecifique() const;
    
    // Overrides
    void afficherProfil() const override;
    std::string getTypeUtilisateur() const override;

    ~EmployeClient() override = default;
};
