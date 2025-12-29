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

    // Variables pour la config reçue
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

        // Parsing de la commande : region;mode;villeDepart;nbCamions
        stringstream ss(requete);
        getline(ss, region, ';');
        getline(ss, mode, ';');
        getline(ss, villeDepart, ';');
        string nbCamionsStr;
        getline(ss, nbCamionsStr, ';');
        try { nbCamions = stoi(nbCamionsStr); } catch (...) { nbCamions = 1; }

        // Nettoyage
        region.erase(0, region.find_first_not_of(" \t\r\n"));
        region.erase(region.find_last_not_of(" \t\r\n") + 1);
        mode.erase(0, mode.find_first_not_of(" \t\r\n"));
        mode.erase(mode.find_last_not_of(" \t\r\n") + 1);
        villeDepart.erase(0, villeDepart.find_first_not_of(" \t\r\n"));
        villeDepart.erase(villeDepart.find_last_not_of(" \t\r\n") + 1);

        // Chargement dynamique des villes et types de routes
        string cheminVilles = trouverCheminAssets("Region/" + region + ".json");
        baseVilles = JsonOutils::chargerDepuisFichier(cheminVilles);
        if (baseVilles.empty()) {
            cerr << "ERREUR : Aucune ville chargée. Vérifiez le chemin : " << cheminVilles << endl;
        } else {
            cout << baseVilles.size() << " villes chargées dynamiquement pour la région " << region << "." << endl;
        }
        string cheminCSV = trouverCheminAssets("Type_de_route/" + region + ".csv");
        mapTypesRoutes = CsvOutils::chargerTypesRoute(cheminCSV);

        // --- 1. FILTRAGE ET INITIALISATION DU GRAPHE ---
        Graphe<double, Ville> grapheDistance;
        vector<Sommet<Ville>*> sommetsSelectionnes;
        IStrategieTSP* strat = nullptr;
        if (mode == "TEMPS") {
            strat = new TSPTemps();
        } else {
            strat = new TSPDistance();
        }

        // On commence par la ville de départ, puis toutes les autres
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

        // --- 2. CREATION DES ARETES (CRUCIAL POUR LE TSP) ---
        // On crée des arêtes entre toutes les villes sélectionnées
        for (size_t i = 0; i < sommetsSelectionnes.size(); ++i) {
            for (size_t j = i + 1; j < sommetsSelectionnes.size(); ++j) {
                double distKm = GeoOutils::calculerDistance(sommetsSelectionnes[i]->v, sommetsSelectionnes[j]->v);
                // RECHERCHE DU TYPE DANS LE CSV
                string nomA = sommetsSelectionnes[i]->v.getName();
                string nomB = sommetsSelectionnes[j]->v.getName();
                string typeRouteReel = CsvOutils::getTypeRoute(mapTypesRoutes, nomA, nomB);
                double poids = strat->calculerPoids(distKm, typeRouteReel);

                grapheDistance.creeArete(poids, sommetsSelectionnes[i], sommetsSelectionnes[j]);
                grapheDistance.creeArete(poids, sommetsSelectionnes[j], sommetsSelectionnes[i]);
            }
        }

        // --- 3. CALCUL DU TSP ---
        vector<Sommet<Ville>*> solution;
        if (!sommetsSelectionnes.empty()) {
            solution = TSP<double, Ville>::plusProcheVoisin(grapheDistance, sommetsSelectionnes[0]);

            std::cout << "Solution trouvee (" << solution.size() << " villes)" << endl;
        }

        // --- 4. CONSTRUCTION DU JSON ---
        json res;
        res["windowName"] = "Tournee TSP - Resultat C++";
        res["vertices"] = json::array();
        res["edges"] = json::array();

        for (auto* s : solution) {
            res["vertices"].push_back({
                {"ville", s->v.getName()}, // Attention : "name" ou "ville" selon Utils.java
                {"latitude", (double)s->v.getLatitude()},  
                {"longitude", (double)s->v.getLongitude()} 
            });
        }

        for (size_t i = 0; i < solution.size(); ++i) {
            size_t prochain = (i + 1) % solution.size();

            // On récupère à nouveau le type pour le JSON
            string typePourJava = CsvOutils::getTypeRoute(mapTypesRoutes, 
                                                         solution[i]->v.getName(), 
                                                         solution[prochain]->v.getName());
                                                         
            res["edges"].push_back({
                {"v1", solution[i]->v.getName()},
                {"v2", solution[prochain]->v.getName()},
                {"poids", (int)GeoOutils::calculerDistance(solution[i]->v, solution[prochain]->v)},
                {"type", typePourJava} // Rouge en AWT
            });
        }

        string response = res.dump() + "\n"; 
        send(new_socket, response.c_str(), response.length(), 0);

        close(new_socket);
        delete strat;
        std::cout << "Solution envoyee au client Java.\n" << endl;
    }

    return 0;
}