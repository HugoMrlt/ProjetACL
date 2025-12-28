#include "schedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"
#include "list.h"
#include "instance.h"

/********************************************************************
 * schedule_node_t
 ********************************************************************/

/**
 * @brief Construire et initialiser un nouveau nœud d'un ordonnancement.
 *
 * @param[in] task Tâche associée au nœud.
 * @param[in] bt Temps de début.
 * @param[in] et Temps de fin.
 * @return schedule_node_t* Pointeur vers le nouveau nœud.
 */
static schedule_node_t *new_schedule_node(task_t *task, unsigned long bt, unsigned long et) {
    assert(bt <= et);
    assert(task);
    schedule_node_t *snode = (schedule_node_t *) malloc(sizeof(schedule_node_t));

    snode->task = task;
    snode->begin_time = bt;
    snode->end_time = et;

    return snode;
}

task_t *get_schedule_node_task(const schedule_node_t *snode) {
    assert(snode);
    return snode->task;
}

unsigned long get_schedule_node_begin_time(const schedule_node_t *snode) {
    assert(snode);
    return snode->begin_time;
}

unsigned long get_schedule_node_end_time(const schedule_node_t *snode) {
    assert(snode);
    return snode->end_time;
}

void set_schedule_node_begin_time(schedule_node_t *snode, unsigned long new_bt) {
    assert(snode);
    if (new_bt > snode->end_time) {
        ShowMessage("set_schedule_node_begin_time : new_bt ne peut être supérieur à end_time", 1);
    }
    snode->begin_time = new_bt;
}

void set_schedule_node_end_time(schedule_node_t *snode, unsigned long new_et) {
    assert(snode);
    if (new_et < snode->begin_time) {
        ShowMessage("set_schedule_node_end_time : new_et ne peut être inférieur à begin_time", 1);
    }
    snode->end_time = new_et;
}

/**
 * @brief Affiche les informations d'un nœud d'ordonnancement.
 *
 * @param[in] snode Nœud à afficher.
 */
void view_schedule_node(const void *snode) {
    assert(snode);
    const schedule_node_t *node = (const schedule_node_t *) snode;
    printf("begin time: %lu ; end time: %lu; task: ", node->begin_time, node->end_time);
    view_task(node->task);
}

/**
 * @brief Libère la mémoire associée à un nœud d'ordonnancement.
 *
 * @param[in] snode Nœud à supprimer.
 */
void delete_schedule_node(void *snode) {
    assert(snode);
    free(snode);
}

schedule_t *new_schedule(int num_m) {
    assert(num_m >= 1);
    schedule_t *S = (schedule_t *) malloc(sizeof(schedule_t));

    S->schedule = (list_t **) malloc(num_m * sizeof(list_t *));
    for (int m = 0; m < num_m; m++) {
        S->schedule[m] = new_list();
    }
    S->num_machines = num_m;

    return S;
}

list_t *get_schedule_of_machine(const schedule_t *S, const int machine) {
    assert(S && machine < S->num_machines);
    return S->schedule[machine];
}

int get_num_machines(const schedule_t *S) {
    assert(S);
    return S->num_machines;
}

void view_schedule(const void *S) {
    assert(S);
    const schedule_t *nS = S;
    printf("Nombre de machines : %d\n\n", get_num_machines(nS));
    for (int m = 0; m < get_num_machines(nS); m++) {
        printf("Machine %d :\n", m);
        view_list(get_schedule_of_machine(nS, m), &view_schedule_node);
        printf("\n");
    }
}

void delete_schedule(void *S) {
    assert(S);
    schedule_t *nS = S;
    for (int m = 0; m < nS->num_machines; m++) {
        delete_list(nS->schedule[m], &delete_schedule_node);
    }
    free(nS->schedule);
    free(S);
}

// Sauvegarde un ordonnancement dans un fichier
void save_schedule(schedule_t *S, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        ShowMessage("save_schedule : échec d'ouverture du fichier", 1);
        return;
    }

    fprintf(file, "%d\n", get_num_machines(S));
    for (int m = 0; m < get_num_machines(S); m++) {
        fprintf(file, "M%d\n", m);
        list_node_t *iterator = get_list_head(get_schedule_of_machine(S, m));
        while (iterator) {
            schedule_node_t *node = get_list_node_data(iterator);
            fprintf(file, "%lu %lu %s\n", node->begin_time, node->end_time, get_task_id(node->task));
            iterator = get_successor(iterator);
        }
    }

    fclose(file);
}

