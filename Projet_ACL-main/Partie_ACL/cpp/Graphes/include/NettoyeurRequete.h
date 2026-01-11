#ifndef NETTOYEUR_REQUETE_H
#define NETTOYEUR_REQUETE_H

#include <string>
#include <algorithm>

/**
 * @brief Foncteur pour nettoyer les chaînes de caractères reçues par socket.
 * Supprime les espaces, tabulations et retours à la ligne au début et à la fin.
 */
struct NettoyeurRequete {
    void operator()(std::string& s) const {
        // Nettoyage au début
        size_t first = s.find_first_not_of(" \t\r\n");
        if (std::string::npos == first) {
            s = "";
            return;
        }
        // Nettoyage à la fin
        size_t last = s.find_last_not_of(" \t\r\n");
        s = s.substr(first, (last - first + 1));
    }
};

#endif