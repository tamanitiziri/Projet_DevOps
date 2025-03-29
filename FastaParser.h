#ifndef FASTAPARSER_H
#define FASTAPARSER_H

// Inclusion des bibliothèques standard nécessaires
#include <string>  // Pour utiliser std::string (manipulation des chaînes de caractères)
#include <vector>  // Pour utiliser std::vector (tableaux dynamiques)

// Définition de la classe FastaParser pour analyser des fichiers au format FASTA
class FastaParser {
private:
    // Membres privés : uniquement accessibles à l'intérieur de la classe
    std::string filePath;                // Chemin vers le fichier FASTA
    std::vector<std::string> headers;    // Liste des headers (lignes commençant par '>')
    std::vector<std::string> sequences;  // Liste des séquences correspondantes
    // Ajoute cette méthode pour récupérer toutes les séquences
    //std::vector<std::string> getSequences() const;

public:
    // Constructeur : initialise un objet avec le chemin du fichier
    FastaParser(const std::string& filePath);

    // Charge le fichier FASTA, extrait les headers et séquences
    bool loadFile();

    // Valide si le fichier FASTA respecte le format standard
    bool validateFasta();

    // Retourne le nombre de séquences présentes dans le fichier FASTA
    size_t countSequences();

    // Retourne le complément inverse d'une séquence ADN donnée
    std::string getReverseComplement(const std::string& sequence);

    // récupérer toutes les séquences
    const std::vector<std::string>& getSequences() const;
    
    // calculer la taille des séquences 
    std::vector<size_t> getSequenceSizes() const;

    //fonction qui recuepre le type de ma sequence (adn , arn , aa, unknown) en utilisant enummeration
    enum SequenceType {
        DNA,  
        RNA,  
        AA,   
        UNKNOWN  
    };
     //getter de la sequence type
     /**
 *  Détermine le type biologique d'une séquence (ADN, ARN, protéique ou inconnu).
 *  SequenceType Enum indiquant le type de séquence (DNA, RNA, AA, UNKNOWN)
 *
 */
    SequenceType getSequenceType(const std::string& sequence);

};

// Fin des include guards pour éviter les inclusions multiples du fichier
#endif // FASTAPARSER_H
