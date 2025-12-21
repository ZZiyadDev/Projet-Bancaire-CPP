#include "SystemeBancaire.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include "CompteProfessionnel.h"
#include "Manager.h"
#include <limits>
#include <typeinfo>

using namespace std;

// --- UTILS ---
void SystemeBancaire::clearScreen() {
    cout << string(50, '\n');
}

void SystemeBancaire::pause() {
    cout << "\nAppuyez sur Entree pour continuer...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --- CONSTRUCTOR / DESTRUCTOR ---

SystemeBancaire::SystemeBancaire() {
    std::cout << "[Systeme] Initialisation..." << std::endl;
    chargerDonnees(); // Load users/accounts from SQL

    if (utilisateurs.empty()) {
        std::cout << "[Systeme] Aucune donnee trouvee. Creation de l'Admin et du Manager par defaut." << std::endl;

        // --- Création Admin ---
        auto admin = std::make_unique<Employe>("Admin", "Root", 20000.0, "EMP001", "admin");
        utilisateurs.push_back(std::move(admin));

        std::string queryAdmin = 
            "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
            "VALUES ('EMP001', 'admin', 'Admin', 'Root', 'Employe', 20000, 'Admin');";
        BDManager::getInstance()->executeQuery(queryAdmin);

        // --- Création Manager ---
        auto manager = std::make_unique<Manager>("Martin", "Sophie", 15000.0, 0, "MNG001", "manager");
        utilisateurs.push_back(std::move(manager));

        std::string queryManager = 
            "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
            "VALUES ('MNG001', 'manager', 'Martin', 'Sophie', 'Employe', 15000, 'Manager');";
        BDManager::getInstance()->executeQuery(queryManager);

        std::cout << "[Systeme] Admin et Manager par defaut crees avec succes." << std::endl;

    }
}


SystemeBancaire::~SystemeBancaire() {
    // Le nettoyage est automatique grâce à std::unique_ptr
}

// --- DATA LOADING ---
void SystemeBancaire::chargerDonnees() {
    std::cout << "[Systeme] Chargement des donnees..." << std::endl;
    sqlite3* db = BDManager::getInstance()->getConnection();
    sqlite3_stmt* stmt = nullptr;

    // --- UTILISATEURS ---
    const char* qUsers = "SELECT id, nom, prenom, type, mdp, dateNaissance, salaire, role FROM Users";
    if (sqlite3_prepare_v2(db, qUsers, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {

            std::string id     = (const char*)sqlite3_column_text(stmt, 0);
            std::string nom    = (const char*)sqlite3_column_text(stmt, 1);
            std::string prenom = (const char*)sqlite3_column_text(stmt, 2);
            std::string type   = (const char*)sqlite3_column_text(stmt, 3);
            std::string mdp    = (const char*)sqlite3_column_text(stmt, 4);

            if (type == "Client") {
                const char* dN = (const char*)sqlite3_column_text(stmt, 5);
                std::string dateN = dN ? dN : "Inconnue";
                utilisateurs.push_back(std::make_unique<Client>(id, mdp, nom, prenom, dateN));
            } 
            else if (type == "Employe") {
                double salaire = sqlite3_column_double(stmt, 6);
                const char* roleTxt = (const char*)sqlite3_column_text(stmt, 7);
                std::string role = roleTxt ? roleTxt : "Employe";

                if (role == "Manager") {
                    utilisateurs.push_back(
                        std::make_unique<Manager>(nom, prenom, salaire, 0, id, mdp)
                    );
                } else {
                    utilisateurs.push_back(
                        std::make_unique<Employe>(nom, prenom, salaire, id, mdp)
                    );
                }
            }

        } // end while
        sqlite3_finalize(stmt);
    } else {
        std::cout << "[Systeme] Erreur lors de la lecture des utilisateurs.\n";
    }

    // --- COMPTES ---
    const char* qComptes = "SELECT numCompte, userId, typeCompte, solde, taux, decouvert FROM Comptes";
    if (sqlite3_prepare_v2(db, qComptes, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string num = (const char*)sqlite3_column_text(stmt, 0);
            std::string userId = (const char*)sqlite3_column_text(stmt, 1);
            std::string type = (const char*)sqlite3_column_text(stmt, 2);
            double solde = sqlite3_column_double(stmt, 3);

            Client* proprietaire = trouverClientParId(userId);
            if (!proprietaire) continue;

            std::string nomTitulaire = proprietaire->getPrenom() + " " + proprietaire->getNom();
            std::unique_ptr<Compte> compte = nullptr;

            if (type == "Epargne") {
                double taux = sqlite3_column_double(stmt, 4);
                compte = std::make_unique<CompteEpargne>(nomTitulaire, solde, taux);
            } else {
                double dec = sqlite3_column_double(stmt, 5);
                compte = std::make_unique<CompteCourant>(nomTitulaire, solde, dec);
            }

            if (compte) {
                compte->setNumCompte(num);
                proprietaire->ajouterCompte(compte.get());
                listeComptes.push_back(std::move(compte));
            }
        }
        sqlite3_finalize(stmt);
    }
}


// --- AUTHENTICATION ---
Utilisateur* SystemeBancaire::authentifier(const string& login, const string& pass) {
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == login && u->verifierMotDePasse(pass)) {
            return u.get(); // Return raw pointer for observation
        }
    }
    return nullptr;
}

