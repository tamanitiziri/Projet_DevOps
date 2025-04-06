#include "KmerIndex.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>

KmerIndex::KmerIndex(const std::string& referenceGenome, std::size_t k, std::size_t step)
    : reference(referenceGenome), 
      kmerSize(k),
      stepSize(step == 0 ? 1 : step), // step minimum à 1
      suffixArray(referenceGenome) {

    if (kmerSize == 0) {
        throw std::invalid_argument("Taille de k-mer invalide");
    }

    if (reference.length() < kmerSize) {
        throw std::invalid_argument("Référence plus courte que kmerSize");
    }
}

void KmerIndex::processSingleRead(const std::string& read,
    std::function<void(const std::string&,
                     const std::vector<std::size_t>&)> callback) const {
    
    std::vector<std::string> kmers;
    if (read.length() >= kmerSize) {
        std::size_t numKmers = (read.length() - kmerSize + stepSize) / stepSize;
        kmers.reserve(numKmers);

        for (std::size_t i = 0; i <= read.length() - kmerSize; i += stepSize) {
            kmers.emplace_back(read, i, kmerSize);
        }
    }

    processKmersBatch(kmers, callback);
}

void KmerIndex::processKmersBatch(const std::vector<std::string>& kmers,
    std::function<void(const std::string&,
                     const std::vector<std::size_t>&)> callback) const {

    std::vector<std::vector<std::size_t>> results(kmers.size());
    std::vector<std::size_t> indices(kmers.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    std::sort(indices.begin(), indices.end(),
        [&kmers](std::size_t a, std::size_t b) { return kmers[a] < kmers[b]; });

    for (std::size_t i = 0; i < indices.size(); ) {
        const auto& currentKmer = kmers[indices[i]];
        auto positions = findKmerPositions(currentKmer);

        while (i < indices.size() && kmers[indices[i]] == currentKmer) {
            results[indices[i]] = positions;
            ++i;
        }
    }

    for (std::size_t i = 0; i < kmers.size(); ++i) {
        callback(kmers[i], results[i]);
    }
}

std::vector<std::size_t> KmerIndex::findKmerPositions(const std::string& kmer) const {
    if (kmer.length() != kmerSize) {
        throw std::invalid_argument("Taille de k-mer incorrecte");
    }
    return suffixArray.findOccurrences(kmer);
}