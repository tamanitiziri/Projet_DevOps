#include "FastaParser.h"
#include <fstream>
#include <iostream>
#include <algorithm>

FastaParser::FastaParser(const std::string& filePath) : filePath(filePath) {}

bool FastaParser::loadFile() {
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return false;
    }
   
    std::string line, currentSequence;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '>' || line[0] == ';') { // ligne d'entête de séquence
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
    return true;
    // Affiche toutes les séquences lues
    //for (const auto& seq : sequences) {
    //    std::cout << "Séquence : " << seq << std::endl;  // Affiche la séquence lue
    //}
}

bool FastaParser::validateFasta() {
 if(!loadFile()) {
        return false; // Retourne false si le chargement du fichier échoue
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
   



//le type de ma sequence (adn , arn , aa, unknown) en utilisant enummeration
FastaParser::SequenceType FastaParser::getSequenceType(const std::string& sequence) {
    // Définir les caractères valides pour l'ADN, l'ARN et les acides aminés + code d'ambiguïté.
    const std::string validDNA = "ACGTacgtRYKMSWBDHVNrykmswbdhvn";
    const std::string validRNA = "ACGUacguRYKMSWBDHVNrykmswbdhvn";
    const std::string validAA = "ACDEFGHIKLMNPQRSTVWYacdefghiklmnpqrstvwy";

    // Vérifier si la séquence est de type ADN.
    if (sequence.find_first_not_of(validDNA) == std::string::npos) {
        return SequenceType::DNA;
    }
    
    // Vérifier si la séquence est de type ARN.
    if (sequence.find_first_not_of(validRNA) == std::string::npos) {
        return SequenceType::RNA;
    }

    // Vérifier si la séquence est de type acides aminés.
    if (sequence.find_first_not_of(validAA) == std::string::npos) {
        return SequenceType::AA;
    }

    // Si aucun des types ci-dessus n'est trouvé, retourner le type inconnu.
    return SequenceType::UNKNOWN;
}



size_t FastaParser::countSequences() {
        return sequences.size(); // Retourne le nombre de séquences
}

//std::vector<std::string> FastaParser::getSequences() const {
//    return sequences;
//}
const std::vector<std::string>& FastaParser::getSequences() const {
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

// la taille des séquences

std::vector<size_t> FastaParser::getSequenceSizes() const {
    std::vector<size_t> sizes;
    for (const auto& sequence : sequences) {
        sizes.push_back(sequence.size());
    }
    return sizes;
}



