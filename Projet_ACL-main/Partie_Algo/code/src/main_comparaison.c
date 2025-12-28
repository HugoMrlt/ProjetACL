#include "instance.h"
#include "fptas.h"
#include "algo_dyn.h"
#include "jackson.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Retrouver une tâche originale par son ID
task_t *find_task_by_id(Instance I, const char *id) {
    list_node_t *node = get_list_head(I);
    while (node) {
        task_t *t = (task_t *)get_list_node_data(node);
        if (strcmp(get_task_id(t), id) == 0) return t;
        node = get_successor(node);
    }
    return NULL;
}

// Recalculer le coût réel d'un ordonnancement simplifié sur l'instance originale
unsigned long evaluate_schedule_on_original(schedule_t *sched_prime, Instance I_original) {
    if (!sched_prime) return 0;
    unsigned long global_lmax = 0;
    int m = get_num_machines(sched_prime);

    for (int i = 0; i < m; i++) {
        list_t *machine_list = get_schedule_of_machine(sched_prime, i);
        list_node_t *node = get_list_head(machine_list);
        unsigned long current_time = 0;

        while (node) {
            schedule_node_t *s_node = (schedule_node_t *)get_list_node_data(node);
            task_t *t_prime = get_schedule_node_task(s_node);

            task_t *t_orig = find_task_by_id(I_original, get_task_id(t_prime));
            if (t_orig) {
                current_time += get_task_processing_time(t_orig);
                unsigned long lmax_task = current_time + get_task_latency(t_orig);
                if (lmax_task > global_lmax) global_lmax = lmax_task;
            }
            node = get_successor(node);
        }
    }
    return global_lmax;
}

// Fonction pour afficher le contenu d'une instance (liste)
void print_instance_content(Instance I) {
    list_node_t *node = get_list_head(I);
    while (node) {
        task_t *t = (task_t *)get_list_node_data(node);
        printf("[%s: p=%lu, q=%lu] ", get_task_id(t), get_task_processing_time(t), get_task_latency(t));
        node = get_successor(node);
    }
    printf("\n");
}

int main() {
    // --- PARAMETRES ---
    struct TestConfig {
        int n;
        int m;
    } tests[] = {
        {10, 2},
        {10, 3},
        {20, 2},
        {12, 3},
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    int max_p = 500;
    int max_q = 500;
    double epsilon_options[] = {0.2};
    int eps_len = sizeof(epsilon_options) / sizeof(epsilon_options[0]);

    // Génération aléatoire
    task_t **all_tasks = generate_random_tasks(20, max_p, max_q);
    if (!all_tasks) return 1;

    printf(">>> GENERATION DU POOL DE TACHES (Source pour tous les tests) <<<\n");
    for(int i=0; i<20; i++) {
        printf("[%s: p=%lu, q=%lu] ", get_task_id(all_tasks[i]), get_task_processing_time(all_tasks[i]), get_task_latency(all_tasks[i]));
        if ((i+1) % 5 == 0) printf("\n"); // Retour à la ligne toutes les 5 tâches pour lisibilité
    }
    printf("\n>>> DEBUT DES TESTS COMPARATIFS <<<\n\n");

    for (int i = 0; i < num_tests; ++i) {
        int num_tasks = tests[i].n;
        int m = tests[i].m;

            printf("--------------------------------------------------\n");
            printf(" TEST : N = %d tâches | m = %d machines\n", num_tasks, m);
            printf("--------------------------------------------------\n");

            Instance I = create_instance_from_tasks(all_tasks, num_tasks);

            // 1. Jackson
            clock_t start = clock();
            unsigned long lmax_jackson = jackson_heuristic(I, m);
            clock_t end = clock();
            double time_jackson = ((double) (end - start)) / CLOCKS_PER_SEC;

            // 2. Optimal (DP Exacte)
            start = clock();
            schedule_t *sched_opt = NULL;
            unsigned long lmax_opt = dynamic_programming_PmLmax(I, m, 0.0, &sched_opt);
            end = clock();
            double time_opt = ((double) (end - start)) / CLOCKS_PER_SEC;

            if (sched_opt) delete_schedule(sched_opt);

            printf(" %-20s : %lu (%.4f s)\n", "Heuristique Jackson", lmax_jackson, time_jackson);
            printf(" %-20s : %lu (%.4f s)\n", "Optimal (Exact)", lmax_opt, time_opt);
            printf("\n --- Résultats FPTAS ---\n");

            // 3. FPTAS (Boucle sur Epsilon)
            for (int k = 0; k < eps_len; ++k) {
                double eps = epsilon_options[k];

                start = clock();
                Instance I_prime = fptas_PmLmax(I, m, eps);

                printf("   [I' Réduite] : ");
                print_instance_content(I_prime);

                schedule_t *sched_prime = NULL;
                dynamic_programming_PmLmax(I_prime, m, 0.0, &sched_prime);
                end = clock();
                double time_fptas = ((double) (end - start)) / CLOCKS_PER_SEC;

                unsigned long lmax_real = evaluate_schedule_on_original(sched_prime, I);

                double ratio = (lmax_opt > 0) ? (double)lmax_real / lmax_opt : 0.0;

                // Affichage compact
                printf(" [eps=%.2f] Lmax: %lu | Ratio: %.4f (Cible <= %.2f) | Time: %.4f s\n",
                       eps, lmax_real, ratio, 1.0 + eps, time_fptas);

                if (sched_prime) delete_schedule(sched_prime);
                delete_instance(I_prime);
            }
            printf("\n");
            delete_instance(I);
    }

    // Nettoyage final
    for (int i = 0; i < 20; i++) delete_task(all_tasks[i]);
    free(all_tasks);

    return 0;
}