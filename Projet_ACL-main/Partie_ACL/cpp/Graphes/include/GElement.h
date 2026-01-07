#ifndef GELEMENT_H
#define GELEMENT_H

#include "AElement.h"
#include <string>
#include <sstream>

/**
 * @brief Classe de base pour les éléments du graphe.
 * 
 * @tparam T - Type des données associées à l'élément.
 */
template <typename T>
class GElement : public AElement {
public:
    // Attribut public de type générique T pour la valeur.
    T v;

    /**
     * @brief Constructeur.
     * Initialise la clef via AElement et la valeur v.
     * 
     * @param c - Valeur de la clef primaire.
     * @param val - Valeur associée à l'élément.
     */
    GElement(int c = 0, T val = T()) : AElement(c), v(val) {}

};

#endif