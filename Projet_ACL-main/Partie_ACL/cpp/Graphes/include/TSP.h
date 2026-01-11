#ifndef TSP_H
#define TSP_H

#include "Graphe.h"
#include <vector>
#include <limits>

/**
 * @brief Algorithme TSP générique (heuristique du plus proche voisin).
 * 
 * @tparam S - Type des poids des arêtes.
 * @tparam T - Type des données des sommets.
 */
template<typename S, typename T>
class TSP {
public:
    /**
     * Calcule un parcours TSP approché (plus proche voisin)
     * @param graphe : le graphe complet
     * @param depart : sommet de départ
     * @return vecteur de sommets dans l'ordre de visite
     */
    template<typename SelecteurVoisin>
    static std::vector<Sommet<T>*> plusProcheVoisin(const Graphe<S, T>& graphe, Sommet<T>* depart,SelecteurVoisin choisir) {
        std::vector<Sommet<T>*> parcours;

        if (!depart || graphe.lSommets == nullptr) return parcours;

        // 1. On récupère tous les sommets à visiter
        std::vector<Sommet<T>*> aVisiter;
        PElement<Sommet<T>*>* curr = graphe.lSommets;
        while (curr != nullptr) {
            if (curr->v != depart) {
                aVisiter.push_back(curr->v);
            }
            curr = curr->suivant;
        }

        // 2. Initialisation du parcours
        Sommet<T>* courant = depart;
        parcours.push_back(courant);

        // 3. Boucle gloutonne
        while (!aVisiter.empty()) {
            //double minDist = std::numeric_limits<double>::max();
            int indexMeilleurVoisin = -1;

            indexMeilleurVoisin = choisir(courant, aVisiter, graphe);
            /**

            // Recherche du voisin le plus proche dans la liste "aVisiter"
            for (size_t i = 0; i < aVisiter.size(); ++i) {
                Sommet<T>* candidat = aVisiter[i];

                Arete<S, T>* a = graphe.getAreteParSommets(courant, candidat);

                if (a != nullptr) {
                    if (a->v < minDist) {
                        minDist = a->v;
                        indexMeilleurVoisin = i;
                    }
                }
            }
            **/

            if (indexMeilleurVoisin == -1) {
                break;
            }

            // 4. Déplacement vers le meilleur voisin
            Sommet<T>* prochain = aVisiter[indexMeilleurVoisin];
            parcours.push_back(prochain);
            courant = prochain;

            // 5. On retire le voisin visité de la liste
            aVisiter[indexMeilleurVoisin] = aVisiter.back();
            aVisiter.pop_back();
        }

        // 6. Retour au départ pour fermer la boucle
        parcours.push_back(depart);

        return parcours;
    }
};

#endif // TSP_H