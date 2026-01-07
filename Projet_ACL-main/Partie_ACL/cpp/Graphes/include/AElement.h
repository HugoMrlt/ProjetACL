#ifndef AELEMENT_H
#define AELEMENT_H

/**
 * @brief Interface de visiteur pour les éléments du graphe.
 * 
 * @tparam S - Type des sommets.
 * @tparam T - Type des données des sommets.
 */
template <typename S, typename T> class IVisiteur;
class Ville;

/**
 * @brief Classe abstraite représentant un élément du graphe.
 */
class AElement {
public:
    // Attribut public servant de clef primaire.
    int clef;

    /**
     * @brief Constructeur de AElement.
     * 
     * @param c - Valeur de la clef primaire.
     */
    AElement(int c = 0) : clef(c) {}

    virtual ~AElement() {};

    //DP Visitor
    /**
     * @brief Accepte un visiteur.
     * 
     * @param v - Visiteur à accepter.
     */
    virtual void accept(IVisiteur<double, Ville>* v) = 0;
};

#endif