#ifndef ADMINIT_H
#define ADMINIT_H

#pragma once

#include <string>
#include "Employe.h"

class AdminIT : public Employe {
private:
    std::string niveauAcces; // ex: "admin", "super-admin"

public:
    AdminIT() noexcept;
    AdminIT(const std::string& nom, const std::string& prenom,
            double salaire, const std::string& niveauAcces,
            const std::string& identifiant, const std::string& motDePasse) noexcept;

    const std::string& getNiveauAcces() const noexcept;
    void setNiveauAcces(const std::string& niveau);

    void afficher() const noexcept override;
    std::string getRole() const noexcept override;

    // IT-specific actions
    void creerUtilisateur() const noexcept;
    void supprimerUtilisateur() const noexcept;

    ~AdminIT() override = default;
};

#endif // ADMINIT_H
