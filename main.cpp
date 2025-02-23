
#include <iostream>
#include "SuffixArray.h"

// Fonction pour exécuter des tests unitaires
void runTests() {
    // Créer une instance de SuffixArray avec la chaîne "banana$"
    //les suffixes sont : banana, anana, nana, ana, na, a, $
    //ordre lexicographique : $, a, ana, anana, banana, na, nana
    SuffixArray sa("banana");  //sa est une instance de la classe SuffixArray

    // Test de la table des suffixes **************************
    std::vector<int> expectedSA = {6, 5, 3, 1, 0, 4, 2};
    if (sa.getSuffixArray() == expectedSA) {
        std::cout << "Test de la table des suffixes réussi !" << std::endl;

        //afficher la table des suffixes
        std::vector<int> suffixArray = sa.getSuffixArray(); 
        std::cout << "Suffix Array: "; 
        for (int i : suffixArray) { // pour chaque element de la table suffixArray
            std::cout << i << " "; 
        }
        std::cout << std::endl;
    } else {
        std::cout << "Test de la table des suffixes échoué !" << std::endl;
    }
  //******************sa équivalente ************** */ 

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

 //************************************************ */   
    // Test de la table LCP
    std::vector<int> expectedLCP = {0, 1, 3, 0, 0, 2, 0};
    if (sa.getLcpArray() == expectedLCP) {
        std::cout << "Test de la table LCP réussi !" << std::endl;

        //afficher la table lcp 
        std::vector<int> lcpArray = sa.getLcpArray();
        std::cout << "LCP Array: ";
        for (int i : lcpArray) { // pour chaque element de la table 
            std::cout << i << " ";
        }
        std::cout << std::endl;

    } else {
        std::cout << "Test de la table LCP échoué !" << std::endl;
    }

     //Test de recherche
    if (sa.search("ana")) {
      std::cout << "Test de recherche réussi !" << std::endl;
    } else {
      std::cout << "Test de recherche échoué !" << std::endl;
    }
}

int main() {
    // Exécuter les tests unitaires
    runTests();
    return 0;
}