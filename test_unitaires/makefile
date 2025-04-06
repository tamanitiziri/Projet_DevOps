# Compilateur et options
CXX := g++
CXXFLAGS := -std=c++20 -Wall -I../include -fopenmp
LDFLAGS := -fopenmp

# Répertoires
SRC_DIR := ../src
BUILD_DIR := build
TEST_DIR := .

# Fichiers sources principaux
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Règle par défaut
all: $(BUILD_DIR)
	@echo "Usage: make test file=mon_test.cpp [args=\"param1 param2\"]"

# Création du répertoire build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilation des fichiers sources principaux
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour compiler et exécuter un test spécifique
test: $(BUILD_DIR) $(OBJS)
	@if [ -z "$(file)" ]; then \
		echo "Erreur: Vous devez spécifier un fichier de test"; \
		echo "Exemple: make test file=mon_test.cpp args=\"param1 param2\""; \
		exit 1; \
	fi
	$(eval TEST_NAME := $(basename $(notdir $(file))))
	$(eval TEST_EXEC := $(BUILD_DIR)/$(TEST_NAME))
	$(eval TEST_OBJ := $(BUILD_DIR)/$(TEST_NAME).o)
	
	@echo "Compilation de $(file)..."
	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/$(file) -o $(TEST_OBJ)
	
	@echo "Édition des liens..."
	$(CXX) $(CXXFLAGS) $(OBJS) $(TEST_OBJ) -o $(TEST_EXEC) $(LDFLAGS)
	
	@echo "Exécution du test..."
	@if [ -z "$(args)" ]; then \
		$(TEST_EXEC); \
	else \
		$(TEST_EXEC) $(args); \
	fi

# Nettoyage
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean test