#include "SystemeBancaire.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include "CompteProfessionnel.h"
#include "Employe.h"
#include "Manager.h"
#include "Caissier.h"
#include "EmployeClient.h"
#include "BDManager.h"
#include "EmployeClient.h"
#include "AdminIT.h"
#include <limits>
#include <typeinfo>
#include <algorithm>

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
    cout << "[Systeme] Initialisation..." << endl;
    chargerDonnees(); // Load users/accounts from SQL

    if (utilisateurs.empty()) {
        cout << "[Systeme] Aucune donnee trouvee. Creation de l'Admin et du Manager par defaut." << endl;

        // --- Création Admin ---
        auto admin = make_unique<AdminIT>("Admin", "Root", 20000.0, "RootAccess", "EMP001", "admin");
        utilisateurs.push_back(move(admin));

        string queryAdmin = 
            "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
            "VALUES ('EMP001', 'admin', 'Admin', 'Root', 'Employe', 20000, 'Admin');";
        BDManager::getInstance()->executeQuery(queryAdmin);

        // --- Création Manager ---
        auto manager = make_unique<Manager>("Martin", "Sophie", 15000.0, 0, "MNG001", "manager");
        utilisateurs.push_back(move(manager));

        string queryManager = 
            "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
            "VALUES ('MNG001', 'manager', 'Martin', 'Sophie', 'Employe', 15000, 'Manager');";
        BDManager::getInstance()->executeQuery(queryManager);


        string queryCaissier = 
            "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
            "VALUES ('CAIS001', 'caissier', 'Dupont', 'Jean', 'Employe', 2500, 'Caissier');";
        BDManager::getInstance()->executeQuery(queryCaissier);

        cout << "[Systeme] Admin, Manager et Caissier par defaut crees avec succes." << endl;
    }

    // --- Verification Caissier (Force Creation if missing) ---
    bool caissierExists = false;
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == "CAIS001") {
            caissierExists = true;
            break;
        }
    }

    if (!caissierExists) {
         cout << "[Systeme] Ajout du Caissier par defaut (manquant)..." << endl;
         auto caissier = make_unique<Caissier>("Dupont", "Jean", 2500.0, "CAIS001", "caissier");
         utilisateurs.push_back(move(caissier));

         string queryCaissier = 
            "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
            "VALUES ('CAIS001', 'caissier', 'Dupont', 'Jean', 'Employe', 2500, 'Caissier');";
         BDManager::getInstance()->executeQuery(queryCaissier);
    }

    // --- Verification EmployeClient (Default for Test) ---
    bool ecExists = false;
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == "EMPCLI001") {
            ecExists = true;
            break;
        }
    }

    if (!ecExists) {
        cout << "[Systeme] Ajout de l'Employe-Client par defaut..." << endl;
        auto ec = make_unique<EmployeClient>("Dubois", "Paul", 3000.0, "EMPCLI001", "123", "Caissier");
        utilisateurs.push_back(move(ec));
        
        string queryEC = 
            "INSERT INTO Users (id, mdp, nom, prenom, type, salaire, role) "
            "VALUES ('EMPCLI001', '123', 'Dubois', 'Paul', 'EmployeClient', 3000, 'Caissier');";
        BDManager::getInstance()->executeQuery(queryEC);
    }
}


SystemeBancaire::~SystemeBancaire() {
    // Le nettoyage est automatique grâce à unique_ptr
}

