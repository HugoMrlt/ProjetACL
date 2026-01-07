#ifndef PROJET_ACL_GEOOUTILS_H
#define PROJET_ACL_GEOOUTILS_H
#include <tgmath.h>

/**
 * @brief Classe utilitaire pour les calculs géographiques.
 * En ACL, on utilise souvent des méthodes statiques pour les "boîtes à outils".
 */
class GeoOutils {
public:
    // Rayon moyen de la Terre en kilomètres
    static constexpr double RAYON_TERRE = 6371.0;
    static constexpr double PI = 3.14159265358979323846;

    /**
     * Calcule la distance géodésique entre deux villes.
     * @param v1 Première ville
     * @param v2 Deuxième ville
     * @return distance en km
     */
    static double calculerDistance(const Ville& v1, const Ville& v2) {

        double lat1 = v1.getLatitude();
        double lon1 = v1.getLongitude();
        double lat2 = v2.getLatitude();
        double lon2 = v2.getLongitude();

        double lat1Rad = degreVersRadiant(lat1);
        double lon1Rad = degreVersRadiant(lon1);
        double lat2Rad = degreVersRadiant(lat2);
        double lon2Rad = degreVersRadiant(lon2);


        double dlat = lat2Rad - lat1Rad;
        double dlon = lon2Rad - lon1Rad;


        double a = pow(sin(dlat / 2), 2) + cos(lat1Rad) * cos(lat2Rad) * pow(sin(dlon / 2), 2);
        double c = 2 * atan2(sqrt(a),sqrt(1 - a));


        return RAYON_TERRE * c;
    }

private:
    /**
     * Convertit des degrés en radians.
     * @param degre Angle en degrés
     * @return Angle en radians
     */
    static double degreVersRadiant(double degre) {
        return degre * (PI / 180.0);
    }
};


#endif //PROJET_ACL_GEOOUTILS_H