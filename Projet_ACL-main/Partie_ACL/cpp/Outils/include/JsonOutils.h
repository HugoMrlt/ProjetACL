#ifndef PROJET_ACL_JSONOUTILS_H
#define PROJET_ACL_JSONOUTILS_H


#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "json.hpp"
#include "Ville.h"

using namespace std;
using json = nlohmann::json;

/**
 * @brief Utilitaires pour la manipulation de fichiers JSON.
 */
class JsonOutils {
public:
    /**
     * @brief Lit un fichier JSON et retourne un vecteur d'objets Ville
     * 
     * @param cheminFichier - Chemin vers le fichier JSON.
     * @return vector<Ville> 
     */
    static vector<Ville> chargerDepuisFichier(const string& cheminFichier) {
        vector<Ville> listeVilles;


        ifstream fichier(cheminFichier);
        if (!fichier.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier JSON");
        }


        json donnees;
        fichier >> donnees;


        for (const auto& item : donnees) {
            string nom = item.value("ville", "Inconnu");
            double lat = item.value("latitude", 0.0);
            double lon = item.value("longitude", 0.0);

            listeVilles.emplace_back(nom, lat, lon);
        }

        return listeVilles;
    }
};


#endif //PROJET_ACL_JSONOUTILS_H