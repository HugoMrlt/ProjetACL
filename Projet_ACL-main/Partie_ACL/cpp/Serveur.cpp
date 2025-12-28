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

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }
    listen(server_fd, 3);

    std::cout << "Serveur TSP C++ pret sur le port 8080..." << endl;

    // Charger la base de données complète une seule fois au démarrage
    vector<Ville> baseVilles = {
        Ville("Strasbourg", 48.58, 7.75), Ville("Colmar", 49.26, 4.03),
        Ville("Forbach", 49.19, 6.90), Ville("Haguenau", 48.82, 7.78),
        Ville("Thionville", 49.37, 6.15), Ville("Sarreguemines", 49.10, 6.18),
        Ville("Metz", 49.11, 6.17), Ville("Nancy", 48.69, 6.18)
    };

    auto mapTypesRoutes = CsvOutils::chargerTypesRoute(trouverCheminAssets("Type_de_route/GrandEst.csv"));

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        char buffer[2048] = {0}; // Buffer plus grand pour les listes de villes
        read(new_socket, buffer, 2048);
        string requete(buffer);
        std::cout << "Requete Java recue : " << requete << endl;

        // --- 1. FILTRAGE ET INITIALISATION DU GRAPHE ---
        Graphe<double, Ville> grapheDistance;
        vector<Sommet<Ville>*> sommetsSelectionnes;
        IStrategieTSP* stratDist = new TSPDistance();

        stringstream ss(requete);
        string nomVille;
        while (getline(ss, nomVille, ',')) {
            // Nettoyage des espaces et retours à la ligne
            nomVille.erase(0, nomVille.find_first_not_of(" \t\r\n"));
            nomVille.erase(nomVille.find_last_not_of(" \t\r\n") + 1);

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
                double poids = stratDist->calculerPoids(distKm, typeRouteReel);

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
        delete stratDist;
        std::cout << "Solution envoyee au client Java.\n" << endl;
    }

    return 0;
}