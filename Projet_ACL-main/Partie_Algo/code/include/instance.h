#ifndef _INSTANCE_H_
#define _INSTANCE_H_
#include "list.h"

/**
 * @brief
 * Une tâche est caractérisée par les attributs suivants :
 * (+) un identifiant (id),
 * (+) sa durée opératoire (processing_time, p_i), et
 * (+) sa durée de latence (latency, q_i).
 */
typedef struct task {
    char *id;
    unsigned long processing_time; // p_i
    unsigned long latency;         // q_i

    unsigned long completion_time; // C_i (calculé par l'ordonnanceur)

} task_t;

/**
 * @brief
 * Construire et initialiser une nouvelle tâche.
 *
 * @param[in] id L'identifiant de la nouvelle tâche.
 * @param[in] proctime Le temps d’exécution (p_i) de la nouvelle tâche.
 * @param[in] latency La latence (q_i) de la nouvelle tâche.
 * @return task_t*
 */
task_t *new_task(char *id, unsigned long proctime, unsigned long latency);

/**
 * @brief Restitue l'identifiant de la tâche \p task.
 */
char *get_task_id(const task_t *task);

/**
 * @brief Restitue le temps d’exécution (p_i) de la tâche \p task.
 */
unsigned long get_task_processing_time(const task_t *task);

/**
 * @brief Restitue la latence (q_i) de la tâche \p task.
 */
unsigned long get_task_latency(const task_t *task);

/**
 * @brief Afficher les caractéristiques de la tâche \p task.
 */
void view_task(const void *task);

/**
 * @brief Supprimer et libérer la mémoire de la tâche \p task.
 */
void delete_task(void *task);

/**
 * @brief
 * Compare 2 tâches pour la règle de Jackson (tri par q_i décroissant).
 * Compatible avec merge_sort_list et quicksort.
 *
 * @param a Pointeur vers un (task_t *)
 * @param b Pointeur vers un (task_t *)
 * @return int > 0 si q_a < q_b, < 0 si q_a > q_b, 0 si égaux
 */
int compare_task_jackson_desc(const void *a, const void *b);

/**
 * @brief
 * Une instance est une liste doublement chaînée des tâches.
 */
typedef list_t * Instance;

/**
 * @brief
 * Construire une nouvelle instance avec le contenu du fichier \p filename.
 * (Le format du fichier est supposé être : id p q)
 *
 * @param[in] filename
 * @return Instance
 */
Instance read_instance(const char *filename);

/**
 * @brief
 * Afficher les tâches de l'instance \p I.
 */
void view_instance(Instance I);

/**
 * @brief
 * Liberer la mémoire d'une instance \p I.
 */
void delete_instance(Instance I);

/**
 * @brief Génère un tableau de tâches aléatoires.
 *
 * @param num_tasks Le nombre de tâches à générer.
 * @param max_p La valeur maximale pour le temps de traitement.
 * @param max_q La valeur maximale pour la latence.
 * @return task_t** Un tableau de pointeurs vers les tâches générées.
 */
task_t **generate_random_tasks(int num_tasks, int max_p, int max_q);

/**
 * @brief Crée une instance à partir d'un tableau de tâches.
 *
 * @param tasks Le tableau de pointeurs de tâches.
 * @param num_tasks Le nombre de tâches du tableau à inclure dans l'instance.
 * @return Instance Une nouvelle instance contenant les tâches spécifiées.
 */
Instance create_instance_from_tasks(task_t **tasks, int num_tasks);

#endif // _INSTANCE_H_
