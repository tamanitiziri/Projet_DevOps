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
    int n = text.length(); //ma taille est tjr positif

    //declarre un vecteur de pair pour stocker les indice et les siffixes 
    std::vector<std::pair<std::string, int>> suffixes;

    //remplire mon vecteur suffixes 
    for(int i = 0; i < n; i++){
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
        int n =  text.length();  // la taille de mon motif 
        lcpArray.resize(n, 0); // redimentionner ma table lcp avec la taille de mon motif  et initialiser a 0
        std::vector<int> rank(n, 0); // il va contenir les rangs(L'indice de table suffixarray) des suffixes dans la table SA

        //remplir la table rank
        for(int i = 0; i < n; i++){
            rank[suffixArray[i]]= i;
        }

        int k = 0;
        for(int i=0; i<n; i++){
            if (rank[i] ==  n-1) {
                k = 0;  //si le rang de est le dernier suffixe alors en reinitialise la longueur de comparaisont k à 0
                continue;
            }
            int j = suffixArray[rank[i]+1]; // le suffixe suivant 
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
    const std::vector<int>& SuffixArray::getSuffixArray() const{
        return suffixArray;
    }

    //get ma table lcp
    const std::vector<int>& SuffixArray::getLcpArray() const{
        return lcpArray;
    }
    
   //chercher un motif dans la table des suffixes (recherghe dichotomique)
bool SuffixArray::search(const std::string& motif) const{
    int left = 0; 
    int right = suffixArray.size() - 1 ; 
    int res , suffixPos;
    int mid;

    while(left <= right){
        mid = left + (right - left) / 2;  // calculer le milieu de sa 

        suffixPos = suffixArray[mid]; // Position du suffixe dans la chaîne d'origine
        std::string suffix = text.substr(suffixPos); // Extraire le suffixe à partir de cette position dans le motif d'origne
        
        // Comparer le motif avec le suffixe
        int res = motif.compare(0, motif.length(), suffix, 0, motif.length());

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

bool compareSuffixes(int i, int j, const std::string& s) {
    return s.substr(i) < s.substr(j);
}

void SuffixArray::buildSuffixArrayEquivalent() {
    int n = text.length();
    std::vector<int> indices(n);

    // Initialiser le vecteur d'indices
    for (int i = 0; i < n; ++i) {
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
    std::sort(indices.begin(), indices.end(), [this](int i, int j) {
        return compareSuffixes(i, j, text);
    });

    // Mettre à jour la table des suffixes
    SuffixArrayEquivalent = indices;
}

//getter de SA equivalent**************
    const std::vector<int>& SuffixArray::getSuffixArrayEquivalent() const{
        return SuffixArrayEquivalent;
    }

//get lowerBound
/**
 * cette fonction retourne la position du premier suffixe dans la table des suffixes qui est supérieur ou égal à un motif donné.
 * elle est de complexité O(logn)
 */
int  SuffixArray::lowerBound(const std::string& motif) const{
    int  left = 0;
    int  right = suffixArray.size()-1; //-1 pour ne pas calculer le $ a la fin de mon motif
    int  result = suffixArray.size(); //initialiser le resultat a la taille de ma table SA
    
    while(left <= right){
        int  mid = left + (right - left) / 2; //calculer le milieu
        int  cmp =text.compare(suffixArray[mid], motif.length(), motif); //comparer le motif avec le suffixe de milieu

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
int SuffixArray::upperBound(const std::string& motif) const{
        int  left = 0;
        int  right = suffixArray.size()-1; //-1 pour ne pas calculer le $ a la fin de mon motif
        int  result = suffixArray.size(); //initialiser le resultat a la taille de ma table SA
        
        while(left <= right){
            int  mid = left + (right - left) / 2; //calculer le milieu
            int  cmp =text.compare(suffixArray[mid], motif.length(), motif); //comparer le motif avec le suffixe de milieu
    
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
int SuffixArray::countOccurrences(const std::string& text) const {
    if (text.empty()) {
        return 0;
    }
    int lower = lowerBound(text);
    int upper = upperBound(text);
    return upper - lower;
}

//fonction getfactor
std::string SuffixArray::getFactor(int i, int k) const {
    if (i + k > text.length() - 1) {
        throw std::out_of_range("Index out of range");
    }
    return text.substr(i, k);
}

    