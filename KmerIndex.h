#ifndef KMERINDEX_H
#define KMERINDEX_H
#include "SuffixArray.h"
#include "SequenceParser.h"
#include <string>


class KmerIndex
{
private:
std::string reference;
size_t kmerSize;
SuffixArray suffixArray;
    
public:
// Constructor
KmerIndex(const std::string& read, size_t k);

//recherche des kamer 
std::vector<size_t> findKmerPositions(const std::string& kmer) const;

//get k-mer support 
size_t getKmerSupport(const std::string& kmer) const;
std::vector<size_t> getKmerPositions(const std::string& kmer) const;

std::vector<size_t> findReverseComplementKmerPositions(const std::string& kmer) const;
  
};

//gestiond de reverse complement
namespace SequenceParser {
    std::string getReverseComplement(const std::string& seq);
}

#endif