//g++ -std=c++20 -o fasta fasta.cpp FastaParser.cpp sequenceParser.cpp fastqFileRreader.cpp FormatFileDetector.cpp
//./fasta
#include "FastaParser.h"
#include "SequenceParser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <array>

FastaParser::FastaParser(const std::string& filePath) : filePath(filePath) {}

bool FastaParser::loadFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return false;
    }

    headers.clear();
    sequences.clear();
    isStreamMode = false;

    std::string line, currentSequence;
    bool expectingHeader = true;
    size_t spaceWarnings = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] == '>' || line[0] == ';') {
            // Gestion du header
            if (!currentSequence.empty()) {
                sequences.push_back(currentSequence);
                currentSequence.clear();
            }
            headers.push_back(line);
            expectingHeader = false;
        } else {
            if (expectingHeader) {
                std::cerr << "Erreur: Le fichier ne commence pas par un header (> ou ;)" << std::endl;
                return false;
            }

            // Suppression des espaces dans la ligne de séquence
            std::string cleanLine;
            for (char c : line) {
                if (!isspace(c)) {
                    cleanLine += c;
                } else if (spaceWarnings < 5) { // Limite les avertissements
                    spaceWarnings++;
                    std::cerr << "Avertissement : Espace ignoré dans la séquence (header: " 
                              << headers.back() << ")\n";
                }
            }
            currentSequence += cleanLine;
        }
    }

    if (!currentSequence.empty()) {
        sequences.push_back(currentSequence);
    }

    if (spaceWarnings >= 5) {
        std::cerr << "..." << spaceWarnings - 5 << " avertissements supplémentaires sur les espaces\n";
    }

    file.close();
    return true;
}

bool FastaParser::processSequences(
    const std::function<void(const std::string& header,
                             const std::string& sequence)>& callback) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return false;
    }
    
    isStreamMode = true;
    std::string line, currentSequence, currentHeader;
    bool inSequence = false;
    size_t spaceWarnings = 0;

    while(std::getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] == '>' || line[0] == ';') {
            if (inSequence) {
                callback(currentHeader, currentSequence);
                currentSequence.clear();
            }
            currentHeader = line;
            inSequence = true;
        } else if (inSequence) {
            // Nettoyage des espaces en streaming
            for (char c : line) {
                if (!isspace(c)) {
                    currentSequence += c;
                } else if (spaceWarnings < 5) {
                    spaceWarnings++;
                    std::cerr << "Avertissement : Espace ignoré dans la séquence (header: " 
                              << currentHeader << ")\n";
                }
            }
        } else {
            std::cerr << "Erreur: Le fichier ne commence pas par un header (> ou ;)" << std::endl;
            file.close();
            return false;
        }
    }

    if(inSequence && !currentSequence.empty()){
        callback(currentHeader, currentSequence);
    }

    if (spaceWarnings >= 5) {
        std::cerr << "..." << spaceWarnings - 5 << " avertissements supplémentaires sur les espaces\n";
    }

    file.close();
    return true;
}



bool FastaParser::validate() const {
 // 1. Vérifier que les données sont chargées
 if (sequences.empty() || headers.empty()) {
    throw std::runtime_error("Pas de données chargées. Veuillez d'abord charger le fichier. verifier que le contiens une entête et une séquence");
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



