#ifndef FormatFileDetector_H
#define FormatFileDetector_H
#include <string>

class FormatFileDetector{
    public:
    enum Format{FASTA, FASTQ, UNKNOWN};
    
    //détecter le format d'un fichier 
    static Format detect(const std::string& filePath);

    //convertir le format enum en nom lisible
    static std::string formatToString(Format format);

};
#endif