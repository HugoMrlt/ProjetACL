#include "jackson.h"
#include "instance.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Comparateur pour l'ordre de Jackson.
 * Règle : Trier les tâches par ordre décroissant de latence (q_i).
 * Si q_i est grand, la tâche est prioritaire.
 */
int compare_tasks_jackson(const void *a, const void *b) {
    task_t *task_a = *(task_t **)a;
    task_t *task_b = *(task_t **)b;
    
    // Critère principal : Latence (q) décroissante
    if (task_b->latency > task_a->latency) return 1;  // b avant a
    if (task_b->latency < task_a->latency) return -1; // a avant b
    
    // Critère secondaire : Durée (p) décroissante en cas d'égalité
    if (task_b->processing_time > task_a->processing_time) return 1;
    if (task_b->processing_time < task_a->processing_time) return -1;

    return 0;
}

/**
 * Heuristique de Jackson pour P,m || Lmax.
 * Complexité : O(N log N) dominée par le tri.
 * Ratio de performance théorique : (2 - 1/m).
 */
unsigned long jackson_heuristic(Instance I, int m) {
    // Vérification des entrées
    if (!I || list_is_empty(I) || m <= 0) {
        return 0;
    }

    unsigned int num_tasks = I->size;

    // 1. Création d'une copie locale des pointeurs de tâches
    task_t **tasks_copy = malloc(num_tasks * sizeof(task_t *));
    if (!tasks_copy) {
        perror("Erreur allocation mémoire (Jackson tasks_copy)");
        return 0;
    }

    // Remplissage du tableau à partir de la liste
    list_node_t *current_node = I->head;
    for (unsigned int i = 0; i < num_tasks; ++i) {
        if (current_node == NULL) break;
        tasks_copy[i] = (task_t *)get_list_node_data(current_node);
        current_node = get_successor(current_node);
    }

    // 2. Tri des tâches selon l'ordre de Jackson (q_i décroissant)
    qsort(tasks_copy, num_tasks, sizeof(task_t *), compare_tasks_jackson);

    unsigned long *machine_finish_times = calloc(m, sizeof(unsigned long));
    if (!machine_finish_times) {
        free(tasks_copy);
        perror("Erreur allocation mémoire (machine_finish_times)");
        return 0;
    }

    unsigned long max_objective = 0; // Stockera le Lmax global

    // 3. Ordonnancement de liste (List Scheduling)
    for (unsigned int i = 0; i < num_tasks; ++i) {
        
        // A. Règle Gloutonne : Trouver la machine disponible au plus tôt
        int earliest_machine_idx = 0;
        for (int j = 1; j < m; ++j) {
            if (machine_finish_times[j] < machine_finish_times[earliest_machine_idx]) {
                earliest_machine_idx = j;
            }
        }

        task_t *current_task = tasks_copy[i];
        
        // B. Affectation de la tâche
        unsigned long completion_time = machine_finish_times[earliest_machine_idx] + current_task->processing_time;
        
        machine_finish_times[earliest_machine_idx] = completion_time;

        // C. Calcul du coût pour cette tâche
        unsigned long current_val = completion_time + current_task->latency;

        if (current_val > max_objective) {
            max_objective = current_val;
        }
    }

    // Libération de la mémoire
    free(tasks_copy);
    free(machine_finish_times);

    return max_objective;
}