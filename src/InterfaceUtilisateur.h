#pragma once

#include "Client.h"
#include "Employe.h"
#include "Manager.h"
#include "Caissier.h"
#include "EmployeClient.h"
#include "AdminIT.h"

// Forward declarations for managers to avoid circular dependencies
class UtilisateurManager;
class CompteManager;

class InterfaceUtilisateur {
public:
    void afficherMenuPrincipal();
    void afficherMenuClient(Client* client, CompteManager& compteManager);
    void afficherMenuEmploye(Employe* employe, UtilisateurManager& utilisateurManager, CompteManager& compteManager);
    void afficherMenuManager(Employe* employe, UtilisateurManager& utilisateurManager);
    void afficherMenuCaissier(Employe* employe, UtilisateurManager& utilisateurManager, CompteManager& compteManager);
    void afficherMenuEmployeClient(EmployeClient* ec, UtilisateurManager& utilisateurManager, CompteManager& compteManager);
    void afficherMenuAdmin(AdminIT* admin, UtilisateurManager& utilisateurManager);

    void clearScreen();
    void pause();
};
