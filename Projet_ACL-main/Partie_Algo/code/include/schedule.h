#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include "instance.h"

/**
 * @brief
 * Un nœud d'un ordonnancement correspond à l’exécution (partielle ou pas) d'une tâche et contient
 * (+) la tâche à exécuter (task),
 * (+) le début de l’exécution (begin_time), et
 * (+) la fin de l’exécution (end_time).
 * Dans le cas d'un ordonnancement préemptif, une tâche peut se retrouver dans plusieurs nœuds.
 * La somme de (end_time - begin_time) de ces nœuds doit être égal à la durée de cette tâche.
 */
typedef struct schedule_node {
    task_t *task;
    unsigned long begin_time;
    unsigned long end_time;
} schedule_node_t;

/**
 * @brief Restitue la tâche du nœud \p snode.
 *
 * @param[in] snode
 * @return task_t*
 */
task_t *get_schedule_node_task(const schedule_node_t *snode);

/**
 * @brief Restitue la date de début du nœud \p snode.
 *
 * @param[in] snode
 * @return unsigned long
 */
unsigned long get_schedule_node_begin_time(const schedule_node_t *snode);


/**
 * @brief Restitue la date de fin du nœud \p snode.
 *
 * @param[in] snode
 * @return unsigned long
 */
unsigned long get_schedule_node_end_time(const schedule_node_t *snode);

/**
 * @brief Remplace la date de début du nœud \p snode par \p new_bt.
 *
 * @param[in] snode
 * @param new_bt
 */
void set_schedule_node_begin_time(schedule_node_t *snode, unsigned long new_bt);

/**
 * @brief Remplace la date de fin du nœud \p snode par \p new_bt.
 *
 * @param[in] snode
 * @param new_et
 */
void set_schedule_node_end_time(schedule_node_t *snode, unsigned long new_et);

/**
 * @brief Afficher les caractéristiques du nœud d'ordonnancement \p snode.
 *
 * @param[in] snode
 */
void view_schedule_node(const void *snode);

/**
 * @brief Supprimer et libérer la mémoire du nœud d'ordonnancement \p snode.
 * Attention ! la mémoire de l'attribut task du nœud \p snode ne doit pas être libérée.
 *
 * @param[in] snode
 */
void delete_schedule_node(void *snode);

/**
 * @brief
 * Un ordonnancement est
 * (+) un tableau de listes (schedule) chacune correspondant à ordonnancement d'une machine, et
 * (+) le nombre de machines de l'ordonnancement (num_machines).
 *
 * NB : l'ordonnancement de chaque machine est une liste qui contient l'enchaînement d’exécution
 * de tâches dans l'ordre croissant par rapport à leur date de début (begin_time dans schedule_node_t).
 */
typedef struct schedule {
    list_t **schedule;
    int num_machines;
} schedule_t;

/**
 * @brief Construire et initialiser un nouveau ordonnancement vide avec \p num_m machines.
 *
 * @param[in] num_m
 * @return schedule_t*
 */
schedule_t *new_schedule(int num_m);

/**
 * @brief Restitue le sous-ordonnancement de la machine \p machine de l'ordonnancement \p S.
 *
 * @param[in] S
 * @param[in] machine
 * @return list_t*
 */
list_t *get_schedule_of_machine(const schedule_t *S, const int machine);

/**
 * @brief Restitue le nombre de machines de l'ordonnancement \p S.
 *
 * @param[in] S
 * @return int
 */
int get_num_machines(const schedule_t *S);

/**
 * @brief Afficher l'ordonnancement \p S.
 *
 * @param[in] S
 */
void view_schedule(const void *S);

/**
 * @brief Supprimer et libérer la mémoire de l'ordonnancement \p S.
 *
 * @param[in] S
 */
void delete_schedule(void *S);

/**
 * @brief Enregistre l'ordonnancement \p S au fichier \p filename.
 *
 * @param[in] S
 * @param[in] filename
 */
void save_schedule(schedule_t *S, const char *filename);

