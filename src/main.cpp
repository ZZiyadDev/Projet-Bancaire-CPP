#include <iostream>
#include <string>
#include <vector>
#include <limits> // Pour vider le buffer cin en cas d'erreur

// Vos fichiers headers
#include "Compte.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include "Client.h"
#include "Transaction.h"

using namespace std;

// Fonction utilitaire pour nettoyer la console (Simulation)
void clearScreen() {
    // Affiche plusieurs sauts de ligne pour "nettoyer" l'écran visuellement
    cout << string(50, '\n');
}

// Fonction pour attendre que l'utilisateur appuie sur Entrée
void pause() {
    cout << "\nAppuyez sur Entree pour continuer...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    // ==========================================
    // 1. INITIALISATION DES DONNEES (BACKEND)
    // ==========================================
    // Dans un vrai projet, ces données viendraient d'un fichier (FileHandler)
    
    Client client1("CLI001", "pass123", "Dupont", "Jean", "1990-05-20");
    CompteEpargne* livretA = new CompteEpargne("Jean Epargne", 1000.0, 0.05);
    CompteCourant* compteCheque = new CompteCourant("Jean Courant", 500.0, 200.0);
    
    client1.ajouterCompte(livretA);
    client1.ajouterCompte(compteCheque);

    // ==========================================
    // 2. ECRAN DE CONNEXION (LOGIN)
    // ==========================================
    string idSaisi, mdpSaisi;
    bool estConnecte = false;

    clearScreen();
    cout << "==========================================" << endl;
    cout << "   BIENVENUE DANS VOTRE BANQUE EN LIGNE   " << endl;
    cout << "==========================================" << endl;

    // Boucle de connexion simple (3 essais max par exemple, ici infini pour tester)
    while (!estConnecte) {
        cout << "\nVeuillez vous identifier." << endl;
        cout << "Identifiant : ";
        cin >> idSaisi;
        cout << "Mot de passe : ";
        cin >> mdpSaisi;

        // Vérification basique (Idéalement via AuthService)
        if (idSaisi == client1.getIdentifiant() && client1.verifierMotDePasse(mdpSaisi)) {
            estConnecte = true;
            cout << "\nConnexion reussie ! Bonjour " << client1.getTypeUtilisateur() << ".\n";
        } else {
            cout << "ERREUR : Identifiant ou mot de passe incorrect. Reesayez.\n";
        }
    }

    // ==========================================
    // 3. MENU INTERACTIF
    // ==========================================
    int choix = -1;
    
    do {
        // Affichage du menu
        cout << "\n------------------------------------------" << endl;
        cout << "           MENU PRINCIPAL                 " << endl;
        cout << "------------------------------------------" << endl;
        cout << "1. Afficher mon Profil" << endl;
        cout << "2. Voir mes Comptes et Soldes" << endl;
        cout << "3. Effectuer un Depot" << endl;
        cout << "4. Effectuer un Retrait" << endl;
        cout << "5. Effectuer un Virement" << endl;
        cout << "6. Historique des Transactions" << endl;
        cout << "0. Deconnexion / Quitter" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Votre choix : ";
        
        // Sécurisation de l'entrée utilisateur
        if (!(cin >> choix)) {
            cout << "Entree invalide. Veuillez entrer un nombre." << endl;
            cin.clear(); // Réinitialise l'état d'erreur
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Vide le buffer
            continue;
        }

        // Exécution de l'action choisie
        switch (choix) {
            case 1: // Profil
                client1.afficherProfil();
                break;

            case 2: // Voir Comptes
                cout << "\n--- Vos Comptes ---" << endl;
                {
                    const auto& comptes = client1.getComptes();
                    for (size_t i = 0; i < comptes.size(); ++i) {
                        cout << i + 1 << ". "; 
                        comptes[i]->afficherInfo(); // Polymorphisme
                    }
                }
                break;

            case 3: // Dépôt
            case 4: // Retrait
            {
                int indexCompte;
                double montant;
                string typeAction = (choix == 3) ? "Depot" : "Retrait";

                cout << "\nSur quel compte ? (Entrez le numero 1, 2...) : ";
                cin >> indexCompte;

                const auto& comptes = client1.getComptes();
                // Vérification index valide (1-based vers 0-based)
                if (indexCompte >= 1 && indexCompte <= (int)comptes.size()) {
                    Compte* compteChoisi = comptes[indexCompte - 1];
                    
                    cout << "Montant du " << typeAction << " : ";
                    cin >> montant;

                    if (choix == 3) {
                        // DEPOT
                        compteChoisi->deposer(montant); // deposer gère son propre affichage
                    } else {
                        // RETRAIT (Avec if/else comme demandé)
                        if (compteChoisi->retirer(montant)) {
                             // Succès déjà affiché par retirer()
                        } else {
                             cout << "ECHEC : Fonds insuffisants." << endl;
                        }
                    }
                } else {
                    cout << "Numero de compte invalide." << endl;
                }
                break;
            }

            case 5: // Virement
            {
                // Simplification : Virement du compte 1 vers compte 2 (ou inversement)
                // Pour faire simple dans le menu, on demande Source et Destination
                int idxSrc, idxDest;
                double montant;
                const auto& comptes = client1.getComptes();

                cout << "\n--- Virement Inter-Comptes ---" << endl;
                cout << "Compte Source (1, 2...) : "; cin >> idxSrc;
                cout << "Compte Destination (1, 2...) : "; cin >> idxDest;
                cout << "Montant : "; cin >> montant;

                if (idxSrc >= 1 && idxSrc <= (int)comptes.size() && 
                    idxDest >= 1 && idxDest <= (int)comptes.size() && idxSrc != idxDest) {
                    
                    Compte* src = comptes[idxSrc - 1];
                    Compte* dest = comptes[idxDest - 1];

                    if (src->virementVers(*dest, montant)) {
                         // Succès affiché par la méthode
                    } else {
                         cout << "ECHEC du virement." << endl;
                    }
                } else {
                    cout << "Erreur : Comptes invalides ou identiques." << endl;
                }
                break;
            }

            case 6: // Historique
            {
                int indexCompte;
                cout << "\nVoir historique de quel compte ? (1, 2...) : ";
                cin >> indexCompte;
                const auto& comptes = client1.getComptes();
                
                if (indexCompte >= 1 && indexCompte <= (int)comptes.size()) {
                    comptes[indexCompte - 1]->afficherHistorique();
                } else {
                    cout << "Numero invalide." << endl;
                }
                break;
            }

            case 0:
                cout << "Deconnexion en cours... Au revoir !" << endl;
                break;

            default:
                cout << "Choix inconnu." << endl;
        }

        if (choix != 0) pause(); // Attend avant de réafficher le menu

    } while (choix != 0);

    return 0;
}