#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/********************************************************************
 * list_node_t
 ********************************************************************/

/**
 * Construit et initialise un nouveau nœud d'une liste doublement chaînée.
 * Le paramètre \p data correspond à la donnée à affecter au nouveau nœud.
 * 
 * @param[in] data
 * @return le nouveau nœud créé
 */
static list_node_t * new_list_node(void * data) {
	list_node_t * res = (list_node_t *)calloc(1, sizeof(list_node_t));
	assert(res);

	//Le calloc a déjà initialisé tous les champs à 0 (pred/suc), on change juste la data
	res->data = data;

	return res;
}

void * get_list_node_data(const list_node_t * node) {
	assert(node);
	
	return node->data;
}

list_node_t * get_successor(const list_node_t* node) {
	assert(node);
	
	return node->successor; 
}

list_node_t * get_predecessor(const list_node_t * node) {
	assert(node);
	
	return node->predecessor;
}

void set_list_node_data(list_node_t * node, void * newData) {
	assert(node);
	
	node->data = newData;
}

void set_successor(list_node_t * node, list_node_t * newSuccessor) {
	assert(node);
	
	node->successor = newSuccessor;
}

void set_predecessor(list_node_t * node, list_node_t * newPredecessor) {
	assert(node);
	
	node->predecessor = newPredecessor;
}

/********************************************************************
 * list_t (Double-Linked List)
 ********************************************************************/

list_t * new_list() {
	list_t * res = (list_t *)calloc(1, sizeof(list_t));
	assert(res);

	//Le calloc a déjà initialisé tous les champs de la struct à 0, donc pas besoin de référencer les champs à NULL/0.
	return res;
}

int list_is_empty(const list_t * L) {
	assert(L);
	
	return L->size == 0;
}

unsigned int get_list_size(const list_t * L) {
	assert(L);
	
	return L->size;
}

list_node_t * get_list_head(const list_t * L) {
	assert(L);
	
	return L->head;
}

list_node_t * get_list_tail(const list_t * L) {
	assert(L);
	
	return L->tail;
}

void increase_list_size(list_t * L) {
	assert(L);
	
	(L->size)++;
}

void decrease_list_size(list_t * L) {
	assert(L);
	
	(L->size)--;
}

void set_list_size(list_t * L, unsigned int newSize) {
	assert(L);
	
	L->size = newSize;
}

void set_list_head(list_t * L, list_node_t * newHead) {
	assert(L);
	
	L->head = newHead;
}

void set_list_tail(list_t * L, list_node_t * newTail) {
	assert(L);
	
	L->tail = newTail;
}

void delete_list(list_t * L, void (*freeData)(void *)) {
	assert(L);

	void * data;

	while (!list_is_empty(L)) {
		data = get_list_node_data(get_list_head(L));

		if (data && freeData)
			(*freeData)(data);
		
		list_remove_first(L);
	}

	free(L);
}

void view_list(const list_t * L, void (*viewData)(const void *)) {
	assert(L);
	
	printf("(");

	for(list_node_t * E = get_list_head(L); E; E = get_successor(E)) {
		(*viewData)(get_list_node_data(E));

		if (get_successor(E) != NULL)
			printf(", ");
	}

	printf(")\n");
}

void list_insert_first(list_t * L, void * data) {
	assert(L);
	
	list_node_t * oldHead = get_list_head(L);
	list_node_t * newHead = new_list_node(data);

	//Remaillionnage
	//Pas besoin de remaillionnage préventif car les champs sont déjà initialisés à NULL pour la nouvelle tête.
	if (oldHead) {
		set_successor(newHead, oldHead);
		set_predecessor(oldHead, newHead);
	}
	else
		//La tête est aussi la queue, car s'il n'y a pas de tête, alors la liste est supposée vide
		set_list_tail(L, newHead);

	//Changement de la head
	set_list_head(L, newHead);

	//++ taille liste
	increase_list_size(L);
}

void list_insert_last(list_t * L, void * data) {
	assert(L);
	
	list_node_t * oldTail = get_list_tail(L);
	list_node_t * newTail = new_list_node(data);

	//Remaillionnage
	//Pas besoin de remaillionnage préventif car les champs sont déjà initialisés à NULL pour la nouvelle tête.
	if (oldTail) {
		set_predecessor(newTail, oldTail);
		set_successor(oldTail, newTail);
	}
	else
		//La queue est aussi la tête, car s'il n'y a pas de queue, alors la liste est supposée vide
		set_list_head(L, newTail);
	
	//Changement de la tail
	set_list_tail(L, newTail);

	//++ taille liste
	increase_list_size(L);
}

