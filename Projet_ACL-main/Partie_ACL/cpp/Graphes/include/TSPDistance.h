#ifndef TSPDISTANCE_H
#define TSPDISTANCE_H
#include "IStrategieTSP.h"

/**
 * @brief Stratégie TSP basée sur la distance.
 */
class TSPDistance : public IStrategieTSP {
public:
    /**
     * @brief Calcule le poids d'une arête en fonction de la distance (ici, le poids est simplement la distance).
     * 
     * @param distanceKm - Distance en kilomètres entre deux villes.
     * @param typeRoute - Type de route (non utilisé dans cette stratégie).
     * @return double - Poids calculé pour l'arête (égal à la distance).
     */
    double calculerPoids(double distanceKm, const std::string& typeRoute) override {
        return distanceKm;
    }
};

#endif // TSPDISTANCE_H
