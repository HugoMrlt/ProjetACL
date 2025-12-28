#pragma once

/**
 * Liste générique doublement chaînée.
 */

/**
 * Un nœud d'une liste doublement chaînée contient :
 * (+) une donnée (\p data),
 * (+) une référence (\p successor) au nœud suivant, et
 * (+) une référence (\p predecessor) au nœud précédent.
 */
struct list_node {
	void * data;
	struct list_node * successor, * predecessor;
};

typedef struct list_node list_node_t;

/**
 * Restitue la donnée du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return la donnée du nœud \p node
 */
void * get_list_node_data(const list_node_t * node);

/**
 * Restitue le successeur du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return le successeur du nœud \p node
 */
list_node_t * get_successor(const list_node_t * node);

/**
 * Restitue le prédécesseur du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return le prédécesseur du nœud \p node
 */
list_node_t * get_predecessor(const list_node_t * node);

/**
 * Remplace la donnée du nœud \p node par \p newData.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newData 
 */
void set_list_node_data(list_node_t * node, void * newData);

/**
 * Remplace le successeur du nœud \p node par \p newSuccessor.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newSuccessor
 */
void set_successor(list_node_t * node, list_node_t * newSuccessor);

/**
 * Remplace le prédécesseur du nœud \p node par \p newPredecessor.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newPredecessor
 */
void set_predecessor(list_node_t * node, list_node_t * newPredecessor);

/**
 * La liste est une structure contenant :
 * (+) une référence (\p head) sur son premier nœud,
 * (+) une référence (\p tail) sur son dernier nœud, et
 * (+) le nombre de ces nœuds (\p size).
 */
typedef struct list {
	list_node_t * head, * tail;
	unsigned int size;
} list_t;

/**
 * Construit et initialise une liste doublement chaînée vide.
 * 
 * @return la nouvelle liste créée
 */
list_t * new_list();

/**
 * Renvoie 1 si la liste \p L est vide, sinon renvoie 0.
 * 
 * @param[in] L 
 * @return vrai (1) si la liste est vide
 */
int list_is_empty(const list_t * L);

/**
 * Restitue la taille (nombre d'éléments) de la liste \p L.
 * 
 * @param[in] L 
 * @return la taille de la liste \p L
 */
unsigned int get_list_size(const list_t * L);

/**
 * Restitue la tête (premier élément) de la liste \p L.
 * 
 * @param[in] L 
 * @return la tête de la liste \p L
 */
list_node_t * get_list_head(const list_t * L);

/**
 * Restitue la queue (dernier élément) de la liste \p L.
 * 
 * @param[in] L 
 * @return la queue de la liste \p L
 */
list_node_t * get_list_tail(const list_t * L);

/**
 * Incrémente la taille de la liste \p L par 1.
 * 
 * @param[in] L 
 */
void increase_list_size(list_t * L);

/**
 * Décrémente la taille de la liste \p L par 1.
 * 
 * @param[in] L 
 */
void decrease_list_size(list_t * L);

/**
 * Met à jour la taille de la liste \p L à \p newSize.
 * 
 * @param[in] L 
 * @param[in] newSize 
 */
void set_list_size(list_t * L, unsigned int newSize);

/**
 * Remplace la tête de la liste \p L par \p newHead.
 * 
 * @param[in] L 
 * @param[in] newHead 
 */
void set_list_head(list_t * L, list_node_t * newHead);

/**
 * Remplace la queue de la liste \p L par \p newTail.
 * 
 * @param[in] L 
 * @param[in] newTail 
 */
void set_list_tail(list_t * L, list_node_t * newTail);

/**
 * Deux possibilités pour libérer la mémoire de la liste \p L :
 * (+) Si le paramètre \p freeData n'est pas NULL,
 *     alors le pointeur de fonction \p freeData
 *     va servir à supprimer les données (data) référencées par
 *     les nœuds de la liste \p L.
 * (+) Si le paramètre \p freeData est NULL,
 *     alors les données (data) référencées par les nœuds
 *     de la liste \p L ne sont pas supprimées.
 * Dans tous les cas, les nœuds de la liste et la liste elle même
 * sont supprimés.
 *
 * @param[in] L 
 * @param[in] freeData
 */
