/* ce fichier est consus pour tester la classe SuffixArray
 * il utilise la classe SuffixArray pour rechercher des motifs dans une séquence d'ADN
 *pour compiler: g++ -std=c++20 main.cpp SuffixArray.cpp -o suffix_array_tester
    *pour executer: ./suffix_array_tester
 *
*/

#include <iostream>
#include "SuffixArray.h"
#include <chrono>  

int main() {
    try {
        std::string genome = "ATCGAATCGTCGATCGTGTATAATCGTATGTAGTACTACGTATCGACGTACATCGATCG";
        SuffixArray sa(genome);

        std::string pattern = "ATCGA";

        // Test de recherche
        if (sa.search(pattern)) {
            std::cout << "Motif trouvé avec search()!" << std::endl;
        } else {
            std::cout << "Motif non trouvé avec search()." << std::endl;
        }

        // Test de countOccurrences
        size_t count = sa.countOccurrences(pattern);
        std::cout << "Nombre d'occurrences avec countOccurrences(): " << count << std::endl;

        // Test de getFactor
        try {
            size_t i = 5;
            size_t k = 10;
            std::string factor = sa.getFactor(i, k);
            std::cout << "Facteur en position " << i << ": " << factor << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Erreur getFactor: " << e.what() << std::endl;
        }

        // Test de findOccurrences
        std::vector<size_t> occurrences = sa.findOccurrences(pattern);
        std::cout << "Occurrences de " << pattern << ": ";
        for (size_t pos : occurrences) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;

        

    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}