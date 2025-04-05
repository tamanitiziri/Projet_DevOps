#include "KmerIndex.h"
#include <algorithm>
#include <numeric>

//constrcuteur + validation des parametres

KmerIndex::KmerIndex(const std::string& referenceGenome, int k, int step)
    : reference(referenceGenome), 
      kmerSize(k),
      stepSize(step > 0 ? step : 1), // Assure que step est au moins 1
      suffixArray(referenceGenome) {

    if (kmerSize <= 0) {
        throw std::invalid_argument("Taille de k-mer invalide veuillez entrer un entier positif superieur à 0");
    }

    if(reference.length() < kmerSize) {
        throw std::invalid_argument("La taille de la référence est inférieure à la taille du k-mer");
    }
}

void KmerIndex::processSingleRead(const std::string& read,
    std::function<void(const std::string&,
                     const std::vector<size_t>&)> callback) const {
// Étape 1: Découpage du read en k-mers - Complexité: O(L/k)
std::vector<std::string> kmers;
size_t numKmers = (read.length() >= kmerSize) ? 
((read.length() - kmerSize + stepSize) / stepSize) : 0;
kmers.reserve(numKmers);

for (size_t i = 0; i <= read.length() - kmerSize; i += stepSize) {
kmers.emplace_back(read, i, kmerSize); // emplace_back pour éviter la copie cette instruction permet de créer une sous chaîne à partir de la chaîne d'origine
}

// Étape 2: Recherche batch des k-mers - Complexité: O(p(log p + log n))
processKmersBatch(kmers, callback);
}


void KmerIndex::processKmersBatch(const std::vector<std::string>& kmers,
    std::function<void(const std::string&,
                     const std::vector<size_t>&)> callback) const {

// Optimisation pour k-mers identiques - Complexité spatiale: O(p)
std::vector<std::vector<size_t>> results(kmers.size());

// Tri des indices - Complexité: O(p log p)
std::vector<size_t> indices(kmers.size());
std::iota(indices.begin(), indices.end(), 0);
std::sort(indices.begin(), indices.end(),
[&kmers](size_t a, size_t b) { return kmers[a] < kmers[b]; });

// Recherche groupée - Complexité: O(p log n)
for (size_t i = 0; i < indices.size(); ) {
const auto& currentKmer = kmers[indices[i]];
auto positions = findKmerPositions(currentKmer);

while (i < indices.size() && kmers[indices[i]] == currentKmer) {
results[indices[i]] = positions;
++i;
}
}

// Émission des résultats - Complexité: O(p)
for (size_t i = 0; i < kmers.size(); ++i) {
callback(kmers[i], results[i]);
}

}

// Recherche de positions individuelles de k-mers - Complexité: O(log n)
std::vector<size_t> KmerIndex::findKmerPositions(const std::string& kmer) const {
    if (kmer.length() != kmerSize) {
        throw std::invalid_argument("La taille du k-mer ne correspond pas à kmerSize");
    }
    return suffixArray.findOccurrences(kmer);
}

