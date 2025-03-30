#ifndef FASTAPARSER_H
#define FASTAPARSER_H


// Inclusion des bibliothèques standard nécessaires
#include <string>  // Pour utiliser std::string (manipulation des chaînes de caractères)
#include <vector>  // Pour utiliser std::vector (tableaux dynamiques)
#include "SequenceParser.h" // Ensure the correct case matches the file name

// Définition de la classe FastaParser pour analyser des fichiers au format FASTA
class FastaParser : public SequenceParser{
private:
    // Membres privés : uniquement accessibles à l'intérieur de la classe
    std::string filePath;                // Chemin vers le fichier FASTA
    //std::vector<std::string> headers;    // Liste des headers (lignes commençant par '>')
    //std::vector<std::string> sequences;  // Liste des séquences correspondantes
    
public:
    // Constructeur : initialise un objet avec le chemin du fichier
    explicit FastaParser(const std::string& filePath);

    // Charge le fichier FASTA, extrait les headers et séquences
    bool loadFile() override ;

    // Valide si le fichier FASTA respecte le format standard
    bool validate() override;

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

    
    //SequenceType getSequenceType(const std::string& sequence);

};

// Fin des include guards pour éviter les inclusions multiples du fichier
#endif // FASTAPARSER_H
