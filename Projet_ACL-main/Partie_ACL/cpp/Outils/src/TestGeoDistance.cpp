#include <iostream>
#include <vector>
#include <string>

#include "JsonOutils.h"
#include "GeoOutils.h"
#include "Ville.h"
#include "Constantes.h"

using namespace std;

int main() {
    try {

        string chemin = trouverCheminAssets("GrandEst.json");
        vector<Ville> Villes = JsonOutils::chargerDepuisFichier(chemin);

        cout << "--- TEST DISTANCE REELLE (JSON) ---" << endl;
        cout << Villes.size() << " villes chargees." << endl;


        Ville* v1 = nullptr;
        Ville* v2 = nullptr;

        for (auto& v : Villes) {
            if (v.getName()== "Strasbourg") v1 = &v;
            if (v.getName() == "Reims") v2 = &v;
        }

        // 3. Calculer et afficher
        if (v1 && v2) {
            double d = GeoOutils::calculerDistance(*v1, *v2);

            cout << "\nCalcul entre deux villes :" << endl;
            cout << "Depart : " << v1->getName()<< " (" << v1->getLatitude() << ", " << v1->getLongitude() << ")" << endl;
            cout << "Arrivee : " << v2->getName() << " (" << v2->getLatitude() << ", " << v2->getLongitude() << ")" << endl;
            cout << "-----------------------------------" << endl;
            cout << "DISTANCE CALCULEE : " << d << " km" << endl;
            cout << "-----------------------------------" << endl;

            // Note : Strasbourg-Reims fait environ 190-200km à vol d'oiseau.
        } else {
            cout << "\nErreur : Strasbourg ou Reims non trouvees dans le JSON." << endl;
            cout << "Villes disponibles : ";
            for(const auto& v : Villes) cout << v.getName() << ", ";
            cout << endl;
        }

    } catch (const exception& e) {
        cerr << "Erreur lors du test : " << e.what() << endl;
    }

    return 0;
}