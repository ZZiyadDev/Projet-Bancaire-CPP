#pragma once
#include "Compte.h"
#include <iostream>

class CompteProfessionnel : public Compte {
private:
    double plafondDecouvert; // Overdraft limit for professional accounts

public:
    // Constructor
    CompteProfessionnel(const std::string& titulaire, double soldeInitial, double plafond);

    // Override methods
    bool retirer(double montant) override;
    void afficherInfo() const override;

    // Destructor
    ~CompteProfessionnel() override;
};

