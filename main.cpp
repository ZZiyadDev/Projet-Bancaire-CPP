#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Compte.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include "Utilisateur.h"
#include "Admin.h"
#include "Client.h"
#include "Transaction.h"
#include "SystemeBancaire.h"

void printHeader(string title) {
    cout << "\n--------------------------------------------------" << endl;
    cout << "  TEST: " << title << endl;
    cout << "--------------------------------------------------" << endl;
}
int main() {
    // ==========================================
    // 1. SETUP: Creating a Client
    // ==========================================
    printHeader("Creation du Client");
    
    // Create a client (ID, Password, Name, Surname, DOB)
    Client client1("CLI001", "pass123", "Dupont", "Jean", "1990-05-20");
    
    // Display initial profile
    client1.afficherProfil();

    // ==========================================
    // 2. SETUP: Creating Accounts
    // ==========================================
    printHeader("Creation et Ajout des Comptes");

    // A. Create a Savings Account (Balance: 1000, Interest: 5%)
    CompteEpargne* livretA = new CompteEpargne("Jean Epargne", 1000.0, 0.05);
    
    // B. Create a Current Account (Balance: 500, Overdraft: 200)
    CompteCourant* compteCheque = new CompteCourant("Jean Courant", 500.0, 200.0);

    // Link them to the client
    client1.ajouterCompte(livretA);
    client1.ajouterCompte(compteCheque);

    cout << "Comptes ajoutes avec succes." << endl;

    // ==========================================
    // 3. TEST: Savings Account Logic
    // ==========================================
    printHeader("Test Compte Epargne (Livret A)");
    
    // Test 1: Normal Deposit
    livretA->deposer(200); // Solde -> 1200

    // Test 2: Interest Application
    livretA->appliquerInteret(); // 1200 + 5% = 1260

    // Test 3: Withdrawal Limit (Should Fail)
    cout << "\n[Tentative de retrait excessif (2000)] :" << endl;
    if (livretA->retirer(2000) == false) {
        cout << "-> SUCCES DU TEST: Le systeme a bloque le retrait (Fonds insuffisants)." << endl;
    }

    // ==========================================
    // 4. TEST: Current Account Logic (Overdraft)
    // ==========================================
    printHeader("Test Compte Courant (Overdraft)");

    // Initial Balance: 500. Overdraft: 200. Total Capacity: 700.
    
    // Test 1: Withdraw more than balance, but within overdraft
    cout << "[Tentative de retrait avec decouvert (600)] :" << endl;
    // Should succeed because 500 (Solde) + 200 (Decouvert) >= 600
    if (compteCheque->retirer(600)) {
        cout << "-> SUCCES DU TEST: Retrait accepte grace au decouvert." << endl;
        cout << "-> Nouveau solde (doit etre -100): " << compteCheque->getSolde() << endl;
    }

    // ==========================================
    // 5. TEST: Transfer (Virement)
    // ==========================================
    printHeader("Test Virement (Epargne -> Courant)");

    cout << "Solde Epargne avant: " << livretA->getSolde() << endl;
    cout << "Solde Courant avant: " << compteCheque->getSolde() << endl;

    // Transfer 100 from Savings to Current
    cout << "\n[Execution Virement de 100...]" << endl;
    livretA->virementVers(*compteCheque, 100);

    cout << "Solde Epargne apres: " << livretA->getSolde() << endl;
    cout << "Solde Courant apres: " << compteCheque->getSolde() << endl;

    // ==========================================
    // 6. TEST: Transaction Class
    // ==========================================
    printHeader("Test Transaction Class");
    Transaction t1("Depot", "2025-11-28", 150.0);
    t1.afficherDetails();

    // ==========================================
    // 7. CLEANUP
    // ==========================================
    printHeader("Fin du Programme");
    
    // When 'client1' goes out of scope here, its Destructor (~Client) is called.
    // That destructor loops through the accounts and calls 'delete'.
    // So we don't need to manually delete livretA or compteCheque here!
    
    return 0;
}

