#include "instance.h"
#include "fptas.h"
#include "list.h"

#include <stdio.h>
#include <string.h>

int main() {
    printf("=== Test FPTAS ===\n");

    // Création d'une instance de test
    Instance I = new_list();

    char *id1 = strdup("T1");
    char *id2 = strdup("T2");
    char *id3 = strdup("T3");
    char *id4 = strdup("T4");

    list_insert_last(I, new_task(id1, 10, 5));
    list_insert_last(I, new_task(id2, 20, 2));
    list_insert_last(I, new_task(id3, 5, 10));
    list_insert_last(I, new_task(id4, 15, 8));

    printf("Instance originale:\n");
    view_instance(I);

    int m = 2;
    double epsilon = 0.5;

    printf("\nLancement FPTAS avec m=%d, epsilon=%.2f\n", m, epsilon);
    Instance I_prime = fptas_PmLmax(I, m, epsilon);

    printf("Instance modifiée I':\n");
    view_instance(I_prime);

    // Nettoyage
    delete_instance(I);
    delete_instance(I_prime);

    printf("=== Fin du test ===\n");
    return 0;
}
