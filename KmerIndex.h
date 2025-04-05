#ifndef KMERINDEX_H
#define KMERINDEX_H
#include "SuffixArray.h"
#include "SequenceParser.h"
#include <string>
#include <functional>

class KmerIndex
{
private:
const std::string& reference;
int kmerSize;
int stepSize;    // Nouveau paramètre pour le décalage des k-mers
SuffixArray suffixArray;

void processKmersBatch(const std::vector<std::string>& kmers,
                       std::function<void(const std::string&,
                       const std::vector<size_t>&)> callback) const;

public:

// Constructor :Prend la référence par const& pour éviter la copie
KmerIndex(const std::string& referenceGenome, int k, int step = 1) 
: reference(referenceGenome), // Référence constante
  kmerSize(k),
  stepSize(step),
  suffixArray(referenceGenome) {}

 /**
 * @brief Traite un read en extrayant les k-mers et en recherchant leurs occurrences.
 *
 * Cette méthode extrait des k-mers à partir d’un vecteur de lectures (reads), effectue une recherche
 * optimisée en batch pour trouver leurs positions (ou index), puis appelle une fonction de rappel
 * (callback) pour chaque k-mer avec les résultats correspondants.
 *
 * L'extraction des k-mers utilise une taille de k-mer (kmerSize) et un pas (stepSize) définis
 * dans l'objet `KmerIndex`. Les k-mers sont extraits uniquement si la lecture a une longueur
 * suffisante (>= kmerSize). Chaque k-mer est ensuite cherché dans l'index à l'aide d'une recherche
 * groupée (`batchSearch`), ce qui permet d'optimiser les performances.
 *
 * @param reads Un vecteur de chaînes représentant les lectures à traiter.
 * @param callback Une fonction de rappel appelée pour chaque k-mer trouvé.
 *                  Elle prend deux arguments :
 *                  - le k-mer (std::string),
 *                  - un vecteur de positions (std::vector<size_t>) correspondant au résultat de la recherche.
 *
 * @note Cette fonction est `const`, ce qui signifie qu'elle ne modifie pas l'état de l'objet.
 */
   // Nouvelle interface pour un seul read
   void processSingleRead(const std::string& read,
                          std::function<void(const std::string&,
                          const std::vector<size_t>&)> callback) const;

//recherche des kamer 
/**
 * @brief findKmerPositions
 * @param kmer Le k-mer à rechercher
 * @return Un vecteur contenant les positions du k-mer dans la séquence
 * @note Cette méthode utilise la méthode de recherche binaire pour trouver
 * Rôle : Trouve les positions d'un k-mer spécifique (recherche binaire via SuffixArray).
 * Cas d'usage : Debug, analyses ponctuelles.
 */
std::vector<size_t> findKmerPositions(const std::string& kmer) const;

 /** 
 * @brief getKmerSupport
 * @param kmer Le k-mer à rechercher
 * @return Le support du k-mer (nombre d'occurrences)
 * @note Cette méthode utilise la méthode countOccurrences du SuffixArray.
 * @note Rôle : Compte les occurrences d'un k-mer (utilise countOccurrences du suffix array).
 * @note Utilité : Filtrage des k-mers trop fréquents/rares.
 */
size_t getKmerSupport(const std::string& kmer) const {
    if (kmer.length() != kmerSize) 
        throw std::invalid_argument("Taille de k-mer invalide");
        
    return suffixArray.countOccurrences(kmer); // Utilise directement countOccurrences
}

/**
 * @brief getKmerPositions
 * @param kmer Le k-mer à rechercher
 * @param includeRevComp Indique si le reverse complement doit être inclus  
 * dans la recherche
 * @return Un vecteur contenant les positions du k-mer dans la séquence
 * @note Cette méthode utilise la méthode de recherche binaire pour trouver
 * les positions du k-mer dans la séquence.
 * @note Si includeRevComp est vrai, elle inclut également les positions du 
 * reverse complement.
 * Pour usage ponctuel - préférer processReadBatch() pour les traitements en masse
 * @throws invalid_argument si kmer.size() != kmerSize 
 */
std::vector<size_t> getKmerPositions(const std::string& kmer, 
                                   bool includeRevComp = false) const {
    auto positions = findKmerPositions(kmer);
    if (includeRevComp) {
        auto revCompPos = findReverseComplementPositions(kmer);
        positions.insert(positions.end(), revCompPos.begin(), revCompPos.end());
    }
    return positions;
}
// Optimisation de la recherche du reverse complement
std::vector<size_t> findReverseComplementPositions(const std::string& kmer) const {
    return findKmerPositions(SequenceParser::getReverseComplement(kmer));
}


};

#endif