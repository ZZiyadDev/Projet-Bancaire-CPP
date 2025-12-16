#pragma once

#include <string>
#include "Employe.h"

class Manager : public Employe {
private:
    int nombreEmployes;

public:
    Manager();
    Manager(const std::string& nom, const std::string& prenom,
            double salaire, int nombreEmployes,
            const std::string& identifiant, const std::string& motDePasse);

    int getNombreEmployes() const;
    void setNombreEmployes(int nombre);

    void afficher() const override;
    std::string getRole() const override;

    ~Manager() override = default;
};