// Trouver une machine vide ou prête à exécuter une tâche à un moment donné
int find_empty_machine(schedule_t *S, unsigned long time) {
    assert(S);
    for (int m = 0; m < get_num_machines(S); m++) {
        list_t *machine_schedule = get_schedule_of_machine(S, m);
        if (list_is_empty(machine_schedule) || time > get_schedule_node_end_time(
                get_list_node_data(get_list_tail(machine_schedule)))) {
            return m;
        }
    }
    return -1;
}

// Trouver une machine sur laquelle une tâche peut être interrompue.
int find_machine_to_interrupt(const schedule_t *S, const unsigned long time, const unsigned long processing_time) {
    assert(S);
    for (int m = 0; m < get_num_machines(S); m++) {
        const list_t *machine_schedule = get_schedule_of_machine(S, m);
        if (processing_time < get_schedule_node_end_time(get_list_node_data(get_list_tail(machine_schedule))) - time) {
            return m;
        }
    }
    return -1;
}

void add_task_to_schedule(const schedule_t *S, task_t *task, const int machine, const unsigned long bt, const unsigned long et) {
    assert(S && task);
    list_t *machine_schedule = get_schedule_of_machine(S, machine);

    if (list_is_empty(machine_schedule) || get_schedule_node_end_time(
            get_list_node_data(get_list_tail(machine_schedule))) <= bt) {
        list_insert_last(machine_schedule, new_schedule_node(task, bt, et));
    }
}

// Préempter une tâche sur une machine
unsigned long preempt_task(schedule_t *S, int machine, unsigned long new_et) {
    assert(S);
    list_t *machine_schedule = get_schedule_of_machine(S, machine);
    schedule_node_t *last_node = get_list_node_data(get_list_tail(machine_schedule));
    unsigned long remaining_time = last_node->end_time - new_et;
    set_schedule_node_end_time(last_node, new_et);
    return remaining_time;
}

// Récupérer le temps de complétion d'une machine
unsigned long get_completion_time(schedule_t *S, const int m) {
    list_t *machine_schedule = get_schedule_of_machine(S, m);
    if (list_is_empty(machine_schedule)) return 0;
    return get_schedule_node_end_time(get_list_node_data(get_list_tail(machine_schedule)));
}

// Récupérer le temps de complétion maximum (makespan)
unsigned long get_makespan(schedule_t *S) {
    assert(S);
    unsigned long makespan = 0;
    for (int m = 0; m < get_num_machines(S); m++) {
        unsigned long machine_completion = get_completion_time(S, m);
        if (machine_completion > makespan) {
            makespan = machine_completion;
        }
    }
    return makespan;
}

unsigned long get_lmax_for_machine(const schedule_t* S, const int machine) {
    const list_t * machine_schedule = get_schedule_of_machine(S, machine);

    if (list_is_empty(machine_schedule)) return 0;

    unsigned long lmax = 0;
    const list_node_t *iterator = get_list_head(machine_schedule);

    while (iterator) {
        const schedule_node_t * node = get_list_node_data(iterator);
        const task_t * task = node->task;
        assert(task);

        const unsigned long Ck = get_schedule_node_end_time(node);
        const unsigned long qk = get_task_latency(task);

        const unsigned long lj = Ck + qk;

        if (lj > lmax) lmax = lj;
        iterator = get_successor(iterator);
    }

    return lmax;
}

unsigned long get_lmax(const schedule_t * S) {
    assert(S);
    unsigned long lmax = 0;

    for (int m = 0; m < get_num_machines(S); m++) {
        const unsigned long machine_lj = get_lmax_for_machine(S, m);

        if (machine_lj > lmax)
            lmax = machine_lj;
    }

    return lmax;
}

schedule_t *copy_schedule(const schedule_t *S) {
    assert(S);

    schedule_t *newS = (schedule_t *) malloc(sizeof(schedule_t));
    int num_m = get_num_machines(S);
    newS->schedule = (list_t **) malloc(num_m * sizeof(list_t *));
    newS->num_machines = num_m;

    for (int m = 0; m < num_m; m++) {
        newS->schedule[m] = new_list();
        list_t *src_list = get_schedule_of_machine(S, m);
        
        // Copie profonde des noeuds
        for (list_node_t *node = get_list_head(src_list); node; node = get_successor(node)) {
            schedule_node_t *src_snode = (schedule_node_t *)get_list_node_data(node);
            // On crée un nouveau noeud avec les mêmes données
            schedule_node_t *new_snode = new_schedule_node(src_snode->task, src_snode->begin_time, src_snode->end_time);
            list_insert_last(newS->schedule[m], new_snode);
        }
    }

    return newS;
}

