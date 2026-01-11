#ifndef SOMMET_H
#define SOMMET_H

#include "GElement.h"
#include "IVisiteur.h"
#include <string>
#include <sstream>

/**
 * @brief Classe représentant un sommet dans un graphe.
 * 
 * @tparam T - Type des données associées au sommet.
 */
template <typename T>
class Sommet : public GElement<T> {
public:
    // Attribut public représentant le degré du sommet.
    int degre;

    /**
     * @brief Constructeur de Sommet.
     * Initialise la clef, la valeur v (via GElement) et le degré à 0 par défaut (sommet isolé).
     * 
     * @param c - Clef primaire.
     * @param val - Valeur associée au sommet.
     * @param d - Degré initial du sommet.
     */
    Sommet(int c = 0, T val = T(), int d = 0) 
        : GElement<T>(c, val), degre(d) {}

    //DP Visitor

    /**
     * @brief Accepte un visiteur.
     * 
     * @param v - Visiteur à accepter.
     */
    void accept(IVisiteur<double, Ville>* v) override {
        if constexpr (std::is_same_v<T, Ville>) {
            v->visiterSommet((Sommet<Ville>*)this);
        }
    }
};

#endif