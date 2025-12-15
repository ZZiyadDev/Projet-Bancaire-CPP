#ifndef EMPLOYE_H
#define EMPLOYE_H

#pragma once

#include <string>
#include "Utilisateur.h"

class Employe : public Utilisateur {
protected:
    double salaire;

public:
    Employe() noexcept;
    Employe(const std::string& nom, const std::string& prenom, double salaire,
            const std::string& identifiant, const std::string& motDePasse) noexcept;

    // Getters
    double getSalaire() const noexcept;

    // Setters
    void setSalaire(double salaire) noexcept;

    // Virtual methods
    virtual void afficher() const noexcept override;
    virtual std::string getRole() const noexcept override;
    virtual void afficherProfil() const noexcept override;
    virtual std::string getTypeUtilisateur() const noexcept override;

    virtual ~Employe() = default;
};

#endif // EMPLOYE_H

