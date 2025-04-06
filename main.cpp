
#include <iostream>
#include "SuffixArray.h"
#include <chrono>  // Pour mesurer le temps d'exécution
/*
// Fonction pour exécuter des tests unitaires
void runTests() {
    // Créer une instance de SuffixArray avec la chaîne "banana$"
    //les suffixes sont : banana, anana, nana, ana, na, a, $
    //ordre lexicographique : $, a, ana, anana, banana, na, nana
    SuffixArray sa("doudoudoududoudou");  //sa est une instance de la classe SuffixArray

    // Test de la table des suffixes **************************
    std::vector<int> expectedSA = {6, 5, 3, 1, 0, 4, 2};
    if (sa.getSuffixArray() == expectedSA) {
        std::cout << "Test de la table des suffixes réussi !" << std::endl;

        
    } else {
        std::cout << "Test de la table des suffixes échoué !" << std::endl;
    }

    //afficher la table des suffixes
    std::vector<int> suffixArray = sa.getSuffixArray(); 
    std::cout << "Suffix Array: "; 
    for (int i : suffixArray) { // pour chaque element de la table suffixArray
        std::cout << i << " "; 
    }
    std::cout << std::endl;
  */
  //******************sa équivalente ************** */ 
  /*
  // Test de la table des suffixes équivalente
  //sa.buildSuffixArrayEquivalent();
  if (sa.getSuffixArrayEquivalent() == expectedSA) {
      std::cout << "Test de la table des suffixes équivalente réussi !" << std::endl;
  } else {
      std::cout << "Test de la table des suffixes équivalente échoué !" << std::endl;
  }
  //afficher la table des suffixes équivalente
    std::vector<int> suffixArrayEquivalent = sa.getSuffixArrayEquivalent();
    std::cout << "Suffix Array Equivalent: ";
    for (int i : suffixArrayEquivalent) { // pour chaque element de la table suffixArrayEquivalent
        std::cout << i << " ";
    }
    std::cout << std::endl;
 */
 //************************************************ */   
 /*
    // Test de la table LCP
    std::vector<int> expectedLCP = {0, 1, 3, 0, 0, 2, 0};
    if (sa.getLcpArray() == expectedLCP) {
        std::cout << "Test de la table LCP réussi !" << std::endl;

        

    } else {
        std::cout << "Test de la table LCP échoué !" << std::endl;
    }

    //afficher la table lcp 
    std::vector<int> lcpArray = sa.getLcpArray();
    std::cout << "LCP Array: ";
    for (int i : lcpArray) { // pour chaque element de la table 
        std::cout << i << " ";
    }
    std::cout << std::endl;


     //Test de recherche
    if (sa.search("ana")) {
      std::cout << "Test de recherche réussi !" << std::endl;
    } else {
      std::cout << "Test de recherche échoué !" << std::endl;
    }
}

*/
//* teste des performances suffixArray vs suffixArrayEquivalent***********************//

/*
void comparePerformance() {

    std::string text = "banana";

    // Mesurer le temps pour la méthode actuelle
    auto start1 = std::chrono::high_resolution_clock::now();
    SuffixArray sa1(text);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::cout << "Temps pour la méthode actuelle : " << elapsed1.count() << " secondes" << std::endl;

    // Mesurer le temps pour la méthode équivalente
    auto start2 = std::chrono::high_resolution_clock::now();
    SuffixArray sa2(text, false);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = end2 - start2;
    std::cout << "Temps pour la méthode équivalente : " << elapsed2.count() << " secondes" << std::endl;
}

int main() {
    // Exécuter les tests unitaires
    runTests();

    // Comparer les performances
    comparePerformance();
    return 0;
}
*/

#include <iostream>
#include "SuffixArray.h"

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