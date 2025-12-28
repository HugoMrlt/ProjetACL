#ifndef GRAPHE_H
#define GRAPHE_H

#include "PElement.h"
#include "Sommet.h"
#include "Arete.h"
#include <string>
#include <sstream>
#include <utility>
#include <map>

template <typename S, typename T>
class Graphe {
public:
    PElement<Sommet<T>*>* lSommets;      // Liste des sommets.
    PElement<Arete<S, T>*>* lAretes;    // Liste des arêtes.
    int prochaineClef;                  // Générateur de clefs primaires.

    Graphe();
    Graphe(const Graphe& other);
    virtual ~Graphe();

    // Création d'éléments
    Sommet<T>* creeSommet(const T& info);
    Arete<S, T>* creeArete(const S& info, Sommet<T>* debut, Sommet<T>* fin);

    // Getters
    std::vector<Sommet<T>*> getSommets() const;
    std::vector<Arete<S, T>*>* getAretes() const;

    // Méthodes d'accès et topologie
    int nombreSommets() const;
    int nombreAretes() const;
    PElement<Sommet<T>*>* voisins(const Sommet<T>* s) const;
    PElement<Arete<S, T>*>* aretesAdjacentes(const Sommet<T>* s) const;
    PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* adjacences(const Sommet<T>* s) const;
    Arete<S, T>* getAreteParSommets(const Sommet<T>* s1, const Sommet<T>* s2) const;

    // Affichage
    operator std::string() const;
    Graphe<S, T>& operator=(const Graphe<S, T>& other);
    friend std::ostream& operator<<(std::ostream& os, const Graphe<S, T>& g) {
        os << (std::string)g;
        return os;
    }
};

// Template implementations
template <typename S, typename T>
Graphe<S, T>::Graphe() : lSommets(nullptr), lAretes(nullptr), prochaineClef(0) {}

template <typename S, typename T>
Graphe<S, T>::Graphe(const Graphe<S, T>& other) 
    : lSommets(nullptr), lAretes(nullptr), prochaineClef(other.prochaineClef) {
    
    std::map<int, Sommet<T>*> mapSommets;
    
    // Copie profonde des sommets
    PElement<Sommet<T>*>* currS = other.lSommets;
    while (currS) {
        Sommet<T>* s = new Sommet<T>(currS->v->clef, currS->v->v, 0);
        mapSommets[currS->v->clef] = s;
        this->lSommets = new PElement<Sommet<T>*>(s, this->lSommets);
        currS = currS->suivant;
    }
    }

template <typename S, typename T>
Graphe<S, T>::~Graphe() {
    // Suppression des arêtes (y compris la chaîne PElement)
    if (lAretes) {
        PElement<Arete<S, T>*>* curr = lAretes;
        // Parcours jusqu'au dernier élément
        while (curr->suivant) curr = curr->suivant;
        
        // Maintenant supprime toute la chaîne depuis le dernier
        delete lAretes;  // Le destructeur de PElement se charge du reste
    }
    
    // Suppression des sommets (y compris la chaîne PElement)
    if (lSommets) {
        PElement<Sommet<T>*>* curr = lSommets;
        // Parcours jusqu'au dernier élément
        while (curr->suivant) curr = curr->suivant;
        
        // Maintenant supprime toute la chaîne depuis le dernier
        delete lSommets;  // Le destructeur de PElement se charge du reste
    }
}

template <typename S, typename T>
Sommet<T>* Graphe<S, T>::creeSommet(const T& info) {
    Sommet<T>* s = new Sommet<T>(prochaineClef++, info);
    lSommets = new PElement<Sommet<T>*>(s, lSommets);
    return s;
}

template <typename S, typename T>
std::vector<Sommet<T>*> Graphe<S, T>::getSommets() const {
    std::vector<Sommet<T>*> result;
    PElement<Sommet<T>*>* courant = lSommets;
    
    while (courant) {
        result.push_back(courant->v);
        courant = courant->suivant;
    }
    
    return result;
}

template <typename S, typename T>
std::vector<Arete<S, T>*>* Graphe<S, T>::getAretes() const {
    std::vector<Arete<S, T>*>* result = new std::vector<Arete<S, T>*>();
    PElement<Arete<S, T>*>* courant = lAretes;
    
    while (courant) {
        result->push_back(courant->v);
        courant = courant->suivant;
    }
    
    return result;
}

template <typename S, typename T>
Arete<S, T>* Graphe<S, T>::creeArete(const S& info, Sommet<T>* d, Sommet<T>* f) {
    Arete<S, T>* a = new Arete<S, T>(prochaineClef++, info, d, f);
    lAretes = new PElement<Arete<S, T>*>(a, lAretes);
    return a;
}

