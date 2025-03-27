#include "FastaParser.h"
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

/**
 * Cette fonction prend une séquence validée (via `validateFasta()`) et retourne son complément inverse,
 * ères non-ADN/ARN sont rejetés par `validateFasta()` avant l'appel.
 * la structure BuildSequenceTable est une fonction lambda qui retourne un tableau de caractères de 128 éléments
 * qui contient les compléments de chaque base.
 *  Instance statique constexpr de la table générée par `buildComplementTable()`(permet de calculer la table lors de la compilation).
 * Optimisée pour :
 * - Accès O(1) via indexation ASCII
 * - Usage en mémoire read-only
 * static constexpr auto buildComplementTable = []() est avec une liste de capture vide []() càd une acces uniquement
 * aux variables statiques et globales.
 * et aucun parametre d'entrée
 * warning
 * Ne pas appeler cette fonction sur des séquences non validées (acides aminés ou données brutes).
 * ce code doit etre exécuter avec une version de c++ 20 ou supérieur
 */
std::string getReverseComplement(const std::string& sequence);


std::string FastaParser::getReverseComplement(const std::string& sequence) {
    SequenceType type = getSequenceType(sequence);
    if (type == SequenceType::AA || type == SequenceType::UNKNOWN) {
        std::cerr << "Erreur : La séquence n'est pas de type ADN ou ARN." << std::endl;
        return ""; // Retourne "N/A" pour les acides aminés et les séquences inconnues
    }
    // Complément de chaque base
    //la syntaxe pour covertire en majuscule est std::toupper(base)
    static constexpr auto buildComplementTable = []() {
        std::array<char, 128> table{};
        table['A'] = 'T'; table['T'] = 'A'; table['U'] = 'A';
        table['C'] = 'G'; table['G'] = 'C';
        table['R'] = 'Y'; table['Y'] = 'R';
        table['S'] = 'S'; table['W'] = 'W';
        table['K'] = 'M'; table['M'] = 'K';
        table['B'] = 'V'; table['D'] = 'H';
        table['H'] = 'D'; table['V'] = 'B';
        table['N'] = 'N';
        return table;
    };
    static constexpr auto complementTable = buildComplementTable();
    std::string complement ;
    complement.reserve(sequence.size()); // Réserve la mémoire pour la séquence complémentaire

    for (char base : sequence) {
        char upper = std::toupper(base);
        char comp = complementTable[static_cast<unsigned char>(upper)];
        complement += (isupper(base)) ? comp : std::tolower(comp);
    }

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



