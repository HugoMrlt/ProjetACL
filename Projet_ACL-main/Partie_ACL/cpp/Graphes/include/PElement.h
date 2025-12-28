#ifndef PELEMENT_H
#define PELEMENT_H

#include <iostream>

template <typename T>
class PElement {
public:
    T v;                       // Donnée stockée (pointeur, valeur, etc.).
    PElement<T>* suivant;      // Pointeur vers l'élément suivant de la liste.

    // Constructeur : permet d'insérer facilement en tête de liste.
    PElement(const T& valeur, PElement<T>* next = nullptr) 
        : v(valeur), suivant(next) {}

    // Destructeur : libère récursivement toute la liste.
    ~PElement() {
        if (suivant) delete suivant;  // Recursively delete the chain
    }

    /**
     * Opérateur << pour afficher une liste complète.
     * Très utile pour vérifier le contenu du graphe.
     */
    friend std::ostream& operator<<(std::ostream& os, const PElement<T>& liste) {
    const PElement<T>* courant = &liste;
    while (courant != nullptr) {
        // On affiche la valeur directement
        os << courant->v;
        
        courant = courant->suivant;
        if (courant != nullptr) os << " -> ";
    }
    return os;
}
};

// Spécialisation pour les pointeurs : déréférence automatiquement
template <typename T>
class PElement<T*> {
public:
    T* v;                      // Pointeur vers la donnée.
    PElement<T*>* suivant;     // Pointeur vers l'élément suivant.

    // Constructeur
    PElement(T* const& valeur, PElement<T*>* next = nullptr) 
        : v(valeur), suivant(next) {}

    // Destructeur
    ~PElement() {
        if (suivant) delete suivant;
    }

    // Opérateur << qui déréférence automatiquement
    friend std::ostream& operator<<(std::ostream& os, const PElement<T*>& liste) {
        const PElement<T*>* courant = &liste;
        while (courant != nullptr) {
            os << *courant->v;  // Déréférence le pointeur
            
            courant = courant->suivant;
            if (courant != nullptr) os << " -> ";
        }
        return os;
    }
};

#endif