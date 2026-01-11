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
#include "SelecteurMeilleurVoisin.h"
#include "NettoyeurRequete.h"

#include <cstring>
extern "C" {
    #include "algo_dyn.h"
    #include "instance.h"
    #include "schedule.h"
    #include "list.h"
    #include "fptas.h"
}

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
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
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

    Graphe<Route, Ville> dernierGrapheCalcule;


    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        char buffer[2048] = {0};
        read(new_socket, buffer, 2048);

        string requeteBrute(buffer);
        NettoyeurRequete nettoyer;
        string requete = requeteBrute;
        nettoyer(requete);

        std::cout << "Requete recue : " << requete << endl;

        if (requete.find("GET_DISTANCE") == 0) {
            int camionId = stoi(requete.substr(13));
            double d = dernierGrapheCalcule.getDistanceTotale(camionId);
            string resp = to_string(d) + "\n";
            send(new_socket, resp.c_str(), resp.length(), 0);
            close(new_socket);
            continue;
        }
        else if (requete.find("GET_DUREE") == 0) {
            int camionId = stoi(requete.substr(10));
            double t = dernierGrapheCalcule.getDureeTotale(camionId);
            string resp = to_string(t) + "\n";
            send(new_socket, resp.c_str(), resp.length(), 0);
            close(new_socket);
            continue;
        }
        else if (requete.find("GET_PROBLEMES") == 0) {
            int camionId = stoi(requete.substr(14));
            auto problemes = dernierGrapheCalcule.getAretesTypeInconnu(camionId);
            string resp = "";
            for(size_t i=0; i<problemes.size(); ++i) {
                resp += to_string(i) + (i == problemes.size()-1 ? "" : ",");
            }
            resp += "\n";
            send(new_socket, resp.c_str(), resp.length(), 0);
            close(new_socket);
            continue;
        }


        dernierGrapheCalcule = Graphe<Route, Ville>();

        string region, mode, villeDepart;
        int nbCamions = 1;
        stringstream ss(requete);
        getline(ss, region, ';');
        getline(ss, mode, ';');
        getline(ss, villeDepart, ';');
        string nbCamionsStr;
        getline(ss, nbCamionsStr, ';');

        try { nbCamions = stoi(nbCamionsStr); if (nbCamions < 1) nbCamions = 1; } catch (...) { nbCamions = 1; }

        string cheminVilles = trouverCheminAssets("Region/" + region + ".json");
        vector<Ville> baseVilles = JsonOutils::chargerDepuisFichier(cheminVilles);
        string cheminCSV = trouverCheminAssets("Type_de_route/" + region + ".csv");
        auto mapTypesRoutes = CsvOutils::chargerTypesRoute(cheminCSV);

        IStrategieTSP* strat = (mode == "TEMPS") ? (IStrategieTSP*)new TSPTemps() : (IStrategieTSP*)new TSPDistance();

        // Trouver le dépôt
        Ville depot("", 0, 0);
        for (const auto& v : baseVilles) { if (v.getName() == villeDepart) { depot = v; break; } }
        if (depot.getName() == "" && !baseVilles.empty()) depot = baseVilles[0];

        // --- PHASE 1 : CLUSTERING (FPTAS + Algo Dyn) ---
        list_t* instanceList = new_list();
        for (const auto& v : baseVilles) {
            if (v.getName() == depot.getName()) continue;
            double dist = GeoOutils::calculerDistance(depot, v);
            string type = CsvOutils::getTypeRoute(mapTypesRoutes, depot.getName(), v.getName());
            unsigned long p_val = (unsigned long)(strat->calculerPoids(dist, type) * 100);
            list_insert_last(instanceList, new_task(strdup(v.getName().c_str()), p_val > 0 ? p_val : 1, 0));
        }

        schedule_t* schedule = NULL;
        Instance instanceReduite = fptas_PmLmax(instanceList, nbCamions, 0.5);
        dynamic_programming_PmLmax(instanceReduite, nbCamions, 0, &schedule);

        json res;
        res["windowName"] = "Tournee - " + to_string(nbCamions) + " Camions";
        res["vertices"] = json::array();
        res["edges"] = json::array();

        for (const auto& v : baseVilles) {
            res["vertices"].push_back({{"ville", v.getName()},{"latitude", v.getLatitude()},{"longitude", v.getLongitude()}});
        }

        if (schedule) {
            for (int m = 0; m < nbCamions; ++m) {
                vector<Ville> clusterVilles;
                clusterVilles.push_back(depot);
                list_t* tasks = get_schedule_of_machine(schedule, m);
                list_node_t* node = get_list_head(tasks);
                while(node) {
                    string villeName = get_task_id(get_schedule_node_task((schedule_node_t*)get_list_node_data(node)));
                    for(const auto& v : baseVilles) { if(v.getName() == villeName) clusterVilles.push_back(v); }
                    node = get_successor(node);
                }

                if (clusterVilles.size() <= 1) continue;

                Graphe<double, Ville> subGraphe;
                vector<Sommet<Ville>*> subSommets;
                for(const auto& v : clusterVilles) subSommets.push_back(subGraphe.creeSommet(v));

                for (size_t i = 0; i < subSommets.size(); ++i) {
                    for (size_t j = i + 1; j < subSommets.size(); ++j) {
                        double d = GeoOutils::calculerDistance(subSommets[i]->v, subSommets[j]->v);
                        string t = CsvOutils::getTypeRoute(mapTypesRoutes, subSommets[i]->v.getName(), subSommets[j]->v.getName());
                        double w = strat->calculerPoids(d, t);
                        subGraphe.creeArete(w, subSommets[i], subSommets[j]);
                        subGraphe.creeArete(w, subSommets[j], subSommets[i]);
                    }
                }

                SelecteurMeilleurVoisin<double, Ville> selecteur;
                auto parcours = TSP<double, Ville>::plusProcheVoisin(subGraphe, subSommets[0], selecteur);

                // --- REMPLISSAGE DU GRAPHE METIER (IMPORTANT) ---
                for (size_t i = 0; i < parcours.size(); ++i) {
                    Sommet<Ville>* s1 = parcours[i];
                    Sommet<Ville>* s2 = parcours[(i + 1) % parcours.size()];

                    int distKm = (int)GeoOutils::calculerDistance(s1->v, s2->v);
                    string typeR = CsvOutils::getTypeRoute(mapTypesRoutes, s1->v.getName(), s2->v.getName());

                    // On ajoute l'arête dans notre objet de calcul métier
                    Route routeData(distKm, typeR, m + 1); // m+1 est le camionId
                    auto sommetA = dernierGrapheCalcule.creeSommet(s1->v);
                    auto sommetB = dernierGrapheCalcule.creeSommet(s2->v);
                    dernierGrapheCalcule.creeArete(routeData, sommetA, sommetB);


                    res["edges"].push_back({
                        {"v1", s1->v.getName()}, {"v2", s2->v.getName()},
                        {"poids", distKm}, {"type", typeR}, {"camion", m + 1}
                    });
                }
            }
            delete_schedule(schedule);
        }

        delete_list(instanceList, &delete_task);
        delete_list(instanceReduite, &delete_task);

        res["distance_totale"] = dernierGrapheCalcule.getDistanceTotale(-1);
        res["duree_totale"] = dernierGrapheCalcule.getDureeTotale(-1);

        string response = res.dump() + "\n";
        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
        delete strat;
    }
    return 0;
}