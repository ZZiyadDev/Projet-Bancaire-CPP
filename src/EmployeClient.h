#pragma once
#include "Employe.h"
#include "Client.h" // Include Client header
#include <iostream>

class EmployeClient : public Employe, public Client {
private:
    std::string roleSpecifique;

public:
    EmployeClient(const std::string& nom, const std::string& prenom, double salaire,
                  const std::string& identifiant, const std::string& motDePasse,
                  const std::string& role, const std::string& dateNaissance);

    // roleSpecifique getter
    std::string getRoleSpecifique() const;
    
    // Overrides
    void afficherProfil() const override;
    std::string getTypeUtilisateur() const override;

    ~EmployeClient() override = default;
};
