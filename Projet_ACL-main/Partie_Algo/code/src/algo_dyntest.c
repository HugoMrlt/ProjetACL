#include "list.h"
#include "instance.h"
#include "schedule.h"
#include "algo_dyn.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

char *dup_id(const char *id) {
    char *new_id = strdup(id);
    assert(new_id);
    return new_id;
}

void test_dp_basic_case() {
    printf("=== Test DP 1 : Cas de base P2||Lmax (T1, T2, T3) ===\n");

    Instance I = (Instance)new_list();
    list_insert_last((list_t *)I, new_task(dup_id("T1"), 3, 10));
    list_insert_last((list_t *)I, new_task(dup_id("T2"), 3, 10)); // p=3, q=10
    list_insert_last((list_t *)I, new_task(dup_id("T3"), 4, 2));  // p=4, q=2

    const int M = 2;
    const double DELTA_EXACT = 0;

    printf("Instance créée (N=%u) : ", get_list_size((const list_t *)I));
    view_instance(I);

    unsigned long optimal_Lmax = dynamic_programming_PmLmax(I, M, DELTA_EXACT, NULL);

    printf("Résultat DP (L_max) : %lu\n", optimal_Lmax);

    const unsigned long EXPECTED_LMAX = 13;
    assert(optimal_Lmax == EXPECTED_LMAX);


    delete_instance(I);

    printf("Test DP 1 OK. L_max trouvé : %lu\n\n", optimal_Lmax);
}


void test_dp_fptas_case() {
    printf("=== Test DP 2 : Utilisation de reduce_states (FPTAS) ===\n");

    Instance I = (Instance)new_list();

    list_insert_last((list_t *)I, new_task(dup_id("T1"), 10, 100));
    list_insert_last((list_t *)I, new_task(dup_id("T2"), 10, 100));

    const int M = 2;

    const double DELTA_FPTAS = 5.0;

    printf("Instance créée (N=%u, M=%d). Delta de réduction: %f\n", get_list_size((const list_t *)I), M, DELTA_FPTAS);


    unsigned long Lmax_approx = dynamic_programming_PmLmax((Instance)I, M, DELTA_FPTAS, NULL);

    printf("Résultat DP (L_max approximatif) : %lu\n", Lmax_approx);


    assert(Lmax_approx >= 110);


    delete_instance(I);

    printf("Test DP 2 OK. L_max approximatif trouvé : %lu\n\n", Lmax_approx);
}

int main() {
    printf("====================================================\n");
    printf("  Tests des fonctions de Programmation Dynamique\n");
    printf("====================================================\n");

    test_dp_basic_case();
    test_dp_fptas_case();

    printf("TOUS LES TESTS DP PASSÉS AVEC SUCCÈS !\n");
    return 0;
}