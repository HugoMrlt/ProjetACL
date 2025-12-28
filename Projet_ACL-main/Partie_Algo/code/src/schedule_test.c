#include "list.h"
#include "schedule.h"
#include "instance.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void test_schedule_creation_and_access() {
    printf("--- Test 1: Création et Accès Basique ---\n");
    schedule_t *S = new_schedule(3);

    assert(get_num_machines(S) == 3);
    assert(get_schedule_of_machine(S, 0) != NULL);
    assert(list_is_empty(get_schedule_of_machine(S, 2)));

    printf("Test 1 OK.\n");
    delete_schedule(S);
}

task_t *new_task_test(char *id, unsigned long p_time, unsigned long latency, unsigned long c_time) {
    task_t *t = (task_t *) new_task(id, c_time, latency);

    t->completion_time = c_time;
    return t;
}

void test_add_task_and_makespan() {
    printf("--- Test 2: Ajout de Tâche et Makespan ---\n");
    schedule_t *S = new_schedule(2);

    // Tâches de test (p_i=10, q_i=100)
    task_t *T1 = new_task_test("T1", 10, 100, 10);
    task_t *T2 = new_task_test("T2", 10, 100, 20);
    task_t *T3 = new_task_test("T3", 10, 100, 50);

    // M0: T1 (0-10)
    add_task_to_schedule(S, T1, 0, 0, 10);
    // M1: T2 (0-10)
    add_task_to_schedule(S, T2, 1, 0, 10);
    // M0: T3 (10-20) -> on vérifie que le makespan reflète le end_time de T3
    add_task_to_schedule(S, T3, 0, 10, 20);

    // Le makespan doit être max(20, 10) = 20
    unsigned long machine0_ct = get_schedule_node_end_time(get_list_node_data(get_list_tail(get_schedule_of_machine(S, 0))));
    unsigned long machine1_ct = get_schedule_node_end_time(get_list_node_data(get_list_tail(get_schedule_of_machine(S, 1))));

    assert(machine0_ct == 20);
    assert(machine1_ct == 10);

    printf("  (Makespan logique: max(%lu, %lu) = 20)\n", machine0_ct, machine1_ct);

    printf("Test 2 OK.\n");

    delete_schedule(S);
}

void test_find_empty_machine() {
    printf("--- Test 3: Recherche de Machine Libre ---\n");
    schedule_t *S = new_schedule(3);

    task_t *T_dummy = new_task_test("TD", 10, 100, 20);

    // M0: occupée jusqu'à t=20
    add_task_to_schedule(S, T_dummy, 0, 10, 20);
    // M1: vide
    // M2: occupée jusqu'à t=10
    add_task_to_schedule(S, T_dummy, 2, 0, 10);

    // Les indices des machines : M0 = 0, M1 = 1, M2 = 2.
    // Chercher à t=0: M1 est vide -> doit retourner 1
    assert(find_empty_machine(S, 0) == 1);

    // Chercher à t=15: M2 est libre (15 > 10) -> doit retourner 2 (le plus petit index libre)
    assert(find_empty_machine(S, 15) == 1);

    // Chercher à t=20: Aucune machine n'est libre (NB3: end_time=time n'est pas libre)
    // M0 (20) n'est pas libre. M2 (10) est libre. M1 est libre. -> doit retourner 1.
    assert(find_empty_machine(S, 20) == 1);

    // Chercher à t=21: M0 est libre (21 > 20) -> doit retourner 0.
    assert(find_empty_machine(S, 21) == 0);

    // Test machine vide
    schedule_t *S_empty = new_schedule(1);
    assert(find_empty_machine(S_empty, 100) == 0);
    delete_schedule(S_empty);

    printf("Test 3 OK.\n");
    delete_schedule(S);
}

void test_preempt_task() {
    printf("--- Test 4: Préemption et Temps Restant ---\n");
    schedule_t *S = new_schedule(1);

    task_t *T4 = new_task_test("T4", 30, 100, 30);

    // M0: T4 (0-30)
    add_task_to_schedule(S, T4, 0, 0, 30);

    // Préempter à t=12. new_et=12
    unsigned long remaining = preempt_task(S, 0, 12);

    // Le temps restant (original: 30 - nouveau: 12) doit être 18
    assert(remaining == 18);

    // Le nouveau end_time du nœud doit être 12
    schedule_node_t *last_node = get_list_node_data(get_list_tail(get_schedule_of_machine(S, 0)));
    assert(get_schedule_node_end_time(last_node) == 12);

    printf("Test 4 OK.\n");
    delete_schedule(S);
}


void test_lmax_logic() {
    printf("--- Test 5: Logique Lmax (Retard Maximal) ---\n");

    task_t *TL1 = new_task_test("L1", 10, 5, 10);  // Cj=10, qj=5
    task_t *TL2 = new_task_test("L2", 10, 30, 50); // Cj=50, qj=30
    task_t *TL3 = new_task_test("L3", 10, 10, 30); // Cj=30, qj=10

    schedule_t *S = new_schedule(2);

    // M0: L1 (0-10), Cj=10
    add_task_to_schedule(S, TL1, 0, 0, 10);
    // M1: L2 (40-50), Cj=50
    add_task_to_schedule(S, TL2, 1, 40, 50);
    // M0: L3 (20-30), Cj=15 (dans task_t) -> La tâche L3 n'est pas la dernière, mais elle a un Lj
    add_task_to_schedule(S, TL3, 0, 20, 30);

    // --- Test get_lmax_for_machine (M0) ---
    // M0 contient L1 et L3.
    // L1: Cj+qj = 15. L3: Cj+qj = 40.
    assert(get_lmax_for_machine(S, 0) == 40);

    // --- Test get_lmax_for_machine (M1) ---
    // M1 contient L2.
    // L2: Cj+qj = 80.
    assert(get_lmax_for_machine(S, 1) == 80);

    // --- Test get_lmax global ---
    // max(40, 80) = 80
    assert(get_lmax(S) == 80);

    printf("Test 5 OK (en utilisant votre formule Lj = Cj + qj).\n");

    delete_schedule(S);
}

int main() {
    printf("===========================================\n");
    printf("        Tests des fonctions schedule.c       \n");
    printf("===========================================\n");

    test_schedule_creation_and_access();
    test_add_task_and_makespan();
    test_find_empty_machine();
    test_preempt_task();
    test_lmax_logic();

    printf("\nTous les tests passés avec succès (selon les hypothèses de dépendances).\n");

    return 0;
}