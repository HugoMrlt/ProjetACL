#ifndef PELEMENT_H
#define PELEMENT_H

#include <iostream>

/**
 * @brief Classe représentant un élément d'une liste chaînée.
 * 
 * @tparam T - Type des données stockées dans l'élément.
 */
template <typename T>
class PElement {
public:
    T v;                       // Donnée stockée (pointeur, valeur, etc.).
    PElement<T>* suivant;      // Pointeur vers l'élément suivant de la liste.

    /**
     * @brief Constructeur : permet d'insérer facilement en tête de liste.
     * 
     * @param valeur - Valeur à stocker dans l'élément.
     * @param next - Pointeur vers l'élément suivant (par défaut nullptr).
     */
    PElement(const T& valeur, PElement<T>* next = nullptr) 
        : v(valeur), suivant(next) {}

    /**
     * @brief Destructeur : libère récursivement toute la liste.
     */
    ~PElement() {
        if (suivant) delete suivant;  // Recursively delete the chain
    }

    /**
     * @brief Surcharge de l'opérateur << pour afficher une liste complète.
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

/**
 * @brief Spécialisation de PElement pour les pointeurs.
 * 
 * @tparam T - Type des données pointées.
 */
template <typename T>
class PElement<T*> {
public:
    T* v;                      // Pointeur vers la donnée.
    PElement<T*>* suivant;     // Pointeur vers l'élément suivant.

    /**
     * @brief Constructeur : permet d'insérer facilement en tête de liste.
     * 
     * @param valeur - Pointeur à stocker dans l'élément.
     * @param next - Pointeur vers l'élément suivant (par défaut nullptr).
     */
    PElement(T* const& valeur, PElement<T*>* next = nullptr) 
        : v(valeur), suivant(next) {}

    /**
     * @brief Destructeur : libère récursivement toute la liste.
     */
    ~PElement() {
        if (suivant) delete suivant;
    }

    /**
     * @brief Surcharge de l'opérateur << pour afficher une liste complète de pointeurs.
     */
    friend std::ostream& operator<<(std::ostream& os, const PElement<T*>& liste) {
        const PElement<T*>* courant = &liste;
        while (courant != nullptr) {
            os << *courant->v;  // Déréférence le pointeur.
            
            courant = courant->suivant;
            if (courant != nullptr) os << " -> ";
        }
        return os;
    }
};

#endif