#include <iostream>
#include <vector>
#include <string>

#include "JsonOutils.h"
#include "CsvOutils.h"
#include "Ville.h"
#include "GeoOutils.h"
#include "Graphe.h"
#include "TSP.h"
#include "Constantes.h"
#include "TSPTemps.h"
#include "TSPDistance.h"
#include "VisiteurAffichage.h"
#include "VisiteurStatistiques.h"

using namespace std;

int main() {
    try {
        // 1. CHARGEMENT DES DONNÉES
        vector<Ville> Villes = JsonOutils::chargerDepuisFichier(trouverCheminAssets("Region/GrandEst.json"));
        auto mapTypesRoutes = CsvOutils::chargerTypesRoute(trouverCheminAssets("Type_de_route/GrandEst.csv"));

        // 2. INSTANCIATION DU VISITEUR
        VisiteurAffichage afficheur;
        VisiteurStatistiques stats;

        // --- SECTION DISTANCE ---
        cout << "==============================================" << endl;
        cout << "   OPTIMISATION DISTANCE (KM)" << endl;
        cout << "==============================================" << endl;

        Graphe<double, Ville> grapheDistance;
        vector<Sommet<Ville>*> sommetsDist;
        IStrategieTSP* stratDist = new TSPDistance();

        for (const auto& v : Villes) {
            sommetsDist.push_back(grapheDistance.creeSommet(v));
        }

        // --- UTILISATION DU VISITEUR POUR LISTER LES VILLES ---
        cout << "\n--- Sommets du Graphe (via Visiteur) ---" << endl;
        PElement<Sommet<Ville>*>* curr = grapheDistance.lSommets;
        while (curr != nullptr) {
            curr->v->accept(&afficheur); // On délègue l'affichage au visiteur
            curr = curr->suivant;
        }

        // Création des arêtes
        for (size_t i = 0; i < sommetsDist.size(); ++i) {
            for (size_t j = i + 1; j < sommetsDist.size(); ++j) {
                double distKm = GeoOutils::calculerDistance(sommetsDist[i]->v, sommetsDist[j]->v);
                double poids = stratDist->calculerPoids(distKm, "Nationale");
                grapheDistance.creeArete(poids, sommetsDist[i], sommetsDist[j]);
            }
        }

        if (!sommetsDist.empty()) {
            auto parcours = TSP<double, Ville>::plusProcheVoisin(grapheDistance, sommetsDist[0]);

            cout << "\n>>> Resultat Parcours DISTANCE :" << endl;
            stats.reset(); // On remet les compteurs à zéro
            for (size_t i = 0; i < parcours.size(); ++i) {
                // Double dispatch : l'élément accepte les deux visiteurs l'un après l'autre [cite: 92, 125]
                parcours[i]->accept(&afficheur);
                parcours[i]->accept(&stats);

                if (i < parcours.size() - 1) {
                    cout << "  |-> ";
                    // On récupère l'arête pour la faire visiter également
                    auto* arete = grapheDistance.getAreteParSommets(parcours[i], parcours[i+1]);
                    if(arete) arete->accept(&stats);
                }
            }
            // Affichage du bilan final via le visiteur
            cout << "\n\nBILAN : " << stats.getEtapes() << " villes visitees sur " << stats.getDistance() << " km." << endl;
        }
        delete stratDist;

        // --- SECTION TEMPS ---
        cout << "\n\n==============================================" << endl;
        cout << "   OPTIMISATION TEMPS (HEURES)" << endl;
        cout << "==============================================" << endl;

        Graphe<double, Ville> grapheTemps;
        vector<Sommet<Ville>*> sommetsTemps;
        IStrategieTSP* stratTemps = new TSPTemps();

        for (const auto& v : Villes) {
            sommetsTemps.push_back(grapheTemps.creeSommet(v));
        }

        for (size_t i = 0; i < sommetsTemps.size(); ++i) {
            for (size_t j = i + 1; j < sommetsTemps.size(); ++j) {
                double distKm = GeoOutils::calculerDistance(sommetsTemps[i]->v, sommetsTemps[j]->v);
                string typeRoute = CsvOutils::getTypeRoute(mapTypesRoutes, sommetsTemps[i]->v.getName(), sommetsTemps[j]->v.getName());
                double poidsHeures = stratTemps->calculerPoids(distKm, typeRoute);
                grapheTemps.creeArete(poidsHeures, sommetsTemps[i], sommetsTemps[j]);
            }
        }

        if (!sommetsTemps.empty()) {
            auto parcours = TSP<double, Ville>::plusProcheVoisin(grapheTemps, sommetsTemps[0]);
            cout << "\n>>> Resultat Parcours TEMPS :" << endl;
            for (size_t i = 0; i < parcours.size(); ++i) {
                parcours[i]->accept(&afficheur); // Affichage de l'étape via visiteur
                if (i < parcours.size() - 1) cout << "  |-> ";
            }
        }
        delete stratTemps;

    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
        return 1;
    }

    cout << "\n=== Fin du Test TSP ===" << endl;

    return 0;
}