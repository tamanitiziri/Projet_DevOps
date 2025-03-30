//compiler: ++ -std=c++20 -o fasta fasta.cpp FastaParser.cpp sequenceParser.cpp fastqFileRreader.cpp FormatFileDetector.cpp
//execute: ./fasta
#include "FastaParser.h"
#include "fastqFileRreader.h"
#include "FormatFileDetector.h"
#include "SequenceParser.h"
#include <fstream>
#include <iostream>
#include <memory>

void testParser(const std::string& filePath, const std::string& description) {
    std::cout << "\n=== Test: " << description << " (" << filePath << ") ===\n";

    // Détection du format
    auto format = FormatFileDetector::detect(filePath);
    std::cout << "Format détecté: " << FormatFileDetector::formatToString(format) << "\n";

    try {
        std::unique_ptr<SequenceParser> parser;
        
        // Création du parser approprié
        switch(format) {
            case FormatFileDetector::FASTA:
                parser = std::make_unique<FastaParser>(filePath);
                break;
            case FormatFileDetector::FASTQ:
                parser = std::make_unique<FastqFileReader>(filePath);
                break;
            default:
                throw std::runtime_error("Format non supporté");
        }

        // Chargement et validation
        if (parser->loadFile() && parser->validate()) {
            std::cout << "Fichier valide.\n";
            std::cout << "Nombre de sequences: " << parser->countSequences() << "\n";

            // Affichage du type de séquence (première séquence)
            auto seq_type = parser->getSequenceType(parser->getSequences()[0]);
            switch (seq_type) {
                case SequenceParser::DNA:
                    std::cout << "Type: ADN\n"; break;
                case SequenceParser::RNA:
                    std::cout << "Type: ARN\n"; break;
                case SequenceParser::AA:
                    std::cout << "Type: Acides Aminés\n"; break;
                default:
                    std::cout << "Type: Inconnu\n";
            }

            // Affichage spécifique au format
            if (format == FormatFileDetector::FASTA) {
                auto* fasta_parser = dynamic_cast<FastaParser*>(parser.get());
                auto sizes = fasta_parser->getSequenceSizes();
                for (size_t i = 0; i < sizes.size(); ++i) {
                    std::cout << "Sequence " << i+1 << " taille: " << sizes[i] << "\n";
                }
            }

            // Complément inverse
            auto sequences = parser->getSequences();
            for (size_t i = 0; i < sequences.size(); ++i) {
                std::cout << "Sequence " << i+1 << " complement inverse:\n"
                          << parser->getReverseComplement(sequences[i]) << "\n\n";
            }
        } else {
            std::cout << "Fichier invalide.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "ERREUR: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    // Tests FASTA
    testParser("test1.fasta", "FASTA ADN simple");
    testParser("test2.fasta", "Multi-FASTA");
    testParser("test5.fasta", "FASTA ARN");
    // Test 4: Fichier avec caractères invalides
    testParser("test4.fasta", "Caractères invalides");
    
    // Test 5: Fichier ARN valide
    testParser("test5.fasta", "FASTA ARN valide");
    
    // Test 6: Fichier acides aminés valide
    testParser("test6.fasta", "FASTA Acides Aminés valide");
    
    return 0;
}