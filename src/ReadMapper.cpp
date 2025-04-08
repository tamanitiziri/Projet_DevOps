#include "ReadMapper.h"
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <stdexcept>

ReadMapper::ReadMapper(const std::string& referenceGenome, std::size_t kmerSize, std::size_t stepSize)
    : kmerIndex(referenceGenome, kmerSize, stepSize), 
      kmerSize(kmerSize), 
      stepSize(stepSize) {}

MappingResult ReadMapper::mapRead(const std::string& read) const {
    MappingResult result;
    
    if (read.length() < kmerSize) {
        return result;
    }

    auto candidates = findCandidatePositions(read);
    if (candidates.empty()) return result;

    std::vector<double> scores;
    for (const auto& candidate : candidates) {
        double score = evaluatePosition(read, candidate.first, candidate.second);
        scores.push_back(score > 0 ? score : 0);
    }

    auto best_it = std::max_element(scores.begin(), scores.end());
    std::size_t best_idx = static_cast<std::size_t>(std::distance(scores.begin(), best_it));
    
    result.referencePos = candidates[best_idx].first;
    result.strand = candidates[best_idx].second;
    result.confidence = scores[best_idx];
    result.isUnique = (std::count(scores.begin(), scores.end(), *best_it) == 1);
    
    result.cigarString = generateCigar(read, result.referencePos, result.strand);
    result.editDistance = calculateEditDistance(
        read,
        kmerIndex.getReference().substr(result.referencePos, read.length())
    );
    
    return result;
}

std::vector<std::pair<std::size_t, Strand>> ReadMapper::findCandidatePositions(const std::string& read) const {
    std::unordered_map<std::size_t, std::pair<int, int>> position_counts;
    
    // Forward strand
    for (std::size_t i = 0; i <= read.length() - kmerSize; i += stepSize) {
        std::string kmer = read.substr(i, kmerSize);
        for (std::size_t pos : kmerIndex.findKmerPositions(kmer)) {
            if (pos >= i) {
                position_counts[pos - i].first++;
            }
        }
    }
    
    // Reverse complement
    std::string rc = getReverseComplement(read);
    for (std::size_t i = 0; i <= rc.length() - kmerSize; i += stepSize) {
        std::string kmer = rc.substr(i, kmerSize);
        for (std::size_t pos : kmerIndex.findKmerPositions(kmer)) {
            if (pos >= i) {
                position_counts[pos - i].second++;
            }
        }
    }
    
    std::vector<std::pair<std::size_t, Strand>> candidates;
    for (const auto& entry : position_counts) {
        if (entry.second.first > 0 || entry.second.second > 0) {
            Strand strand = (entry.second.first >= entry.second.second) ? 
                          Strand::FORWARD : Strand::REVERSE_COMPLEMENT;
            candidates.emplace_back(entry.first, strand);
        }
    }
    
    return candidates;
}

double ReadMapper::evaluatePosition(const std::string& read, std::size_t pos, Strand strand) const {
    const std::string& reference = kmerIndex.getReference();
    const std::size_t k = kmerIndex.getKmerSize();
    const std::size_t step = kmerIndex.getStepSize();
    
    if (pos + read.length() > reference.length()) {
        return 0.0;
    }
    
    std::string refSegment = reference.substr(pos, read.length());
    if (strand == Strand::REVERSE_COMPLEMENT) {
        refSegment = getReverseComplement(refSegment);
    }
    
    int matchCount = 0;
    for (std::size_t i = 0; i <= read.length() - k; i += step) {
        if (read.substr(i, k) == refSegment.substr(i, k)) {
            matchCount++;
        }
    }
    
    double maxPossible = static_cast<double>(read.length() - k + step) / step;
    return static_cast<double>(matchCount) / maxPossible;
}

std::string ReadMapper::generateCigar(const std::string& read, std::size_t pos, Strand strand) const {
    (void)pos; (void)strand; // Supprime les avertissements unused parameter
    return std::to_string(read.length()) + "M";
}

int ReadMapper::calculateEditDistance(const std::string& s1, const std::string& s2) const {
    std::size_t min_len = std::min(s1.length(), s2.length());
    int distance = static_cast<int>(std::max(s1.length(), s2.length()) - min_len);
    
    for (std::size_t i = 0; i < min_len; ++i) {
        if (s1[i] != s2[i]) {
            distance++;
        }
    }
    return distance;
}

std::string ReadMapper::getReverseComplement(const std::string& seq) {
    static constexpr auto complement = [](char c) {
        switch (c) {
            case 'A': return 'T'; case 'T': return 'A';
            case 'C': return 'G'; case 'G': return 'C';
            case 'a': return 't'; case 't': return 'a';
            case 'c': return 'g'; case 'g': return 'c';
            default: return c;
        }
    };

    std::string rc;
    rc.reserve(seq.size());
    for (auto it = seq.rbegin(); it != seq.rend(); ++it) {
        rc += complement(*it);
    }
    return rc;
}
