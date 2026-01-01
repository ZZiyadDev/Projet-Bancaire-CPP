#include "InterfaceUtilisateur.h"
#include "UtilisateurManager.h"
#include "CompteManager.h"
#include "Compte.h"
#include "AdminIT.h"
#include "Manager.h"
#include "Caissier.h"
#include "Client.h"
#include "EmployeClient.h"
#include "Employe.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <cstdlib> // For system()

void InterfaceUtilisateur::clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void InterfaceUtilisateur::pause() {
    std::cout << "\nAppuyez sur Entree pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void InterfaceUtilisateur::printHeader(const std::string& title, const std::string& subtitle) {
    clearScreen();
    std::cout << UI::Colors::CYAN << "+-------------------------------------------------+" << UI::Colors::RESET << std::endl;
    std::cout << UI::Colors::CYAN << "| " << UI::Colors::YELLOW << title << std::string(48 - title.length(), ' ') << UI::Colors::CYAN << "|" << UI::Colors::RESET << std::endl;
    if (!subtitle.empty()) {
        std::cout << UI::Colors::CYAN << "| " << UI::Colors::RESET << subtitle << std::string(48 - subtitle.length(), ' ') << UI::Colors::CYAN << "|" << UI::Colors::RESET << std::endl;
    }
    std::cout << UI::Colors::CYAN << "+-------------------------------------------------+" << UI::Colors::RESET << std::endl;
    std::cout << std::endl;
}

void InterfaceUtilisateur::printFooter() {
    std::cout << std::endl;
    std::cout << UI::Colors::CYAN << "+-------------------------------------------------+" << UI::Colors::RESET << std::endl;
}

void InterfaceUtilisateur::printMessage(const std::string& message, const std::string& color) {
    std::cout << color << message << UI::Colors::RESET << std::endl;
}

