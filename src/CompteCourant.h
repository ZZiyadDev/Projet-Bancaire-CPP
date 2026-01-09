#pragma once

#include <iostream>
#include "Compte.h"

class CompteCourant : public Compte {
    private:
        double decouvertAutorise;
    public:
        //Constructeur
        CompteCourant(std::string t, double sd, double da) : Compte(t, sd), decouvertAutorise(da) {}
    
        //methodes
        void retirer(double montant) override;
        void afficherInfo() const override;
        void afficher(std::ostream& os) const override;

        //Destructeur
        ~CompteCourant() override {}
};

