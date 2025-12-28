#include "list.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void test_basic_operations() {
    printf("=== Test 1 : opérations de base (int) ===\n");

    list_t *L = new_list();

    int a = 10, b = 20, c = 30;

    list_insert_last(L, &a);
    list_insert_last(L, &b);
    list_insert_last(L, &c);

    printf("Liste après 3 insert_last : ");
    view_list(L, viewInt);               // attendu : (10, 20, 30)

    assert(get_list_size(L) == 3);
    assert(*(int*)get_list_node_data(get_list_head(L)) == 10);
    assert(*(int*)get_list_node_data(get_list_tail(L)) == 30);

    list_insert_first(L, &c);  // c devient tête
    printf("Après insert_first(30) : ");
    view_list(L, viewInt);               // (30, 10, 20, 30)

    assert(get_list_size(L) == 4);

    delete_list(L, NULL);
    printf("Test 1 OK\n\n");
}

void test_remove() {
    printf("=== Test 2 : suppressions ===\n");

    list_t *L = new_list();
    int vals[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) list_insert_last(L, &vals[i]);

    printf("Initiale : ");
    view_list(L, viewInt);               // (1, 2, 3, 4, 5)

    int *removed = list_remove_first(L);
    assert(*removed == 1 && get_list_size(L) == 4);
    printf("Après remove_first : ");
    view_list(L, viewInt);               // (2, 3, 4, 5)

    removed = list_remove_last(L);
    assert(*removed == 5 && get_list_size(L) == 3);
    printf("Après remove_last : ");
    view_list(L, viewInt);               // (2, 3, 4)

    // Suppression au milieu
    list_node_t *node = get_successor(get_list_head(L)); // pointe sur 3
    removed = list_remove_node(L, node);
    assert(*removed == 3 && get_list_size(L) == 2);
    printf("Après remove_node(3) : ");
    view_list(L, viewInt);               // (2, 4)

    delete_list(L, NULL);
    printf("Test 2 OK\n\n");
}

void test_insert_after() {
    printf("=== Test 3 : list_insert_after ===\n");

    list_t *L = new_list();
    int a = 100, b = 200, c = 300;

    list_insert_last(L, &a);
    list_insert_last(L, &c);

    list_node_t *node_a = get_list_head(L);
    list_insert_after(L, &b, node_a);

    printf("Après insert_after(200 après 100) : ");
    view_list(L, viewInt);               // (100, 200, 300)

    // Insertion en fin via insert_after
    list_insert_after(L, &a, get_list_tail(L)); // ajoute un autre 100 à la fin
    printf("Après insert_after en fin : ");
    view_list(L, viewInt);               // (100, 200, 300, 100)

    delete_list(L, NULL);
    printf("Test 3 OK\n\n");
}

void test_swap_and_exist() {
    printf("=== Test 4 : swap et existence ===\n");

    list_t *L = new_list();
    int x = 1, y = 999;

    list_insert_last(L, &x);
    list_insert_last(L, &y);

    list_node_t *n1 = get_list_head(L);
    list_node_t *n2 = get_successor(n1);

    list_swap_nodes_data(n1, n2);

    printf("Après swap : ");
    view_list(L, viewInt);               // (999, 1)

    assert(list_data_exist(L, &x));
    assert(list_data_exist(L, &y));

    delete_list(L, NULL);
    printf("Test 4 OK\n\n");
}

void test_string_list() {
    printf("=== Test 5 : liste de chaînes (allocation dynamique) ===\n");

    list_t *L = new_list();

    char *s1 = strdup("Bonjour");
    char *s2 = strdup("le");
    char *s3 = strdup("monde");

    list_insert_last(L, s1);
    list_insert_last(L, s2);
    list_insert_last(L, s3);

    printf("Liste de strings : ");
    view_list(L, view_string);            // ("Bonjour", "le", "monde")

    // Nettoyage propre
    delete_list(L, free_string);
    printf("Test 5 OK (mémoire libérée proprement)\n\n");
}

