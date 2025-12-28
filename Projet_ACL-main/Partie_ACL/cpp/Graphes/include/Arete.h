#ifndef ARETE_H
#define ARETE_H

#include "GElement.h"
#include "Sommet.h"
#include <string>
#include <iostream>

// Arete<S, T> dérive de GElement<S>.
template <typename S, typename T>
class Arete : public GElement<S> {
public:
    // Attributs publics : pointeurs sur les sommets extrémités.
    Sommet<T>* debut;
    Sommet<T>* fin;

    /**
     * Constructeur :
     * Initialise la clef, la valeur v et les pointeurs.
     * Met à jour le degré des sommets connectés en les incrémentant.
     */
    Arete(const int clef, const S& v, Sommet<T>* d, Sommet<T>* f)
        : GElement<S>(clef, v), debut(d), fin(f) {
        if (debut) debut->degre++; // Incrémente le degré du sommet de début.
        if (fin) fin->degre++;     // Incrémente le degré du sommet de fin.
    }

    /**
     * Destructeur :
     * Nécessaire pour décrémenter le degré des sommets lorsque l'arête est supprimée.
     */
    virtual ~Arete() {
        if (debut) debut->degre--;
        if (fin) fin->degre--;
    }

    /**
     * Méthode estEgal :
     * Vérifie si l'arête relie les sommets s1 et s2 (sans tenir compte de l'ordre).
     */
    bool estEgal(const Sommet<T>* s1, const Sommet<T>* s2) const {
        return (debut == s1 && fin == s2) || (debut == s2 && fin == s1);
    }

    /**
     * Opérateur de conversion en string :
     * Indique les clefs primaires des sommets debut et fin.
     */
    operator std::string() const {
        std::string s = "Arete " + AElement::operator std::string();
        s += " (" + (debut ? std::to_string(debut->clef) : "NULL");
        s += " <-> " + (fin ? std::to_string(fin->clef) : "NULL") + ")";
        return s;
    }

    /**
     * Opérateur << d'écriture sur un flux :
     * Utilise l'opérateur de conversion en string.
     */
    friend std::ostream& operator<<(std::ostream& os, const Arete<S, T>& obj) {
        os << (std::string)obj;
        return os;
    }
};

#endif