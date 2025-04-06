#ifndef SEQUENCEPARSER_H
#define SEQUENCEPARSER_H
#include <vector>   // Pour std::vector
#include <string>   // Pour std::string
#include <cstddef> // Pour std::size_t

class SequenceParser {
    protected:
    std::vector<std::string> sequences;
    std::vector<std::string> headers;  
      
    public:
        virtual ~SequenceParser() = default;
    
        // Méthodes à implémenter par les classes filles
        virtual bool loadFile()  = 0;
        virtual bool validate() const  = 0;
        
        // Getters standards
        virtual const std::vector<std::string>& getHeaders() const = 0;
        virtual const std::vector<std::string>& getSequences() const = 0;
        virtual size_t countSequences() const = 0;
    
        // Méthodes communes implémentées ici
        static std::string getReverseComplement(const std::string& seq);
        enum SequenceType {
            DNA,  
            RNA,  
            AA,   
            UNKNOWN  
        };
        static SequenceType getSequenceType(const std::string& sequence);
    };

#endif