// --- MAIN LOOP ---
void SystemeBancaire::operations() {
    bool applicationActive = true;

    while (applicationActive) {
        clearScreen();
        cout << "==========================================" << endl;
        cout << "      BANQUE CPP - CONNEXION              " << endl;
        cout << "==========================================" << endl;

        string login, pass;
        cout << "\nIdentifiant (ou 'exit') : ";
        cin >> login;

        if (login == "exit") break;

        cout << "Mot de passe : ";
        cin >> pass;

        Utilisateur* user = authentifier(login, pass);

        if (user) {
            cout << "\nConnexion reussie : " << user->getPrenom() << " " << user->getNom() << endl;
            // Polymorphic Dispatch
            if (auto* client = dynamic_cast<Client*>(user)) {
                sessionClient(client);
            } 
            else if (auto* manager = dynamic_cast<Manager*>(user)) {
                sessionManager(manager); // New: manager-specific session
            }
            else if (auto* emp = dynamic_cast<Employe*>(user)) {
                sessionEmploye(emp);
            } 
            else {
                std::cout << "\n[!] Profil non reconnu." << std::endl;
                pause();
            }
        } else {
            cout << "\n[!] Identifiants incorrects." << endl;
            pause();
        }
    }

    cout << "Fermeture de l'application." << endl; // Moved outside the loop
}


