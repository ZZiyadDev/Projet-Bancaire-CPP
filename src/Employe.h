#ifndef EMPLOYE_H
#define EMPLOYE_H

#pragma once

#include <string>

class Employe {
protected:
    int id;
    std::string nom;
    std::string prenom;
    double salaire;

public:
    Employe() noexcept;
    Employe(int id, const std::string& nom, const std::string& prenom, double salaire) noexcept;

    // Getters
    int getId() const noexcept;
    const std::string& getNom() const noexcept;
    const std::string& getPrenom() const noexcept;
    double getSalaire() const noexcept;

    // Setters
    void setSalaire(double salaire) noexcept;

    // Virtual methods
    virtual void afficher() const noexcept;
    virtual std::string getRole() const noexcept;

    virtual ~Employe() = default;
};

#endif // EMPLOYE_H

