#include "instance.h"
#include "utils.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/********************************************************************
 * task_t
 ********************************************************************/

task_t *new_task(char *id, unsigned long proctime, unsigned long latency) {
    assert(id);
    if (strcmp(id, "") == 0) {
        ShowMessage("new_task : id ne peut pas être vide", 1);
    }

    task_t *T = (task_t *) calloc(1, sizeof(task_t));
    assert(T);

    T->id = id;
    T->processing_time = proctime;
    T->latency = latency;
    T->completion_time = 0;

    return T;
}

char *get_task_id(const task_t *task) {
    assert(task);
    return task->id;
}

unsigned long get_task_processing_time(const task_t *task) {
    assert(task);
    return task->processing_time;
}

/**
 * @brief Restitue la latence (q_i) de la tâche \p task.
 */
unsigned long get_task_latency(const task_t *task) {
    assert(task);
    return task->latency; // Adapté à votre projet
}

/**
 * @brief Affiche une tâche
 */
void view_task(const void *task) {
    assert(task);
    const task_t *T = (const task_t *) task;

    printf("(id:%s, p:%lu, q:%lu)",
           get_task_id(T),
           get_task_processing_time(T),
           get_task_latency(T));
}

void delete_task(void *task) {
    assert(task);

    task_t *T = (task_t *) task;
    free(T->id);
    free(T);
}

/**
 * @brief
 * Compare 2 tâches pour la règle de Jackson (tri par q_i décroissant).
 * Compatible avec merge_sort_list et quicksort (qui attendent des void**).
 */
int compare_task_jackson_desc(const void *a, const void *b) {

    const task_t *task_a = *(const task_t **)a;
    const task_t *task_b = *(const task_t **)b;

    const unsigned long q_a = get_task_latency(task_a);
    const unsigned long q_b = get_task_latency(task_b);

    // Tri décroissant :
    return compare_long_desc(&q_a, &q_b);
}


/********************************************************************
 * Instance
 ********************************************************************/

Instance read_instance(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        ShowMessage("read_instance : échec lors de l'ouverture du fichier", 1);
        return NULL;
    }

    Instance instance = new_list();
    if (!instance) {
        ShowMessage("read_instance : échec de new_list()", 1);
        fclose(file);
        return NULL;
    }

    char id_buffer[100]; // Buffer temporaire pour lire l'ID
    unsigned long proc_time, latency; // p_i et q_i


    while (fscanf(file, "%99s %lu %lu", id_buffer, &proc_time, &latency) == 3) {

        char *id_alloc = strdup(id_buffer);
        if (!id_alloc) {
            ShowMessage("read_instance : échec de strdup()", 1);
            break;
        }

        task_t *task = new_task(id_alloc, proc_time, latency);
        list_insert_last(instance, task);
    }

    fclose(file);
    return instance;
}

void view_instance(Instance I) {
    assert(I);
    view_list(I, view_task);
}

void delete_instance(Instance I) {
    assert(I);
    delete_list(I, delete_task);
}

task_t **generate_random_tasks(int num_tasks, int max_p, int max_q) {
    task_t **tasks = (task_t **)malloc(num_tasks * sizeof(task_t *));
    if (!tasks) {
        ShowMessage("generate_random_tasks: Échec de l'allocation mémoire pour le tableau de tâches", 1);
        return NULL;
    }
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    for (int i = 0; i < num_tasks; ++i) {
        char id[10];
        sprintf(id, "T%d", i + 1);
        unsigned long p = (rand() % max_p) + 1;
        unsigned long q = (rand() % max_q) + 1;
        tasks[i] = new_task(strdup(id), p, q);
    }

    return tasks;
}

Instance create_instance_from_tasks(task_t **tasks, int num_tasks) {
    if (!tasks) return NULL;
    Instance I = new_list();
    for (int i = 0; i < num_tasks; i++) {
        task_t *original_task = tasks[i];
        // Crée une copie de la tâche pour l'instance
        task_t *new_t = new_task(
            strdup(get_task_id(original_task)),
            get_task_processing_time(original_task),
            get_task_latency(original_task)
        );
        list_insert_last(I, new_t);
    }
    return I;
}

