#ifndef GRAPHE_H
#define GRAPHE_H

#include "PElement.h"
#include "Sommet.h"
#include "Arete.h"
#include <string>
#include <sstream>
#include <utility>
#include <map>

/**
 * @brief Classe représentant un graphe.
 * 
 * @tparam S - Type des données associées aux arêtes.
 * @tparam T - Type des données associées aux sommets.
 */
template <typename S, typename T>
class Graphe {
public:
    PElement<Sommet<T>*>* lSommets;      // Liste des sommets.
    PElement<Arete<S, T>*>* lAretes;    // Liste des arêtes.
    int prochaineClef;                  // Générateur de clefs primaires.

    /**
     * @brief Constructeur par défaut.
     */
    Graphe();

    /**
     * @brief Constructeur de copie.
     * 
     * @param other - Graphe à copier.
     */
    Graphe(const Graphe& other);

    /**
     * @brief Destructeur.
     * Libère toute la mémoire allouée pour les sommets et les arêtes.
     */
    virtual ~Graphe();

    /*-------------Création d'éléments-------------*/

    /**
     * @brief Crée un sommet dans le graphe.
     * 
     * @param info - Informations associées au sommet.
     * @return Sommet<T>* - Pointeur vers le nouveau sommet.
     */
    Sommet<T>* creeSommet(const T& info);

    /**
     * @brief Crée une arête dans le graphe.
     * 
     * @param info - Informations associées à l'arête.
     * @param debut - Pointeur vers le sommet de début.
     * @param fin - Pointeur vers le sommet de fin.
     * @return Arete<S, T>* - Pointeur vers la nouvelle arête.
     */
    Arete<S, T>* creeArete(const S& info, Sommet<T>* debut, Sommet<T>* fin);

    /*---------------Getters---------------*/

    /**
     * @brief Récupère tous les sommets du graphe.
     * 
     * @return std::vector<Sommet<T>*> - Vecteur des pointeurs sur les sommets.
     */
    std::vector<Sommet<T>*> getSommets() const;

    /**
     * @brief Récupère toutes les arêtes du graphe.
     * 
     * @return std::vector<Arete<S, T>*>* - Vecteur des pointeurs sur les arêtes.
     */
    std::vector<Arete<S, T>*>* getAretes() const;

    /*-------------Méthodes d'accès et topologie-------------*/

    /**
     * @brief Récupère le nombre de sommets du graphe.
     * 
     * @return int - Nombre de sommets.
     */
    int nombreSommets() const;

    /**
     * @brief Récupère le nombre d'arêtes du graphe.
     * 
     * @return int - Nombre d'arêtes.
     */
    int nombreAretes() const;

    /**
     * @brief Récupère les voisins d'un sommet donné.
     * 
     * @param s - Pointeur vers le sommet dont on veut les voisins.
     * @return PElement<Sommet<T>*>* - Liste des voisins.
     */
    PElement<Sommet<T>*>* voisins(const Sommet<T>* s) const;

    /**
     * @brief Récupère les arêtes adjacentes à un sommet donné.
     * 
     * @param s - Pointeur vers le sommet dont on veut les arêtes adjacentes.
     * @return PElement<Arete<S, T>*>* - Liste des arêtes adjacentes.
     */
    PElement<Arete<S, T>*>* aretesAdjacentes(const Sommet<T>* s) const;

    /**
     * @brief Récupère les adjacences d'un sommet donné.
     * 
     * @param s - Pointeur vers le sommet dont on veut les adjacences.
     * @return PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* - Liste des adjacences.
     */
    PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* adjacences(const Sommet<T>* s) const;

    /**
     * @brief Récupère une arête entre deux sommets donnés.
     * 
     * @param s1 - Pointeur vers le sommet de début.
     * @param s2 - Pointeur vers le sommet de fin.
     * @return Arete<S, T>* - Pointeur vers l'arête trouvée, ou nullptr si non trouvée.
     */
    Arete<S, T>* getAreteParSommets(const Sommet<T>* s1, const Sommet<T>* s2) const;

