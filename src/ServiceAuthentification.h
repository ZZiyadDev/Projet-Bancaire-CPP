#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Utilisateur.h"

class ServiceAuthentification {
public:
    Utilisateur* authentifier(const std::string& login, const std::string& pass, const std::vector<std::unique_ptr<Utilisateur>>& utilisateurs);
};