// --- CLIENT SESSION (Logic moved from old main.cpp) ---
void SystemeBancaire::sessionClient(Client* client) {
    int choix = -1;
    do {
        clearScreen();
        // Affichage du menu amélioré
        std::cout << "=== ESPACE CLIENT : " << client->getPrenom() << " " << client->getNom() << " ===\n";
        std::cout << "1. Voir mes Comptes\n";
        std::cout << "2. Faire un Depot\n";
        std::cout << "3. Faire un Retrait\n";
        std::cout << "4. Faire un Virement\n";
        std::cout << "5. Consulter l'Historique\n";
        std::cout << "0. Deconnexion\n";
        std::cout << "Choix : ";
        
        // Sécurisation de l'entrée (évite les bugs si on tape une lettre)
        if (!(std::cin >> choix)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Récupération des comptes du client
        const std::vector<Compte*>& comptes = client->getComptes();
            int choix = -1;
        if (choix == 1) {
            std::cout << "\n--- Vos Comptes ---\n";
            if (comptes.empty()) {
                std::cout << "Aucun compte actif.\n";
            } else {
                for (size_t i = 0; i < comptes.size(); ++i) {
                    std::cout << "Compte n°" << (i + 1) << " : ";
                    comptes[i]->afficherInfo(); 
                }
            }
        }
        else if (choix == 2 || choix == 3) { // DEPOT ou RETRAIT
            if (comptes.empty()) {
                std::cout << "Erreur : Vous n'avez aucun compte.\n";
            } else {
                int index = 0;
                std::cout << "\nSur quel compte ? (Entrez le numero 1, 2...) : ";
                std::cin >> index;
                
                // Vérification que le numéro est valide
                if (index > 0 && index <= (int)comptes.size()) {
                    double montant;
                    std::cout << "Montant : ";
                    std::cin >> montant;
                    
                    if (choix == 2) {
                        // Action DEPOT
                        comptes[index - 1]->deposer(montant);
                    } else {
                        // Action RETRAIT
                        // La méthode retirer() renvoie false si solde insuffisant
                        if (comptes[index - 1]->retirer(montant)) {
                            std::cout << "Retrait effectue avec succes.\n";
                        } else {
                            std::cout << "Erreur : Solde insuffisant ou montant invalide.\n";
                        }
                    }
                } else {
                    std::cout << "Numero de compte invalide.\n";
                }
            }
        }
        else if (choix == 4) { // VIREMENT
            if (comptes.empty()) {
                std::cout << "Erreur : Il vous faut un compte source.\n";
            } else {
                // 1. Choisir le compte source
                int indexSrc = 0;
                std::cout << "Depuis quel compte ? (1-" << comptes.size() << ") : ";
                std::cin >> indexSrc;
                
                if (indexSrc > 0 && indexSrc <= (int)comptes.size()) {
                    // 2. Choisir le destinataire (ID du compte, ex: "CPT_123")
                    std::string destID;
                    std::cout << "ID du compte destinataire : ";
                    std::cin >> destID;
                    
                    // On cherche le compte destinataire dans TOUTE la banque
                    Compte* destCompte = trouverCompte(destID); // Utilise la méthode de SystemeBancaire

                    if (destCompte != nullptr) {
                        double montant;
                        std::cout << "Montant du virement : ";
                        std::cin >> montant;
                        
                        // Exécution du virement
                        if (comptes[indexSrc - 1]->virementVers(*destCompte, montant)) {
                            // Le message de succès est souvent déjà dans virementVers, 
                            // mais on peut confirmer ici si besoin.
                        }
                    } else {
                        std::cout << "Erreur : Compte destinataire introuvable.\n";
                    }
                } else {
                     std::cout << "Compte source invalide.\n";
                }
            }
        }
        if (choix == 5) {
            if (comptes.empty()) {
                std::cout << "Aucun compte disponible.\n";
            } else {
                std::cout << "\n--- Choix du Compte pour Historique ---\n";
                for (size_t i = 0; i < comptes.size(); ++i) {
                    // CORRECTION ICI : On utilise afficherInfo() au lieu de getType()
                    std::cout << (i + 1) << ". "; 
                    comptes[i]->afficherInfo(); 
                }
                
                int index = 0;
                std::cout << "Votre choix : ";
                std::cin >> index;

                if (index > 0 && index <= (int)comptes.size()) {
                    comptes[index - 1]->afficherHistorique();
                } else {
                    std::cout << "Compte invalide.\n";
                }
            }
        }
        
        if (choix != 0) pause();
    } while (choix!= 0);
}

// --- EMPLOYEE SESSION ---
void SystemeBancaire::sessionEmploye(Employe* employe) {
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
            ajouterNouveauClient(n, p, d);
        }
        else if (choix == 2) {
            string id, type; double dep;
            cout << "ID Client: "; cin >> id;
            cout << "Type (Epargne/Courant): "; cin >> type;
            cout << "Depot: "; cin >> dep;
            ouvrirNouveauCompte(id, type, dep);
        }

        if (choix != 0) pause();
    } while (choix != 0);
}

// --- CREATION METHODS ---
std::string SystemeBancaire::genererIdClient() {
    return "CLI" + std::to_string(utilisateurs.size() + 1);
}

void SystemeBancaire::ajouterNouveauClient(string nom, string prenom, string dateN) {
    string id = genererIdClient();
    auto newClient = std::make_unique<Client>(id, "1234", nom, prenom, dateN); // Default password
    
    // Save to DB
    string query = "INSERT INTO Users (id, mdp, nom, prenom, type, dateNaissance) VALUES ('" 
                   + id + "', '1234', '" + nom + "', '" + prenom + "', 'Client', '" + dateN + "');";
    
    if (BDManager::getInstance()->executeQuery(query)) {
        utilisateurs.push_back(std::move(newClient));
        cout << "Client cree avec ID : " << id << endl;
    } else {
        cout << "Erreur SQL: Impossible de creer le client." << endl;
        // newClient is automatically deleted when it goes out of scope
    }
}

