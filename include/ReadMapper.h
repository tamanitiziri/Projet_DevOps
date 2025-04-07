#ifndef READMAPPER_H
#define READMAPPER_H

#include "KmerIndex.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <cstddef> 

enum class Strand {
    FORWARD,
    REVERSE_COMPLEMENT,
    UNKNOWN
};

/**
 * \struct MappingResult
 * \brief Structure contenant les résultats du mappage
 * Cette structure inclut des informations telles que la position de référence,
 * le brin (strand), la confiance, la chaîne CIGAR, la distance d'édition, 
 * et si le mappage est unique.
 */ 

 struct MappingResult {
    std::size_t referencePos;
    Strand strand;
    double confidence;
    std::string cigarString;
    int editDistance;
    bool isUnique;
    std::vector<std::size_t> kmerMatches;
};  

/**
 * @class ReadMapper
 * @brief Classe pour mapper des séquences reads sur un génome de référence.
 *
 * Cette classe utilise un index de k-mers pour trouver les positions candidates
 * et évaluer les alignements des séquences reads.
 */

class ReadMapper {
public:
    ReadMapper(const std::string& referenceGenome, std::size_t kmerSize = 20, std::size_t stepSize = 3);
    
    MappingResult mapRead(const std::string& read) const;
    
    const KmerIndex& getIndex() const { return kmerIndex; }
    
private:
    KmerIndex kmerIndex;
    std::size_t kmerSize;
    std::size_t stepSize;
    
    std::vector<std::pair<std::size_t, Strand>> findCandidatePositions(const std::string& read) const;
    double evaluatePosition(const std::string& read, std::size_t pos, Strand strand) const;
    std::string generateCigar(const std::string& read, std::size_t pos, Strand strand) const;
    int calculateEditDistance(const std::string& s1, const std::string& s2) const;
    
    static std::string getReverseComplement(const std::string& seq);
};

#endif