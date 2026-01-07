#ifndef VISITEUR_AFFICHAGE_H
#define VISITEUR_AFFICHAGE_H

#include "IVisiteur.h"
#include "Ville.h"
#include "Sommet.h"
#include "Arete.h"

#include <iostream>

/**
 * @brief Visiteur d'affichage pour afficher les sommets et arêtes d'un graphe.
 * 
 */
class VisiteurAffichage : public IVisiteur<double, Ville> {
public:
    /**
     * @brief Visite un sommet et affiche ses informations.
     * 
     * @param s Le sommet à visiter.
     */
    void visiterSommet(Sommet<Ville>* s) override {
        std::cout << "- Ville : " << s->v.getName()
                  << " (Lat: " << s->v.getLatitude()
                  << ", Long: " << s->v.getLongitude() << ")" << std::endl;
    }

    /**
     * @brief Visite une arête et affiche ses informations.
     * 
     * @param a L'arête à visiter.
     */
    void visiterArete(Arete<double, Ville>* a) override {
        std::cout << "  Route [" << a->debut->v.getName() << " <-> " << a->fin->v.getName()
                  << "] Poids: " << a->v << std::endl;
    }
};

#endif