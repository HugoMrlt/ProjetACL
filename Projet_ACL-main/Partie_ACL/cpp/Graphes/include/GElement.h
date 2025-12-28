#ifndef GELEMENT_H
#define GELEMENT_H

#include "AElement.h"
#include <string>
#include <sstream>

template <typename T>
class GElement : public AElement {
public:
    // Attribut public de type générique T pour la valeur.
    T v;

    // Constructeur : initialise la clef via AElement et la valeur v.
    GElement(int c = 0, T val = T()) : AElement(c), v(val) {}

    // Opérateur de conversion en string.
    operator std::string() const {
        std::ostringstream oss;
        // On concatène la conversion de la base (clef) et la valeur v.
        oss << AElement::operator std::string() << " (v: " << v << ")";
        return oss.str();
    }

    // Opérateur << d'écriture sur un flux.
    friend std::ostream& operator<<(std::ostream& os, const GElement<T>& obj) {
        os << (std::string)obj;
        return os;
    }
};

#endif