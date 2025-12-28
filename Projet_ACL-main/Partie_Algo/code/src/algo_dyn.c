#include "list.h"
#include "instance.h"
#include "schedule.h"
#include "state.h"
#include "algo_dyn.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Helper for qsort
int compare_ulong(const void *a, const void *b) {
    unsigned long arg1 = *(const unsigned long *)a;
    unsigned long arg2 = *(const unsigned long *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

/**
 * Vérifie la dominance entre deux états.
 * Utilise des VLA (Variable Length Arrays) pour la rapidité (évite malloc en boucle).
 */
int check_dominance_arrays(unsigned long *cand_c, unsigned long cand_lmax, 
                          unsigned long *exist_c, unsigned long exist_lmax, int m) {
    
    // Optimisation : Si m est trop grand pour la pile, revenir au malloc (sécurité)
    // Mais pour m=3 ou 6, c'est négligeable.
    
    // Cas 1 : Le candidat est pire sur Lmax, il est peut-être dominé par l'existant.
    if (cand_lmax > exist_lmax) {
        // On vérifie si Existing <= Candidate sur les charges
        unsigned long c1[m];
        unsigned long c2[m];
        
        // Copie pour trier sans modifier les originaux
        memcpy(c1, cand_c, m * sizeof(unsigned long));
        memcpy(c2, exist_c, m * sizeof(unsigned long));
        
        qsort(c1, m, sizeof(unsigned long), compare_ulong);
        qsort(c2, m, sizeof(unsigned long), compare_ulong);
        
        // Vérification : Est-ce que Existing (c2) est meilleur partout que Candidate (c1) ?
        int exist_dominates = 1;
        for(int i=0; i<m; i++) {
            if (c2[i] > c1[i]) { exist_dominates = 0; break; }
        }
        
        if (exist_dominates) return -1; // Existing dominates Candidate
        return 0;
    }
    
    // Cas 2 : Le candidat est meilleur ou égal sur Lmax, il domine peut-être l'existant.
    unsigned long c1[m];
    unsigned long c2[m];
    
    memcpy(c1, cand_c, m * sizeof(unsigned long));
    memcpy(c2, exist_c, m * sizeof(unsigned long));
    
    qsort(c1, m, sizeof(unsigned long), compare_ulong);
    qsort(c2, m, sizeof(unsigned long), compare_ulong);
    
    int cand_dominates = 1;
    for(int i=0; i<m; i++) {
        if (c1[i] > c2[i]) { cand_dominates = 0; break; }
    }
    
    if (cand_dominates) {
        // Si égalité parfaite partout, on considère que le candidat remplace l'ancien
        // (pour éviter les doublons stricts)
        if (cand_lmax == exist_lmax) {
             int exist_dominates = 1;
             for(int i=0; i<m; i++) {
                 if (c2[i] > c1[i]) { exist_dominates = 0; break; }
             }
             if (exist_dominates) return 1; 
        }
        return 1;
    }
    
    return 0;
}

void try_insert_state(list_t *states, state_t S_prev, task_t *task, int machine_idx, 
                     unsigned long *new_c, unsigned long new_lmax, int m) {
    
    list_node_t *current = get_list_head(states);
    list_node_t *next_node;
    
    while (current) {
        next_node = get_successor(current);
        state_t s_exist = (state_t)get_list_node_data(current);
        
        // Récupération des charges de l'état existant
        unsigned long exist_c[m];
        for(int i=0; i<m; i++) exist_c[i] = get_completion_time(s_exist, i);
        
        unsigned long exist_lmax = get_lmax(s_exist);
        
        int dom = check_dominance_arrays(new_c, new_lmax, exist_c, exist_lmax, m);
        
        if (dom == -1) {
            // Le candidat est dominé, on l'abandonne immédiatement.
            return;
        }
        
        if (dom == 1) {
            // Le candidat domine l'état existant : on supprime l'état existant.
            delete_schedule(s_exist);
            list_remove_node(states, current);
        }
        
        current = next_node;
    }
    
    // Si on arrive ici, le candidat n'est dominé par personne. On l'ajoute.
    state_t S_new = copy_schedule(S_prev);
    unsigned long bt = get_completion_time(S_prev, machine_idx);
    unsigned long et = bt + get_task_processing_time(task);
    
    add_task_to_schedule(S_new, task, machine_idx, bt, et);
    list_insert_last(states, S_new);
}

unsigned long dynamic_programming_PmLmax(Instance tasks_instance, int num_machines, double delta, schedule_t **out_best_schedule) {
    const unsigned int N = get_list_size((const list_t *)tasks_instance);
    if (N == 0) return 0;

    list_t *X_current = new_list();
    list_t *X_previous;

    state_t initial_state = new_schedule(num_machines);
    list_insert_last(X_current, initial_state);

    list_node_t *task_node = get_list_head((const list_t *)tasks_instance);

    for (unsigned int j = 1; j <= N; j++) {
        X_previous = X_current; // L'ensemble d'états Xj-1.
        X_current = new_list(); // l'ensemble d'états Xj.
        task_t *T_j = (task_t *)get_list_node_data(task_node); // La tâche j à placer.

        const unsigned long p_j = get_task_processing_time(T_j);
        const unsigned long q_j = get_task_latency(T_j);

        // Parcours des états de Xj-1.
        for (list_node_t *state_node = get_list_head(X_previous);
             state_node;
             state_node = get_successor(state_node))
        {
            state_t S_prev = (state_t)get_list_node_data(state_node);

            // Optimisation Symétrie : ne pas tester les machines ayant la même charge
            unsigned long processed_times[num_machines]; // VLA
            int processed_count = 0;

            for (int i = 0; i < num_machines; i++) {
                unsigned long current_ct = get_completion_time(S_prev, i);
                
                // Vérifie si on a déjà traité une machine avec cette charge pour cet état
                int already_processed = 0;
                for(int k=0; k<processed_count; k++) {
                    if(processed_times[k] == current_ct) {
                        already_processed = 1;
                        break;
                    }
                }
                if(already_processed) continue;
                
                processed_times[processed_count++] = current_ct;

                // Pré-calcul du nouvel état (Tableaux temporaires)
                unsigned long new_c[num_machines]; // VLA
                for(int k=0; k<num_machines; k++) new_c[k] = get_completion_time(S_prev, k);
                new_c[i] += p_j;
                
                unsigned long new_lmax = get_lmax(S_prev);
                unsigned long task_lmax = new_c[i] + q_j;
                if (task_lmax > new_lmax) new_lmax = task_lmax;
                
                // Insertion avec vérification de dominance
                try_insert_state(X_current, S_prev, T_j, i, new_c, new_lmax, num_machines);
            }
        }

        delete_list(X_previous, &delete_schedule);

        // Si FPTAS, on réduit l'espace d'états (si implémenté dans reduce_states)
        if (delta > 0)
            reduce_states(X_current, delta);

        task_node = get_successor(task_node);
    }

    // Recherche du meilleur état final
    unsigned long min_f = ULONG_MAX;
    state_t best_state = NULL;

    for (list_node_t *state_node = get_list_head(X_current);
         state_node;
         state_node = get_successor(state_node))
    {
        state_t S = get_list_node_data(state_node);
        unsigned long current_Lmax = get_lmax(S);

        if (current_Lmax < min_f) {
            min_f = current_Lmax;
            best_state = S;
        }
    }

    // Export de la solution
    if (out_best_schedule && best_state) {
        *out_best_schedule = copy_schedule(best_state);
    }

    delete_list(X_current, &delete_schedule);

    return min_f;
}
