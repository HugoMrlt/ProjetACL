#include "state.h"

int compare_state(const void *a, const void *b) {
    state_t *aCopy = (state_t *) a;
    state_t *bCopy = (state_t *) b;

    if (get_makespan(*aCopy) < get_makespan(*bCopy)) {
        return -1;
    }
    if (get_makespan(*aCopy) > get_makespan(*bCopy)) {
        return 1;
    }
    return 0;
}

int compare_zone(const void *a, const void *b, double d) {
    state_t *aCopy = (state_t *) a;
    state_t *bCopy = (state_t *) b;

    for (int i = 0; i < get_num_machines(*aCopy); i++) {
        if ((double) get_completion_time(*aCopy, i) / d < (double) get_completion_time(*bCopy, i) / d) {
            return -1;
        }
        if ((double) get_completion_time(*aCopy, i) / d > (double) get_completion_time(*bCopy, i) / d) {
            return 1;
        }
    }
    return 0;
}

void reduce_states(list_t *list, double delta) {
    list_node_t *iterator1 = get_list_head(list);
    list_node_t *aux;
    while (iterator1) {
        list_node_t *iterator2 = iterator1->successor;
        while (iterator2) {
            state_t S1 = get_list_node_data(iterator1);
            state_t S2 = get_list_node_data(iterator2);
            if (compare_zone(&S1, &S2, delta) == 0) {
                if (get_completion_time(S1, 0) <= get_completion_time(S2, 0)) {
                    aux = iterator2;
                    iterator2 = aux->successor;
                    list_remove_node(list, aux);
                } else {
                    aux = iterator1;
                    iterator1 = aux->successor;
                    list_remove_node(list, aux);
                    iterator2 = iterator1->successor;
                }
            } else {
                iterator2 = iterator2->successor;
            }
        }
        iterator1 = iterator1->successor;
    }
}
