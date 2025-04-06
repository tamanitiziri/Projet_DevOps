#ifndef KMERINDEX_H
#define KMERINDEX_H
#include "SuffixArray.h"
#include "SequenceParser.h"
#include <string>
#include <functional>
#include <cstddef> // Pour size_t

class KmerIndex
{
private:
    const std::string& reference;
    std::size_t kmerSize;
    std::size_t stepSize;
    SuffixArray suffixArray;

    void processKmersBatch(const std::vector<std::string>& kmers,
                         std::function<void(const std::string&,
                         const std::vector<std::size_t>&)> callback) const;

public:
    KmerIndex(const std::string& referenceGenome, std::size_t k, std::size_t step = 1);

    void processSingleRead(const std::string& read,
                          std::function<void(const std::string&,
                          const std::vector<std::size_t>&)> callback) const;

    std::vector<std::size_t> findKmerPositions(const std::string& kmer) const;

    std::size_t getKmerSupport(const std::string& kmer) const {
        if (kmer.length() != kmerSize) 
            throw std::invalid_argument("Taille de k-mer invalide");
        return suffixArray.countOccurrences(kmer);
    }

    std::vector<std::size_t> getKmerPositions(const std::string& kmer, 
                                            bool includeRevComp = false) const {
        auto positions = findKmerPositions(kmer);
        if (includeRevComp) {
            auto revCompPos = findReverseComplementPositions(kmer);
            positions.insert(positions.end(), revCompPos.begin(), revCompPos.end());
        }
        return positions;
    }

    std::vector<std::size_t> findReverseComplementPositions(const std::string& kmer) const {
        return findKmerPositions(SequenceParser::getReverseComplement(kmer));
    }

    const std::string& getReference() const { return reference; }
    std::size_t getKmerSize() const { return kmerSize; }
    std::size_t getStepSize() const { return stepSize; }
};

#endif