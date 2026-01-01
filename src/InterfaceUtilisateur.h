#ifndef INTERFACE_UTILISATEUR_H
#define INTERFACE_UTILISATEUR_H

#include "Client.h"
#include "Employe.h"
#include "Manager.h"
#include "Caissier.h"
#include "EmployeClient.h"
#include "AdminIT.h"
#include <string>

// Forward declarations for managers to avoid circular dependencies
class UtilisateurManager;
class CompteManager;

namespace UI {
    namespace Colors {
        const std::string RESET = "\033[0m";
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string BLUE = "\033[34m";
        const std::string MAGENTA = "\033[35m";
        const std::string CYAN = "\033[36m";
    }
}

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

public:
    void printHeader(const std::string& title, const std::string& subtitle = "");
    void printFooter();
    void printMessage(const std::string& message, const std::string& color = UI::Colors::RESET);
};

#endif // INTERFACE_UTILISATEUR_H
