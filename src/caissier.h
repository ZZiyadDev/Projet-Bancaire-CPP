#pragma once

#include <string>
#include "Employe.h"

class Caissier : public Employe {
public:
    Caissier();
    Caissier(const std::string& nom, const std::string& prenom, double salaire,
             const std::string& identifiant, const std::string& motDePasse);

    void afficher() const override;
    std::string getRole() const override;

    ~Caissier() override = default;
};
