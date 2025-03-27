#include "FastaParser.h"
#include <iostream>

void testFastaParser(const std::string& filePath, const std::string& description) {
    std::cout << "\n=== Test: " << description << " (" << filePath << ") ===\n";
    
    FastaParser parser(filePath);
    
    if (parser.validateFasta()) {
        std::cout << "Fichier FASTA valide.\n";
        std::cout << "Nombre de sequences: " << parser.countSequences() << "\n";
        
        auto sizes = parser.getSequenceSizes();
        for (size_t i = 0; i < sizes.size(); ++i) {
            std::cout << "Sequence " << i+1 << " taille: " << sizes[i] << "\n";
        }
        
        auto sequences = parser.getSequences();
        for (size_t i = 0; i < sequences.size(); ++i) {
            std::cout << "Sequence " << i+1 << " complement inverse:\n"
                      << parser.getReverseComplement(sequences[i]) << "\n\n";
        }
    } else {
        std::cout << "Fichier FASTA invalide.\n";
    }
}

int main(int argc, char* argv[]) {
    // Test 1: Fichier FASTA simple valide (ADN)
    testFastaParser("test1.fasta", "FASTA ADN valide simple");
    
    // Test 2: Fichier multi-FASTA valide
    testFastaParser("test2.fasta", "Multi-FASTA valide");
    
    // Test 3: Fichier avec header invalide
    testFastaParser("test3.fasta", "Header invalide");
    
    // Test 4: Fichier avec caractères invalides
    testFastaParser("test4.fasta", "Caractères invalides");
    
    // Test 5: Fichier ARN valide
    testFastaParser("test5.fasta", "FASTA ARN valide");
    
    // Test 6: Fichier acides aminés valide
    testFastaParser("test6.fasta", "FASTA Acides Aminés valide");
    
    return 0;
}