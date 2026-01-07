#ifndef VISITEUR_STATISTIQUES_H
#define VISITEUR_STATISTIQUES_H

#include "IVisiteur.h"
#include "Ville.h"
#include "Sommet.h"
#include "Arete.h"

/**
 * @brief Visiteur pour calculer des statistiques sur un graphe.
 */
class VisiteurStatistiques : public IVisiteur<double, Ville> {
private:
    double distanceCumulee;
    int compteurEtapes;

public:
    /**
     * @brief Constructeur par défaut.
     */
    VisiteurStatistiques() : distanceCumulee(0.0), compteurEtapes(0) {}

    /**
     * @brief Visite un sommet et incrémente le compteur d'étapes.
     * 
     * @param s - Le sommet à visiter.
     */
    void visiterSommet(Sommet<Ville>* s) override {
        compteurEtapes++;
    }

    /**
     * @brief Visite une arête et ajoute son poids à la distance cumulée.
     * 
     * @param a - L'arête à visiter.
     */
    void visiterArete(Arete<double, Ville>* a) override {
        distanceCumulee += a->v;
    }

    /**
     * @brief Récupère la distance cumulée.
     * 
     * @return double - Distance totale parcourue.
     */
    double getDistance() const { return distanceCumulee; }

    /**
     * @brief Récupère le nombre d'étapes (sommets visités).
     * 
     * @return int - Nombre d'étapes.
     */
    int getEtapes() const { return compteurEtapes; }

    /**
     * @brief Réinitialise les statistiques.
     */
    void reset() {
        distanceCumulee = 0.0;
        compteurEtapes = 0;
    }
};

#endif