// --- DATA LOADING ---
void SystemeBancaire::chargerDonnees() {
    cout << "[Systeme] Chargement des donnees..." << endl;
    sqlite3* db = BDManager::getInstance()->getConnection();
    sqlite3_stmt* stmt = nullptr;

    // --- UTILISATEURS ---
    const char* qUsers = "SELECT id, nom, prenom, type, mdp, dateNaissance, salaire, role FROM Users";
    if (sqlite3_prepare_v2(db, qUsers, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {

            string id     = (const char*)sqlite3_column_text(stmt, 0);
            string nom    = (const char*)sqlite3_column_text(stmt, 1);
            string prenom = (const char*)sqlite3_column_text(stmt, 2);
            string type   = (const char*)sqlite3_column_text(stmt, 3);
            string mdp    = (const char*)sqlite3_column_text(stmt, 4);

            if (type == "Client") {
                const char* dN = (const char*)sqlite3_column_text(stmt, 5);
                string dateN = dN ? dN : "Inconnue";
                utilisateurs.push_back(make_unique<Client>(id, mdp, nom, prenom, dateN));
            } 
            else if (type == "Employe") {
                double salaire = sqlite3_column_double(stmt, 6);
                const char* roleTxt = (const char*)sqlite3_column_text(stmt, 7);
                string role = roleTxt ? roleTxt : "Employe";

            if (role == "Admin") {
            // We create an AdminIT object. 
            // Note: Your constructor requires 'niveauAcces'. We'll pass "SuperUser" or the role.
            utilisateurs.push_back(
            make_unique<AdminIT>(nom, prenom, salaire, "SuperUser", id, mdp)
            );
            }
            else if (role == "Manager") {
            utilisateurs.push_back(
            make_unique<Manager>(nom, prenom, salaire, 0, id, mdp)
            );
            }
            else if (role == "Caissier") {
                utilisateurs.push_back(
                    make_unique<Caissier>(nom, prenom, salaire, id, mdp)
                );
            }
            else {
            utilisateurs.push_back(
            make_unique<Employe>(nom, prenom, salaire, id, mdp)
            );
            }
            }

        } // end while
        sqlite3_finalize(stmt);
    } else {
        cout << "[Systeme] Erreur lors de la lecture des utilisateurs.\n";
    }

    // --- COMPTES ---
    const char* qComptes = "SELECT numCompte, userId, typeCompte, solde, taux, decouvert FROM Comptes";
    if (sqlite3_prepare_v2(db, qComptes, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string num = (const char*)sqlite3_column_text(stmt, 0);
            string userId = (const char*)sqlite3_column_text(stmt, 1);
            string type = (const char*)sqlite3_column_text(stmt, 2);
            double solde = sqlite3_column_double(stmt, 3);

            Client* proprietaire = trouverClientParId(userId);
            if (!proprietaire) continue;

            string nomTitulaire = proprietaire->getPrenom() + " " + proprietaire->getNom();
            unique_ptr<Compte> compte = nullptr;

            if (type == "Epargne") {
                double taux = sqlite3_column_double(stmt, 4);
                compte = make_unique<CompteEpargne>(nomTitulaire, solde, taux);
            } else {
                double dec = sqlite3_column_double(stmt, 5);
                compte = make_unique<CompteCourant>(nomTitulaire, solde, dec);
            }

            if (compte) {
                compte->setNumCompte(num);
                
                // Check if owner is Client or EmployeClient
                if (proprietaire) {
                    proprietaire->ajouterCompte(compte.get());
                } else {
                    // Try to find EmployeClient
                     for (const auto& u : utilisateurs) {
                        if (auto* ec = dynamic_cast<EmployeClient*>(u.get())) {
                            if (ec->getIdentifiant() == userId) {
                                ec->ajouterCompte(compte.get());
                                break;
                            }
                        }
                    }
                }

                listeComptes.push_back(move(compte));
            }
        }
        sqlite3_finalize(stmt);
    }
}


// --- AUTHENTICATION ---
// Replace the existing authentifier method with this temporarily
Utilisateur* SystemeBancaire::authentifier(const string& login, const string& pass) {
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == login && u->verifierMotDePasse(pass)) {
            return u.get();
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

            // 1. Check for ADMIN IT
            if (auto* admin = dynamic_cast<AdminIT*>(user)) {
                sessionAdmin(admin); 
            }
            // 2. Check for MANAGER
            else if (auto* manager = dynamic_cast<Manager*>(user)) {
                sessionManager(manager);
            }
            // 3. Check for CAISSIER
            else if (auto* caissier = dynamic_cast<Caissier*>(user)) {
                sessionCaissier(caissier);
            }
            // 4. Check for CLIENT
            else if (auto* client = dynamic_cast<Client*>(user)) {
                sessionClient(client);
            }
            // 5. Check for EmployeClient
            else if (auto* ec = dynamic_cast<EmployeClient*>(user)) {
                sessionEmployeClient(ec);
            }
            // 6. Check for EMPLOYE (Default fallback)
            else if (auto* emp = dynamic_cast<Employe*>(user)) {
                sessionEmploye(emp);
            }
            else {
                 cout << "Erreur: Type d'utilisateur inconnu." << endl;
                 pause();
            }

        } else {
            // This 'else' belongs to 'if (user)'
            cout << "\n[!] Identifiants incorrects." << endl;
            pause();
        }
    }
    cout << "Fermeture de l'application." << endl;
}


