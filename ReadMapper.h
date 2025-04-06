#ifndef READMAPPER_H
#define READMAPPER_H

#include "KmerIndex.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <cstddef> // Pour size_t

enum class Strand {
    FORWARD,
    REVERSE_COMPLEMENT,
    UNKNOWN
};

struct MappingResult {
    std::size_t referencePos;
    Strand strand;
    double confidence;
    std::string cigarString;
    int editDistance;
    bool isUnique;
    std::vector<std::size_t> kmerMatches;
};

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