#ifndef FASTQFILEREADER_H
#define FASTQFILEREADER_H
#include "SequenceParser.h"
#include <string>
#include <vector>
#include <fstream>
#include <functional>

class FastqFileReader : public SequenceParser {

private:
    std::string filePath;
    mutable std::vector<std::string> qualityScores;

    /**
     * Indique si le fichier est en mode stream (non chargé en mémoire)
     * mutable permet de modifier une variable membre dans une méthode const
     */
    mutable bool isStreamMode = false;

    /**
     * Vérifie si la ligne courante est un début de séquence FASTQ (commence par '@')
     */
    bool isSequenceStart(const std::string& line) const;
    
    /**
     * Vérifie si la ligne courante est un séparateur de séquence FASTQ (commence par '+')
     */
    bool isQualitySeparator(const std::string& line) const;
    
    /**
     * Parse les scores de qualité d'une séquence FASTQ
     */
    void parseQualityScores(std::ifstream& file, size_t sequenceLength);


public:
explicit FastqFileReader(const std::string& filePath);
/**
 * Désactive la copie pour eviter les accidents avec les gros fichiers
 *cette instrcution operator= est une fonction membre de la classe FastqFileReader
 *qui prend un argument de type FastqFileReader et qui retourne un objet de type FastqFileReader
 */
FastqFileReader(const FastqFileReader&) = delete;
FastqFileReader& operator=(const FastqFileReader&) = delete;


//methode pour charger le fichier
bool loadFile() override;
//methode pour valider le fichier
bool validate() const override;

//getters
const std::vector<std::string>& getHeaders() const override;
const std::vector<std::string>& getSequences() const override ;
//methode pour recuperer les scores de qualite
const std::vector<std::string>& getQualityScores();

size_t countSequences() const override;

// Nouvelle méthode stream
/**
 * processSequences permet de lire un fichier FASTQ en mode stream (sans charger le fichier en mémoire)
 * et d'appeler une fonction de rappel pour chaque séquence lue.
 * La fonction de rappel doit prendre trois arguments : l'en-tête, la séquence et les scores de qualité.
 * La fonction de rappel doit renvoyer un booléen indiquant si le traitement doit se poursuivre ou non.
 */
bool processSequences(
    const std::function<void(const std::string& header, 
                           const std::string& sequence,
                           const std::string& quality)>& callback);


};


#endif
