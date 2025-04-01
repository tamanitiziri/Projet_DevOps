#include "SuffixArray.h"
#include <algorithm>  // Pour utiliser std::sort>

// initialiser le constructeur et definir ma un character vide $ a la fin de mon motif 
/*
SuffixArray::SuffixArray(const std::string& inputtext, bool buildSA) : text(inputtext + '$') {
   if (buildSA){
    buildSuffixArray(); // chrono
    //buildLcpArray();
    //buildSuffixArrayEquivalent();
   }else{
         buildSuffixArrayEquivalent(); // chrono
         //buildLcpArray();
   }
}
*/
// Constructeur
SuffixArray::SuffixArray(const std::string& inputText) : text(inputText + '$') {
    buildSuffixArray(); // Construire la table des suffixes
    buildLcpArray();    // Construire la table LCP (facultatif)
}


// methode de ma table SA ********************************

void SuffixArray::buildSuffixArray(){
    size_t n = text.length(); //ma taille est tjr positif

    //declarre un vecteur de pair pour stocker les indice et les siffixes 
    std::vector<std::pair<std::string, size_t>> suffixes;

    //remplire mon vecteur suffixes 
    for(size_t i = 0; i < n; i++){
        suffixes.push_back({text.substr(i), i}); // stocker mon suffixe text.substr(i) et ca pos i
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
        size_t n =  text.length();  // la taille de mon motif 
        lcpArray.resize(n, 0); // redimentionner ma table lcp avec la taille de mon motif  et initialiser a 0
        std::vector<size_t> rank(n, 0); // il va contenir les rangs(L'indice de table suffixarray) des suffixes dans la table SA

        //remplir la table rank
        for(size_t i = 0; i < n; i++){
            rank[suffixArray[i]]= i;
        }

        size_t k = 0;
        for(size_t i=0; i<n; i++){
            if (rank[i] ==  n-1) {
                k = 0;  //si le rang de est le dernier suffixe alors en reinitialise la longueur de comparaisont k à 0
                continue;
            }
            size_t j = suffixArray[rank[i]+1]; // le suffixe suivant 
            while(i+k <n && j+k <n && text[i+k] == text[j+k]){
                k++; //incrementre si y a une correspondance 
            }
            lcpArray[rank[i]] = k; //stocker la longueur de correspondance
            if (k > 0){
                k--;  // décrémenter k pour optimiser le calcul de la LCP pour le suffixe suivant
                //(les k-1 char identifies sont identique dans les autres suffuxes -order lexicographique )
            }

        }
    }
    //methode gitter de SA 
    const std::vector<size_t>& SuffixArray::getSuffixArray() const{
        return suffixArray;
    }

    //get ma table lcp
    const std::vector<size_t>& SuffixArray::getLcpArray() const{
        return lcpArray;
    }
    
   //chercher un motif dans la table des suffixes (recherghe dichotomique)
bool SuffixArray::search(const std::string& motif) const{
    size_t left = 0; 
    size_t right = suffixArray.size() - 1 ; 
    size_t res , suffixPos;
    size_t mid;

    while(left <= right){
        mid = left + (right - left) / 2;  // calculer le milieu de sa 

        suffixPos = suffixArray[mid]; // Position du suffixe dans la chaîne d'origine
        std::string suffix = text.substr(suffixPos); // Extraire le suffixe à partir de cette position dans le motif d'origne
        
        // Comparer le motif avec le suffixe
        size_t res = motif.compare(0, motif.length(), suffix, 0, motif.length());

        if (res == 0){ /// motif trouvé 
            return true;
        } else if (res < 0){ // le motif plus petit que le suffixe de milieu
            right = mid - 1 ;  // chercher dans la partie gauche 
        } else{
            left = mid + 1; // chercher dans la partie droite 
        }

    }

    return false; // motif non trouvé 
}

// ***************Amelioration de la fonction de ta table SA************************************
//l'amelioration ici est de construire une table SA equivalent
//cette table est construite en utilisant la fonction de comparaison personnalisée qui compare les suffixes de la chaîne d'origine
//au lieu de comparer les suffixes de la chaîne de caractères
//cette table est utilisée pour la recherche de motifs dans la chaîne d'origine

bool compareSuffixes(size_t i, size_t j, const std::string& s) {
    return s.substr(i) < s.substr(j);
}

void SuffixArray::buildSuffixArrayEquivalent() {
    size_t n = text.length();
    std::vector<size_t> indices(n);

    // Initialiser le vecteur d'indices
    for (size_t i = 0; i < n; ++i) {
        indices[i] = i;
    }

    // Trier les indices en utilisant la fonction de comparaison personnalisée
    /**
     * Ici la fonction de comparaison personnalisée compare les suffixes de la chaîne d'origine
     * au lieu de comparer les suffixes de la chaîne de caractères
     * elle est de complexité O(nlogn) càd la diffrence avec l'utilisation e sorte sns modifie la fonction lambda
     * est que la fonction de comparaison personnalisée compare les suffixes de la chaîne d'origine
     * au lieu de comparer les suffixes de la chaîne de caractères qui d'une complexité O(n^2logn)   
     * 
     */
    std::sort(indices.begin(), indices.end(), [this](size_t i, size_t j) {
        return compareSuffixes(i, j, text);
    });

    // Mettre à jour la table des suffixes
    SuffixArrayEquivalent = indices;
}

//getter de SA equivalent**************
    const std::vector<size_t>& SuffixArray::getSuffixArrayEquivalent() const{
        return SuffixArrayEquivalent;
    }

//get lowerBound
/**
 * cette fonction retourne la position du premier suffixe dans la table des suffixes qui est supérieur ou égal à un motif donné.
 * elle est de complexité O(logn)
 */
size_t  SuffixArray::lowerBound(const std::string& motif) const{
    size_t  left = 0;
    size_t  right = suffixArray.size()-1; //-1 pour ne pas calculer le $ a la fin de mon motif
    size_t  result = suffixArray.size(); //initialiser le resultat a la taille de ma table SA
    
    while(left <= right){
        size_t  mid = left + (right - left) / 2; //calculer le milieu
        size_t  cmp =text.compare(suffixArray[mid], motif.length(), motif); //comparer le motif avec le suffixe de milieu

        if(cmp >= 0){
            result = mid; //stocker le resultat
            right = mid - 1; //chercher dans la partie gauche
        }else{
            left = mid + 1; //chercher dans la partie droite
        }   

    }
    return result;
}

//get upperBound
/**
 * cette fonction retourne la position du premier suffixe dans la table des suffixes qui est strictement supérieur à un motif donné.
 * elle est de complexité O(logn)
 */
size_t SuffixArray::upperBound(const std::string& motif) const{
        size_t  left = 0;
        size_t  right = suffixArray.size()-1; //-1 pour ne pas calculer le $ a la fin de mon motif
        size_t  result = suffixArray.size(); //initialiser le resultat a la taille de ma table SA
        
        while(left <= right){
            size_t  mid = left + (right - left) / 2; //calculer le milieu
            size_t  cmp =text.compare(suffixArray[mid], motif.length(), motif); //comparer le motif avec le suffixe de milieu
    
            if(cmp > 0){
                result = mid; //stocker le resultat
                right = mid - 1; //chercher dans la partie gauche
            }else{
                left = mid + 1; //chercher dans la partie droite
            }   
    
        }
        return result; 
}


// Retourne le nombre d'occurrences du motif
/**
 * cette fonction retourne le nombre d'occurrences du motif dans la chaîne d'origine.
 * elle est de complexité O(logn)
 * l'utlisation des deux fonctions lowerBound et upperBound permet de calculer le nombre d'occurrences du motif permet 
 * de calculer le nombre d'occurrences du motif dans la chaîne d'origine en utilisant la table des suffixes.
 * la complexité spaciale est de O(1) car on utilise la table des suffixes déja calculer pour calculer le nombre d'occurrences du motif.
 */
size_t SuffixArray::countOccurrences(const std::string& text) const {
    if (text.empty()) {
        return 0;
    }
    size_t lower = lowerBound(text);
    size_t upper = upperBound(text);
    return upper - lower;
}

//fonction getfactor
std::string SuffixArray::getFactor(size_t i, size_t k) const {
    if (i + k > text.length() - 1) {
        throw std::out_of_range("Index out of range");
    }
    return text.substr(i, k);
}

    