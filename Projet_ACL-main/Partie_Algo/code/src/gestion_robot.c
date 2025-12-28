#include "gestion_robot.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned long calculer_temps_transport(Instance I) {
    if (list_is_empty((list_t * )I)) return 0;

    // Etape 1: Initialisation
    // On travaille sur une copie de la liste pour ne pas modifier l'instance originale
    list_t *L = copy_list((list_t * )I);

    unsigned long tr1 = 0;
    unsigned long tr2 = 0;

    printf("=== Debut Algo Transport LPT ===\n");

    // Etape 2: Boucle principale (tant qu'il reste des tâches)
    while (!list_is_empty(L)) {
        
        // Etape 3: Choix du robot disponible le plus tot
        int robot_choisi = 1; 
        unsigned long t_courant = tr1;

        if (tr2 < tr1) {
            robot_choisi = 2;
            t_courant = tr2;
        }

        // Etape 4: Recherche de la tache candidate (LPT)
        list_node_t *iterator = get_list_head(L);
        list_node_t *meilleur_noeud = NULL;
        unsigned long max_qi = 0;
        int trouve = 0;

        while (iterator) {
            task_t *t = (task_t *)get_list_node_data(iterator);

            // r_i = completion_time, q_i = latency
            if (t->completion_time <= t_courant) {
                if (trouve == 0 || t->latency > max_qi) {
                    max_qi = t->latency;
                    meilleur_noeud = iterator;
                    trouve = 1;
                }
            }
            iterator = get_successor(iterator);
        }

        // Etape 5: Mise a jour
        if (trouve) {
            task_t *t_choisie = (task_t *)get_list_node_data(meilleur_noeud);
            
            if (robot_choisi == 1) {
                tr1 = t_courant + t_choisie->latency;
                printf("R1 : %s (fin = %lu)\n", get_task_id(t_choisie), tr1);
            } else {
                tr2 = t_courant + t_choisie->latency;
                printf("R2 : %s (fin = %lu)\n", get_task_id(t_choisie), tr2);
            }

            list_remove_node(L, meilleur_noeud);
        } else {
            // Aucun job disponible, on avance le temps
            unsigned long min_ri = ULONG_MAX;
            iterator = get_list_head(L);

            while (iterator) {
                task_t *t = (task_t *)get_list_node_data(iterator);
                if (t->completion_time < min_ri) {
                    min_ri = t->completion_time;
                }
                iterator = get_successor(iterator);
            }

            if (robot_choisi == 1) tr1 = min_ri;
            else tr2 = min_ri;
        }
    }

    delete_list(L, &delete_task);

    // Etape 6: Retourner le max des deux robots
    if (tr1 > tr2) return tr1;
    return tr2;
}