void delete_list(list_t * L, void (*freeData)(void *));

/**
 * Affiche les éléments de la liste \p L.
 * Les données de chaque nœud sont affichées grâce au pointeur
 * de fonction \p viewData.
 * 
 * @param[in] L 
 * @param[in] viewData
 */
void view_list(const list_t * L, void (*viewData)(const void *));

/**
 * Insère en tête de la liste \p L un nouveau nœud de donnée \p data.
 * 
 * @param[in] L 
 * @param[in] data
 */
void list_insert_first(list_t * L, void * data);

/**
 * Insère à la fin de la liste \p L un nouveau nœud de donnée \p data.
 * 
 * @param[in] L 
 * @param[in] data
 */
void list_insert_last(list_t * L, void * data);

/**
 * Insère un nouveau nœud de donnée \p data dans la liste \p L
 * après le nœud indiqué par le pointeur \p node.
 * Le pointeur \p node désigne obligatoirement un nœud de la liste \p L.
 * 
 * @param[in] L
 * @param[in] data
 * @param[in] node
 */
void list_insert_after(list_t * L, void * data, list_node_t * node);

/**
 * Supprime le premier nœud de la liste \p L et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * La liste \p L ne peut pas être vide.
 * 
 * @param[in] L
 * @return la donnée du nœud supprimé
 */
void * list_remove_first(list_t * L);

/**
 * Supprime le dernier nœud de la liste \p L et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * La liste \p L ne peut pas être vide.
 * 
 * @param[in] L
 * @return la donnée du nœud supprimé
 */
void * list_remove_last(list_t * L);

/**
 * Supprime le nœud de la liste \p L indiqué par le pointeur \p node
 * et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * La liste \p L ne peut pas être vide.
 * Le pointeur \p node désigne obligatoirement un nœud de la liste \p L.
 * 
 * @param[in] L
 * @param[in] node
 * @return la donnée du nœud supprimé
 */
void * list_remove_node(list_t * L, list_node_t * node);

/**
 * Permute les données des nœuds \p node1 et \p node2.
 * Les nœuds \p node1 et \p node2 ne peuvent pas être vides.
 *
 * @param[in] node1
 * @param[in] node2
 */
void list_swap_nodes_data(list_node_t * node1, list_node_t * node2);

/**
 * Renvoie 1 si la donnée \p data existe dans la liste \p L, sinon renvoie 0.
 *
 * @param[in] L
 * @param[in] data
 * @return vrai (1) si la donnée \p data existe dans la liste \p L
 */
int list_data_exist(const list_t * L, const void * data);

/**
 * Copie la liste (pas de duplication des données)
 *
 * @param original
 * @return la copie de la liste
 */
list_t *copy_list(const list_t *original);

/**
 * @brief Trie une liste doublement chaînée en ordre décroissant en utilisant le tri par fusion.
 *
 * @param list La liste à trier.
 * @param compare Fonction de comparaison. Doit retourner :
 *        - Une valeur positive si data1 > data2,
 *        - Une valeur négative si data1 < data2,
 *        - 0 si data1 == data2.
 */
void merge_sort_list(list_t *list, int (*compare)(const void *a, const void *b), void (*freeData)(void *));

/**
 * @brief Algorithme de tri rapide pour une liste chainée
 * @param list la liste a trier
 * @param compare Méthode à utiliser pour comparer les éléments de la liste
 */
void quicksort(list_t *list, int (*compare)(const void *a, const void *b), void (*viewData)(const void *));

/**
 * @brief Supprime de la liste les états identiques grâce à la fonction compare
 * @param list
 * @param compare
 */
void remove_duplicates(list_t *list, int (*compare)(const void *a, const void *b));
