#pragma once

#include <string>
#include <iostream>
#include <iomanip> // Pour le formatage date
#include <sstream>
#include <vector>
#include <ctime>

#include "Transaction.h"

class Compte{
    protected:
     std::string numCompte;
        std::string titulaire;
        double solde;
        std::vector <Transaction> historique;

        void enregistrerOperation(std::string type, double montant);

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
        
        virtual void afficherHistorique() const;

        const std::string& getNumCompte() const { return numCompte; }

        void setNumCompte(std::string id) { 
            this->numCompte = id; 
        }
        
        //destructeur virtuel
        virtual ~Compte() {}
};
