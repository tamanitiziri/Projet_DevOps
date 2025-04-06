/* ce fichier est un exemple de code pour mapper des séquences de reads
 * sur une référence génomique en utilisant un algorithme de k-mer.
 * Il inclut la gestion des fichiers FASTA et FASTQ, ainsi que l'analyse des résultats.
 * pour compiler ce code, utilisez la commande suivante :
 * g++ -std=c++20 -o mapper mapper.cpp ReadMapper.cpp KmerIndex.cpp SuffixArray.cpp FastqFileReader.cpp FastaParser.cpp SequenceParser.cpp FormatFileDetector.cpp
 * pour exécuter le code, utilisez la commande suivante :
 * ./mapper reference.fasta reads.fastq [k=21] [step=1]
*/
#include "ReadMapper.h"
#include "FastqFileRreader.h"
#include "FastaParser.h"
#include "FormatFileDetector.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>

void explainCIGAR() {
    // Dictionnaire des codes CIGAR
    std::unordered_map<char, std::string> cigar_codes = {
        {'M', "Match/Mismatch: Alignement (peut être un match OU un mismatch)"},
        {'I', "Insertion: Base(s) présente(s) dans la séquence mais PAS dans la référence"},
        {'D', "Délétion: Base(s) présente(s) dans la référence mais PAS dans la séquence"},
        {'N', "Saut (Skipped): Région de la référence ignorée (ex: intron en RNA-seq)"},
        {'S', "Soft Clipping: Bases tronquées aux extrémités (conservées dans la séquence)"},
        {'H', "Hard Clipping: Bases supprimées (non stockées dans le fichier BAM/SAM)"},
        {'P', "Padding: Espace réservé pour alignement structural (rare)"},
        {'=', "Match Exact: Base identique à la référence"},
        {'X', "Mismatch Explicite: Base différente de la référence"}
    };

    // Affichage explicatif
    std::cout << "\n=== Guide des Codes CIGAR ===" << std::endl;
    std::cout << "Chaque lettre décrit une opération d'alignement :\n" << std::endl;

    for (const auto& entry : cigar_codes) {
        std::cout << "  * '" << entry.first << "' : " << entry.second << std::endl;
    }

    std::cout << "\n=== Exemples d'Utilisation ===" << std::endl;
    std::cout << "  - '30M2I5D' : 30 bases alignées → 2 insertions → 5 délétions" << std::endl;
    
}

void analyzeMapping(const MappingResult& result, const std::string& readId) {
    std::cout << "\n=== Résultat pour " << readId << " ===\n";
    std::cout << "Position: " << result.referencePos << " | Brin: " 
              << (result.strand == Strand::FORWARD ? "Forward" : "Reverse") << "\n";
    std::cout << "Confidence: " << std::fixed << std::setprecision(2) << result.confidence * 100 << "%\n";
    std::cout << "Distance d'édition: " << result.editDistance << "\n";
    std::cout << "CIGAR: " << result.cigarString << "\n";
    std::cout << "Mapping unique: " << (result.isUnique ? "Oui" : "Non") << "\n";
}

void processFile(const std::string& refFile, const std::string& readFile, int k, int step) {
    try {
        // Chargement référence
        FastaParser refParser(refFile);
        if (!refParser.loadFile() || !refParser.validate()) {
            throw std::runtime_error("Erreur référence FASTA");
        }
        std::string reference = refParser.getSequences()[0];
        
        // Initialisation mapper
        ReadMapper mapper(reference, k, step);
        
        // Détection format reads
        FormatFileDetector detector;
        auto format = detector.detect(readFile);
        
        if (format == FormatFileDetector::FASTQ) {
            FastqFileReader reader(readFile);
            reader.processSequences([&](const std::string& header, 
                                      const std::string& seq, 
                                      const std::string& qual) {
                auto result = mapper.mapRead(seq);
                analyzeMapping(result, header.substr(0, header.find(' ')));
            });
        } else if (format == FormatFileDetector::FASTA) {
            FastaParser parser(readFile);
            parser.processSequences([&](const std::string& header, 
                                       const std::string& seq) {
                auto result = mapper.mapRead(seq);
                analyzeMapping(result, header.substr(0, header.find(' ')));
            });
        } else {
            throw std::runtime_error("Format de fichier non supporté");
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <reference.fasta> <reads.(fastq|fasta)> [k=21] [step=1]\n";
        std::cout <<"Exemple d'éxécusion  : ./executable genome.fasta reads.fastq taille_kmer pas \n" << std::endl;
        return 1;
    }
    
    int k = argc > 3 ? std::stoi(argv[3]) : 21;
    int step = argc > 4 ? std::stoi(argv[4]) : 1;

    // Validation des paramètres
    if (k <= 0 || step <= 0) {
        std::cerr << "Erreur: k et step doivent être > 0\n";
        return 1;
    }
    
    std::cout << "Paramètres:\n";
    std::cout << " - Taille k-mer: " << k << "\n";
    std::cout << " - Pas: " << step << "\n\n";

    explainCIGAR();
    
    try {
        processFile(argv[1], argv[2], k, step);
    } catch (const std::exception& e) {
        std::cerr << "Erreur non gérée: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}