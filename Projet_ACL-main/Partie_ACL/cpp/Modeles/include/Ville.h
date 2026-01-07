#ifndef PROJET_ACL_VILLE_H
#define PROJET_ACL_VILLE_H

#include <string>
#include <iostream>

using namespace std;

/**
 * @brief Représente une ville en fonction de son nom et ses coordonnées géographiques.
 */
class Ville {
    std::string name;
    double latitude;
    double longitude;

public:
    /**
     * @brief Constructeur de Ville.
     * 
     * @param name - Nom de la ville.
     * @param latitude - Latitude géographique.
     * @param longitude - Longitude géographique.
     */
    Ville(const string &name, double latitude, double longitude)
        : name(name),
          latitude(latitude),
          longitude(longitude) {
    }

    /**
     * @brief Destructeur de Ville.
     */
    virtual ~Ville() = default;

    /**
     * @brief Récupère le nom de la ville.
     * 
     * @return string 
     */
    [[nodiscard]] string getName() const { return name; }

    /**
     * @brief Récupère la latitude de la ville.
     * 
     * @return double 
     */
    [[nodiscard]] double getLatitude() const { return latitude; }

    /**
     * @brief Récupère la longitude de la ville.
     * 
     * @return double 
     */
    [[nodiscard]] double getLongitude() const { return longitude; }

    /**
     * @brief Surcharge de l'opérateur de conversion en chaîne de caractères.
     * 
     * @return std::string - Représentation textuelle de la ville.
     */
    virtual operator string() const {
        return name;
    }

    /**
     * @brief Surcharge de l'opérateur d'égalité.
     * 
     * @param lhs - Gauche.
     * @param rhs - Droite.
     * @return true 
     * @return false 
     */
    friend bool operator==(const Ville &lhs, const Ville &rhs) {
        return lhs.name == rhs.name;
    }

    /**
     * @brief Surcharge de l'opérateur de différence.
     * 
     * @param lhs - Gauche.
     * @param rhs - Droite.
     * @return true 
     * @return false 
     */
    friend bool operator!=(const Ville &lhs, const Ville &rhs) {
        return !(lhs == rhs);
    }

    /**
     * @brief Surcharge de l'opérateur d'affichage.
     * 
     * @param os - Flux de sortie.
     * @param obj - Objet Ville à afficher.
     * @return ostream& - Flux de sortie mis à jour.
     */
    friend ostream &operator<<(ostream &os, const Ville &obj) {
        os << static_cast<string>(obj);
        return os;
    }
};

#endif //PROJET_ACL_VILLE_H