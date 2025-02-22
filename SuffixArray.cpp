#include "SuffixArray.h"
#include <algorithm>  // Pour utiliser std::sort

// initialiser le constructeur et definir ma un character vide $ a la fin de mon motif 

SuffixArray::SuffixArray(const std::string& inputPattern) : pattern(inputPattern + '$') {
    buildSuffixArray();
    buildLcpArray();
}

// methode de ma table SA ********************************
void SuffixArray::buildSuffixArray(){
    int n = pattern.length(); //ma taille est tjr positif

    //declarre un vecteur de pair pour stocker les indice et les siffixes 
    std::vector<std::pair<std::string, int>> suffixes;

    //remplire mon vecteur suffixes 
    for(int i = 0; i < n; i++){
        suffixes.push_back({pattern.substr(i), i}); // stocker mon suffixe pattern.substr(i) et ca pos i
    }

    //trier mes suffixes avec l'ordre lexicographique
    std::sort(suffixes.begin(), suffixes.end());

    // remplire ma tabel suffixes avec les modifications 
    for(const auto& suffix : suffixes){
        suffixArray.push_back(suffix.second);
    }

} 
 //* construire ma table  lcp *****************************
    void SuffixArray::buildLcpArray(){
        int n =  pattern.length();  // la taille de mon motif 
        lcpArray.resize(n, 0); // redimentionner ma table lcp avec la taille de mon motif  et initialiser a 0
        std::vector<int> rank(n, 0); // il va contenir les rangs(L'indice de table suffixarray) des suffixes dans la table SA

        //remplir la table rank
        for(int i = 0; i < n; i++){
            rank[suffixArray[i]]= i;
        }
        int k = 0;
    }