    /*-------------Surcharge d'opérateurs-------------*/

    /**
     * @brief Surcharge de l'opérateur de conversion en chaîne de caractères.
     * 
     * @return std::string - Représentation textuelle du graphe.
     */
    operator std::string() const;

    /**
     * @brief Surcharge de l'opérateur d'affectation.
     * 
     * @param other - Graphe à copier.
     * @return Graphe<S, T>& - Référence sur le graphe courant.
     */
    Graphe<S, T>& operator=(const Graphe<S, T>& other);

    double getDistanceTotale(int camionId) const;
    double getDureeTotale(int camionId) const;
    std::vector<Arete<S, T>*> getAretesTypeInconnu(int camionId) const;

};

/*-------------Implémentation des méthodes-------------*/
template <typename S, typename T>
Graphe<S, T>::Graphe() : lSommets(nullptr), lAretes(nullptr), prochaineClef(0) {}

template <typename S, typename T>
Graphe<S, T>::Graphe(const Graphe<S, T>& other) 
    : lSommets(nullptr), lAretes(nullptr), prochaineClef(other.prochaineClef) {
    
    std::map<int, Sommet<T>*> mapSommets;

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

    if (lAretes) {
        PElement<Arete<S, T>*>* curr = lAretes;
        while (curr->suivant) curr = curr->suivant;
        delete lAretes;  // Le destructeur de PElement se charge du reste
    }
    
    // Suppression des sommets (y compris la chaîne PElement)
    if (lSommets) {
        PElement<Sommet<T>*>* curr = lSommets;
        while (curr->suivant) curr = curr->suivant;
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
double Graphe<S, T>::getDistanceTotale(int camionId) const {
    double total = 0.0;
    bool trouve = false;
    PElement<Arete<S, T>*>* curr = lAretes;

    while (curr) {
        if (camionId == -1 || curr->v->v.camionId == camionId) {
            total += curr->v->v.poids;
            trouve = true;
        }
        curr = curr->suivant;
    }
    return trouve ? total : -1.0;
}

template <typename S, typename T>
double Graphe<S, T>::getDureeTotale(int camionId) const {
    double totalHeures = 0.0;
    PElement<Arete<S, T>*>* curr = lAretes;

    while (curr) {
        if (camionId == -1 || curr->v->v.camionId == camionId) {
            double vitesse = 0;
            string t = curr->v->v.type;

            if (t == "Autoroute") vitesse = 90.0;
            else if (t == "Voie rapide" || t == "Route européenne") vitesse = 80.0;
            else if (t == "Nationale") vitesse = 70.0;
            else if (t == "Départementale") vitesse = 60.0;
            else if (t == "Communale") vitesse = 40.0;

            if (vitesse > 0) {
                totalHeures += (double)curr->v->v.poids / vitesse;
            }
        }
        curr = curr->suivant;
    }
    return totalHeures;
}

template <typename S, typename T>
std::vector<Arete<S, T>*> Graphe<S, T>::getAretesTypeInconnu(int camionId) const {
    std::vector<Arete<S, T>*> problemes;
    PElement<Arete<S, T>*>* curr = lAretes;

    // Utilisation de la même logique que pour la durée
    auto estVitesseInvalide = [](std::string type) -> bool {
        if (type == "Autoroute") return false;
        if (type == "Voie rapide" || type == "Route européenne") return false;
        if (type == "Nationale") return false;
        if (type == "Départementale") return false;
        if (type == "Communale") return false;
        return true;
    };

    while (curr) {
        if (camionId == -1 || curr->v->v.camionId == camionId) {
            if (estVitesseInvalide(curr->v->v.type)) {
                problemes.push_back(curr->v);
            }
        }
        curr = curr->suivant;
    }
    return problemes;
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