#ifndef CODE_ALGO_DYN_H
#define CODE_ALGO_DYN_H

#include "instance.h"
#include "schedule.h"

/**
 * @brief Algorithme de Programmation Dynamique pour P,m||Lmax.
 *
 * Implémente la résolution de la Question 2 en utilisant la méthode de l'espace d'états et la réduction d'état.
 *
 * @param tasks_instance La liste des tâches (Instance) à ordonnancer.
 * @param num_machines Le nombre de machines (m).
 * @param delta Le facteur delta pour la réduction d'état (0 pour l'optimum exact, > 0 pour FPTAS).
 * @param out_best_schedule Pointeur vers un schedule_t* pour récupérer le meilleur ordonnancement (peut être NULL).
 * @return unsigned long Le L_max optimal (f minimal) trouvé.
 */
unsigned long dynamic_programming_PmLmax(const Instance tasks_instance, int num_machines, double delta, schedule_t **out_best_schedule);

#endif //CODE_ALGO_DYN_H