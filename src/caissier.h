#ifndef CAISSIER_H
#define CAISSIER_H

#include "Employe.h"

class Caissier : public Employe {
public:
    Caissier();
    Caissier(int id, const std::string& nom, const std::string& prenom, double salaire);

    void afficher() const override;
    std::string getRole() const override;
};

#endif