void test_empty_and_edge_cases() {
    printf("=== Test 6 : cas limites (liste vide, 1 élément) ===\n");

    list_t *L = new_list();
    assert(list_is_empty(L));
    assert(get_list_size(L) == 0);
    assert(get_list_head(L) == NULL);
    assert(get_list_tail(L) == NULL);

    int val = 42;
    list_insert_first(L, &val);
    assert(get_list_size(L) == 1);
    assert(get_list_head(L) == get_list_tail(L));

    list_remove_first(L);
    assert(list_is_empty(L));

    // Suppression sur liste vide → assert va planter → on ne le fait pas
    // mais on vérifie que delete_list marche sur liste vide
    delete_list(L, NULL);

    printf("Test 6 OK\n\n");
}
void test_copy_list() {
    printf("=== Test : copy_list ===\n");
    list_t *orig = new_list();
    int vals[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) list_insert_last(orig, &vals[i]);

    list_t *copie = copy_list(orig);

    printf("Original : "); view_list(orig, viewInt);
    printf("Copie    : "); view_list(copie, viewInt);

    assert(get_list_size(copie) == 3);
    assert(*(int*)get_list_node_data(get_list_head(copie)) == 10);

    // Modification de l'original → copie inchangée
    int neuf = 999;
    set_list_node_data(get_list_head(orig), &neuf);
    assert(*(int*)get_list_node_data(get_list_head(copie)) == 10);

    delete_list(orig, NULL);
    delete_list(copie, NULL);
    printf("copy_list OK\n\n");
}

void test_merge_sort_list() {
    printf("=== Test : merge_sort_list (croissant) ===\n");
    list_t *L = new_list();
    int vals[] = {64, 8, 123, 1, 42, 7, 99};
    for (int i = 0; i < 7; i++) list_insert_last(L, &vals[i]);

    printf("Avant tri : "); view_list(L, viewInt);

    merge_sort_list(L, compare_int_asc, NULL);

    printf("Après tri : "); view_list(L, viewInt);

    // Vérification stricte du tri
    const list_node_t *n = get_list_head(L);
    int prev = *(int*)get_list_node_data(n);
    for (n = get_successor(n); n; n = get_successor(n)) {
        const int curr = *(int*)get_list_node_data(n);
        assert(prev <= curr);
        prev = curr;
    }
    delete_list(L, NULL);
    printf("merge_sort_list OK\n\n");
}

void test_quicksort() {
    printf("=== Test : quicksort (décroissant) ===\n");
    list_t *L = new_list();
    int vals[] = {5, 1, 4, 9, 2, 8};
    for (int i = 0; i < 6; i++) list_insert_last(L, &vals[i]);

    printf("Avant quicksort : "); view_list(L, viewInt);

    quicksort(L, compare_int_desc, viewInt);

    printf("Après quicksort (décroissant) : "); view_list(L, viewInt);

    const list_node_t *n = get_list_head(L);
    int prev = *(int*)get_list_node_data(n);
    for (n = get_successor(n); n; n = get_successor(n)) {
        const int curr = *(int*)get_list_node_data(n);
        assert(prev >= curr);
        prev = curr;
    }
    delete_list(L, NULL);
    printf("quicksort OK\n\n");
}

void test_remove_duplicates() {
    printf("=== Test : remove_duplicates ===\n");
    list_t *L = new_list();
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    for (int i = 0; i < 11; i++) list_insert_last(L, &vals[i]);

    printf("Avant suppression : "); view_list(L, viewInt);

    remove_duplicates(L, int_equal);

    printf("Après suppression des doublons : "); view_list(L, viewInt);
    assert(get_list_size(L) == 7);  // 1,2,3,4,5,6,9

    delete_list(L, NULL);
    printf("remove_duplicates OK\n\n");
}

int main() {
    printf("Début des tests pour list.c\n\n");

    test_basic_operations();
    test_remove();
    test_insert_after();
    test_swap_and_exist();
    test_string_list();
    test_empty_and_edge_cases();

    test_copy_list();
    test_merge_sort_list();
    test_quicksort();
    test_remove_duplicates();

    printf("TOUS LES TESTS SONT PASSÉS AVEC SUCCÈS !\n");
    return 0;
}