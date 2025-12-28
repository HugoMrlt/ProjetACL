#ifndef ISTRATEGIETSP_H
#define ISTRATEGIETSP_H
#include <string>

class IStrategieTSP {
public:
    virtual ~IStrategieTSP() = default;
    virtual double calculerPoids(double distanceKm, const std::string& typeRoute) = 0;
};

#endif // ISTRATEGIETSP_H

