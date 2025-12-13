#ifndef MANAGER_H
#define MANAGER_H

#include "Employe.h"

class Manager : public Employe {
private:
    int nombreEmployes;

public:
    Manager();
    Manager(int id, const std::string& nom, const std::string& prenom,
            double salaire, int nombreEmployes);

    int getNombreEmployes() const;
    void setNombreEmployes(int nombre);

    void afficher() const override;
    std::string getRole() const override;
};

#endif
