#include <iostream>
#include <vector>
#include "Graphe.h"
#include "Ville.h"
#include "VisiteurAffichage.h"

int main() {
    std::cout << "=== Test de la classe Graphe avec Visitor ===" << std::endl;

    // 1. Création du graphe (Poids: double, Donnée Sommet: Ville)
    Graphe<double, Ville> monReseau;
    VisiteurAffichage monAfficheur;

    // 2. Ajout de sommets (Villes)
    Sommet<Ville>* v1 = monReseau.creeSommet(Ville("Montreal", 45.5, -73.5));
    Sommet<Ville>* v2 = monReseau.creeSommet(Ville("Quebec", 46.8, -71.2));
    Sommet<Ville>* v3 = monReseau.creeSommet(Ville("Sherbrooke", 45.4, -71.8));

    // 3. Ajout d'arêtes (Routes avec distances)
    monReseau.creeArete(250.0, v1, v2); // Montreal <-> Quebec
    monReseau.creeArete(160.0, v1, v3); // Montreal <-> Sherbrooke

    // 4. Affichage du contenu via le Visiteur
    std::cout << "\n--- Liste des Sommets du Graphe ---" << std::endl;
    std::vector<Sommet<Ville>*> sommets = monReseau.getSommets();
    for (auto s : sommets) {
        s->accept(&monAfficheur);
    }

    std::cout << "\n--- Liste des Aretes du Graphe ---" << std::endl;
    std::vector<Arete<double, Ville>*>* aretes = monReseau.getAretes();
    for (auto a : *aretes) {
        a->accept(&monAfficheur);
    }
    delete aretes; // Car getAretes() retourne un pointeur new std::vector

    // 5. Test de topologie : Voisins de Montreal
    std::cout << "\nVoisins de " << v1->v.getName() << ":" << std::endl;
    PElement<Sommet<Ville>*>* listeVoisins = monReseau.voisins(v1);
    PElement<Sommet<Ville>*>* curr = listeVoisins;
    
    while (curr) {
        // On utilise aussi le visiteur pour les voisins
        curr->v->accept(&monAfficheur);
        curr = curr->suivant;
    }

    // Nettoyage de la liste temporaire des voisins (PElement)
    // Note: Dans votre logique, PElement n'a pas forcément de delete cascade ici
    // Il faudrait s'assurer de ne pas fuir la mémoire de la liste chaînée de voisins.

    std::cout << "\n=== Fin du Test Graphe ===" << std::endl;

    return 0;
}