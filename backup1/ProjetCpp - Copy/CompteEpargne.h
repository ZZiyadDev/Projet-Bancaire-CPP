#pragma once

#include "Compte.h"
#include <iostream>


class CompteEpargne : public Compte {
    private:
        double tauxInteret;
    public:
        //Constructeur
        CompteEpargne(std::string t, double sd, double ti) : Compte(t, sd), tauxInteret(ti) {}
    
        //methodes
        bool retirer(double montant) override;
        void afficherInfo() const override;
        void appliquerInteret();

        //Destructeur
        ~CompteEpargne() override {}
};

