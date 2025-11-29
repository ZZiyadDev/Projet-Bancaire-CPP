#pragma once

#include <string>
#include <iostream>

class Compte{
    protected:
        std::string titulaire;
        double solde;
    public:
        //constructeur
        Compte(std::string t,double sd) : titulaire(t),solde(sd) {}

        //Explication: Méthode pure (Abstraite) -> Obligation de l'implémenter dans les classes dérivées
        virtual void afficherInfo() const = 0;
        
        virtual bool retirer(double montant) = 0;

        //Explication: Méthode virtuelle (Peut être redéfinie dans les classes dérivées)   
        virtual void deposer(double montant);

        //getters
        double getSolde() const {return solde;}
        std::string getTitulaire() const {return titulaire;}

        virtual bool virementVers(Compte& destinataire, double montant);  

        
        //destructeur virtuel
        virtual ~Compte() {}
};
