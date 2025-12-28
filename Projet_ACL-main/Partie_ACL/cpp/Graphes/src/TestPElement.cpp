#include <iostream>
#include <string>
#include "PElement.h"
#include "Sommet.h"

int main() {
    // --- TEST 1 : Liste d'entiers simples ---
    std::cout << "--- Test PElement avec des int* ---" << std::endl;
    
    // Création d'une liste : 30 -> 20 -> 10 -> NULL.
    int* p1 = new int(10);
    int* p2 = new int(20);
    int* p3 = new int(30);
    
    PElement<int*>* listeInt = new PElement<int*>(p1);
    listeInt = new PElement<int*>(p2, listeInt);
    listeInt = new PElement<int*>(p3, listeInt);

    std::cout << "Contenu de la liste : ";
    std::cout << *listeInt; // Utilise l'opérateur << de PElement.
    std::cout << std::endl << std::endl;


    // --- TEST 2 : Liste de Sommets ---
    // C'est le cas d'utilisation réel pour lSommets.
    std::cout << "--- Test PElement avec des Sommet<string>* ---" << std::endl;

    Sommet<std::string>* s1 = new Sommet<std::string>(1, "Sommet_A");
    Sommet<std::string>* s2 = new Sommet<std::string>(2, "Sommet_B");

    // Création de la liste de sommets
    PElement<Sommet<std::string>*>* maListeSommets = new PElement<Sommet<std::string>*>(s1);
    maListeSommets = new PElement<Sommet<std::string>*>(s2, maListeSommets);

    // Affichage via l'opérateur << (qui déréférence automatiquement).
    std::cout << "Contenu de la liste de sommets : ";
    std::cout << *maListeSommets; // Affiche les sommets pointés.
    std::cout << std::endl;

    // Nettoyage rapide (en attendant le destructeur du Graphe).
    delete maListeSommets->suivant;
    delete maListeSommets;
    delete s1;
    delete s2;

    return 0;
}