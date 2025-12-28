#include <iostream>
#include "GElement.h"

int main() {
    // Test avec un type int.
    GElement<int> geInt(1, 100);
    std::cout << "GElement<int>    : " << geInt << std::endl;

    // Test avec un type string.
    GElement<std::string> geStr(2, "Donnee");
    std::cout << "GElement<string> : " << geStr << std::endl;

    // Test explicite de l'opérateur de conversion.
    std::string s = (std::string)geInt;
    std::cout << "Test conversion  : " << s << std::endl;

    return 0;
}