void InterfaceUtilisateur::afficherMenuClient(Client* client, CompteManager& compteManager) {
    int choix = -1;
    do {
        std::string subtitle = "Connecte en tant que : " + client->getPrenom() + " " + client->getNom();
        printHeader("ESPACE CLIENT", subtitle);

        std::cout << "   1. Voir mes Comptes" << std::endl;
        std::cout << "   2. Faire un Depot" << std::endl;
        std::cout << "   3. Faire un Retrait" << std::endl;
        std::cout << "   4. Faire un Virement" << std::endl;
        std::cout << "   5. Consulter l'Historique" << std::endl;
        std::cout << std::endl;
        std::cout << "   " << UI::Colors::YELLOW << "0. Deconnexion" << UI::Colors::RESET << std::endl;
        
        printFooter();
        std::cout << "> Votre choix : ";
        
        if (!(std::cin >> choix)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        const std::vector<Compte*>& comptes = client->getComptes();

        if (choix == 1) {
            printHeader("VOS COMPTES", subtitle);
            if (comptes.empty()) {
                printMessage("Aucun compte actif.", UI::Colors::YELLOW);
            } else {
                for (size_t i = 0; i < comptes.size(); ++i) {
                    std::cout << "Compte n°" << (i + 1) << " : ";
                    comptes[i]->afficherInfo(); 
                }
            }
        }
        else if (choix == 2 || choix == 3) { // DEPOT ou RETRAIT
            printHeader(choix == 2 ? "DEPOT" : "RETRAIT", subtitle);
            if (comptes.empty()) {
                printMessage("Erreur : Vous n'avez aucun compte.", UI::Colors::RED);
            } else {
                int index = 0;
                std::cout << "\nSur quel compte ? (Entrez le numero 1, 2...) : ";
                std::cin >> index;
                
                if (index > 0 && index <= (int)comptes.size()) {
                    double montant;
                    std::cout << "Montant : ";
                    std::cin >> montant;
                    
                    if (choix == 2) {
                        comptes[index - 1]->deposer(montant);
                        printMessage("Depot effectue avec succes.", UI::Colors::GREEN);
                    } else {
                        if (comptes[index - 1]->retirer(montant)) {
                            printMessage("Retrait effectue avec succes.", UI::Colors::GREEN);
                        } else {
                            printMessage("Erreur : Solde insuffisant ou montant invalide.", UI::Colors::RED);
                        }
                    }
                } else {
                    printMessage("Numero de compte invalide.", UI::Colors::RED);
                }
            }
        }
        else if (choix == 4) { // VIREMENT
             printHeader("VIREMENT", subtitle);
            if (comptes.empty()) {
                printMessage("Erreur : Il vous faut un compte source.", UI::Colors::RED);
            } else {
                int indexSrc = 0;
                std::cout << "Depuis quel compte ? (1-" << comptes.size() << ") : ";
                std::cin >> indexSrc;
                
                if (indexSrc > 0 && indexSrc <= (int)comptes.size()) {
                    std::string destID;
                    std::cout << "ID du compte destinataire : ";
                    std::cin >> destID;
                    
                    Compte* destCompte = compteManager.trouverCompte(destID);

                    if (destCompte != nullptr) {
                        double montant;
                        std::cout << "Montant du virement : ";
                        std::cin >> montant;
                        
                        comptes[indexSrc - 1]->virementVers(*destCompte, montant);
                         printMessage("Virement effectue avec succes.", UI::Colors::GREEN);
                    } else {
                        printMessage("Erreur : Compte destinataire introuvable.", UI::Colors::RED);
                    }
                } else {
                     printMessage("Compte source invalide.", UI::Colors::RED);
                }
            }
        }
        if (choix == 5) {
            printHeader("HISTORIQUE DE COMPTE", subtitle);
            if (comptes.empty()) {
                printMessage("Aucun compte disponible.", UI::Colors::YELLOW);
            } else {
                std::cout << "\n--- Choix du Compte pour Historique ---\n";
                for (size_t i = 0; i < comptes.size(); ++i) {
                    std::cout << (i + 1) << ". "; 
                    comptes[i]->afficherInfo(); 
                }
                
                int index = 0;
                std::cout << "Votre choix : ";
                std::cin >> index;

                if (index > 0 && index <= (int)comptes.size()) {
                    comptes[index - 1]->afficherHistorique();
                } else {
                    printMessage("Compte invalide.", UI::Colors::RED);
                }
            }
        }
        
        if (choix != 0) pause();
    } while (choix!= 0);
}

void InterfaceUtilisateur::afficherMenuEmploye(Employe* employe, UtilisateurManager& utilisateurManager, CompteManager& compteManager) {
    int choix = -1;
    do {
        printHeader("ESPACE EMPLOYE", "Role: " + employe->getRole());
        std::cout << "1. Creer un Client" << std::endl;
        std::cout << "2. Ouvrir un Compte" << std::endl;
        std::cout << std::endl;
        std::cout << "0. Deconnexion" << std::endl;
        printFooter();
        std::cout << "> Choix : ";
        std::cin >> choix;

        if (choix == 1) {
            std::string n, p, d;
            std::cout << "Nom: "; std::cin >> n;
            std::cout << "Prenom: "; std::cin >> p;
            std::cout << "Date Naissance: "; std::cin >> d;
            utilisateurManager.ajouterNouveauClient(n, p, d);
        }
        else if (choix == 2) {
            std::string id, type; double dep;
            std::cout << "ID Client: "; std::cin >> id;
            std::cout << "Type (Epargne/Courant): "; std::cin >> type;
            std::cout << "Depot: "; std::cin >> dep;
            compteManager.ouvrirNouveauCompte(id, type, dep, utilisateurManager);
        }

        if (choix != 0) pause();
    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuManager(Employe* employe, UtilisateurManager& utilisateurManager) {
    Manager* manager = dynamic_cast<Manager*>(employe);
    if (!manager) {
        printMessage("Erreur : L'employe n'est pas un manager valide.", UI::Colors::RED);
        pause();
        return;
    }
    int choix = -1;

    do {
        std::string subtitle = manager->getPrenom() + " " + manager->getNom();
        printHeader("ESPACE MANAGER", subtitle);
        
        std::cout << "1. Voir la liste des employés" << std::endl;
        std::cout << "2. Ajuster le salaire d'un employé" << std::endl;
        std::cout << "3. Voir tous les clients" << std::endl;
        std::cout << std::endl;
        std::cout << "0. Deconnexion" << std::endl;
        printFooter();
        std::cout << "> Votre choix : ";

        if (!(std::cin >> choix)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choix) {
            case 1: {
                printHeader("LISTE DES EMPLOYES", subtitle);
                for (const auto& u : utilisateurManager.getAllUtilisateurs()) {
                    if (auto* emp = dynamic_cast<Employe*>(u.get())) {
                        emp->afficher();
                    }
                }
                pause();
                break;
            }
            case 2: {
                 printHeader("AJUSTER SALAIRE", subtitle);
                std::string id;
                double nouveauSalaire;

                std::cout << "ID de l'employé à modifier : ";
                std::cin >> id;

                Utilisateur* user = utilisateurManager.getUtilisateur(id);
                if(user) {
                    Employe* empTrouve = dynamic_cast<Employe*>(user);
                    if (empTrouve) {
                        std::cout << "Salaire actuel : " 
                                  << empTrouve->getSalaire() << " MAD\n";
                        std::cout << "Nouveau salaire : ";
                        std::cin >> nouveauSalaire;
                        empTrouve->setSalaire(nouveauSalaire);
                        printMessage("Salaire mis à jour !", UI::Colors::GREEN);
                    } else {
                        printMessage("L'utilisateur n'est pas un employe.", UI::Colors::RED);
                    }
                } else {
                    printMessage("Employé introuvable.", UI::Colors::RED);
                }
                pause();
                break;
            }
            case 3: {
                printHeader("LISTE DES CLIENTS", subtitle);
                for (const auto& u : utilisateurManager.getAllUtilisateurs()) {
                    if (auto* client = dynamic_cast<Client*>(u.get())) {
                        client->afficherProfil();
                    }
                }
                pause();
                break;
            }
            case 0:
                printMessage("Deconnexion du manager...", UI::Colors::YELLOW);
                break;
            default:
                printMessage("Choix invalide.", UI::Colors::RED);
                pause();
        }
    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuAdmin(AdminIT* admin, UtilisateurManager& utilisateurManager) {
    int choix = -1;
    do {
        printHeader("CONSOLE ADMIN IT", "Niveau: " + admin->getNiveauAcces());
        std::cout << "1. Creer un nouvel utilisateur (Employe/Manager/EmployeClient)" << std::endl;
        std::cout << "2. Supprimer un utilisateur" << std::endl;
        std::cout << "3. Voir tous les logs (Debug)" << std::endl;
        std::cout << std::endl;
        std::cout << "0. Deconnexion" << std::endl;
        printFooter();
        std::cout << "> Choix : ";
        std::cin >> choix;

        switch(choix) {
            case 1: {
                auto nouvelUtilisateur = admin->creerUtilisateur();
                if (nouvelUtilisateur) {
                    utilisateurManager.ajouterUtilisateur(std::move(nouvelUtilisateur));
                    printMessage("Confirmation: Utilisateur ajoute au systeme.", UI::Colors::GREEN);
                } else {
                    printMessage("L'operation de creation a echoue ou a ete annulee.", UI::Colors::YELLOW);
                }
                pause();
                break;
            }
            case 2: {
                std::string idASupprimer = admin->supprimerUtilisateur();
                if (!idASupprimer.empty()) {
                    if (utilisateurManager.supprimerUtilisateur(idASupprimer)) {
                        printMessage("Confirmation: Utilisateur " + idASupprimer + " supprime.", UI::Colors::GREEN);
                    } else {
                        printMessage("Echec de la suppression.", UI::Colors::RED);
                    }
                } else {
                    printMessage("L'operation de suppression a echoue ou a ete annulee.", UI::Colors::YELLOW);
                }
                pause();
                break;
            }
            case 3:
                std::cout << "Total Utilisateurs en RAM : " << utilisateurManager.getAllUtilisateurs().size() << "\n";
                pause();
                break;
        }
    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuCaissier(Employe* employe, UtilisateurManager& utilisateurManager, CompteManager& compteManager) {
    Caissier* caissier = dynamic_cast<Caissier*>(employe);
    if (!caissier) {
        printMessage("Erreur : L'employe n'est pas un caissier valide.", UI::Colors::RED);
        pause();
        return;
    }
    int choix = -1;
    do {
        std::string subtitle = caissier->getPrenom() + " " + caissier->getNom();
        printHeader("ESPACE CAISSIER", subtitle);

        std::cout << "1. Rechercher un Client par ID" << std::endl;
        std::cout << "2. Rechercher un Client par Nom/Prenom" << std::endl;
        std::cout << "3. Voir la liste de tous les Clients" << std::endl;
        std::cout << "4. Ajouter un nouveau Client" << std::endl;
        std::cout << "5. Ouvrir un Compte pour un Client" << std::endl;
        std::cout << std::endl;
        std::cout << "0. Deconnexion" << std::endl;
        printFooter();
        std::cout << "> Choix : ";
        std::cin >> choix;

        Client* clientSelectionne = nullptr;

        if (choix == 1) {
            std::string id;
            std::cout << "ID Client : ";
            std::cin >> id;
            clientSelectionne = utilisateurManager.getClientById(id);
            if (!clientSelectionne) printMessage("Client introuvable.", UI::Colors::RED);
        }
        else if (choix == 2) {
            std::string nom, prenom;
            std::cout << "Nom : "; std::cin >> nom;
            std::cout << "Prenom : "; std::cin >> prenom;
            clientSelectionne = utilisateurManager.getClient(nom, prenom);
            if (!clientSelectionne) printMessage("Client introuvable.", UI::Colors::RED);
        }
        else if (choix == 3) {
            printHeader("LISTE DE TOUS LES CLIENTS", subtitle);
            for (const auto& u : utilisateurManager.getAllUtilisateurs()) {
                if (auto* c = dynamic_cast<Client*>(u.get())) {
                    std::cout << "ID: " << c->getIdentifiant() 
                         << " | " << c->getNom() << " " << c->getPrenom() << std::endl;
                }
            }
            pause();
            continue;
        }

        if (clientSelectionne) {
            int sousChoix = -1;
            do {
                std::string clientSubtitle = clientSelectionne->getNom() + " " + clientSelectionne->getPrenom() + " (" + clientSelectionne->getIdentifiant() + ")";
                printHeader("MENU CLIENT (CAISSIER)", clientSubtitle);
                
                std::cout << "1. Voir les comptes" << std::endl;
                std::cout << "2. Effectuer un Depot" << std::endl;
                std::cout << "3. Effectuer un Retrait" << std::endl;
                std::cout << std::endl;
                std::cout << "0. Retour Recherche" << std::endl;
                printFooter();
                std::cout << "> Choix : ";
                std::cin >> sousChoix;

                const std::vector<Compte*>& comptes = clientSelectionne->getComptes();

                if (sousChoix == 1) {
                    if (comptes.empty()) {
                        printMessage("Ce client n'a aucun compte.", UI::Colors::YELLOW);
                    } else {
                        std::cout << "\n--- Comptes du Client ---\n";
                        for (auto* c : comptes) {
                            c->afficherInfo();
                        }
                    }
                    pause();
                }
                else if (sousChoix == 2 || sousChoix == 3) {
                    if (comptes.empty()) {
                        printMessage("Aucun compte pour effectuer une operation.", UI::Colors::YELLOW);
                        pause();
                    } else {
                        int index = 0;
                        std::cout << "\nSelectionnez le compte (1-" << comptes.size() << ") : ";
                        std::cin >> index;

                        if (index > 0 && index <= (int)comptes.size()) {
                            double montant;
                            std::cout << "Montant : ";
                            std::cin >> montant;

                            if (sousChoix == 2) {
                                comptes[index - 1]->deposer(montant);
                                printMessage("Depot effectue par le caissier.", UI::Colors::GREEN);
                            } else {
                                if (comptes[index - 1]->retirer(montant)) {
                                    printMessage("Retrait effectue par le caissier.", UI::Colors::GREEN);
                                } else {
                                    printMessage("Solde insuffisant pour ce retrait.", UI::Colors::RED);
                                }
                            }
                        } else {
                            printMessage("Compte invalide.", UI::Colors::RED);
                        }
                        pause();
                    }
                }

            } while (sousChoix != 0);
        } else if (choix == 4) {
            std::string n, p, d;
            printHeader("NOUVEAU CLIENT", subtitle);
            std::cout << "Nom: "; std::cin >> n;
            std::cout << "Prenom: "; std::cin >> p;
            std::cout << "Date Naissance: "; std::cin >> d;
            utilisateurManager.ajouterNouveauClient(n, p, d);
            pause();
        }
        else if (choix == 5) {
            std::string id, type; double dep;
            printHeader("NOUVEAU COMPTE", subtitle);
            std::cout << "ID Client: "; std::cin >> id;
            std::cout << "Type (Epargne/Courant): "; std::cin >> type;
            std::cout << "Depot Initial: "; std::cin >> dep;
            compteManager.ouvrirNouveauCompte(id, type, dep, utilisateurManager);
            pause();
        }
        else if (choix != 0 && choix != 3 && !clientSelectionne) { 
             pause();
        }

    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuEmployeClient(EmployeClient* ec, UtilisateurManager& utilisateurManager, CompteManager& compteManager) {
    int choix = -1;
    do {
        std::string subtitle = ec->Utilisateur::getPrenom() + " " + ec->Utilisateur::getNom();
        printHeader("ESPACE EMPLOYE-CLIENT", subtitle);
        
        std::cout << "1. Espace Client (Mes Comptes)" << std::endl;
        std::cout << "2. Espace Employe" << std::endl;
        std::cout << std::endl;
        std::cout << "0. Deconnexion" << std::endl;
        printFooter();
        std::cout << "> Choix : ";
        std::cin >> choix;

        if (choix == 1) {
            const std::vector<Compte*>& comptes = ec->getComptes();
            printHeader("VOS COMPTES PERSONNELS", subtitle);
            if (comptes.empty()) {
                printMessage("Aucun compte actif.", UI::Colors::YELLOW);
            } else {
                 for (size_t i = 0; i < comptes.size(); ++i) {
                    std::cout << "Compte n°" << (i + 1) << " : ";
                    comptes[i]->afficherInfo(); 
                }
            }
            pause();
        }
        else if (choix == 2) {
             std::string role = ec->getRoleSpecifique();
             
             if (role == "Manager" || role == "manager") {
                 afficherMenuManager(ec, utilisateurManager);
             }
             else if (role == "Caissier" || role == "caissier") {
                 afficherMenuCaissier(ec, utilisateurManager, compteManager);
             }
             else {
                 afficherMenuEmploye(ec, utilisateurManager, compteManager);
             }
        }

    } while (choix != 0);
}
