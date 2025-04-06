#include "ReadMapper.h"
#include "FastqFileRreader.h"
#include "FastaParser.h"
#include "FormatFileDetector.h"
#include <iostream>
#include <iomanip>

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
    
    try {
        processFile(argv[1], argv[2], k, step);
    } catch (const std::exception& e) {
        std::cerr << "Erreur non gérée: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}