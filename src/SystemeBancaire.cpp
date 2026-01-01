#include "SystemeBancaire.h"
#include "AdminIT.h"
#include "Manager.h"
#include "Caissier.h"
#include "Client.h"
#include "EmployeClient.h"
#include <iostream>

SystemeBancaire::SystemeBancaire() {
    // The managers are initialized in the member initializer list.
    // Their constructors now handle loading data and creating default users.
    std::cout << "[SystemeBancaire] Initialisation..." << std::endl;
    compteManager.chargerDonnees(utilisateurManager);
    std::cout << "[SystemeBancaire] Systeme pret." << std::endl;
}

SystemeBancaire::~SystemeBancaire() {
    // Destructors of manager classes will handle cleanup
}

void SystemeBancaire::lancer() {
    bool applicationActive = true;

    while (applicationActive) {
        interfaceUtilisateur.printHeader("BANQUE CPP - CONNEXION", "Veuillez vous authentifier");

        std::string login, pass;
        std::cout << "   (Tapez 'exit' pour quitter)" << std::endl;
        std::cout << "   " << UI::Colors::YELLOW << "> " << UI::Colors::RESET << "Identifiant : ";
        std::cin >> login;

        if (login == "exit") {
            applicationActive = false;
            continue;
        }

        std::cout << "   " << UI::Colors::YELLOW << "> " << UI::Colors::RESET << "Mot de passe : ";
        std::cin >> pass;
        interfaceUtilisateur.printFooter();

        Utilisateur* user = serviceAuthentification.authentifier(login, pass, utilisateurManager.getAllUtilisateurs());

        if (user) {
            interfaceUtilisateur.printMessage("\nConnexion reussie : " + user->getPrenom() + " " + user->getNom(), UI::Colors::GREEN);
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
                 interfaceUtilisateur.printMessage("Erreur: Type d'utilisateur inconnu.", UI::Colors::RED);
                 interfaceUtilisateur.pause();
            }
        } else {
            interfaceUtilisateur.printMessage("\n[!] Identifiants incorrects.", UI::Colors::RED);
            interfaceUtilisateur.pause();
        }
    }
    interfaceUtilisateur.printMessage("Fermeture de l'application.", UI::Colors::YELLOW);
}