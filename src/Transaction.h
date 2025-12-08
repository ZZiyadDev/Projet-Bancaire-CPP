#pragma once

#include <iostream>
#include <string.h>


class Transaction {
   private :
   std::string type;
   std::string date;
   float montant;

   public:
    // Constructor
    Transaction(const std::string& t, const std::string& d, float m)
        : type(t), date(d), montant(m) {}

    // Getters
    std::string getType() const { return type; }
    std::string getDate() const { return date; }
    float getMontant() const { return montant; }

    // Display details
    void afficherDetails() const {
    std::cout << "Type: " << type  << ", Montant: " << montant 
            << ", Date: " << date << std::endl;
    }
    

};