void list_insert_after(list_t * L, void * data, list_node_t * node) {
	assert(L);

	//Si l'insertion se fait à la fin,
	if (node == get_list_tail(L))
		list_insert_last(L, data);

	//Sinon, si l'insertion se fait en début/milieu de liste
	else {
		list_node_t * newNode = new_list_node(data);
		list_node_t * oldSuc = get_successor(node);

		//Remaillionnage gauche
		set_successor(node, newNode);
		set_predecessor(newNode, node);

		//Remaillionnage droit
		set_predecessor(oldSuc, newNode);
		set_successor(newNode, oldSuc);

		increase_list_size(L);
	}
}

void * list_remove_first(list_t * L) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
	
	list_node_t * head = get_list_head(L);
	list_node_t * suc = get_successor(head);

	void * data = get_list_node_data(head);

	//On met la nouvelle tête. Peut être NULL.
	

	if (suc) {
		set_list_head(L, suc);
		set_predecessor(suc, NULL); //On enlève le prédécesseur du nouveau head
	}

	else { //Dans le cas d'un seul elm dans la liste (qu'on supprime)
		set_list_head(L, NULL);
		set_list_tail(L, NULL);
	}

	free(head);
	decrease_list_size(L);

	return data;
}

void * list_remove_last(list_t * L) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
	
	list_node_t * tail = get_list_tail(L);
	list_node_t * pred = get_predecessor(tail);

	void * data = get_list_node_data(tail);

	if (pred) {
		set_list_tail(L, pred);
		set_successor(pred, NULL); //On enlève le successeur de la nouvelle tail
	}

	else { //Si 1 seul maillon dans la liste (grosso modo)
		set_list_tail(L, NULL);
		set_list_head(L, NULL);
	}

	free(tail);
	decrease_list_size(L);
	
	return data;
}

void * list_remove_node(list_t * L, list_node_t * node) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
	
	if (get_list_head(L) == node)
		return list_remove_first(L); //On invoque la fonction pour la tail, et on return la data par cheminement
	
	else if (get_list_tail(L) == node)
		return list_remove_last(L); //Voir commentaires en haut

	else {
		list_node_t * pred = get_predecessor(node);
		list_node_t * suc = get_successor(node);

		void * data = get_list_node_data(node);

		//Remaillonnage
		set_successor(pred, suc);
		set_predecessor(suc, pred);

		//Liberation et autre
		free(node);
		decrease_list_size(L);

		return data;
	}
}

void list_swap_nodes_data(list_node_t * node1, list_node_t * node2) {
	assert(node1);
	assert(node2);
	
	if (node1 != node2) {
		void *data1 = get_list_node_data(node1);
		void *data2 = get_list_node_data(node2);

		//Swap
		set_list_node_data(node1, data2);
		set_list_node_data(node2, data1);
	}
}

int list_data_exist(const list_t * L, const void * data) {
	assert(L);
	
	//On boucle
	for (list_node_t * E = get_list_head(L); E; E = get_successor(E)) {
		if (get_list_node_data(E) == data)
			return 1;
	}

	return 0;
}

list_t *copy_list(const list_t *original) {
    if (!original) {
        return NULL;
    }

    list_t *copy = new_list();

    for (list_node_t *current = original->head; current != NULL; current = current->successor) {
        list_insert_last(copy, current->data);
    }

    return copy;
}

/**
 * @brief Algorithme de séparation d'une liste en deux
 *
 * @param list_original la liste à séparée
 * @param list_part1 la liste qui contiendra la première partie de list_original
 * @param list_part2 la liste qui contiendra la deuxième partie de list_original
 */
void split_List(list_t *list_original, list_t *list_part1, list_t *list_part2) {
    //Calcul du milieu de la liste pour pouvoir la couper de manière équitable
    const unsigned int mid = get_list_size(list_original) / 2;

    //Insertion de la première partie de list_original dans la liste correspondante
    //Au fur et à mesure, la valeur tête de list_original est supprimée pour pouvoir
    //accéder aux valeurs suivantes
    for (int i = 0; i < mid; i++) {
        list_insert_last(list_part1, list_remove_first(list_original));
    }

    //Insertion de la deuxième partie de list_original dans la l
    while (!list_is_empty(list_original)) {
        list_insert_last(list_part2, list_remove_first(list_original));
    }
}

