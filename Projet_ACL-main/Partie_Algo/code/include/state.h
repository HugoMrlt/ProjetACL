#ifndef STATE_H
#define STATE_H
#include <schedule.h>

/**
 * @brief un état est un ordonnancement contenant des Liste des indices des tâches pour chaque machine
 */
typedef schedule_t *state_t;

/**
 * @brief Compare le makespan et retourne une valeur précisent le cas
 * @param a
 * @param b
 * @return
 */
int compare_state(const void *a, const void *b);

/**
 * @brief Compare les deux zones de deux state
 * @param a
 * @param b
 * @param d l'espacement des zones
 * @return 1 si les deux états appartiennent à la même zone, sinon 0
 */
int compare_zone(const void *a, const void *b, double d);

/**
 * @brief Garde seulement un état dans chaque zone avec le completeTime de la machine 1 le plus faible
 * @param list
 * @param delta
 */
void reduce_states(list_t *list, double delta);


#endif //STATE_H
