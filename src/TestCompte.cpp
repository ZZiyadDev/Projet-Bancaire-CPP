#include <iostream>
#include <cassert>
#include <string>
#include "../src/Compte.h"

// Concrete implementation of the abstract class Compte for testing purposes
class ConcreteCompte : public Compte {
public:
    ConcreteCompte(std::string t, double sd) : Compte(t, sd) {}

    // Implementation of pure virtual method
    void afficherInfo() const override {
        std::cout << "Compte de " << titulaire << " | Solde: " << solde << std::endl;
    }

    // Implementation of pure virtual method
    bool retirer(double montant) override {
        if (montant > 0 && solde >= montant) {
            solde -= montant;
            enregistrerOperation("Retrait", montant);
            return true;
        }
        return false;
    }
};

void test_constructor() {
    std::cout << "Running test_constructor..." << std::endl;
    ConcreteCompte c("Alice", 1000.0);
    assert(c.getTitulaire() == "Alice");
    assert(c.getSolde() == 1000.0);
    std::cout << "PASSED" << std::endl;
}

void test_deposer() {
    std::cout << "Running test_deposer..." << std::endl;
    ConcreteCompte c("Bob", 500.0);
    
    c.deposer(200.0);
    assert(c.getSolde() == 700.0);
    
    // Test invalid deposit (negative)
    c.deposer(-50.0);
    assert(c.getSolde() == 700.0); // Balance should remain unchanged
    std::cout << "PASSED" << std::endl;
}

void test_retirer() {
    std::cout << "Running test_retirer..." << std::endl;
    ConcreteCompte c("Charlie", 500.0);
    
    bool success = c.retirer(100.0);
    assert(success == true);
    assert(c.getSolde() == 400.0);
    
    // Test insufficient funds
    success = c.retirer(1000.0);
    assert(success == false);
    assert(c.getSolde() == 400.0);
    std::cout << "PASSED" << std::endl;
}

void test_virementVers() {
    std::cout << "Running test_virementVers..." << std::endl;
    ConcreteCompte source("Source", 1000.0);
    ConcreteCompte dest("Dest", 500.0);
    
    bool success = source.virementVers(dest, 300.0);
    assert(success == true);
    assert(source.getSolde() == 700.0);
    assert(dest.getSolde() == 800.0);
    
    // Test failed transfer (insufficient funds)
    success = source.virementVers(dest, 2000.0);
    assert(success == false);
    assert(source.getSolde() == 700.0);
    assert(dest.getSolde() == 800.0);
    std::cout << "PASSED" << std::endl;
}

int main() {
    test_constructor();
    test_deposer();
    test_retirer();
    test_virementVers();
    
    std::cout << "\nAll unit tests passed successfully!" << std::endl;
    return 0;
}