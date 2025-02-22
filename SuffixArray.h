#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H
#include <string>  // Pour utiliser std::string (manipulation des chaînes de caractères)
#include <vector>

class SuffixArray{
    private:
    std::string pattern;  //mon motif 
    std::vector<int> suffixArray;   // ma table SA
    std::vector<int> lcpArray;     // ma table lcp

    //contruire SA
    void buildSuffixArray();

    //contruire lcp
    void buildLcpArray();

    public:
    //le constructure de ma classe
    SuffixArray(const std::string& inputPattern);

    //getter de SA
    const std::vector<int>& getSuffixArray() const;

    //getter de lcp
    const std::vector<int>& getLcpArray() const;

    //rechrache d'un facteur dans ma table SA (avce une methode déchotomique) 
    bool search(const std::string& factor) const;

};

#endif