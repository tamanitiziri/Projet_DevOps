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
bool compareSuffixes(size_t i, size_t j, const std::string& s) {
    const size_t n = s.length();
    while (i < n && j < n) {
        if (s[i] < s[j]) return true;
        if (s[i] > s[j]) return false;
        ++i;
        ++j;
    }
    return j != n; // i a attesize_t la fin → j est plus long, donc i < j
}


void SuffixArray::buildSuffixArray(){
    size_t n = text.length();
    std::vector<size_t> indices(n);

    // Initialiser le vecteur d'indices
    for (size_t i = 0; i < n; ++i) {
        indices[i] = i;
    }

    // Trier les indices en utilisant la fonction de comparaison personnalisée
    std::sort(indices.begin(), indices.end(), [this](size_t i, size_t j) {
        return compareSuffixes(i, j, text);
    });

    // Mettre à jour la table des suffixes
    suffixArray = indices;

} 
 //* construire ma table  lcp *****************************
 void SuffixArray::buildLcpArray() {
    const size_t n = text.length();
    lcpArray.resize(n); // Alloue l'espace pour le tableau LCP
    std::vector<size_t> rank(n); // Tableau de rang inverse

    /**
     * @brief Phase 1 : Construction du rang inverse (parallélisable)
     * 
     * @par Principe :
     * Chaque élément rank[i] contient la position du suffixe text[i..n] 
     * dans le suffix array trié.
     * 
     * @par Optimisation :
     * La boucle est parallélisable car chaque écriture est indépendante
     */
    #pragma omp parallel for  // Directive OpenMP pour parallélisation
    for (size_t i = 0; i < n; ++i) {
        rank[suffixArray[i]] = i; // rank[suffixArray[i]] = position dans le SA
    }

    size_t k = 0; // Compteur de préfixe commun
    for (size_t i = 0; i < n; ++i) {
        if (rank[i] == n - 1) {
            // Cas particulier : dernier suffixe dans l'ordre lexicographique
            lcpArray[rank[i]] = 0;
            continue;
        }
        
        size_t j = suffixArray[rank[i] + 1]; // Suffixe suivant dans le SA
        
        /**
         * @brief Heuristique clé : k = max(0, k-1)
         * 
         * @justification :
         * D'après les propriétés du LCP, on a LCP[i] ≥ LCP[i-1] - 1.
         * Cela permet de démarrer la comparaison à k-1 plutôt qu'à 0,
         * économisant ainsi de nombreuses comparaisons de caractères.
         */
        k = (k > 0) ? k - 1 : 0;
        
        // Calcul du LCP entre text[i..] et text[j..]
        while (i + k < n && j + k < n && text[i + k] == text[j + k]) {
            ++k;
        }
        lcpArray[rank[i]] = k; // Stocke la longueur du LCP
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
    size_t Pos;
    size_t mid;
    size_t m = motif.length();

    while(left <= right){
        mid = left + (right - left) / 2;  // calculer le milieu de sa 

        Pos = suffixArray[mid]; // Position du suffixe dans la chaîne d'origine
        //std::string suffix = text.substr(Pas); // Extraire le suffixe à partir de cette position dans le motif d'origne
        bool match = true;
        for( size_t i=0; i < m; ++i){
            if (Pos + i > text.length() || motif[i] != text[Pos + i]){
                match = false; // le motif ne correspond Pos au suffixe
                break;
            }
            
        }


        // Comparer le motif avec le suffixe
        if(match){
            return true; // motif trouvé
        }
        else if (text.compare(Pos, m, motif) < 0){  // si le suffixe lexicographiquement inférieur au motif → on va à droite.
         left = mid + 1; // chercher dans la partie droite
        } else{
            right = mid - 1; // chercher dans la partie gauche
        } 
    }

    return false; // motif non trouvé 
}



//get lowerBound
size_t  SuffixArray::lowerBound(const std::string& motif) const{
    if (motif.empty() || motif.length() > text.length()) return suffixArray.size();
    
    size_t  left = 0;
    size_t  right = suffixArray.size()-1; //-1 pour ne pas calculer le $ a la fin de mon motif
    size_t  result = suffixArray.size(); //initialiser le resultat a la taille de ma table SA
    
    
    while(left <= right){
        size_t  mid = left + (right - left) / 2; //calculer le milieu
        
        size_t compare_len = std::min(motif.length(), text.length() - suffixArray[mid]);
            int  cmp =text.compare(suffixArray[mid], compare_len, motif, 0, compare_len); //comparer le motif avec le suffixe de milieu

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
size_t SuffixArray::upperBound(const std::string& motif) const{
    size_t  left = 0;
    size_t  right = suffixArray.size()-1; //-1 pour ne pas calculer le $ a la fin de mon motif
    size_t  result = suffixArray.size(); //initialiser le resultat a la taille de ma table SA
        
        while(left <= right){
            size_t  mid = left + (right - left) / 2; //calculer le milieu

            // Vérification des limites pour éviter le débordement
            size_t compare_len = std::min(motif.length(), text.length() - suffixArray[mid]);
            int  cmp =text.compare(suffixArray[mid], compare_len, motif, 0, compare_len); //comparer le motif avec le suffixe de milieu
    
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

// Fonction pour trouver les occurrences d'un motif dans la chaîne d'origine
std::vector<size_t> SuffixArray::findOccurrences(const std::string& pattern) const {
    std::vector<size_t> occurrences;
    const size_t m = pattern.length();
    if (m == 0 || m > text.length()) return occurrences;

    // Utilise les bornes existantes pour une recherche O(log n)
    size_t lower = lowerBound(pattern);
    size_t upper = upperBound(pattern);
    
    // Récupère toutes les occurrences
    occurrences.reserve(upper - lower);
    for (size_t i = lower; i < upper; ++i) {
        occurrences.push_back(suffixArray[i]);
    }

    return occurrences;
}
    