bool SystemeBancaire::ouvrirNouveauCompte(std::string idClient, std::string typeCompte, double depotInitial) {
    Client* target = trouverClientParId(idClient);
    if (!target) {
        std::cout << "Client introuvable.\n";
        return false;
    }

    std::string nomTitulaire = target->getPrenom() + " " + target->getNom();

    std::unique_ptr<Compte> compte = nullptr;
    if (typeCompte == "Epargne") 
        compte = std::make_unique<CompteEpargne>(nomTitulaire, depotInitial, 0.05);
    else 
        compte = std::make_unique<CompteCourant>(nomTitulaire, depotInitial, 200.0);

    if (compte) {
        std::string uniqueID = "CPT" + std::to_string(std::time(nullptr)) + std::to_string(rand() % 1000);
        compte->setNumCompte(uniqueID);

        std::string q = "INSERT INTO Comptes (numCompte, userId, typeCompte, solde) VALUES ('"
                   + uniqueID + "', '" + idClient + "', '" + typeCompte + "', " 
                   + std::to_string(depotInitial) + ");";
        
        if (BDManager::getInstance()->executeQuery(q)) {
            target->ajouterCompte(compte.get()); // Pass non-owning pointer
            listeComptes.push_back(std::move(compte)); // Transfer ownership to system
            std::cout << "Compte cree avec succes (ID: " << uniqueID << ")\n";
            return true;
        } else {
            std::cout << "Erreur SQL : Impossible de creer le compte.\n";
            return false; // 'compte' is auto-deleted here
        }
    }
    return false;
}

Compte* SystemeBancaire::trouverCompte(std::string numCompte) {
    for (const auto& compte : listeComptes) {
        if (compte->getNumCompte() == numCompte) {
            return compte.get();
        }
    }
    return nullptr;
}

Client* SystemeBancaire::trouverClientParId(const std::string& idClient) {
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == idClient) {
            return dynamic_cast<Client*>(u.get()); 
        }
    }
    return nullptr;
}

Client* SystemeBancaire::trouverClient(std::string nom, std::string prenom) {
    for (const auto& u : utilisateurs) {
        if (u->getNom() == nom && u->getPrenom() == prenom) {
            return dynamic_cast<Client*>(u.get());
        }
    }
    return nullptr;
}


void SystemeBancaire::sessionManager(Manager* manager) {
    int choix = -1;

    do {
        clearScreen();
        std::cout << "=== ESPACE MANAGER : " 
                  << manager->getPrenom() << " " 
                  << manager->getNom() << " ===\n";
        std::cout << "1. Voir la liste des employés\n";
        std::cout << "2. Ajuster le salaire d'un employé\n";
        std::cout << "3. Voir tous les clients\n";
        std::cout << "0. Deconnexion\n";
        std::cout << "Votre choix : ";

        if (!(std::cin >> choix)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choix) {

            case 1: {
                std::cout << "\n--- Liste des Employes ---\n";
                for (const auto& u : utilisateurs) {
                    if (auto* emp = dynamic_cast<Employe*>(u.get())) {
                        emp->afficher();
                    }
                }
                pause();
                break;
            }

            case 2: {
                std::string id;
                double nouveauSalaire;

                std::cout << "ID de l'employé à modifier : ";
                std::cin >> id;

                Employe* empTrouve = nullptr;

                for (auto& u : utilisateurs) {
                    if (auto* emp = dynamic_cast<Employe*>(u.get())) {
                        if (emp->getIdentifiant() == id) {
                            empTrouve = emp;
                            break;
                        }
                    }
                }

                if (empTrouve) {
                    std::cout << "Salaire actuel : " 
                              << empTrouve->getSalaire() << " MAD\n";
                    std::cout << "Nouveau salaire : ";
                    std::cin >> nouveauSalaire;
                    empTrouve->setSalaire(nouveauSalaire);
                    std::cout << "Salaire mis à jour !\n";
                } else {
                    std::cout << "Employé introuvable.\n";
                }

                pause();
                break;
            }

            case 3: {
                std::cout << "\n--- Liste des Clients ---\n";
                for (const auto& u : utilisateurs) {
                    if (auto* client = dynamic_cast<Client*>(u.get())) {
                        client->afficherProfil();
                    }
                }
                pause();
                break;
            }

            case 0:
                std::cout << "Deconnexion du manager...\n";
                break;

            default:
                std::cout << "Choix invalide.\n";
                pause();
        }

    } while (choix != 0);
}

