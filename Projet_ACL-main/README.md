# Projet Graphes

Implémentation d'un graphe générique en C++ comme décrit dans le PDF.

## Structure
- `include/` : Headers (PElement.h, AElement.h, etc.).
- `src/` : Tests (test_AElement.cpp, TestGrapheGeneral.cpp, etc.).
- `bin/` : Exécutables générés.

## Commandes Make
| Commande                      | Description                   | Exemple                               |
|-------------------------------|-------------------------------|---------------------------------------|
| `make`                        | Compile tous les tests.       | `make`                                |
| `make bin/nom`                | Compile un test spécifique.   | `make bin/test_Arete`                 |
| `make debug`                  | Recompile en mode debug.      | `make debug`                          |
| `make run prog=nom`           | Exécute un test.              | `make run prog=TestGrapheGeneral`     |
| `make valgrind prog=nom`      | Valgrind complet sur un test. | `make valgrind prog=test_Graphe`      |
| `make valgrind-lite prog=nom` | Valgrind léger.               | `make valgrind-lite prog=test_Arete`  |
| `make clean`                  | Nettoie les binaires.         | `make clean`                          |

Lancer les tests un par un pour valider chaque classe, puis le global.

# Partie java
Si exécutée avec le script, changer le chemin en `REGION_DIR = "Partie_ACL/Assets/Region/"` dans `RegionUtils`.