#include "../include/fptas.h"
#include "../include/list.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

/**
 * Afficher le résultat du fptas.
 * @param lower_bound - Borne inférieure.
 * @param delta
 */
static void print_result(unsigned long lower_bound, double delta) {
    printf("=== FPTAS Reduction Info ===\n");
    printf("LB = %lu\n", lower_bound);
    printf("Delta = %f\n", delta);
    printf("============================\n");
}

Instance fptas_PmLmax(const Instance I, int m, double epsilon) {
    if (list_is_empty(I)) return new_list();

    // Etape 1: Calculer une borne inférieure (LB)
    unsigned long max_pj_qj = 0;
    unsigned long sum_pj = 0;
    unsigned int N = get_list_size(I);

    list_node_t *node = get_list_head(I);
    while (node) {
        task_t *t = (task_t *)get_list_node_data(node);
        unsigned long p = get_task_processing_time(t);
        unsigned long q = get_task_latency(t);

        if (p + q > max_pj_qj) {
            max_pj_qj = p + q;
        }
        sum_pj += p;

        node = get_successor(node);
    }

    unsigned long term2 = sum_pj / m;
    unsigned long LB = (max_pj_qj > term2) ? max_pj_qj : term2;

    if (LB == 0) return new_list();

    // Etape 2: Calculer le facteur d'échelle (delta)
    double delta = (epsilon * (double)LB) / (double)(N + 1);

    if (delta <= 1e-9) delta = 1.0;

    // Etape 3: Créer l'instance modifiée I'
    Instance I_prime = new_list();
    node = get_list_head(I);
    while (node) {
        task_t *t = (task_t *)get_list_node_data(node);
        unsigned long p = get_task_processing_time(t);
        unsigned long q = get_task_latency(t);

        unsigned long p_prime = (unsigned long)floor((double)p / delta);
        unsigned long q_prime = (unsigned long)floor((double)q / delta);

        char *id_copy = strdup(get_task_id(t));
        task_t *new_t = new_task(id_copy, p_prime, q_prime);
        list_insert_last(I_prime, new_t);

        node = get_successor(node);
    }

    print_result(LB, delta);

    return I_prime;
}