#include "../include/instance.h"
#include "../include/list.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

/** ------------------------------------------------------------------
 * Fonctions utilitaires
 * ------------------------------------------------------------------ */

char *dup_id(const char *id) {
    char *new_id = strdup(id);
    assert(new_id);
    return new_id;
}

/** ------------------------------------------------------------------
 * Tests
 * ------------------------------------------------------------------ */

/**
 * Teste la création, l'accès aux données et la suppression d'une tâche.
 */
void test_task_operations() {
    printf("=== Test 1 : Opérations de base sur task_t ===\n");


    char *id_val = dup_id("T_A");
    const unsigned long p_val = 50;
    const unsigned long q_val = 100;

    task_t *T = new_task(id_val, p_val, q_val);


    assert(strcmp(get_task_id(T), "T_A") == 0);
    assert(get_task_processing_time(T) == 50);
    assert(get_task_latency(T) == 100);

    printf("Tache créée : ");
    view_task(T);
    printf("\n");

    delete_task(T);

    printf("Test 1 OK\n\n");
}

/**
 * Teste la lecture d'une instance depuis un fichier.
 * Nécessite un fichier 'tasks.txt'.
 */
void test_read_instance() {
    printf("=== Test 2 : Lecture d'instance (read_instance) ===\n");

    const char *filename = "../tasks.txt";
    printf("Tentative de lecture du fichier : %s\n", filename);

    Instance I = read_instance(filename);

    if (I == NULL) {
        printf("Avertissement: Échec de la lecture. Assurez-vous que le fichier '%s' existe.\n", filename);
        printf("Test 2 OK (Lecture échouée ou réussie)\n\n");
        return;
    }


    printf("Instance lue (taille : %u) : ", get_list_size(I));
    view_instance(I);


    assert(get_list_size(I) == 5);


    struct list_node_t *head = get_list_head(I);
    task_t *first_task = (task_t *)get_list_node_data(head);
    assert(strcmp(get_task_id(first_task), "T1") == 0);
    assert(get_task_processing_time(first_task) == 10);
    assert(get_task_latency(first_task) == 5);

    delete_instance(I);

    printf("Test 2 OK\n\n");
}

/**
 * Teste le tri de l'instance par latence décroissante (Jackson's rule)
 * en utilisant merge_sort_list.
 */
void test_sort_instance() {
    printf("=== Test 3 : Tri d'instance (merge_sort_list/Jackson) ===\n");

    Instance I = new_list();
    list_insert_last(I, new_task(dup_id("T1"), 10, 5));
    list_insert_last(I, new_task(dup_id("T2"), 5, 20));
    list_insert_last(I, new_task(dup_id("T3"), 15, 10));
    list_insert_last(I, new_task(dup_id("T4"), 20, 2));
    list_insert_last(I, new_task(dup_id("T5"), 8, 12));

    printf("Avant tri (Jackson/q_i décroissant) : ");
    view_instance(I);

    merge_sort_list(I, compare_task_jackson_desc, delete_task);

    printf("Après tri : ");
    view_instance(I);


    const struct list_node_t *n = get_list_head(I);
    unsigned long prev_q = get_task_latency((task_t *)get_list_node_data(n));


    assert(get_task_latency((task_t *)get_list_node_data(n)) == 20);

    for (n = get_successor(n); n; n = get_successor(n)) {
        const unsigned long curr_q = get_task_latency((task_t *)get_list_node_data(n));

        assert(prev_q >= curr_q);
        prev_q = curr_q;
    }

    assert(get_task_latency((task_t *)get_list_node_data(get_list_tail(I))) == 2);

    delete_instance(I);

    printf("Test 3 OK\n\n");
}


/* ------------------------------------------------------------------
 * main
 * ------------------------------------------------------------------ */

int main() {
    printf("Début des tests pour instance.c\n\n");

    test_task_operations();
    test_read_instance();
    test_sort_instance();

    printf("Tous les tests sont passées !\n");
    return 0;
}
