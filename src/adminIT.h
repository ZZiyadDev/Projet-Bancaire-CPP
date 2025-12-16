#pragma once

#include <string>
#include "Employe.h"

class AdminIT : public Employe {
private:
    std::string niveauAcces; // ex: "admin", "super-admin"

public:
    AdminIT();
    AdminIT(const std::string& nom, const std::string& prenom,
            double salaire, const std::string& niveauAcces,
            const std::string& identifiant, const std::string& motDePasse);

    const std::string& getNiveauAcces() const;
    void setNiveauAcces(const std::string& niveau);

    void afficher() const override;
    std::string getRole() const override;

    // IT-specific actions
    void creerUtilisateur() const;
    void supprimerUtilisateur() const;

    ~AdminIT() override = default;
};
