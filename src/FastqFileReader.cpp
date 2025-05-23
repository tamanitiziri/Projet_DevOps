/*pour compiler:  g++ -std=c++17 fastaq.cpp sequenceParser.cpp fastqFileRreader.cpp FastaParser.cpp FormatFileDetector.cpp -o fastaq_tester
 *pour executer: ./fastaq_tester
 */


#include "FastqFileRreader.h"
#include "SequenceParser.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

FastqFileReader::FastqFileReader(const std::string& filePath) : filePath(filePath) {}

bool FastqFileReader::loadFile() {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Erreur : Impossible d'ouvrir le fichier " + filePath);
    }
    headers.clear();
    sequences.clear();
    qualityScores.clear();

    std::string line;
    while(std::getline(file, line)) {
        if(isSequenceStart(line)) {
            // Header
            headers.push_back(line);

            // Sequence
            
            if(!std::getline(file, line)) throw std::runtime_error("Séquence manquante");;
            sequences.push_back(line);

            //separator
            if (!std::getline(file, line)) throw std::runtime_error("Séparateur manquant");
            if (!isQualitySeparator(line)) throw std::runtime_error("Séparateur de la séquence qualité est invalide dans le sequence avec le header suivant: " + headers.back());
            
            // Quality
            if(!std::getline(file, line)) throw std::runtime_error("Séquence qualité manquante");
            qualityScores.push_back(line);
        }
       
    }
    isStreamMode = false;
    return true;
}

/**
 *cette fonction permet de valider le fichier
 *elle retourne un booleen true si le fichier est valide et false sinon
 *elle fonctionne en verifiant si le fichier est chargé
 *@retun et si le fichier est en mode stream ou non
 *et si le fichier est en mode stream elle retourne true
 *sinon elle retourne false
 *elle est de complexité temporelle O(1) et spatiale O(1)
 */

bool FastqFileReader::processSequences(
    const std::function<void(const std::string&,
                             const std::string&,
                              const std::string&)>& callback){
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::string header, sequence, separator, quality;
    while(std::getline(file, header)) {
        if(isSequenceStart(header)) {
            if(!std::getline(file, sequence)) break;
            std::getline(file, separator);
            if(!std::getline(file, quality)) break;

            callback(header, sequence, quality);
        }
    }
    isStreamMode = true;
    return true;
}

bool FastqFileReader::validate() const {
    std::string validDNA = "ACGTacgtRYKMSWBDHVNrykmswbdhvn";
    if (isStreamMode) {
        throw std::runtime_error("Validation impossible en mode stream.");
    }

    if (sequences.size() != qualityScores.size() || sequences.size() != headers.size()) {
        throw std::runtime_error("Nombre de séquences, headers et scores de qualité incohérents.");
    }

    bool isValid = true; // Indicateur global de validité

    for (size_t i = 0; i < sequences.size(); ++i) {
        // Vérifier les headers
        if (!isSequenceStart(headers[i])) {
            std::cerr << "Erreur : Header invalide à la ligne " << (i + 1) << ": " << headers[i] << std::endl;
            isValid = false;
            continue; // Passer à la séquence suivante
        }

        // Vérifier si la séquence de qualité est plus longue que la séquence moléculaire
        if (qualityScores[i].length() > sequences[i].length()) {
            qualityScores[i] = qualityScores[i].substr(0, sequences[i].length());
            std::cerr << "Warning: La séquence de qualité pour le header \"" 
                      << headers[i] 
                      << "\" a été tronquée pour correspondre à la longueur de la séquence moléculaire." 
                      << std::endl;
        } else if (qualityScores[i].length() < sequences[i].length()) {
            std::cerr << "Erreur : Séquence de qualité plus courte que la séquence moléculaire pour le header \"" 
                      << headers[i] << "\"" << std::endl;
            isValid = false;
            continue; // Passer à la séquence suivante
        }

        // Validation des caractères de la séquence moléculaire
        size_t invalidPos = sequences[i].find_first_not_of(validDNA);
        if (invalidPos != std::string::npos) {
            std::cerr << "Erreur : Caractère invalide dans la séquence avec le header \"" 
                      << headers[i] << "\" à la position " << (invalidPos + 1) 
                      << ": '" << sequences[i][invalidPos] << "'" << std::endl;
            isValid = false;
            continue; // Passer à la séquence suivante
        }

        // Validation des caractères de la séquence de qualité
        auto it = qualityScores[i].begin();
        for (; it != qualityScores[i].end(); ++it) {
            if (*it < '!' || *it > '~') break; // Caractère invalide trouvé
        }
        if (it != qualityScores[i].end()) {
            std::cerr << "Erreur : Score de qualité invalide pour le header \"" 
                      << headers[i] << "\" à la position " 
                      << (it - qualityScores[i].begin() + 1) 
                      << ": '" << *it << "' (ASCII " << static_cast<int>(*it) << ")" 
                      << std::endl;
            isValid = false;
        }
    }

    return isValid;
}

//getters

const std::vector<std::string>& FastqFileReader::getHeaders() const {
    if (isStreamMode) {
        throw std::runtime_error("Accès impossible en mode stream.");
    }
    return headers;
}

const std::vector<std::string>& FastqFileReader::getSequences() const {
    if (isStreamMode) {
        throw std::runtime_error("Accès impossible en mode stream.");
    }
    return sequences;
}

const std::vector<std::string>& FastqFileReader::getQualityScores() {
    if (isStreamMode) {
        throw std::runtime_error("Accès impossible en mode stream.");
    }
    return qualityScores;
}

size_t FastqFileReader::countSequences() const {
    if (isStreamMode) {
        throw std::runtime_error("Accès impossible en mode stream.");
    }
    return sequences.size();
}


//Méthode internes

bool FastqFileReader::isSequenceStart(const std::string& line) const {
    return !line.empty() && line[0] == '@';
}

bool FastqFileReader::isQualitySeparator(const std::string& line) const {
    if(line.empty()) return false;
    // soit exatement un '+' soit un '+' suivi de la meme sequence que le header
    return (line == "+") || (line[0] == '+' && line.substr(1) == headers.back().substr(1));    
}

void FastqFileReader::parseQualityScores(std::ifstream& file, size_t seqLength) {
    std::string quality;
    if (std::getline(file, quality) && quality.length() == seqLength) {
        qualityScores.push_back(quality);
    }
}







