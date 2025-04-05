/*pour compiler:  g++ -std=c++17 fastaq.cpp sequenceParser.cpp fastqFileRreader.cpp FastaParser.cpp FormatFileDetector.cpp -o fastaq_tester
 *pour executer: ./fastaq_tester
 */


#include "fastqFileRreader.h"
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
            if(!std::getline(file, line)) throw std::runtime_error("Qualité manquante");
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
        //return false;
    }
    //si le fichier est chargé
    if (sequences.size() != qualityScores.size() || sequences.size() != headers.size()) {
        throw std::runtime_error("Nombre de séquences, headers et scores de qualité incohérents.");
        return false;
    }
    // dans ce cas je veut si la sequence de qualité et plusgrande que la sequence moleculaire
    // couper la sequence de qualité et de renvoyer la sequence de qualité de la meme taille que la sequence moleculaire
    for(size_t i = 0; i < sequences.size(); ++i) {
        //1 verifier les headers
        if (!isSequenceStart(headers[i])) {
            // Afficher un avertissement avec le header correspondant
           throw std::runtime_error("Header invalide : (ligne " + std::to_string(i + 1) + "): " + headers[i]);
            
        }

        //3. Vérifier si la séquence de qualité est plus longue que la séquence moléculaire
        if (qualityScores[i].length() > sequences[i].length()) {
            // Couper la séquence de qualité
            qualityScores[i] = qualityScores[i].substr(0, sequences[i].length());

            // Afficher un avertissement avec le header correspondant
            std::cerr << "Warning: La séquence de qualité pour le header \"" 
                      << headers[i] 
                      << "\" a été tronquée pour correspondre à la longueur de la séquence moléculaire." 
                      << std::endl;
            return true;
        }
        else if (qualityScores[i].length() < sequences[i].length()) {
            throw std::runtime_error("Séquence de qualité est plus petite que la séquence moléculaire" + std::to_string(i+1)+ " dont le header \"" + headers[i] + "\"");
        }

        //4 validation des caracteres de la sequence moleculaire
        if(getSequenceType(sequences[i]) == SequenceType::UNKNOWN) {
            // Afficher un avertissement avec le header correspondant et la position du caractère invalide
            throw std::runtime_error("Caractère invalide dans la séquence avec le header \"" + headers[i] + "\"" "à la position " + std::to_string(sequences[i].find_first_not_of(validDNA))); 
            return false;
        }

        // Vérification des caractères de la séquence de qualité
        
        auto it = qualityScores[i].begin();
        for (; it != qualityScores[i].end(); ++it) {
            if (*it < '!' || *it > '~') break;  // Caractère invalide trouvé
        }
        if (it != qualityScores[i].end()) {
            throw std::runtime_error(
                "Score de qualité invalide (header '" + headers[i] + "')\n"
                "→ Caractère fautif: '" + std::string(1, *it) 
                + "' (ASCII " + std::to_string(static_cast<int>(*it)) 
                + ") à la position " + std::to_string(it - qualityScores[i].begin() + 1)
            );
        }
    }
    return true;
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







