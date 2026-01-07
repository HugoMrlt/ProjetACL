#ifndef TSPTEMPS_H
#define TSPTEMPS_H
#include "IStrategieTSP.h"
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

/**
 * @brief Stratégie TSP basée sur le temps de trajet.
 */
class TSPTemps : public IStrategieTSP {
private:
    std::map<std::string, double> vitesses;
public:
    /**
     * @brief Constructeur : initialise les vitesses moyennes par type de route.
     */
    TSPTemps() {
        vitesses["Autoroute"] = 90.0;
        vitesses["Voie rapide"] = 80.0;
        vitesses["Route européenne"] = 80.0;
        vitesses["Nationale"] = 70.0;
        vitesses["Départementale"] = 60.0;
        vitesses["Communale"] = 40.0;
    }

    /**
     * @brief Calcule le poids d'une arête en fonction du temps de trajet.
     * 
     * @param distanceKm - Distance en kilomètres entre deux villes.
     * @param typeRoute - Type de route (utilisé pour déterminer la vitesse moyenne).
     * @return double - Poids calculé pour l'arête (temps de trajet).
     */
    double calculerPoids(double distanceKm, const std::string& typeRoute) override {
        std::string cle = typeRoute;

        size_t first = cle.find_first_not_of(" \t\r\n");
        if (first != std::string::npos) {
            size_t last = cle.find_last_not_of(" \t\r\n");
            cle = cle.substr(first, (last - first + 1));
        } else {
            cle = "";
        }

        if (!vitesses.count(cle)) {
            std::cerr << "Type de route inconnu : '" << cle << "'" << std::endl;
            throw std::invalid_argument("Type de route inconnu : " + cle);
        }
        double v = vitesses[cle];
        return distanceKm / v;
    }
};
#endif // TSPTEMPS_H

