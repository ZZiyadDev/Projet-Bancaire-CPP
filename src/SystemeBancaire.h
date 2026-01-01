#pragma once

#include "UtilisateurManager.h"
#include "CompteManager.h"
#include "ServiceAuthentification.h"
#include "InterfaceUtilisateur.h"

class SystemeBancaire {
private:
    UtilisateurManager utilisateurManager;
    CompteManager compteManager;
    ServiceAuthentification serviceAuthentification;
    InterfaceUtilisateur interfaceUtilisateur;

    void sessionAdmin(AdminIT* admin);
    void sessionManager(Manager* manager);
    void sessionCaissier(Caissier* caissier);
    void sessionClient(Client* client);
    void sessionEmployeClient(EmployeClient* ec);

public:
    SystemeBancaire(); 
    ~SystemeBancaire();

    void lancer(); 
};
