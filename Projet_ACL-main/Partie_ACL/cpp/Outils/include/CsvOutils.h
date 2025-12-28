#ifndef CSVOUTILS_H
#define CSVOUTILS_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;


class CsvOutils {
public:
    static string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    static map<string, map<string, string>> chargerTypesRoute(const string& cheminFichier) {
        map<string, map<string, string>> types;
        ifstream fichier(cheminFichier);
        if (!fichier.is_open()) throw runtime_error("Impossible d'ouvrir le fichier CSV");
        string ligne;
        vector<string> villes;

        if (getline(fichier, ligne)) {
            stringstream ss(ligne);
            string cell;
            getline(ss, cell, ';');
            while (getline(ss, cell, ';')) villes.push_back(trim(cell));
        }

        while (getline(fichier, ligne)) {
            stringstream ss(ligne);
            string villeA;
            getline(ss, villeA, ';');
            villeA = trim(villeA);
            for (size_t i = 0; i < villes.size(); ++i) {
                string type;
                getline(ss, type, ';');
                type = trim(type);
                if (!type.empty()) types[villeA][villes[i]] = type;
            }
        }
        return types;
    }

    static string getTypeRoute(const map<string, map<string, string>>& types, const string& villeA, const string& villeB) {
        string vA = trim(villeA);
        string vB = trim(villeB);
        auto itA = types.find(vA);
        if (itA != types.end()) {
            auto itB = itA->second.find(vB);
            if (itB != itA->second.end()) return itB->second;
        }

        auto itB = types.find(vB);
        if (itB != types.end()) {
            auto itA2 = itB->second.find(vA);
            if (itA2 != itB->second.end()) return itA2->second;
        }
        return "Aucun Type";
    }
};

#endif // CSVOUTILS_H
