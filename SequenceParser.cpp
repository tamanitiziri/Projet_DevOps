#include "SequenceParser.h"
#include<algorithm>
#include <vector>
#include <iostream>
#include <array>

//le type de ma sequence (adn , arn , aa, unknown) en utilisant enummeration

SequenceParser::SequenceType SequenceParser::getSequenceType(const std::string& sequence) {
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



size_t SequenceParser::countSequences() const {
        return sequences.size(); // Retourne le nombre de séquences
}

//std::vector<std::string> FastaParser::getSequences() const {
//    return sequences;
//}
const std::vector<std::string>& SequenceParser::getSequences() const {
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


std::string SequenceParser::getReverseComplement(const std::string& sequence) {
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
