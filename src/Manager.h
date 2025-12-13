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
    Manager(int id, const std::string& nom, const std::string& prenom,
            double salaire, int nombreEmployes) noexcept;

    int getNombreEmployes() const noexcept;
    void setNombreEmployes(int nombre) noexcept;

    void afficher() const noexcept override;
    std::string getRole() const noexcept override;

    ~Manager() override = default;
};

#endif // MANAGER_H