template <typename S, typename T>
int Graphe<S, T>::nombreSommets() const {
    int count = 0;
    PElement<Sommet<T>*>* curr = lSommets;
    while (curr) {
        count++;
        curr = curr->suivant;
    }
    return count;
}

template <typename S, typename T>
int Graphe<S, T>::nombreAretes() const {
    int count = 0;
    PElement<Arete<S, T>*>* curr = lAretes;
    while (curr) {
        count++;
        curr = curr->suivant;
    }
    return count;
}

template <typename S, typename T>
PElement<Sommet<T>*>* Graphe<S, T>::voisins(const Sommet<T>* s) const {
    PElement<Sommet<T>*>* res = nullptr;
    PElement<Arete<S, T>*>* curr = lAretes;
    while (curr) {
        if (curr->v->debut == s || curr->v->fin == s) {
            Sommet<T>* voisin = (curr->v->debut == s) ? curr->v->fin : curr->v->debut;
            res = new PElement<Sommet<T>*>(voisin, res);
        }
        curr = curr->suivant;
    }
    return res;
}

template <typename S, typename T>
PElement<Arete<S, T>*>* Graphe<S, T>::aretesAdjacentes(const Sommet<T>* s) const {
    PElement<Arete<S, T>*>* res = nullptr;
    PElement<Arete<S, T>*>* curr = lAretes;
    while (curr) {
        if (curr->v->debut == s || curr->v->fin == s) {
            res = new PElement<Arete<S, T>*>(curr->v, res);
        }
        curr = curr->suivant;
    }
    return res;
}

template <typename S, typename T>
PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* Graphe<S, T>::adjacences(const Sommet<T>* s) const {
    PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* res = nullptr;
    PElement<Arete<S, T>*>* curr = lAretes;
    while (curr) {
        if (curr->v->debut == s || curr->v->fin == s) {
            Sommet<T>* voisin = (curr->v->debut == s) ? curr->v->fin : curr->v->debut;
            res = new PElement<std::pair<Sommet<T>*, Arete<S, T>*>>(std::make_pair(voisin, curr->v), res);
        }
        curr = curr->suivant;
    }
    return res;
}

template <typename S, typename T>
Arete<S, T>* Graphe<S, T>::getAreteParSommets(const Sommet<T>* s1, const Sommet<T>* s2) const {
    PElement<Arete<S, T>*>* curr = lAretes;
    while (curr) {
        if (curr->v->estEgal(s1, s2)) return curr->v;
        curr = curr->suivant;
    }
    return nullptr;
}

template <typename S, typename T>
Graphe<S, T>& Graphe<S, T>::operator=(const Graphe<S, T>& other) {
    if (this == &other) return *this;
    
    // Libère l'état actuel
    if (this->lSommets) delete this->lSommets;
    if (this->lAretes) delete this->lAretes;
    
    // Réinitialise
    this->lSommets = nullptr;
    this->lAretes = nullptr;
    this->prochaineClef = other.prochaineClef;
    
    std::map<int, Sommet<T>*> mapSommets;
    
    // Copie les sommets
    PElement<Sommet<T>*>* currS = other.lSommets;
    while (currS) {
        Sommet<T>* s = new Sommet<T>(currS->v->clef, currS->v->v, 0);
        mapSommets[currS->v->clef] = s;
        this->lSommets = new PElement<Sommet<T>*>(s, this->lSommets);
        currS = currS->suivant;
    }
    
    // Copie les arêtes
    PElement<Arete<S, T>*>* currA = other.lAretes;
    while (currA) {
        Sommet<T>* d = mapSommets[currA->v->debut->clef];
        Sommet<T>* f = mapSommets[currA->v->fin->clef];
        Arete<S, T>* a = new Arete<S, T>(currA->v->clef, currA->v->v, d, f);
        this->lAretes = new PElement<Arete<S, T>*>(a, this->lAretes);
        currA = currA->suivant;
    }
    
    return *this;
}

template <typename S, typename T>
Graphe<S, T>::operator std::string() const {
    std::ostringstream oss;
    oss << "--- GRAPHE ---\nSommets:\n";
    
    PElement<Sommet<T>*>* s = lSommets;
    while(s) { 
        oss << "  " << *(s->v) << "\n"; 
        s = s->suivant; 
    }
    oss << "Aretes:\n";
    
    PElement<Arete<S, T>*>* a = lAretes;
    while(a) { 
        oss << "  " << *(a->v) << "\n"; 
        a = a->suivant; 
    }
    return oss.str();
}

#endif