/**
 * @brief Algorithme de fusion de deux listes
 *
 * @param list la liste fusionnée
 * @param listA la première liste à fusionner
 * @param listB la deuxième liste à fusionner
 * @param compare méthode utilisée pour comparer les éléments de listes
 */
void merge_list(list_t *list, list_t *listA, list_t *listB, int (*compare)(const void *a, const void *b)) {
    //Les deux listes ont des éléments
    while (!list_is_empty(listA) && !list_is_empty(listB)) {
        const void *dataA = get_list_node_data(get_list_head(listA));
        const void *dataB = get_list_node_data(get_list_head(listB));

        //On insère la tâche ayant la durée la plus élevée
        if (compare(&dataA, &dataB) <= 0) {
            list_insert_last(list, list_remove_first(listA));
        } else {
            list_insert_last(list, list_remove_first(listB));
        }
    }
    //Dans le cas ou les deux listes n'ont pas le même nombre d'éléments
    //(lorsque la liste initiale avait un nombre impair d'éléments),
    //il faut ensuite ajouter cet élément à la liste finale
    while (!list_is_empty(listA)) {
        list_insert_last(list, list_remove_first(listA));
    }
    while (!list_is_empty(listB)) {
        list_insert_last(list, list_remove_first(listB));
    }
}

void merge_sort_list(list_t *list, int (*compare)(const void *a, const void *b), void (*freeData)(void *)) {
    assert(list && compare);
    const unsigned int N = get_list_size(list);
    if (N <= 1) {
        return; // Rien à faire si la liste est vide ou avec un seul élément
    }
    //Création des listes auxiliaires
    list_t *listA = new_list();
    list_t *listB = new_list();

    //Séparation de la liste en deux
    split_List(list, listA, listB);

    //Tri de chaque listes
    merge_sort_list(listA, compare, freeData);
    merge_sort_list(listB, compare, freeData);

    //Fusion des listes
    merge_list(list, listA, listB, compare);

    //Libération de la mémoire pour listes auxilliaires utilisées
	free(listA);
	free(listB);
}

void quicksort(list_t *list, int (*compare)(const void *a, const void *b), void (*viewData)(const void *)) {
    const unsigned int N = get_list_size(list);
    if (N <= 1) {
        return; // Si la liste est vide ou a un seul élément, pas besoin de trier
    }

    //task_t *tab[N];
    void *tab[N];

    // Étape 1 : Extraire les éléments de la liste dans un tableau
    for (int i = 0; i < N; i++) {
        tab[i] = list_remove_first(list);
    }

    // Étape 2 : Trier le tableau à l'aide de qsort
    qsort(tab, N, sizeof(void *), compare);

    // Étape 3 : Réinsérer les éléments triés dans la liste
    for (int i = 0; i < N; i++) {
        list_insert_last(list, tab[i]);
    }
    view_list(list, viewData);
}

void remove_duplicates(list_t *list, int (*compare)(const void *a, const void *b)) {
	const list_node_t *iterator = get_list_head(list);

	/* On parcourt tous les éléments avec iterator */
	while (iterator != NULL) {
		list_node_t *iteratorSuccessor = get_successor(iterator);

		/* On compare iterator avec TOUS les éléments qui le suivent */
		while (iteratorSuccessor != NULL) {
			void *S1 = get_list_node_data(iterator);
			void *S2 = get_list_node_data(iteratorSuccessor);

			/* On compare les valeurs (et pas les adresses) */
			if (compare(&S1, &S2) == 0) {
				/* On garde iteratorSuccessor pour le tour suivant AVANT de supprimer */
				list_node_t *to_delete = iteratorSuccessor;
				iteratorSuccessor = get_successor(iteratorSuccessor);

				list_remove_node(list, to_delete);
			}
			else {
				/* On avance seulement si ce n’est pas un doublon */
				iteratorSuccessor = get_successor(iteratorSuccessor);
			}
		}

		/* On passe à l’élément suivant (celui qui n’a plus de doublons après lui) */
		iterator = get_successor(iterator);
	}
}