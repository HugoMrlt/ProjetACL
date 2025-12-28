#ifndef TSPDISTANCE_H
#define TSPDISTANCE_H
#include "IStrategieTSP.h"

class TSPDistance : public IStrategieTSP {
public:
    double calculerPoids(double distanceKm, const std::string& typeRoute) override {
        return distanceKm;
    }
};

#endif // TSPDISTANCE_H
