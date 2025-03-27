#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H
#include <string>  // Pour utiliser std::string (manipulation des chaînes de caractères)
#include <vector>
#include <stdexcept>  // Pour utiliser std::invalid_argument (gestion des erreurs)

// doxygen documentation
/**
 * Classe SuffixArray
 * 
 * Cette classe permet de construire un tableau de suffixes (SA) et un tableau de valeurs de préfixes les plus longs (LCP) pour une chaîne de caractères donnée.
 * Elle permet également de rechercher un facteur dans la chaîne de caractères.
 */
class SuffixArray{
    private:
   // std::string text;  //ma chaine de caractere
    std::string text;  //mon motif 
    std::vector<int> suffixArray;   // ma table SA
    std::vector<int> lcpArray;     // ma table lcp
    std::vector<int> SuffixArrayEquivalent; // ma table SA equivalent ******

    //contruire SA
    void buildSuffixArray();

    //contruire lcp
    void buildLcpArray();

    //contruire SA equivalent *********************
    /**
     * Construit la table des suffixes équivalents.
     * l'amelioration ici est de construire une table SA equivalent
     * cette table est construite en utilisant la fonction de comparaison personnalisée qui compare les suffixes de la chaîne d'origine
     * au lieu de comparer les suffixes de la chaîne de caractères
     * cette table est utilisée pour la recherche de motifs dans la chaîne d'origine
     */
    void buildSuffixArrayEquivalent();

    /**
      * Fonctions pour la recherche d'occurrences
      * documentation de ces deux fonction:
      * lowerBound: retourne la position du premier suffixe dans la table des suffixes qui est supérieur ou égal à un motif donné.
      * upperBound: retourne la position du premier suffixe dans la table des suffixes qui est strictement supérieur à un motif donné.
    
    */
    int lowerBound(const std::string& text) const;
    int upperBound(const std::string& text) const;


    public:
    /** le constructure de ma classe
     * explicite est une methode qui permet de construire un objet de la classe SuffixArray à partir d'une chaine de caractères
     * sont but est d'emppecher le compilateur de convertir un objet de la classe SuffixArray à partir d'une chaine de caractères
     * c'est une bonne pratique de l'utiliser pour eviter des erreurs de compilation
     */
     //explicit SuffixArray(const std::string& inputtext, bool buildSA = true);

     // Constructeur
    explicit SuffixArray(const std::string& inputtext);


    //getter de SA
    /**
     * Retourne la table des suffixes
     */
    const std::vector<int>& getSuffixArray() const;

    //getter de lcp
    const std::vector<int>& getLcpArray() const;

    //getter de SA equivalent **********************
    /**
     * Retourne la table des suffixes équivalents.
     */
    const std::vector<int>& getSuffixArrayEquivalent() const;

    /** rechrache d'un facteur dans ma table SA (avce une methode déchotomique) */
    bool search(const std::string& factor) const;

    /**  Retourne le nombre d'occurrences du motif */
    int countOccurrences(const std::string& motif) const;

     /**  Retourne le i-ème facteur de longueur k de la séquence */
     std::string getFactor(int i, int k) const;


};

#endif