#include <iostream>
#include <cassert>
#include "Sommet.h"
#include "Ville.h"
#include "VisiteurAffichage.h"

int main() {
    std::cout << "=== Test de la classe Sommet ===" << std::endl;

    // 1. Préparation des données de test
    Ville montreal("Montreal", 45.50, -73.56);
    int clefTest = 50;
    int degreInitial = 0;

    // 2. Instanciation d'un Sommet
    // On utilise Sommet<Ville> car votre visiteur est spécialisé pour ce type
    Sommet<Ville> sommet(clefTest, montreal, degreInitial);

    // 3. Vérification des attributs (Tests unitaires)
    std::cout << "[1] Verification des attributs..." << std::endl;
    if (sommet.clef == clefTest && sommet.degre == degreInitial) {
        std::cout << "    OK : Attributs initialises correctement." << std::endl;
    } else {
        std::cerr << "    ERREUR : Echec de l'initialisation." << std::endl;
    }

    // 4. Test du Design Pattern Visitor
    // Le visiteur va s'occuper de l'affichage à la place de l'opérateur <<
    std::cout << "[2] Test de la methode accept (Visitor)..." << std::endl;
    
    VisiteurAffichage afficheur;
    
    std::cout << "    Resultat de l'affichage : ";
    // C'est ici que la magie du DP Visitor opere :
    // Le sommet appelle v->visiterSommet(this)
    sommet.accept(&afficheur);

    // 5. Test de modification du degré
    // Utile pour vérifier que les arêtes pourront impacter le sommet
    std::cout << "[3] Test de modification du degre..." << std::endl;
    sommet.degre++;
    if (sommet.degre == 1) {
        std::cout << "    OK : Degre incremente avec succes." << std::endl;
    }

    std::cout << "=== Fin des tests Sommet ===" << std::endl;

    return 0;
}