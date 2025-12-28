#ifndef FPTAS_H
#define FPTAS_H

#include "instance.h"

/**
 * @brief Implémente le schéma FPTAS par modification des données.
 *
 * @param I Instance originale.
 * @param m Nombre de machines.
 * @param epsilon Erreur epsilon > 0.
 * @return Instance L'instance modifiée.
 */
Instance fptas_PmLmax(const Instance I, int m, double epsilon);

#endif // FPTAS_H
