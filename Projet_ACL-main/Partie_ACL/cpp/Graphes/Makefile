# Makefile pour projet Graphes - compilation multi-programmes avec tests

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
DEBUG_FLAGS = -g -O0

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Création des dossiers
$(shell mkdir -p $(BIN_DIR))

# Liste tous les .cpp dans src/
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Pour chaque source, un exécutable correspondant dans bin/
EXECUTABLES = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SOURCES))

# Compilation normale de tous
all: $(EXECUTABLES)

# Compilation d'un seul fichier (ex: make bin/test_AElement)
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Debug mode (ajoute -g -O0 pour Valgrind)
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Exécuter un programme spécifique
run:
	@if [ -z "$(prog)" ]; then \
		echo "Utilisation : make run prog=nom_du_fichier_sans_.cpp"; \
		echo "Exemple : make run prog=TestGElement"; \
		exit 1; \
	fi
	@if [ ! -f "$(BIN_DIR)/$(prog)" ]; then \
		echo "Erreur : programme $(prog) non compilé. Faites 'make' ou 'make $(BIN_DIR)/$(prog)' d'abord."; \
		exit 1; \
	fi
	./$(BIN_DIR)/$(prog)

# Valgrind sur un programme spécifique (mode complet)
valgrind: debug
	@if [ -z "$(prog)" ]; then \
		echo "Utilisation : make valgrind prog=nom_du_fichier_sans_.cpp"; \
		exit 1; \
	fi
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_DIR)/$(prog)

# Valgrind lite (moins verbeux)
valgrind-lite: debug
	@if [ -z "$(prog)" ]; then \
		echo "Utilisation : make valgrind-lite prog=nom_du_fichier_sans_.cpp"; \
		exit 1; \
	fi
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN_DIR)/$(prog)

# Nettoyage
clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all debug run valgrind valgrind-lite clean