// --- CLIENT SESSION (Logic moved from old main.cpp) ---
void SystemeBancaire::sessionClient(Client* client) {
    int choix = -1;
    do {
        clearScreen();
        // Affichage du menu amélioré
        cout << "=== ESPACE CLIENT : " << client->getPrenom() << " " << client->getNom() << " ===\n";
        cout << "1. Voir mes Comptes\n";
        cout << "2. Faire un Depot\n";
        cout << "3. Faire un Retrait\n";
        cout << "4. Faire un Virement\n";
        cout << "5. Consulter l'Historique\n";
        cout << "0. Deconnexion\n";
        cout << "Choix : ";
        
        // Sécurisation de l'entrée (évite les bugs si on tape une lettre)
        if (!(cin >> choix)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Récupération des comptes du client
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
                
                // Vérification que le numéro est valide
                if (index > 0 && index <= (int)comptes.size()) {
                    double montant;
                    cout << "Montant : ";
                    cin >> montant;
                    
                    if (choix == 2) {
                        // Action DEPOT
                        comptes[index - 1]->deposer(montant);
                    } else {
                        // Action RETRAIT
                        // La méthode retirer() renvoie false si solde insuffisant
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
                // 1. Choisir le compte source
                int indexSrc = 0;
                cout << "Depuis quel compte ? (1-" << comptes.size() << ") : ";
                cin >> indexSrc;
                
                if (indexSrc > 0 && indexSrc <= (int)comptes.size()) {
                    // 2. Choisir le destinataire (ID du compte, ex: "CPT_123")
                    string destID;
                    cout << "ID du compte destinataire : ";
                    cin >> destID;
                    
                    // On cherche le compte destinataire dans TOUTE la banque
                    Compte* destCompte = trouverCompte(destID); // Utilise la méthode de SystemeBancaire

                    if (destCompte != nullptr) {
                        double montant;
                        cout << "Montant du virement : ";
                        cin >> montant;
                        
                        // Exécution du virement
                        if (comptes[indexSrc - 1]->virementVers(*destCompte, montant)) {
                            // Le message de succès est souvent déjà dans virementVers, 
                            // mais on peut confirmer ici si besoin.
                        }
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
                    // CORRECTION ICI : On utilise afficherInfo() au lieu de getType()
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
string SystemeBancaire::genererIdClient() {
    return "CLI" + to_string(utilisateurs.size() + 1);
}

void SystemeBancaire::ajouterNouveauClient(string nom, string prenom, string dateN) {
    string id = genererIdClient();
    string defaultPassword = "1234";
    
    // Save to DB securely
    string query = "INSERT INTO Users (id, mdp, nom, prenom, type, dateNaissance) VALUES (?, ?, ?, ?, 'Client', ?);";
    vector<string> params = {id, defaultPassword, nom, prenom, dateN};

    if (BDManager::getInstance()->executePrepared(query, params)) {
        // Also add to in-memory vector
        auto newClient = make_unique<Client>(id, defaultPassword, nom, prenom, dateN);
        utilisateurs.push_back(move(newClient));
        cout << "Client cree avec succes. ID : " << id << endl;
    } else {
        cout << "Erreur SQL: Impossible de creer le client." << endl;
    }
}

bool SystemeBancaire::ouvrirNouveauCompte(string idClient, string typeCompte, double depotInitial) {
    Client* target = trouverClientParId(idClient);
    if (!target) {
        cout << "Client introuvable.\n";
        return false;
    }

    string nomTitulaire = target->getPrenom() + " " + target->getNom();

    unique_ptr<Compte> compte = nullptr;
    if (typeCompte == "Epargne") 
        compte = make_unique<CompteEpargne>(nomTitulaire, depotInitial, 0.05);
    else 
        compte = make_unique<CompteCourant>(nomTitulaire, depotInitial, 200.0);

    if (compte) {
        string uniqueID = "CPT" + to_string(time(nullptr)) + to_string(rand() % 1000);
        compte->setNumCompte(uniqueID);

        string q = "INSERT INTO Comptes (numCompte, userId, typeCompte, solde, taux, decouvert) VALUES (?, ?, ?, ?, ?, ?);";
        
        // Prepare parameters based on account type
        double taux = (typeCompte == "Epargne") ? 0.05 : 0.0;
        double decouvert = (typeCompte == "Courant") ? 200.0 : 0.0;

        vector<string> params = {
            uniqueID,
            idClient,
            typeCompte,
            to_string(depotInitial),
            to_string(taux),
            to_string(decouvert)
        };
        
        if (BDManager::getInstance()->executePrepared(q, params)) {
            target->ajouterCompte(compte.get()); // Pass non-owning pointer
            listeComptes.push_back(move(compte)); // Transfer ownership to system
            cout << "Compte cree avec succes (ID: " << uniqueID << ")\n";
            return true;
        } else {
            cout << "Erreur SQL : Impossible de creer le compte.\n";
            return false;
        }
    }
    return false;
}

Compte* SystemeBancaire::trouverCompte(string numCompte) {
    for (const auto& compte : listeComptes) {
        if (compte->getNumCompte() == numCompte) {
            return compte.get();
        }
    }
    return nullptr;
}

Client* SystemeBancaire::trouverClientParId(const string& idClient) {
    for (const auto& u : utilisateurs) {
        if (u->getIdentifiant() == idClient) {
            return dynamic_cast<Client*>(u.get()); 
        }
    }
    return nullptr;
}

Client* SystemeBancaire::trouverClient(string nom, string prenom) {
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
                for (const auto& u : utilisateurs) {
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
                    cout << "Salaire actuel : " 
                              << empTrouve->getSalaire() << " MAD\n";
                    cout << "Nouveau salaire : ";
                    cin >> nouveauSalaire;
                    empTrouve->setSalaire(nouveauSalaire);
                    cout << "Salaire mis à jour !\n";
                } else {
                    cout << "Employé introuvable.\n";
                }

                pause();
                break;
            }

            case 3: {
                cout << "\n--- Liste des Clients ---\n";
                for (const auto& u : utilisateurs) {
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

void SystemeBancaire::sessionAdmin(AdminIT* admin) {
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
                    utilisateurs.push_back(std::move(nouvelUtilisateur));
                    cout << "Confirmation: Utilisateur ajoute au systeme en memoire." << endl;
                } else {
                    cout << "L'operation de creation a echoue ou a ete annulee." << endl;
                }
                pause();
                break;
            }
            case 2: {
                std::string idASupprimer = admin->supprimerUtilisateur();
                if (!idASupprimer.empty()) {
                    auto it = std::remove_if(utilisateurs.begin(), utilisateurs.end(),
                        [&](const std::unique_ptr<Utilisateur>& u) {
                            return u->getIdentifiant() == idASupprimer;
                        });

                    if (it != utilisateurs.end()) {
                        utilisateurs.erase(it, utilisateurs.end());
                        cout << "Confirmation: Utilisateur " << idASupprimer << " supprime de la memoire." << endl;
                    }
                } else {
                    cout << "L'operation de suppression a echoue ou a ete annulee." << endl;
                }
                pause();
                break;
            }
            case 3:
                cout << "Total Utilisateurs en RAM : " << utilisateurs.size() << "\n";
                pause();
                break;
        }
    } while (choix != 0);
}

void SystemeBancaire::sessionCaissier(Caissier* caissier) {
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
            clientSelectionne = trouverClientParId(id);
            if (!clientSelectionne) cout << "Client introuvable.\n";
        }
        else if (choix == 2) {
            string nom, prenom;
            cout << "Nom : "; cin >> nom;
            cout << "Prenom : "; cin >> prenom;
            clientSelectionne = trouverClient(nom, prenom);
            if (!clientSelectionne) cout << "Client introuvable.\n";
        }
        else if (choix == 3) {
            clearScreen();
            cout << "\n--- Liste des Clients ---\n";
            for (const auto& u : utilisateurs) {
                if (auto* c = dynamic_cast<Client*>(u.get())) {
                    cout << "ID: " << c->getIdentifiant() 
                         << " | " << c->getNom() << " " << c->getPrenom() << endl;
                }
            }
            pause();
            continue;
        }

        // Once a client is selected, show operations
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
            ajouterNouveauClient(n, p, d);
            pause();
        }
        else if (choix == 5) {
            string id, type; double dep;
            cout << "\n--- Nouveau Compte ---\n";
            cout << "ID Client: "; cin >> id;
            cout << "Type (Epargne/Courant): "; cin >> type;
            cout << "Depot Initial: "; cin >> dep;
            ouvrirNouveauCompte(id, type, dep);
            pause();
        }
        else if (choix != 0 && choix != 3) { // If search failed and not exiting or listing
             pause();
        }

    } while (choix != 0);
}

void SystemeBancaire::sessionEmployeClient(EmployeClient* ec) {
    int choix = -1;
    do {
        clearScreen();
        cout << "=== ESPACE EMPLOYE-CLIENT : " << ec->getPrenom() << " " << ec->getNom() << " ===\n";
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
             // Transform role to lower case just in case
             // transform(role.begin(), role.end(), role.begin(), ::tolower); // Need <algorithm>
             
             cout << "\n--- Acces Espace Employe (" << role << ") ---\n";
             
             if (role == "Manager" || role == "manager") {
                 // Create Temporary Manager Wrapper
                 Manager tempM(ec->getNom(), ec->getPrenom(), ec->getSalaire(), 0, ec->getIdentifiant(), "temp");
                 sessionManager(&tempM);
             }
             else if (role == "Caissier" || role == "caissier") {
                 // Create Temporary Caissier Wrapper
                 Caissier tempC(ec->getNom(), ec->getPrenom(), ec->getSalaire(), ec->getIdentifiant(), "temp");
                 sessionCaissier(&tempC);
             }
             else {
                 // Fallback for generic employee
                 sessionEmploye(ec);
             }
        }

    } while (choix != 0);
}
