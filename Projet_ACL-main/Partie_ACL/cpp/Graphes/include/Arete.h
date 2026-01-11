#ifndef ARETE_H
#define ARETE_H

#include "GElement.h"
#include "Sommet.h"
#include "IVisiteur.h"
#include <string>
#include <iostream>



/**
 * @brief Structure contenant les données métier d'une route.
 * C'est ce qui sera utilisé comme type 'S' dans Arete<S, T>.
 */
struct Route {
    int poids;           // Distance en km
    std::string type;    // "Autoroute", "Nationale", etc.
    int camionId;        // ID du camion qui l'emprunte

    // Petit constructeur pratique
    Route(int p = 0, std::string t = "", int c = -1)
        : poids(p), type(t), camionId(c) {}

    // Surcharge pour l'affichage (utilisé par l'opérateur << de GElement)
    friend std::ostream& operator<<(std::ostream& os, const Route& r) {
        os << r.type << " (" << r.poids << "km, Camion:" << r.camionId << ")";
        return os;
    }
};

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


    /**
     * @brief Accepte un visiteur pour appliquer un algorithme ou un calcul sur l'arête.
     * Utilise 'if constexpr' pour vérifier au moment de la compilation si le type
     * de données de l'arête (S) est compatible avec le visiteur (double).
     * * @param v - Pointeur vers le visiteur (IVisiteur) à accepter.
     */
    void accept(IVisiteur<double, Ville>* v) override {
        if constexpr (std::is_same_v<S, double>) {
            v->visiterArete((Arete<double, Ville>*)this);
        }
    }
};

#endif