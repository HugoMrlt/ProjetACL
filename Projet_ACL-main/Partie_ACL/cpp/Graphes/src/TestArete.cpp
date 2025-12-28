#include <iostream>
#include "Arete.h"

int main() {
    // Création de deux sommets.
    Sommet<std::string> s1(1, "Station A");
    Sommet<std::string> s2(2, "Station B");

    // Affichage des sommets initiaux.
    std::cout << "Avant création de l'arête :" << std::endl;
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;

    // Création d'une arête entre les deux sommets.
    Arete<double, std::string> a1(101, 5.0, &s1, &s2);

    // Affichage de l'arête créée.
    std::cout << "Arête créée :" << std::endl;
    std::cout << a1 << std::endl;

    // Affichage des sommets après création de l'arête (degrés mis à jour).
    std::cout << "Après création de l'arête :" << std::endl;
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;

    // Suppression de l'arête (destructeur appelé automatiquement ici).
    {
        Arete<double, std::string> a2(102, 10.0, &s1, &s2);
        std::cout << "Deuxième arête créée :" << std::endl;
        std::cout << a2 << std::endl;
        std::cout << "Après création de la deuxième arête :" << std::endl;
        std::cout << s1 << std::endl;
        std::cout << s2 << std::endl;
    } // a2 est détruit ici.

    // Affichage des sommets après destruction de la deuxième arête (degrés mis à jour).
    std::cout << "Après destruction de la deuxième arête :" << std::endl;
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;

    return 0;
}