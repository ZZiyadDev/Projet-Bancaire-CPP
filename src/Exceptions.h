#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Exception thrown when an account cannot be found
class CompteIntrouvable : public std::runtime_error {
public:
    explicit CompteIntrouvable(const std::string& identifiant) 
        : std::runtime_error("Compte introuvable (ID: " + identifiant + ")") {}
};

// Exception thrown when the balance is insufficient for a transaction
class SoldeInsuffisant : public std::runtime_error {
public:
    explicit SoldeInsuffisant(const std::string& message = "Solde insuffisant pour effectuer cette operation.") 
        : std::runtime_error(message) {}
};

// Exception thrown when an input amount is invalid (e.g., negative)
class MontantInvalide : public std::runtime_error {
public:
    explicit MontantInvalide(const std::string& message = "Le montant saisi est invalide.") 
        : std::runtime_error(message) {}
};

#endif // EXCEPTIONS_H