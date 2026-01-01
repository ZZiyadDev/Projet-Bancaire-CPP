#pragma once

#include <string>
#include "Utilisateur.h"

class Employe : virtual public Utilisateur {
protected:
    double salaire;

public:
    Employe();
    Employe(const std::string& nom, const std::string& prenom, double salaire,
            const std::string& identifiant, const std::string& motDePasse);

    // Getters
    double getSalaire() const;

    // Setters
    void setSalaire(double salaire);

    // Virtual methods
    virtual void afficher() const;
    virtual std::string getRole() const;
    virtual void afficherProfil() const;
    virtual std::string getTypeUtilisateur() const override;

    virtual ~Employe() = default;
};
