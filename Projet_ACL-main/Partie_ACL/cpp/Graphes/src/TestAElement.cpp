#include <iostream>
#include "AElement.h"

int main() {
    // Test du constructeur.
    AElement el(101);

    // Test de l'opérateur de conversion en string.
    std::string s = (std::string)el;
    std::cout << "Conversion string : " << s << std::endl;

    // Test de l'opérateur <<.
    std::cout << "Affichage via << : " << el << std::endl;

    return 0;
}