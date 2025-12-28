#pragma once

/**
 * @brief Fonction générique qui calcule et retourne le max entre
 * deux éléments de type numérique quelconque
 */
#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; })

/**
 * @brief Fonction générique qui calcule et retourne le min entre
 * deux éléments de type numérique quelconque.
 */
#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

/**
 * @brief Affiche le message d'erreur \p msg et
 * interrompt l'exécution du programme si le paramètre \p interrupt vaut 1.
 *
 * @param[in] msg
 * @param[in] interrupt
 */
void ShowMessage(char * msg, int interrupt);

/**
 * @brief Affiche l'entier de l'adresse mémoire \p i.
 *
 * @param[in] i le type de l'argument est de void* et pas de int*
 *          afin d'éviter un "warning" dans la compilation.
 */
void viewInt(const void * i);

/**
 * @brief Libère la mémoire de l'entier indiqué par le pointeur \p i.
 *
 * @param[in] i le type de l'argument est de void* et pas de int*
 *          afin d'éviter un "warning" dans la compilation.
 */
void freeInt(void * i);

/**
 * @brief Compare les entiers \p a et \p b.
 *
 * Le type des argument est de void* et pas de int*
 * afin d'éviter un "warning" dans la compilation.
 *
 * @param[in] a
 * @param[in] b
 * @return int int 1 si a>b, sinon 0
 */
int intSmallerThan(const void * a, const void * b);

/**
 * @brief Compare les entiers \p a et \p b.
 *
 * Le type des argument est de void* et pas de int*
 * afin d'éviter un "warning" dans la compilation.
 *
 * @param[in] a
 * @param[in] b
 * @return int 1 si a>b, sinon 0
 */
int intGreaterThan(const void * a, const void * b);

/**
 * @brief Compare les entiers \p a et \p b.
 *
 * Le type des argument est de void* et pas de int*
 * afin d'éviter un "warning" dans la compilation.
 *
 * @param[in] a
 * @param[in] b
 * @return int 1 si a==b, sinon 0
 */
int intEqualTo(const void * a, const void * b);

/**
 * @brief Affiche une chaîne de caractères pointée par \p data.
 *
 * Utilisée comme fonction d'affichage pour les structures génériques
 * (ex. : Liste, Arbre, Tas, etc.).
 *
 * @param[in] data Pointeur vers la chaîne de caractères (char*)
 */
void view_string(const void *data);

/**
 * @brief Libère la mémoire allouée pour une chaîne de caractères.
 *
 * À utiliser comme fonction de libération dans les conteneurs génériques
 * quand les données sont allouées dynamiquement.
 *
 * @param[in,out] data Pointeur vers la chaîne à libérer
 */
void free_string(void *data);

/**
 * @brief Compare deux entiers pour un tri croissant.
 *
 * Fonction de comparaison compatible avec qsort(), les arbres AVL,
 * les tas, etc. Retourne une valeur négative, nulle ou positive.
 *
 * @param[in] a Premier entier (void*)
 * @param[in] b Deuxième entier (void*)
 * @return int < 0 si a < b, 0 si a == b, > 0 si a > b
 */
int compare_int_asc(const void *a, const void *b);

/**
 * @brief Compare deux entiers pour un tri décroissant.
 *
 * Identique à compare_int_asc mais dans l'ordre inverse.
 *
 * @param[in] a Premier entier (void*)
 * @param[in] b Deuxième entier (void*)
 * @return int < 0 si a > b, 0 si a == b, > 0 si a < b
 */
int compare_int_desc(const void *a, const void *b);

/**
 * @brief Compare deux unsigned long pour un tri croissant.
 *
 * Fonction de comparaison compatible avec qsort(), les arbres AVL,
 * les tas, etc. Retourne une valeur négative, nulle ou positive.
 *
 * @param[in] a Première valeur (void*)
 * @param[in] b Deuxième valeur (void*)
 * @return int < 0 si a < b, 0 si a == b, > 0 si a > b
 */
int compare_long_asc(const void *a, const void *b);

/**
 * @brief Compare deux unsigned long pour un tri décroissant.
 *
 * Identique à compare_long_asc mais dans l'ordre inverse.
 *
 * @param[in] a Première valeur (void*)
 * @param[in] b Deuxième valeur (void*)
 * @return int < 0 si a > b, 0 si a == b, > 0 si a < b
 */
int compare_long_desc(const void *a, const void *b);

/**
 * @brief Teste l'égalité entre deux entiers.
 *
 * Retourne 1 si les deux valeurs sont égales, 0 sinon.
 * Utilisée notamment comme fonction d'égalité dans les tables de hachage,
 * les ensembles, etc.
 *
 * @param[in] a Premier entier (void*)
 * @param[in] b Deuxième entier (void*)
 * @return int 1 si *a == *b, 0 sinon
 */
int int_equal(const void *a, const void *b);

/**
 * @brief Compare deux chaînes de caractères (ordre lexicographique).
 *
 * Compatible avec les conteneurs génériques (qsort, arbres, etc.).
 * Utilise strcmp() en interne.
 *
 * @param[in] a Pointeur vers la première chaîne (const char*)
 * @param[in] b Pointeur vers la deuxième chaîne (const char*)
 * @return int < 0 si a < b, 0 si a == b, > 0 si a > b
 */
int compare_string(const void *a, const void *b);