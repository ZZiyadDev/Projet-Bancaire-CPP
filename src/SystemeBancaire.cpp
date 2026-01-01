#include "SystemeBancaire.h"
#include "AdminIT.h"
#include "Manager.h"
#include "Caissier.h"
#include "Client.h"
#include "EmployeClient.h"
#include <iostream>

using namespace std;

SystemeBancaire::SystemeBancaire() {
    // The managers are initialized in the member initializer list.
    // Their constructors now handle loading data and creating default users.
    cout << "[SystemeBancaire] Initialisation..." << endl;
    compteManager.chargerDonnees(utilisateurManager);
    cout << "[SystemeBancaire] Systeme pret." << endl;
}

SystemeBancaire::~SystemeBancaire() {
    // Destructors of manager classes will handle cleanup
}

void SystemeBancaire::lancer() {
    bool applicationActive = true;

    while (applicationActive) {
        interfaceUtilisateur.clearScreen();
        cout << "==========================================" << endl;
        cout << "      BANQUE CPP - CONNEXION              " << endl;
        cout << "==========================================" << endl;

        string login, pass;
        cout << "\nIdentifiant (ou 'exit') : ";
        cin >> login;

        if (login == "exit") {
            applicationActive = false;
            continue;
        }

        cout << "Mot de passe : ";
        cin >> pass;

        Utilisateur* user = serviceAuthentification.authentifier(login, pass, utilisateurManager.getAllUtilisateurs());

        if (user) {
            cout << "\nConnexion reussie : " << user->getPrenom() << " " << user->getNom() << endl;
            cout << "Type d'utilisateur: " << user->getTypeUtilisateur() << endl; // DEBUG
            interfaceUtilisateur.pause();

            if (auto* admin = dynamic_cast<AdminIT*>(user)) {
                interfaceUtilisateur.afficherMenuAdmin(admin, utilisateurManager); 
            }
            else if (auto* manager = dynamic_cast<Manager*>(user)) {
                interfaceUtilisateur.afficherMenuManager(manager, utilisateurManager);
            }
            else if (auto* caissier = dynamic_cast<Caissier*>(user)) {
                interfaceUtilisateur.afficherMenuCaissier(caissier, utilisateurManager, compteManager);
            }
            else if (auto* ec = dynamic_cast<EmployeClient*>(user)) {
                interfaceUtilisateur.afficherMenuEmployeClient(ec, utilisateurManager, compteManager);
            }
            else if (auto* client = dynamic_cast<Client*>(user)) {
                interfaceUtilisateur.afficherMenuClient(client, compteManager);
            }
            else if (auto* emp = dynamic_cast<Employe*>(user)) {
                interfaceUtilisateur.afficherMenuEmploye(emp, utilisateurManager, compteManager);
            }
            else {
                 cout << "Erreur: Type d'utilisateur inconnu." << endl;
                 interfaceUtilisateur.pause();
            }
        } else {
            cout << "\n[!] Identifiants incorrects." << endl;
            interfaceUtilisateur.pause();
        }
    }
    cout << "Fermeture de l'application." << endl;
}