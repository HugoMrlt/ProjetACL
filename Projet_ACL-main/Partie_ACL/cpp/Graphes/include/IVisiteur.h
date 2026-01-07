#ifndef IVISITEUR_H
#define IVISITEUR_H

// Pré-déclarations.
template <typename T> class Sommet;
template <typename S, typename T> class Arete;

/**
 * @brief Interface de visiteur pour les éléments du graphe.
 * 
 * @tparam S - Type des sommets.
 * @tparam T - Type des données des sommets.
 */
template <typename S, typename T>
class IVisiteur {
public:
    /**
     * @brief Destructeur virtuel.
     */
    virtual ~IVisiteur() {}

    /**
     * @brief Visite un sommet.
     * 
     * @param s - Pointeur vers le sommet à visiter.
     */
    virtual void visiterSommet(Sommet<T>* s) = 0;

    /**
     * @brief Visite une arête.
     * 
     * @param a - Pointeur vers l'arête à visiter.
     */
    virtual void visiterArete(Arete<S, T>* a) = 0;
};
#endif