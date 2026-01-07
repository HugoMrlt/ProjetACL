#include <iostream>
#include "Ville.h"
#include "Sommet.h"
#include "Arete.h"
#include "VisiteurAffichage.h"

int main() {
    // 1. Initialisation des sommets (avec des adresses pour les pointeurs)
    Ville v1("Montreal", 45.5, -73.5);
    Ville v2("Quebec", 46.8, -71.2);

    Sommet<Ville> s1(1, v1);
    Sommet<Ville> s2(2, v2);

    // 2. Correction de l'instanciation de l'Arete
    // Signature : Arete(int clef, const S& v, Sommet<T>* d, Sommet<T>* f)
    // Ici S est double (le poids) et T est Ville.
    Arete<double, Ville> route(101, 255.5, &s1, &s2);

    // 3. Test du visiteur
    VisiteurAffichage visiteur;
    
    std::cout << "--- Test du Visiteur ---" << std::endl;
    
    std::cout << "Sommet 1 : ";
    s1.accept(&visiteur);

    std::cout << "Sommet 2 : ";
    s2.accept(&visiteur);

    std::cout << "Arete : ";
    route.accept(&visiteur);

    return 0;
}