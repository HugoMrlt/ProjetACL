#ifndef PROJET_ACL_VILLE_H
#define PROJET_ACL_VILLE_H

#include <string>
#include <iostream>

using namespace std;

/**
 * Représente une ville en fonction de son nom et ses coordonnées géographiques.
 */
class Ville {
    std::string name;
    double latitude;
    double longitude;

public:

    Ville(const string &name, double latitude, double longitude)
        : name(name),
          latitude(latitude),
          longitude(longitude) {
    }

    virtual ~Ville() = default;


    [[nodiscard]] string getName() const { return name; }
    [[nodiscard]] double getLatitude() const { return latitude; }
    [[nodiscard]] double getLongitude() const { return longitude; }


    virtual operator string() const {
        return name;
    }


    friend bool operator==(const Ville &lhs, const Ville &rhs) {
        return lhs.name == rhs.name;
    }

    friend bool operator!=(const Ville &lhs, const Ville &rhs) {
        return !(lhs == rhs);
    }


    friend ostream &operator<<(ostream &os, const Ville &obj) {
        os << static_cast<string>(obj);
        return os;
    }
};

#endif //PROJET_ACL_VILLE_H