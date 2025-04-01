#ifndef FASTAPARSER_H
#define FASTAPARSER_H
#include <fstream> // Pour lire un fichier
#include <string>  // Pour utiliser std::string (manipulation des chaînes de caractères)
#include <vector>  // Pour utiliser std::vector (tableaux dynamiques)
#include "SequenceParser.h" // Ensure the correct case matches the file name
#include <functional> // Pour utiliser std::function

// Définition de la classe FastaParser pour analyser des fichiers au format FASTA
class FastaParser : public SequenceParser{
private:
    // Membres privés : uniquement accessibles à l'intérieur de la classe
    std::string filePath;                // Chemin vers le fichier FASTA
    mutable bool isStreamMode = false; 

public:
    // Constructeur : initialise un objet avec le chemin du fichier
    explicit FastaParser(const std::string& filePath);

    // Charge le fichier FASTA, extrait les headers et séquences
    bool loadFile() override ;

    // Valide si le fichier FASTA respecte le format standard
    bool validate() const override;

    // Retourne le nombre de séquences présentes dans le fichier FASTA
    size_t countSequences() const override {return sequences.size();};

    // Retourne le complément inverse d'une séquence ADN donnée
    //std::string getReverseComplement(const std::string& sequence);

    // récupérer toutes les séquences
    const std::vector<std::string>& getSequences() const override {return sequences;}
    
    // calculer la taille des séquences 
    std::vector<size_t> getSequenceSizes() const;

    //getheaders
    const std::vector<std::string>& getHeaders() const override {return headers;}

    
    //methode de streaming 
    bool processSequences(
        const std::function<void(const std::string& header,
                                 const std::string& sequence)>& callback);
    

};

// Fin des include guards pour éviter les inclusions multiples du fichier
#endif // FASTAPARSER_H
