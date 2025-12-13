#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <string>

class Employe {
protected:
    int id;
    std::string nom;
    std::string prenom;
    double salaire;

public:
    Employe();
    Employe(int id, const std::string& nom, const std::string& prenom, double salaire);

    // Getters
    int getId() const;
    std::string getNom() const;
    std::string getPrenom() const;
    double getSalaire() const;

    // Setters
    void setSalaire(double salaire);

    // Virtual methods
    virtual void afficher() const;
    virtual std::string getRole() const;

    virtual ~Employe();
};

#endif

