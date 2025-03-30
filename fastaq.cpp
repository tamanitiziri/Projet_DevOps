
#include "FastaParser.h"
#include "fastqFileRreader.h"
#include "FormatFileDetector.h"
#include "SequenceParser.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

// Génère des fichiers FASTQ/FASTA de test
void generateTestFiles() {
    // Fichier FASTQ valide
    std::ofstream valid_fastq("valid.fq");
    valid_fastq << "@SEQ1\nACGT\n+\nIIII\n@SEQ2\nTGCA\n+\nFFFF\n";
    valid_fastq.close();

    // Fichier FASTQ avec séparateur manquant
    std::ofstream missing_sep("missing_sep.fq");
    missing_sep << "@SEQ1\nACGT\nXXXX\nIIII\n";
    missing_sep.close();

    // Fichier FASTQ avec qualité tronquée
    std::ofstream bad_qual("bad_qual.fq");
    bad_qual << "@SEQ1\nACGT\n+\nIII\n";  // Manque 1 score
    bad_qual.close();

    // Fichier FASTA valide
    std::ofstream valid_fasta("valid.fa");
    valid_fasta << ">SEQ1\nACGT\n>SEQ2\nTGCA\n";
    valid_fasta.close();

    // Fichier vide
    std::ofstream empty("empty.fq");
    empty.close();

    // Gros fichier (1M séquences)
    std::ofstream big_file("big.fq");
    for (int i = 0; i < 1000000; ++i) {
        big_file << "@SEQ" << i << "\nACGT\n+\nIIII\n";
    }
    big_file.close();
}

// Teste un fichier avec le bon parser
void testFile(const std::string& filename) {
    std::cout << "\n=== Testing: " << filename << " ===\n";

    auto format = FormatFileDetector::detect(filename);
    std::cout << "Detected format: " << FormatFileDetector::formatToString(format) << "\n";

    try {
        if (format == FormatFileDetector::FASTQ) {
            FastqFileReader reader(filename);
            
            // Test mode chargement complet
            if (fs::file_size(filename) < 1000000) {  // Si < 1MB
                std::cout << "-- Testing loadFile() --\n";
                if (reader.loadFile()) {
                    std::cout << "Loaded " << reader.countSequences() << " sequences\n";
                    if (reader.validate()) {
                        std::cout << "Validation PASSED\n";
                    }
                }
            }
            
            // Test mode stream
            std::cout << "-- Testing processSequences() --\n";
            size_t count = 0;
            reader.processSequences([&count](auto&&...) { count++; });
            std::cout << "Streamed " << count << " sequences\n";
            
        } else if (format == FormatFileDetector::FASTA) {
            FastaParser parser(filename);
            if (parser.loadFile()) {
                std::cout << "Loaded " << parser.countSequences() << " FASTA sequences\n";
            }
        } else {
            std::cout << "Unknown format - skipping\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
    }
}

int main() {
    // Génère les fichiers de test
    generateTestFiles();

    // Liste des fichiers à tester
    const std::vector<std::string> test_files = {
        "valid.fq",
        "missing_sep.fq",
        "bad_qual.fq",
        "valid.fa", 
        "empty.fq",
        "big.fq",
        "nonexistent.fq"  // Test fichier manquant
    };

    // Exécute les tests
    for (const auto& file : test_files) {
        testFile(file);
    }

    // Nettoyage (optionnel)
    for (const auto& file : test_files) {
        if (fs::exists(file)) fs::remove(file);
    }

    return 0;
}