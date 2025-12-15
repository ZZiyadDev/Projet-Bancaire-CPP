#ifndef CAISSIER_H
#define CAISSIER_H

#pragma once

#include <string>
#include "Employe.h"

class Caissier : public Employe {
public:
    Caissier() noexcept;
    Caissier(const std::string& nom, const std::string& prenom, double salaire,
             const std::string& identifiant, const std::string& motDePasse) noexcept;

    void afficher() const noexcept override;
    std::string getRole() const noexcept override;

    ~Caissier() override = default;
};

#endif // CAISSIER_H
