#ifndef ISTRATEGIETSP_H
#define ISTRATEGIETSP_H
#include <string>

/**
 * @brief Interface pour les stratégies de calcul du poids dans le TSP.
 */
class IStrategieTSP {
public:
    /**
     * @brief Destructeur virtuel.
     */
    virtual ~IStrategieTSP() = default;

    /**
     * @brief Calcule le poids d'une arête en fonction de la distance et du type de route.
     * 
     * @param distanceKm - Distance en kilomètres entre deux villes.
     * @param typeRoute - Type de route (ex: "autoroute", "route nationale", "route départementale").
     * @return double - Poids calculé pour l'arête.
     */
    virtual double calculerPoids(double distanceKm, const std::string& typeRoute) = 0;
};

#endif // ISTRATEGIETSP_H

