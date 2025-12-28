#ifndef GESTION_ROBOT_H
#define GESTION_ROBOT_H

#include "instance.h"

/**
 * @brief Calcule le temps total (Cmax) pour le transport avec 2 robots (Heuristique LPT).
 *
 * @param I Instance contenant les tâches de transport.
 * @return unsigned long Le Cmax (date de fin du dernier transport).
 */
unsigned long calculer_temps_transport(Instance I);

#endif // GESTION_ROBOT_H