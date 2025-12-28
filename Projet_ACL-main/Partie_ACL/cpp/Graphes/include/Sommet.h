#ifndef SOMMET_H
#define SOMMET_H

#include "GElement.h"
#include <string>
#include <sstream>

template <typename T>
class Sommet : public GElement<T> {
public:
    // Attribut public représentant le degré du sommet.
    int degre;

    // Constructeur : initialise la clef, la valeur v (via GElement).
    // et le degré à 0 par défaut (sommet isolé).
    Sommet(int c = 0, T val = T(), int d = 0) 
        : GElement<T>(c, val), degre(d) {}

    // Opérateur de conversion en string.
    operator std::string() const {
        std::ostringstream oss;
        // On concatène la conversion de GElement et le degré.
        oss << GElement<T>::operator std::string() << " [degre: " << degre << "]";
        return oss.str();
    }

    // Opérateur << d'écriture sur un flux.
    friend std::ostream& operator<<(std::ostream& os, const Sommet<T>& obj) {
        os << (std::string)obj;
        return os;
    }
};

#endif