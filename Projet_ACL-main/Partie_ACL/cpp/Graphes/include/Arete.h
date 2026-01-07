#ifndef ARETE_H
#define ARETE_H

#include "GElement.h"
#include "Sommet.h"
#include "IVisiteur.h"
#include <string>
#include <iostream>

/**
 * @brief Classe représentant une arête dans un graphe.
 * 
 * @tparam S - Type des données associées à l'arête.
 * @tparam T - Type des données associées aux sommets.
 */
template <typename S, typename T>
class Arete : public GElement<S> {
public:
    // Attributs publics : pointeurs sur les sommets extrémités.
    Sommet<T>* debut;
    Sommet<T>* fin;

    /**
     * @brief Constructeur de Arete.
     * Initialise la clef, la valeur v et les pointeurs.
     * Met à jour le degré des sommets connectés en les incrémentant.
     * 
     * @param clef - Clef primaire de l'arête.
     * @param v - Valeur associée à l'arête.
     * @param d - Pointeur vers le sommet de début.
     * @param f - Pointeur vers le sommet de fin.
     */
    Arete(const int clef, const S& v, Sommet<T>* d, Sommet<T>* f)
        : GElement<S>(clef, v), debut(d), fin(f) {
        if (debut) debut->degre++; // Incrémente le degré du sommet de début.
        if (fin) fin->degre++;     // Incrémente le degré du sommet de fin.
    }

    /**
     * @brief Destructeur de Arete.
     * Nécessaire pour décrémenter le degré des sommets lorsque l'arête est supprimée.
     */
    virtual ~Arete() {
        if (debut) debut->degre--;
        if (fin) fin->degre--;
    }

    /**
     * @brief Méthode pour vérifier l'égalité entre deux arêtes basées sur leurs sommets.
     * Vérifie si l'arête relie les sommets s1 et s2 (sans tenir compte de l'ordre).
     * 
     * @param s1 - Pointeur vers le premier sommet.
     * @param s2 - Pointeur vers le second sommet.
     * @return true - Si l'arête relie s1 et s2.
     * @return false - Sinon.
     */
    bool estEgal(const Sommet<T>* s1, const Sommet<T>* s2) const {
        return (debut == s1 && fin == s2) || (debut == s2 && fin == s1);
    }



    //DP Visitor
    /**
     * @brief Accepte un visiteur.
     * 
     * @param v - Visiteur à accepter.
     */
    void accept(IVisiteur<double, Ville>* v) override {
        v->visiterArete(this);
    }
};

#endif