#include <iostream>
#include <vector>

#include "JsonOutils.h"
#include "CsvOutils.h"
#include "Ville.h"
#include "GeoOutils.h"
#include "Graphe.h"
#include "TSP.h"
#include "Constantes.h"
#include "TSPTemps.h"
#include "TSPDistance.h"

using namespace std;

int main() {
    try {

        vector<Ville> Villes = JsonOutils::chargerDepuisFichier(trouverCheminAssets("Region/GrandEst.json"));

        auto mapTypesRoutes = CsvOutils::chargerTypesRoute(trouverCheminAssets("Type_de_route/GrandEst.csv"));

        Graphe<double, Ville> grapheDistance;
        vector<Sommet<Ville>*> sommetsDist;
        IStrategieTSP* stratDist = new TSPDistance();

        // 1.1 Création Sommets
        for (const auto& v : Villes) {
            sommetsDist.push_back(grapheDistance.creeSommet(v));
        }

        cout << "\n--- Liste des villes (Sommets du Graphe) ---" << endl;
        PElement<Sommet<Ville>*>* curr = grapheDistance.lSommets;
        while (curr != nullptr) {
            cout << "- " << curr->v->v.getName()
                 << " (Lat: " << curr->v->v.getLatitude()
                 << ", Lon: " << curr->v->v.getLongitude() << ")" << endl;
            curr = curr->suivant;
        }
        cout << "--------------------------------------------\n" << endl;


        // 1.2 Création Arêtes (Mode Distance)
        for (size_t i = 0; i < sommetsDist.size(); ++i) {
            for (size_t j = i + 1; j < sommetsDist.size(); ++j) {
                double distKm = GeoOutils::calculerDistance(sommetsDist[i]->v, sommetsDist[j]->v);

                // Poids = Distance pure
                double poids = stratDist->calculerPoids(distKm, "Nationale");

                grapheDistance.creeArete(poids, sommetsDist[i], sommetsDist[j]);
                grapheDistance.creeArete(poids, sommetsDist[j], sommetsDist[i]);
            }
        }

        cout << "\n==============================================" << endl;
        cout << "   TEST 1 : OPTIMISATION DISTANCE (KM)" << endl;
        cout << "==============================================" << endl;

        // 1.3 Exécution TSP Distance
        if (!sommetsDist.empty()) {
            cout << "Calcul du TSP (Plus Proche Voisin)..." << endl;
            auto parcours = TSP<double, Ville>::plusProcheVoisin(grapheDistance, sommetsDist[0]);

            cout << "\n>>> Resultat Parcours DISTANCE :" << endl;
            for (size_t i = 0; i < parcours.size(); ++i) {
                cout << parcours[i]->v.getName();
                if (i < parcours.size() - 1) cout << " -> ";
            }
            cout << "\n" << endl;
        }
        delete stratDist;

        cout << "\n==============================================" << endl;
        cout << "   TEST 2 : OPTIMISATION TEMPS (HEURES)" << endl;
        cout << "==============================================" << endl;

        Graphe<double, Ville> grapheTemps;
        vector<Sommet<Ville>*> sommetsTemps;
        IStrategieTSP* stratTemps = new TSPTemps();

        // 2.1 Création Sommets
        for (const auto& v : Villes) {
            sommetsTemps.push_back(grapheTemps.creeSommet(v));
        }

        // 2.2 Création Arêtes (Mode Temps avec CSV)
        for (size_t i = 0; i < sommetsTemps.size(); ++i) {
            for (size_t j = i + 1; j < sommetsTemps.size(); ++j) {
                // a. Distance physique
                double distKm = GeoOutils::calculerDistance(sommetsTemps[i]->v, sommetsTemps[j]->v);

                // b. Type de route (via CSV)
                string nomA = sommetsTemps[i]->v.getName();
                string nomB = sommetsTemps[j]->v.getName();
                string typeRoute = CsvOutils::getTypeRoute(mapTypesRoutes, nomA, nomB);

                // c. Poids = Temps en heures
                double poidsHeures = stratTemps->calculerPoids(distKm, typeRoute);

                grapheTemps.creeArete(poidsHeures, sommetsTemps[i], sommetsTemps[j]);
                grapheTemps.creeArete(poidsHeures, sommetsTemps[j], sommetsTemps[i]);
            }
        }

        // 2.3 Exécution TSP Temps
        if (!sommetsTemps.empty()) {
            cout << "Calcul du TSP (Temps de parcours)..." << endl;
            auto parcours = TSP<double, Ville>::plusProcheVoisin(grapheTemps, sommetsTemps[0]);

            cout << "\n>>> Resultat Parcours TEMPS :" << endl;
            for (size_t i = 0; i < parcours.size(); ++i) {
                cout << parcours[i]->v.getName();
                if (i < parcours.size() - 1) cout << " -> ";
            }
            cout << "\n" << endl;
        }
        delete stratTemps;

    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
    }

    return 0;
}