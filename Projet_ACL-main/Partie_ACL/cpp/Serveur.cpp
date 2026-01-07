#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <json.hpp> 

#include "Ville.h"
#include "GeoOutils.h"
#include "Graphe.h"
#include "TSP.h"
#include "TSPDistance.h"
#include "TSPTemps.h"
#include "JsonOutils.h"
#include "Constantes.h"
#include "CsvOutils.h"

using json = nlohmann::json;
using namespace std;

/**
 * @brief Point d'entrée du serveur C++ pour le TSP.
 * 
 * @return int 
 */
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (::bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }
    listen(server_fd, 3);
    std::cout << "Serveur TSP C++ pret sur le port 8080..." << endl;

    string region, mode, villeDepart;
    int nbCamions = 1;
    vector<Ville> baseVilles;
    auto mapTypesRoutes = std::map<std::string, std::map<std::string, std::string>>();

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        char buffer[2048] = {0};
        read(new_socket, buffer, 2048);
        string requete(buffer);
        std::cout << "Requete Java recue : " << requete << endl;

        stringstream ss(requete);
        getline(ss, region, ';');
        getline(ss, mode, ';');
        getline(ss, villeDepart, ';');
        string nbCamionsStr;
        getline(ss, nbCamionsStr, ';');

        // RECUPERATION DU NOMBRE DE CAMIONS DEPUIS JAVA
        try {
            nbCamions = stoi(nbCamionsStr);
            if (nbCamions < 1) nbCamions = 1;
        } catch (...) {
            nbCamions = 1;
        }

        region.erase(0, region.find_first_not_of(" \t\r\n"));
        region.erase(region.find_last_not_of(" \t\r\n") + 1);
        mode.erase(0, mode.find_first_not_of(" \t\r\n"));
        mode.erase(mode.find_last_not_of(" \t\r\n") + 1);
        villeDepart.erase(0, villeDepart.find_first_not_of(" \t\r\n"));
        villeDepart.erase(villeDepart.find_last_not_of(" \t\r\n") + 1);

        string cheminVilles = trouverCheminAssets("Region/" + region + ".json");
        baseVilles = JsonOutils::chargerDepuisFichier(cheminVilles);
        string cheminCSV = trouverCheminAssets("Type_de_route/" + region + ".csv");
        mapTypesRoutes = CsvOutils::chargerTypesRoute(cheminCSV);

        Graphe<double, Ville> grapheDistance;
        vector<Sommet<Ville>*> sommetsSelectionnes;
        IStrategieTSP* strat = (mode == "TEMPS") ? (IStrategieTSP*)new TSPTemps() : (IStrategieTSP*)new TSPDistance();

        vector<string> nomsVilles;
        nomsVilles.push_back(villeDepart);
        for (const auto& v : baseVilles) {
            if (v.getName() != villeDepart) nomsVilles.push_back(v.getName());
        }
        for (const auto& nomVille : nomsVilles) {
            for (const auto& v : baseVilles) {
                if (v.getName() == nomVille) {
                    sommetsSelectionnes.push_back(grapheDistance.creeSommet(v));
                    break;
                }
            }
        }

        for (size_t i = 0; i < sommetsSelectionnes.size(); ++i) {
            for (size_t j = i + 1; j < sommetsSelectionnes.size(); ++j) {
                double distKm = GeoOutils::calculerDistance(sommetsSelectionnes[i]->v, sommetsSelectionnes[j]->v);
                string typeRouteReel = CsvOutils::getTypeRoute(mapTypesRoutes, sommetsSelectionnes[i]->v.getName(), sommetsSelectionnes[j]->v.getName());
                double poids = strat->calculerPoids(distKm, typeRouteReel);
                grapheDistance.creeArete(poids, sommetsSelectionnes[i], sommetsSelectionnes[j]);
                grapheDistance.creeArete(poids, sommetsSelectionnes[j], sommetsSelectionnes[i]);
            }
        }

        vector<Sommet<Ville>*> solution = TSP<double, Ville>::plusProcheVoisin(grapheDistance, sommetsSelectionnes[0]);

        json res;
        res["windowName"] = "Tournee - " + to_string(nbCamions) + " Camions";
        res["vertices"] = json::array();
        res["edges"] = json::array();

        for (auto* s : sommetsSelectionnes) {
            res["vertices"].push_back({
                {"ville", s->v.getName()},
                {"latitude", (double)s->v.getLatitude()},
                {"longitude", (double)s->v.getLongitude()}
            });
        }

        if (!solution.empty()) {
            auto depot = solution[0];
            vector<Sommet<Ville>*> clients(solution.begin() + 1, solution.end());
            int total = clients.size();
            int parCamion = (total + nbCamions - 1) / nbCamions;

            for (int c = 0; c < nbCamions; ++c) {
                int startIdx = c * parCamion;
                if (startIdx >= total) break;
                int endIdx = min(startIdx + parCamion, total);
                int currentTruckId = c + 1;

                vector<Sommet<Ville>*> trajet;
                trajet.push_back(depot);
                for (int i = startIdx; i < endIdx; ++i) trajet.push_back(clients[i]);
                trajet.push_back(depot);

                for (size_t i = 0; i < trajet.size() - 1; ++i) {
                    if (trajet[i]->v.getName() == trajet[i+1]->v.getName()) continue;

                    res["edges"].push_back({
                        {"v1", trajet[i]->v.getName()},
                        {"v2", trajet[i+1]->v.getName()},
                        {"poids", (int)GeoOutils::calculerDistance(trajet[i]->v, trajet[i+1]->v)},
                        {"type", CsvOutils::getTypeRoute(mapTypesRoutes, trajet[i]->v.getName(), trajet[i+1]->v.getName())},
                        {"camion", currentTruckId} // TRANSMISSION DE L'ID AU JAVA
                    });
                }
            }
        }

        string response = res.dump() + "\n";
        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
        delete strat;
    }
    return 0;
}