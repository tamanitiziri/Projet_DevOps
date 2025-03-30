#include "FastaParser.h"
#include "SequenceParser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <array>

FastaParser::FastaParser(const std::string& filePath) : filePath(filePath) {}

bool FastaParser::loadFile(){
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return false;
    }
   
    std::string line, currentSequence;
    bool expectingHeader = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '>' || line[0] == ';') { // ligne d'entête de séquence
           
            if (!currentSequence.empty()) {
                sequences.push_back(currentSequence);
                currentSequence.clear();
            }
            headers.push_back(line);
            expectingHeader = false;
        } else {
            if(expectingHeader){
                std::cerr << "Erreur: Le fichier ne commence pas par un header (> ou ;)" << std::endl;
                return false;
            }
            currentSequence += line; // Ajoute la ligne de séquence à la séquence courante
        }
    }
    if (!currentSequence.empty()) {
        sequences.push_back(currentSequence); // Ajoute la dernière séquence
    }
    file.close();
    return true;
    // Affiche toutes les séquences lues
    //for (const auto& seq : sequences) {
    //    std::cout << "Séquence : " << seq << std::endl;  // Affiche la séquence lue
    //}
}

bool FastaParser::validate() const {
 // 1. Vérifier que les données sont chargées
 if (sequences.empty() || headers.empty()) {
    throw std::runtime_error("Data not loaded. Call loadFile() first.");
    return false;  //chargement des donneés des donner a echoué
 }
 else{
    for (const auto& header : headers) {
        if (header.empty() || (header[0] != '>' && header[0] != ';')) {
            std::cerr << "Header invalide : " << header << std::endl;
            return false;
        }
    }
    // Définir les caractères valides (ACGT + ambiguïtés).
    //const std::string validChars = "ACGTacgtRYKMSWBDHVNrykmswbdhvn";
    const std::string validDNA = "ACGTacgtRYKMSWBDHVNrykmswbdhvn";
    const std::string validRNA = "ACGUacguRYKMSWBDHVNrykmswbdhvn";
    const std::string validAA = "ACDEFGHIKLMNPQRSTVWYacdefghiklmnpqrstvwy";

    for (size_t lineNum = 0; lineNum < sequences.size(); ++lineNum) {
        SequenceType type = getSequenceType(sequences[lineNum]);
        if(type == SequenceType::UNKNOWN){

         //const std::string& sequence = sequences[lineNum];

         for (size_t pos = 0; pos < sequences[lineNum].length(); ++pos) {
                char c = sequences[lineNum][pos]; //acces direct à ma variable original
                if (validDNA.find(c) == std::string::npos && 
                    validRNA.find(c) == std::string::npos && 
                    validAA.find(c) == std::string::npos) {
                         std::cerr << "Caractère invalide '" << c << "' dans la séquence " 
                                    << lineNum + 1 << " position " << pos + 1 << std::endl;
                    return false; // Retourne false si un caractère invalide est trouvé.
                 }

             }
        
        }
    }
 }
    return true; // Retourne true si le fichier FASTA est valide.
}
// la taille des séquences

std::vector<size_t> FastaParser::getSequenceSizes() const {
    std::vector<size_t> sizes;
    for (const auto& sequence : sequences) {
        sizes.push_back(sequence.size());
    }
    return sizes;
}



