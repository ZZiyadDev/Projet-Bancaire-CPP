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
        std::string titulaire;
        double solde;
        std::vector <Transaction> historique;

        void enregistrerOperation(std::string type, double montant) {
        // 1. Récupérer la date actuelle automatiquement
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        
        std::stringstream dateStream;
        dateStream << (now->tm_year + 1900) << '-' 
                   << (now->tm_mon + 1) << '-'
                   <<  now->tm_mday;
        
        // 2. Créer et ajouter la transaction
        historique.push_back(Transaction(type, dateStream.str(), montant));
    }
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
        
        virtual void afficherHistorique() const {
        std::cout << "\n--- Historique du compte de " << titulaire << " ---" << std::endl;
        for (const auto& transaction : historique) {
            transaction.afficherDetails();
        }
        std::cout << "-------------------------------------------" << std::endl;
    }

        
        //destructeur virtuel
        virtual ~Compte() {}
};
