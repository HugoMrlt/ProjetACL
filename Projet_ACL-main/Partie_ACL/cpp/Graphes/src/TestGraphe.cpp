#include <iostream>
#include <string>
#include "Graphe.h"

using namespace std;

int main() {
    char ch;
    Graphe<char, string> g2; // Graphe vide initialement.
    {
        Graphe<char, string> g1; // Création à vide.

        Sommet<string> *s0, *s1, *s2, *s3;

        // Insertion des sommets
        s0 = g1.creeSommet("King's Landing");
        s1 = g1.creeSommet("Winterfell");
        s2 = g1.creeSommet("DragonStone");
        s3 = g1.creeSommet("The wall");

        // Connexion des sommets
        g1.creeArete('a', s1, s0);
        g1.creeArete('b', s2, s1);
        g1.creeArete('c', s3, s2);
        g1.creeArete('d', s3, s1);

        cout << "Le graphe cree g1 est :" << endl << g1 << endl;
        
        cout << "Nombre de sommets : " << g1.nombreSommets() << endl;
        cout << "Nombre d'aretes : " << g1.nombreAretes() << endl;

        // Test des voisins de s0
        PElement<Sommet<string>*>* l0 = g1.voisins(s0);
        cout << "Voisins de s0 : " << endl;
        PElement<Sommet<string>*>* tmp = l0;
        while(tmp) { cout << " - " << tmp->v->v << endl; tmp = tmp->suivant; }
        delete l0;

        // Test des aretes adjacentes de s1
        PElement<Arete<char, string>*>* adj1 = g1.aretesAdjacentes(s1);
        cout << "Aretes adjacentes a s1 : " << endl;
        PElement<Arete<char, string>*>* tmpA = adj1;
        while(tmpA) { cout << " - Arete clef " << tmpA->v->clef << endl; tmpA = tmpA->suivant; }
        delete adj1;

        // Recherche d'une arete spécifique
        Arete<char, string>* a = g1.getAreteParSommets(s1, s3);
        if(a) cout << "L'arete joignant s1 et s3 est : " << *a << endl;

        g2 = g1; // Utilisation du constructeur de copie.
    }

    cout <<"le graphe créé g2 comme copie de g1 est :"<< endl << g2 << endl;
    cin >> ch;

    return 0; 
}