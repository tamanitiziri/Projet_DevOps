/* ce fichier est conçu pour analyser des fichiers FASTA et FASTQ
 * il utilise la classe FormatFileDetector pour détecter le format du fichier
 * et la classe FastaParser pour analyser les fichiers FASTA
 * et la classe FastqFileReader pour analyser les fichiers FASTQ
 * il utilise également la classe SequenceParser pour déterminer le type de séquence
 * et la classe ReadMapper pour mapper les reads sur un génome de référence
 * pour l'exécuter, utilisez la commande suivante:
 * g++ -std=c++20 -o file file.cpp FastaParser.cpp sequenceParser.cpp fastqFileReader.cpp FormatFileDetector.cpp
 * ./file <fichier>
*/

#include <iostream>
#include <memory>
#include "FormatFileDetector.h"
#include "FastaParser.h"
#include "FastqFileRreader.h"

void analyzeFile(const std::string& filePath) {
    // Vérifier d'abord si le fichier est vide
    std::ifstream file(filePath, std::ios::ate);
    if (file.tellg() == 0) {
        std::cout << "Analyse du fichier: " << filePath << "\n";
        std::cout << "Erreur: Le fichier est vide\n";
        return;
    }
    file.close();

    // Détection du format
    FormatFileDetector::Format format = FormatFileDetector::detect(filePath);
    std::string formatName = FormatFileDetector::formatToString(format);
    
    std::cout << "Analyse du fichier: " << filePath << "\n";
    std::cout << "Format détecté: " << formatName << "\n\n";

    if (format == FormatFileDetector::UNKNOWN) {
        std::cerr << "Erreur: Impossible de déterminer le format du fichier (fichier vide ou format non supporté)\n";
        return;
    }

    try {
        if (format == FormatFileDetector::FASTA) {
            FastaParser parser(filePath);
            if (parser.loadFile()) {
                std::cout << "Nombre de séquences: " << parser.countSequences() << "\n";
                
                // Afficher les tailles des 5 premières séquences (pour éviter l'affichage long)
                if(parser.countSequences() != 0){
                auto sizes = parser.getSequenceSizes();
                size_t displayCount = std::min(sizes.size(), static_cast<size_t>(5));
                std::cout << "Tailles des " << displayCount << " premières séquences: ";
                for (size_t i = 0; i < displayCount; ++i) {
                    std::cout << sizes[i];
                    if (i < displayCount - 1) std::cout << ", ";
                }
                if (sizes.size() > displayCount) std::cout << ", ...";
                std::cout << "\n";
                 } else {
                    std::cout << "Aucune séquence trouvée dans le fichier FASTA (séquence vide)\n";
                 }
                // Validation
                if (parser.validate()) {
                    std::cout << "Statut: Fichier FASTA valide\n";
                } else {
                    std::cout << "Statut: Fichier FASTA invalide\n";
                }
                
                // Type des séquences
                if (!parser.getSequences().empty()) { 
                    SequenceParser::SequenceType type = SequenceParser::getSequenceType(parser.getSequences()[0]);
                    std::string typeStr;
                    switch (type) {
                        case SequenceParser::DNA: typeStr = "ADN"; break;
                        case SequenceParser::RNA: typeStr = "ARN"; break;
                        case SequenceParser::AA: typeStr = "Acides aminés"; break;
                        default: typeStr = "Inconnu";
                    }
                    std::cout << "Type de la séquence: " << typeStr << "\n";
                }
            } else {
                std::cerr << "Erreur: Impossible de charger le fichier FASTA\n";
            }
        }
        else if (format == FormatFileDetector::FASTQ) {
            FastqFileReader reader(filePath);
            if (reader.loadFile()) {
                if(reader.countSequences() != 0){
                std::cout << "Nombre de séquences: " << reader.countSequences() << "\n";
                } else {
                    std::cout << "Aucune séquence trouvée dans le fichier FASTQ.\n";
                }
                // Afficher les tailles des 5 premières séquences
                const auto& sequences = reader.getSequences();
                size_t displayCount = std::min(sequences.size(), static_cast<size_t>(5));
                std::cout << "Tailles des " << displayCount << " premières séquences: ";
                for (size_t i = 0; i < displayCount; ++i) {
                    std::cout << sequences[i].length();
                    if (i < displayCount - 1) std::cout << ", ";
                }
                if (sequences.size() > displayCount) std::cout << ", ...";
                std::cout << "\n";
                
                // Validation
                if (reader.validate()) {
                    std::cout << "Statut: Fichier FASTQ valide\n";
                } else {
                    std::cout << "Statut: Fichier FASTQ invalide\n";
                }
                
                // Type des séquences
                if (!sequences.empty()) {
                    SequenceParser::SequenceType type = SequenceParser::getSequenceType(sequences[0]);
                    std::string typeStr;
                    switch (type) {
                        case SequenceParser::DNA: typeStr = "ADN"; break;
                        case SequenceParser::RNA: typeStr = "ARN"; break;
                        case SequenceParser::AA: typeStr = "Acides aminés"; break;
                        default: typeStr = "Inconnu";
                    }
                    std::cout << "Type de séquence: " << typeStr << "\n";
                }
            } else {
                std::cerr << "Erreur: Impossible de charger le fichier FASTQ\n";
            }
        }
        else {
            std::cerr << "Erreur: Format de fichier non reconnu ou non supporté\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'analyse: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <fichier>\n";
        return 1;
    }
    
    std::string filePath = argv[1];
    analyzeFile(filePath);
    
    return 0;
}