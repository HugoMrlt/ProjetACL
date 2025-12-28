#ifndef AELEMENT_H
#define AELEMENT_H

#include <iostream>
#include <string>

class AElement {
public:
    // Attribut public servant de clef primaire.
    int clef;

    // Constructeur.
    AElement(int c = 0) : clef(c) {}

    // Opérateur de conversion en string.
    operator std::string() const {
        return std::to_string(clef);
    }

    // Opérateur << d'écriture sur un flux.
    friend std::ostream& operator<<(std::ostream& os, const AElement& obj) {
        os << (std::string)obj; 
        return os;
    }
};

#endif