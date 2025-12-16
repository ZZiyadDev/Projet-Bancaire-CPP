#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <typeinfo> // Nécessaire pour dynamic_cast

// Vos fichiers headers
#include "Utilisateur.h"
#include "Client.h"
#include "Employe.h"
#include "Compte.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"

using namespace std;

// --- FONCTIONS UTILITAIRES ---

void clearScreen() {
    cout << string(50, '\n');
}

void pause() {
    cout << "\nAppuyez sur Entree pour continuer...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --- MENU SPECIFIQUE AUX CLIENTS ---
// (C'est votre ancien code de menu, encapsulé dans une fonction)
void sessionClient(Client* client) {
    int choix = -1;
    do {
        clearScreen();
        cout << "=== ESPACE CLIENT : " << client->getPrenom() << " " << client->getNom() << " ===" << endl;
        cout << "1. Afficher mon Profil" << endl;
        cout << "2. Voir mes Comptes et Soldes" << endl;
        cout << "3. Effectuer un Depot" << endl;
        cout << "4. Effectuer un Retrait" << endl;
        cout << "5. Effectuer un Virement" << endl;
        cout << "6. Historique des Transactions" << endl;
        cout << "0. Deconnexion" << endl;
        cout << "Votre choix : ";
        
        if (!(cin >> choix)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choix) {
            case 1: 
                client->afficherProfil(); 
                break;
            case 2: {
                cout << "\n--- Vos Comptes ---" << endl;
                const auto& comptes = client->getComptes();
                for (size_t i = 0; i < comptes.size(); ++i) {
                    cout << i + 1 << ". "; comptes[i]->afficherInfo();
                }
                break;
            }
            case 3: // Depot
            case 4: // Retrait
            {
                int idx; double mt;
                string action = (choix == 3) ? "Depot" : "Retrait";
                cout << "Compte n° : "; cin >> idx;
                const auto& comptes = client->getComptes();
                if (idx >= 1 && idx <= (int)comptes.size()) {
                    cout << "Montant : "; cin >> mt;
                    if (choix == 3) comptes[idx-1]->deposer(mt);
                    else comptes[idx-1]->retirer(mt);
                }
                break;
            }
            case 5: // Virement (Simplifié)
            {
                int src, dst; double mt;
                cout << "Source n°: "; cin >> src;
                cout << "Dest n°: "; cin >> dst;
                cout << "Montant: "; cin >> mt;
                const auto& tab = client->getComptes();
                if (src >= 1 && dst >= 1 && src <= (int)tab.size() && dst <= (int)tab.size()) {
                    tab[src-1]->virementVers(*tab[dst-1], mt);
                }
                break;
            }
            case 6: // Historique
            {
                int idx;
                cout << "Compte n° : "; cin >> idx;
                const auto& tab = client->getComptes();
                if (idx >= 1 && idx <= (int)tab.size()) tab[idx-1]->afficherHistorique();
                break;
            }
            case 0: cout << "Fermeture de la session client..." << endl; break;
            default: cout << "Choix invalide." << endl;
        }
        if (choix != 0) pause();
    } while (choix != 0);
}

// --- MENU SPECIFIQUE AUX EMPLOYES ---
void sessionEmploye(Employe* employe) {
    int choix = -1;
    do {
        clearScreen();
        cout << "=== ESPACE EMPLOYE : " << employe->getPrenom() << " (" << employe->getRole() << ") ===" << endl;
        cout << "1. Voir mon Profil RH" << endl;
        cout << "2. Consulter mon Salaire" << endl;
        cout << "3. Tâche administrative (Simulation)" << endl;
        cout << "0. Deconnexion" << endl;
        cout << "Votre choix : ";

        if (!(cin >> choix)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choix) {
            case 1:
                employe->afficherProfil(); // Affiche matricule, poste, etc.
                break;
            case 2:
                cout << "\nVotre salaire actuel est de : " << employe->getSalaire() << " MAD" << endl;
                break;
            case 3:
                cout << "\nExecution d'une tache administrative..." << endl;
                cout << "[SYSTEM] Verification des dossiers en cours... OK." << endl;
                break;
            case 0: cout << "Fin de service. Au revoir." << endl; break;
            default: cout << "Option inconnue." << endl;
        }
        if (choix != 0) pause();
    } while (choix != 0);
}

// --- MAIN PRINCIPAL ---

int main() {
    // ==========================================
    // 1. INITIALISATION DES DONNEES (Polymorphisme)
    // ==========================================
    vector<Utilisateur*> baseDeDonnees;

    // A. Création d'un Client
    Client* c1 = new Client("CLI001", "1234", "Dupont", "Jean", "1990-05-20");
    c1->ajouterCompte(new CompteEpargne("Livret A", 1000.0, 0.05));
    c1->ajouterCompte(new CompteCourant("Compte Chq", 500.0, 200.0));
    baseDeDonnees.push_back(c1);

    // B. Création d'un Employé (Manager)
    // Supposons que le constructeur est : Employe(nom, prenom, salaire, id, mdp)
    Employe* e1 = new Employe("Martin", "Sophie", 15000.0, "EMP001", "admin");
    baseDeDonnees.push_back(e1);

    // ==========================================
    // 2. BOUCLE PRINCIPALE DE L'APPLICATION
    // ==========================================
    bool applicationActive = true;

    while (applicationActive) {
        clearScreen();
        cout << "==========================================" << endl;
        cout << "      BANQUE CPP - ECRAN D'ACCUEIL        " << endl;
        cout << "==========================================" << endl;
        
        string login, pass;
        cout << "\nIdentifiant (ou 'exit' pour quitter) : ";
        cin >> login;
        
        if (login == "exit") {
            applicationActive = false;
            break;
        }

        cout << "Mot de passe : ";
        cin >> pass;

        // Recherche de l'utilisateur dans le vecteur
        Utilisateur* utilisateurConnecte = nullptr;
        
        for (Utilisateur* u : baseDeDonnees) {
            if (u->getIdentifiant() == login && u->verifierMotDePasse(pass)) {
                utilisateurConnecte = u;
                break;
            }
        }

        // ==========================================
        // 3. DISPATCH (AIGUILLAGE) SELON LE TYPE
        // ==========================================
        if (utilisateurConnecte != nullptr) {
            cout << "\nConnexion reussie !" << endl;
            pause();

            // On essaie de voir si c'est un Client
            Client* clientPtr = dynamic_cast<Client*>(utilisateurConnecte);
            
            // On essaie de voir si c'est un Employe
            Employe* employePtr = dynamic_cast<Employe*>(utilisateurConnecte);

            if (clientPtr != nullptr) {
                // C'est un Client -> Menu Client
                sessionClient(clientPtr);
            } 
            else if (employePtr != nullptr) {
                // C'est un Employé -> Menu Employé
                sessionEmploye(employePtr);
            }
            else {
                // Cas générique (Ex: Admin système basique)
                cout << "Profil generique detecte. Pas de menu specifique." << endl;
                utilisateurConnecte->afficherProfil();
                pause();
            }

        } else {
            cout << "\nERREUR : Identifiants incorrects." << endl;
            pause();
        }
    }

    // Nettoyage de la mémoire
    cout << "\nNettoyage de la memoire..." << endl;
    for (Utilisateur* u : baseDeDonnees) {
        // Note: Si Client est propriétaire des comptes, son destructeur doit les supprimer
        delete u; 
    }
    baseDeDonnees.clear();

    return 0;
}