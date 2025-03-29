#include "FormatFileDetector.h"
#include <fstream>

FormatFileDetector::Format FormatFileDetector::detect(const std::string& filePath){
    
    std::ifstream file(filePath);
    if (!file) return UNKNOWN;

    std::string firstLine;
    std::getline(file, firstLine);

    if (firstLine.empty()) return UNKNOWN;
    return (firstLine[0] == '>' || firstLine[0] == ';') ? FASTA: 
           (firstLine[0] == '@') ? FASTQ : UNKNOWN;
}

std::string FormatFileDetector::formatToString(Format format) {
    switch (format) {
        case FASTA: return "FASTA";
        case FASTQ: return "FASTQ";
        default: return "UNKNOWN";
    }
}