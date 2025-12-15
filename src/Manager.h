#ifndef MANAGER_H
#define MANAGER_H

#pragma once

#include <string>
#include "Employe.h"

class Manager : public Employe {
private:
    int nombreEmployes;

public:
    Manager() noexcept;
    Manager(const std::string& nom, const std::string& prenom,
            double salaire, int nombreEmployes,
            const std::string& identifiant, const std::string& motDePasse) noexcept;

    int getNombreEmployes() const noexcept;
    void setNombreEmployes(int nombre) noexcept;

    void afficher() const noexcept override;
    std::string getRole() const noexcept override;

    ~Manager() override = default;
};

#endif // MANAGER_H
