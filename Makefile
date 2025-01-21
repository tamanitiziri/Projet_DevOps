# Définition des variables
CXX = g++                # Le compilateur C++
CXXFLAGS = -std=c++17 -Wall -I./include # Les options de compilation (C++17 et tous les avertissements)
LDFLAGS =                 # Options de linkage
SRC = src/main.cpp src/FastaParser.cpp # Les fichiers source dans le dossier src/
OBJ = $(SRC:.cpp=.o)      # Les fichiers objets générés (.o)
EXEC = fasta_api          # Nom du programme exécutable

# Règle pour compiler le programme
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Règle pour compiler les fichiers source en objets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ) $(EXEC)
