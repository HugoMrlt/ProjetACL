#include <iostream>
#include "Sommet.h"

int main() {
    // Test Sommet<double>.
    Sommet<double> s1(1, 3.14);
    std::cout << "Sommet<double> : " << s1 << std::endl;

    // Test Sommet<string>.
    Sommet<std::string> s2(2, "Station A");
    s2.degre = 3; // On simule des connexions.
    std::cout << "Sommet<string> : " << s2 << std::endl;

    // Test du constructeur avec tous les paramètres.
    Sommet<int> s3(3, 100, 5);
    std::cout << "Sommet complet : " << s3 << std::endl;

    return 0;
}