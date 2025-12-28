#include "instance.h"
#include "list.h"
#include "gestion_robot.h"

#include <stdio.h>
#include <string.h>

int main() {
    printf("=== Test Gestion Robot ===\n");

    // Création d'une instance de test
    Instance I = new_list();

    char *id1 = strdup("TacheA");
    char *id2 = strdup("TacheB");
    char *id3 = strdup("TacheC");

    task_t *t1 = new_task(id1, 0, 20); // q_i = 20
    t1->completion_time = 2;           // r_i = 2

    task_t *t2 = new_task(id2, 0, 5);  // q_i = 5
    t2->completion_time = 10;          // r_i = 10

    task_t *t3 = new_task(id3, 0, 8);  // q_i = 8
    t3->completion_time = 12;          // r_i = 12

    list_insert_last(I, t1);
    list_insert_last(I, t2);
    list_insert_last(I, t3);

    printf("Instance de test (r_i, q_i):\n");
    printf("TacheA: 2, 20\n");
    printf("TacheB: 10, 5\n");
    printf("TacheC: 12, 8\n");

    unsigned long cmax = calculer_temps_transport(I);

    printf("\nResultat Cmax = %lu\n", cmax);

    // Nettoyage
    delete_instance(I);

    printf("=== Fin du test ===\n");
    return 0;
}