/**
 * @brief Restitue la première machine de l'ordonnancement \p S qui est disponible au moment \p time.
 * Si aucune machine n'est disponible en \p time, la valeur retournée est -1.
 *
 * NB1 : Il faut chercher les machines de façon itérative en commençant par la première.
 * La machine disponible avec le plus petit indice doit être restituée.
 *
 * NB2 : Afin de décider si une machine est disponible au moment \p time, il suffit de comparer \p time
 * avec le end_time du dernier nœud de cette machine (il n'y a pas besoin de parcourir tous les nœuds).
 *
 * NB3 : Une machine n'est pas considérée comme disponible au moment \p time si il y a une tâche qui
 * termine son exécution en ce moment (c'est-à-dire, si end_time=time).
 *
 * @param[in] S
 * @param[in] time
 * @return int
 */
int find_empty_machine(schedule_t *S, unsigned long time);

/**
 * @brief Restitue la première machine de l'ordonnancement \p S qui peut être interrompue au moment \p time.
 * Si aucune machine ne peut pas être interrompue en \p time, la valeur retournée est -1.
 *
 * NB1 : Il faut chercher les machines de façon itérative en commençant par la première.
 * La machine à interrompre avec le plus petit indice doit être restituée.
 *
 * NB2 : Afin de décider si une machine est susceptible à être interrompue au moment \p time,
 * il suffit de comparer le temps d’exécution \p processing_time de la tâche qu'on veut commencer
 * avec le temps restant ( \p end_time - time ) de la tâche qui est en train d’exécuter
 * (dernier nœud de cette machine, il n'y a pas besoin de parcourir tous les nœuds).
 *
 * @param[in] S
 * @param[in] time
 * @param[in] processing_time
 * @return int
 */
int find_machine_to_interrupt(const schedule_t *S, unsigned long time, unsigned long processing_time);

/**
 * @brief Ajouter dans l'ordonnancement \p S la tâche \p task dans la machine \p machine.
 * L’exécution commence au moment \p bt et finit au moment \p et.
 *
 * NB1 : il faut s'assurer avant l'appel de cette procédure que la machine \p machine est libre au moment \p bt,
 * et qu'il n'y a pas d'autre tâche ordonnancée après cette date.
 *
 * @param[in] S
 * @param[in] task
 * @param[in] machine
 * @param[in] bt
 * @param[in] et
 */
void add_task_to_schedule(const schedule_t *S, task_t *task,
                          int machine, unsigned long bt, unsigned long et);

/**
 * @brief Mettre à jour la date de fin (end_time) de la dernière tâche de la machine \p machine
 * de l'ordonnancement \p S avec la valeur \p new_et.
 *
 * @param[in] S
 * @param[in] machine
 * @param[in] new_et
 * @return unsigned long
 */
unsigned long preempt_task(schedule_t *S, int machine, unsigned long new_et);

/**
* @brief Calculer et renvoyer la valeur de l'objectif "makespan" de l'ordonnancement \p S.
 * Le "makespan" est la date de fin de la dernière tâche de l'ordonnancement.
 *
 * @param[in] S
 * @param[in] m
 * @return unsigned long
 */
unsigned long get_completion_time(schedule_t *S, const int m);

/**
 * @brief Calculer et renvoyer la valeur de l'objectif "makespan" de l'ordonnancement \p S.
 * Le "makespan" est la date de fin de la dernière tâche de l'ordonnancement.
 *
 * @param[in] S
 * @return unsigned long
 */
unsigned long get_makespan(schedule_t *S);

/**
 * Calcule le retard maximal (Lmax) pour la machine passée en paramètre.
 *
 * @param S L'ordonnancement de toutes les tâches.
 * @param machine La machine pour laquelle on calcule Lmax.
 * @return unsigned long Le retard maximal (Lmax = max(Cj + qj)).
 */
unsigned long get_lmax_for_machine(const schedule_t *S, int machine);

/**
 * @brief Calcule le retard maximal (Lmax) de l'ordonnancement.
 *
 * Cette fonction suppose que le champ 'completion_time' (Cj) de CHAQUE task_t* * a été correctement mis à jour par la fonction d'ordonnancement,
 * et que le champ 'latency' (qi) est utilisé comme date d'échéance (dj).
 *
 * @param[in] S L'ordonnancement de toutes les tâches.
 * @return unsigned long Le retard maximal (Lmax = max(Cj + qj)) sur toutes les machines.
 */
unsigned long get_lmax(const schedule_t * S);

/**
 * @brief Copier le schedule donné et retourne une copie.
 *
 * @param[in] S
 * @return schedule_t
 *
*/
schedule_t *copy_schedule(const schedule_t *S);

#endif // _SCHEDULE_H_
