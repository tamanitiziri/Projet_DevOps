# Compilateur et options
CXX = g++
CXXFLAGS = -std=c++20 -Wall -I./include -fopenmp
LDFLAGS = -fopenmp

# Répertoires
SRC_DIR = src
BUILD_DIR = build
DATA_DIR = data

# Fichiers sources
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
EXEC = mapper

# Règle par défaut
all: $(BUILD_DIR) $(EXEC)

# Création du répertoire build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Règle générique pour la compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Lien final
$(EXEC): $(OBJS) tests/mapper.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Nettoyage
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Exécution avec paramètres
run:
	@if [ -z "$(ref)" ] || [ -z "$(reads)" ]; then \
		echo "Usage: make run ref=<fichier_reference.fasta> reads=<fichier_reads.fastq> k=<taille_kmer> step=<pas>"; \
		exit 1; \
	fi
	./$(EXEC) $(DATA_DIR)/$(ref) $(DATA_DIR)/$(reads) $(k) $(step)

.PHONY: all clean run