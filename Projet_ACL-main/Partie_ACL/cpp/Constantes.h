#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>

inline std::string trouverCheminAssets(const std::string& cheminRelatif) {
    std::vector<std::string> basesPossibles = {
        "Assets/",
        "../Assets/",
        "../../Assets/",
        "../../../Assets/"
    };

    for (const auto& base : basesPossibles) {
        std::string cheminComplet = base + cheminRelatif;
        if (std::filesystem::exists(cheminComplet)) {
            return cheminComplet;
        }
    }
    throw std::runtime_error("Fichier non trouvé : " + cheminRelatif);
}

const std::string CHEMIN_FICHIER_JSON_GRAND_EST = trouverCheminAssets("Region/GrandEst.json");