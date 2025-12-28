#ifndef PROJET_ACL_JACKSON_H
#define PROJET_ACL_JACKSON_H

#include "instance.h"

/**
 * @brief Implémente l'heuristique de Jackson (règle LPT) pour le problème P||Lmax.
 *
 * @param I L'instance contenant les tâches.
 * @param m Le nombre de machines.
 * @return Le retard maximum (Lmax) trouvé par l'heuristique.
 */
unsigned long jackson_heuristic(Instance I, int m);

#endif //PROJET_ACL_JACKSON_H

