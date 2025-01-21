#include "FastaParser.h"
#include <fstream>
#include <iostream>
#include <algorithm>

FastaParser::FastaParser(const std::string& filePath) : filePath(filePath) {}

void FastaParser::loadFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return;
    }

    std::string line, currentSequence;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '>') { // ligne d'entête de séquence
            headers.push_back(line);
            if (!currentSequence.empty()) {
                sequences.push_back(currentSequence);
                currentSequence.clear();
            }
        } else {
            currentSequence += line; // Ajoute la ligne de séquence à la séquence courante
        }
    }
    if (!currentSequence.empty()) {
        sequences.push_back(currentSequence); // Ajoute la dernière séquence
    }
    file.close();

    // Affiche toutes les séquences lues
    for (const auto& seq : sequences) {
        std::cout << "Séquence : " << seq << std::endl;  // Affiche la séquence lue
    }
}

bool FastaParser::validateFasta() {
    loadFile();
    for (const auto& sequence : sequences) {
        if (sequence.find_first_not_of("ACGTacgt") != std::string::npos) {
            std::cerr << "Caractère invalide trouvé." << std::endl;
            return false; // Retourne false si un caractère invalide est trouvé
        }
    }
    return true; // Retourne true si toutes les séquences sont valides
}

size_t FastaParser::countSequences() {
        return sequences.size(); // Retourne le nombre de séquences
}

std::vector<std::string> FastaParser::getSequences() const {
    return sequences;
}


std::string FastaParser::getReverseComplement(const std::string& sequence) {
    // Complément de chaque base
    std::string complement = "";
    for (char base : sequence) 
       {
        if (base == 'A') {
            complement += 'T';
        } else if (base == 'T') {
            complement += 'A';
        } else if (base == 'C') {
            complement += 'G';
        } else if (base == 'G') {
            complement += 'C';
        } else {
            // Gérer les caractères non valides (si nécessaire)
            complement += 'N';  // N pour un base inconnue
        }
    }
    
    // Récupère toutes les séquences du fichier
        //std::vector<std::string> sequences = parser.getSequences();
    

    // Inverser la séquence complémentaire
    std::reverse(complement.begin(), complement.end());
    
    return complement;
}





