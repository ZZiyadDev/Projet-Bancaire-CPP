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

using namespace std;

void InterfaceUtilisateur::clearScreen() {
    cout << string(50, '\n');
}

void InterfaceUtilisateur::pause() {
    cout << "\nAppuyez sur Entree pour continuer...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void InterfaceUtilisateur::afficherMenuClient(Client* client, CompteManager& compteManager) {
    int choix = -1;
    do {
        clearScreen();
        cout << "=== ESPACE CLIENT : " << client->getPrenom() << " " << client->getNom() << " ===\n";
        cout << "1. Voir mes Comptes\n";
        cout << "2. Faire un Depot\n";
        cout << "3. Faire un Retrait\n";
        cout << "4. Faire un Virement\n";
        cout << "5. Consulter l'Historique\n";
        cout << "0. Deconnexion\n";
        cout << "Choix : ";
        
        if (!(cin >> choix)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        const vector<Compte*>& comptes = client->getComptes();

        if (choix == 1) {
            cout << "\n--- Vos Comptes ---\n";
            if (comptes.empty()) {
                cout << "Aucun compte actif.\n";
            } else {
                for (size_t i = 0; i < comptes.size(); ++i) {
                    cout << "Compte n°" << (i + 1) << " : ";
                    comptes[i]->afficherInfo(); 
                }
            }
        }
        else if (choix == 2 || choix == 3) { // DEPOT ou RETRAIT
            if (comptes.empty()) {
                cout << "Erreur : Vous n'avez aucun compte.\n";
            } else {
                int index = 0;
                cout << "\nSur quel compte ? (Entrez le numero 1, 2...) : ";
                cin >> index;
                
                if (index > 0 && index <= (int)comptes.size()) {
                    double montant;
                    cout << "Montant : ";
                    cin >> montant;
                    
                    if (choix == 2) {
                        comptes[index - 1]->deposer(montant);
                    } else {
                        if (comptes[index - 1]->retirer(montant)) {
                            cout << "Retrait effectue avec succes.\n";
                        } else {
                            cout << "Erreur : Solde insuffisant ou montant invalide.\n";
                        }
                    }
                } else {
                    cout << "Numero de compte invalide.\n";
                }
            }
        }
        else if (choix == 4) { // VIREMENT
            if (comptes.empty()) {
                cout << "Erreur : Il vous faut un compte source.\n";
            } else {
                int indexSrc = 0;
                cout << "Depuis quel compte ? (1-" << comptes.size() << ") : ";
                cin >> indexSrc;
                
                if (indexSrc > 0 && indexSrc <= (int)comptes.size()) {
                    string destID;
                    cout << "ID du compte destinataire : ";
                    cin >> destID;
                    
                    Compte* destCompte = compteManager.trouverCompte(destID);

                    if (destCompte != nullptr) {
                        double montant;
                        cout << "Montant du virement : ";
                        cin >> montant;
                        
                        comptes[indexSrc - 1]->virementVers(*destCompte, montant);
                    } else {
                        cout << "Erreur : Compte destinataire introuvable.\n";
                    }
                } else {
                     cout << "Compte source invalide.\n";
                }
            }
        }
        if (choix == 5) {
            if (comptes.empty()) {
                cout << "Aucun compte disponible.\n";
            } else {
                cout << "\n--- Choix du Compte pour Historique ---\n";
                for (size_t i = 0; i < comptes.size(); ++i) {
                    cout << (i + 1) << ". "; 
                    comptes[i]->afficherInfo(); 
                }
                
                int index = 0;
                cout << "Votre choix : ";
                cin >> index;

                if (index > 0 && index <= (int)comptes.size()) {
                    comptes[index - 1]->afficherHistorique();
                } else {
                    cout << "Compte invalide.\n";
                }
            }
        }
        
        if (choix != 0) pause();
    } while (choix!= 0);
}

void InterfaceUtilisateur::afficherMenuEmploye(Employe* employe, UtilisateurManager& utilisateurManager, CompteManager& compteManager) {
    int choix = -1;
    do {
        clearScreen();
        cout << "=== ESPACE EMPLOYE : " << employe->getRole() << " ===" << endl;
        cout << "1. Creer un Client" << endl;
        cout << "2. Ouvrir un Compte" << endl;
        cout << "0. Deconnexion" << endl;
        cout << "Choix : ";
        cin >> choix;

        if (choix == 1) {
            string n, p, d;
            cout << "Nom: "; cin >> n;
            cout << "Prenom: "; cin >> p;
            cout << "Date Naissance: "; cin >> d;
            utilisateurManager.ajouterNouveauClient(n, p, d);
        }
        else if (choix == 2) {
            string id, type; double dep;
            cout << "ID Client: "; cin >> id;
            cout << "Type (Epargne/Courant): "; cin >> type;
            cout << "Depot: "; cin >> dep;
            compteManager.ouvrirNouveauCompte(id, type, dep, utilisateurManager);
        }

        if (choix != 0) pause();
    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuManager(Employe* employe, UtilisateurManager& utilisateurManager) {
    Manager* manager = dynamic_cast<Manager*>(employe);
    if (!manager) {
        cout << "Erreur : L'employe n'est pas un manager valide." << endl;
        pause();
        return;
    }
    int choix = -1;

    do {
        clearScreen();
        cout << "=== ESPACE MANAGER : " 
                  << manager->getPrenom() << " " 
                  << manager->getNom() << " ===\n";
        cout << "1. Voir la liste des employés\n";
        cout << "2. Ajuster le salaire d'un employé\n";
        cout << "3. Voir tous les clients\n";
        cout << "0. Deconnexion\n";
        cout << "Votre choix : ";

        if (!(cin >> choix)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choix) {

            case 1: {
                cout << "\n--- Liste des Employes ---\n";
                for (const auto& u : utilisateurManager.getAllUtilisateurs()) {
                    if (auto* emp = dynamic_cast<Employe*>(u.get())) {
                        emp->afficher();
                    }
                }
                pause();
                break;
            }

            case 2: {
                string id;
                double nouveauSalaire;

                cout << "ID de l'employé à modifier : ";
                cin >> id;

                Utilisateur* user = utilisateurManager.getUtilisateur(id);
                if(user) {
                    Employe* empTrouve = dynamic_cast<Employe*>(user);
                    if (empTrouve) {
                        cout << "Salaire actuel : " 
                                  << empTrouve->getSalaire() << " MAD\n";
                        cout << "Nouveau salaire : ";
                        cin >> nouveauSalaire;
                        empTrouve->setSalaire(nouveauSalaire);
                        cout << "Salaire mis à jour !\n";
                    } else {
                        cout << "L'utilisateur n'est pas un employe.\n";
                    }
                } else {
                    cout << "Employé introuvable.\n";
                }

                pause();
                break;
            }

            case 3: {
                cout << "\n--- Liste des Clients ---\n";
                for (const auto& u : utilisateurManager.getAllUtilisateurs()) {
                    if (auto* client = dynamic_cast<Client*>(u.get())) {
                        client->afficherProfil();
                    }
                }
                pause();
                break;
            }

            case 0:
                cout << "Deconnexion du manager...\n";
                break;

            default:
                cout << "Choix invalide.\n";
                pause();
        }

    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuAdmin(AdminIT* admin, UtilisateurManager& utilisateurManager) {
    int choix = -1;
    do {
        clearScreen();
        cout << "=== CONSOLE ADMIN IT (" << admin->getNiveauAcces() << ") ===\n";
        cout << "1. Creer un nouvel utilisateur (Employe/Manager)\n";
        cout << "2. Supprimer un utilisateur\n";
        cout << "3. Voir tous les logs (Debug)\n";
        cout << "0. Deconnexion\n";
        cout << "Choix : ";
        cin >> choix;

        switch(choix) {
            case 1: {
                auto nouvelUtilisateur = admin->creerUtilisateur();
                if (nouvelUtilisateur) {
                    utilisateurManager.ajouterUtilisateur(move(nouvelUtilisateur));
                    cout << "Confirmation: Utilisateur ajoute au systeme." << endl;
                } else {
                    cout << "L'operation de creation a echoue ou a ete annulee." << endl;
                }
                pause();
                break;
            }
            case 2: {
                std::string idASupprimer = admin->supprimerUtilisateur();
                if (!idASupprimer.empty()) {
                    if (utilisateurManager.supprimerUtilisateur(idASupprimer)) {
                        cout << "Confirmation: Utilisateur " << idASupprimer << " supprime." << endl;
                    } else {
                        cout << "Echec de la suppression." << endl;
                    }
                } else {
                    cout << "L'operation de suppression a echoue ou a ete annulee." << endl;
                }
                pause();
                break;
            }
            case 3:
                cout << "Total Utilisateurs en RAM : " << utilisateurManager.getAllUtilisateurs().size() << "\n";
                pause();
                break;
        }
    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuCaissier(Employe* employe, UtilisateurManager& utilisateurManager, CompteManager& compteManager) {
    Caissier* caissier = dynamic_cast<Caissier*>(employe);
    if (!caissier) {
        cout << "Erreur : L'employe n'est pas un caissier valide." << endl;
        pause();
        return;
    }
    int choix = -1;
    do {
        clearScreen();
        cout << "=== ESPACE CAISSIER : " << caissier->getPrenom() << " " << caissier->getNom() << " ===\n";
        cout << "1. Rechercher un Client par ID\n";
        cout << "2. Rechercher un Client par Nom/Prenom\n";
        cout << "3. Voir la liste de tous les Clients\n";
        cout << "4. Ajouter un nouveau Client\n";
        cout << "5. Ouvrir un Compte pour un Client\n";
        cout << "0. Deconnexion\n";
        cout << "Choix : ";
        cin >> choix;

        Client* clientSelectionne = nullptr;

        if (choix == 1) {
            string id;
            cout << "ID Client : ";
            cin >> id;
            clientSelectionne = utilisateurManager.getClientById(id);
            if (!clientSelectionne) cout << "Client introuvable.\n";
        }
        else if (choix == 2) {
            string nom, prenom;
            cout << "Nom : "; cin >> nom;
            cout << "Prenom : "; cin >> prenom;
            clientSelectionne = utilisateurManager.getClient(nom, prenom);
            if (!clientSelectionne) cout << "Client introuvable.\n";
        }
        else if (choix == 3) {
            clearScreen();
            cout << "\n--- Liste des Clients ---\n";
            for (const auto& u : utilisateurManager.getAllUtilisateurs()) {
                if (auto* c = dynamic_cast<Client*>(u.get())) {
                    cout << "ID: " << c->getIdentifiant() 
                         << " | " << c->getNom() << " " << c->getPrenom() << endl;
                }
            }
            pause();
            continue;
        }

        if (clientSelectionne) {
            int sousChoix = -1;
            do {
                clearScreen();
                cout << "--- Client : " << clientSelectionne->getNom() << " " << clientSelectionne->getPrenom() << " (" << clientSelectionne->getIdentifiant() << ") ---\n";
                cout << "1. Voir les comptes\n";
                cout << "2. Effectuer un Depot\n";
                cout << "3. Effectuer un Retrait\n";
                cout << "0. Retour Recherche\n";
                cout << "Choix : ";
                cin >> sousChoix;

                const vector<Compte*>& comptes = clientSelectionne->getComptes();

                if (sousChoix == 1) {
                    if (comptes.empty()) {
                        cout << "Ce client n'a aucun compte.\n";
                    } else {
                        cout << "\n--- Comptes du Client ---\n";
                        for (auto* c : comptes) {
                            c->afficherInfo();
                        }
                    }
                    pause();
                }
                else if (sousChoix == 2 || sousChoix == 3) {
                    if (comptes.empty()) {
                        cout << "Aucun compte pour effectuer une operation.\n";
                        pause();
                    } else {
                        int index = 0;
                        cout << "\nSelectionnez le compte (1-" << comptes.size() << ") : ";
                        cin >> index;

                        if (index > 0 && index <= (int)comptes.size()) {
                            double montant;
                            cout << "Montant : ";
                            cin >> montant;

                            if (sousChoix == 2) {
                                comptes[index - 1]->deposer(montant);
                                cout << "Depot effectue par le caissier.\n";
                            } else {
                                if (comptes[index - 1]->retirer(montant)) {
                                    cout << "Retrait effectue par le caissier.\n";
                                } else {
                                    cout << "Solde insuffisant pour ce retrait.\n";
                                }
                            }
                        } else {
                            cout << "Compte invalide.\n";
                        }
                        pause();
                    }
                }

            } while (sousChoix != 0);
        } else if (choix == 4) {
            string n, p, d;
            cout << "\n--- Nouveau Client ---\n";
            cout << "Nom: "; cin >> n;
            cout << "Prenom: "; cin >> p;
            cout << "Date Naissance: "; cin >> d;
            utilisateurManager.ajouterNouveauClient(n, p, d);
            pause();
        }
        else if (choix == 5) {
            string id, type; double dep;
            cout << "\n--- Nouveau Compte ---\n";
            cout << "ID Client: "; cin >> id;
            cout << "Type (Epargne/Courant): "; cin >> type;
            cout << "Depot Initial: "; cin >> dep;
            compteManager.ouvrirNouveauCompte(id, type, dep, utilisateurManager);
            pause();
        }
        else if (choix != 0 && choix != 3) { 
             pause();
        }

    } while (choix != 0);
}

void InterfaceUtilisateur::afficherMenuEmployeClient(EmployeClient* ec, UtilisateurManager& utilisateurManager, CompteManager& compteManager) {
    int choix = -1;
    do {
        clearScreen();
        cout << "=== ESPACE EMPLOYE-CLIENT : " << ec->Utilisateur::getPrenom() << " " << ec->Utilisateur::getNom() << " ===\n";
        cout << "1. Espace Client (Mes Comptes)\n";
        cout << "2. Espace Employe\n";
        cout << "0. Deconnexion\n";
        cout << "Choix : ";
        cin >> choix;

        if (choix == 1) {
            const vector<Compte*>& comptes = ec->getComptes();
            cout << "\n--- Vos Comptes Personnels ---\n";
            if (comptes.empty()) {
                cout << "Aucun compte actif.\n";
            } else {
                 for (size_t i = 0; i < comptes.size(); ++i) {
                    cout << "Compte n°" << (i + 1) << " : ";
                    comptes[i]->afficherInfo(); 
                }
            }
            pause();
        }
        else if (choix == 2) {
             string role = ec->getRoleSpecifique();
             cout << "\n--- Acces Espace Employe (" << role << ") ---\n";
             
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
