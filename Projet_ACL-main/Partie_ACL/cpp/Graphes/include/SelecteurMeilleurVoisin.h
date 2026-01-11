//
// Created by guill on 07/01/2026.
//

#ifndef PARTIE_ACL_SELECTEURMEILLEURVOISIN_H
#define PARTIE_ACL_SELECTEURMEILLEURVOISIN_H

template<typename S, typename T>
class SelecteurMeilleurVoisin {
public:
    int operator()(Sommet<T>* courant, const std::vector<Sommet<T>*>& candidats, const Graphe<S, T>& graphe) {
        double minPoids = std::numeric_limits<double>::max();
        int indexMeilleur = -1;

        for (size_t i = 0; i < candidats.size(); ++i) {
            Arete<S, T>* a = graphe.getAreteParSommets(courant, candidats[i]);
            if (a != nullptr && a->v < minPoids) {
                minPoids = a->v;
                indexMeilleur = i;
            }
        }
        return indexMeilleur;
    }
};

#endif //PARTIE_ACL_SELECTEURMEILLEURVOISIN_H