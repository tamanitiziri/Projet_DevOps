#include "FastaParser.h"  // Inclut l'en-tête "FastaParser.h" où la classe FastaParser est définie
#include <iostream>        // Inclut la bibliothèque iostream pour utiliser les flux d'entrée et de sortie (std::cout, std::cerr)

int main(int argc, char* argv[]) {  // Fonction principale, prend des arguments de ligne de commande (argc : nombre d'arguments, argv : tableau des arguments)
    
    // Vérifier si un argument est fourni pour le chemin du fichier
    if (argc < 2) {  // Si le nombre d'arguments est inférieur à 2 (c'est-à-dire qu'aucun fichier n'a été fourni)
        std::cerr << "Usage: " << argv[0] << " <chemin_du_fichier_fasta>\n";  // Affiche un message d'erreur et la façon d'utiliser le programme
        return 1;  // Retourne 1 pour signaler une erreur, le programme s'arrête ici
    }

    std::string filePath = argv[1];  // Récupère le chemin du fichier FASTA passé en argument et le stocke dans la variable 'filePath'
    FastaParser parser(filePath);  // Crée un objet de la classe FastaParser avec le chemin du fichier

    // Vérifie si le fichier FASTA est valide
    if (parser.validateFasta()) {  // Appelle la méthode validateFasta() de l'objet parser pour valider le fichier
        std::cout << "Fichier FASTA valide.\n";  // Si le fichier est valide, affiche un message de succès
        std::cout << "Nombre de séquences : " << parser.countSequences() << "\n";  // Affiche le nombre de séquences dans le fichier
       // std::string seq = "ACGT";  // Déclare une séquence d'ADN d'exemple (ici : "ACGT")
       
       // Récupère toutes les séquences du fichier
        std::vector<std::string> sequences = parser.getSequences();

        // Calcul et affichage du complément inverse pour chaque séquence
        for (const auto& seq : sequences) {
            std::string reverseComplement = parser.getReverseComplement(seq);
            //std::cout << "Séquence originale : " << seq << "\n";
            std::cout << "Complément inverse : " << reverseComplement << "\n\n";
        }
    } else {
        std::cerr << "Fichier FASTA invalide.\n";
    }

    return 0;  // Retourne 0 pour indiquer que le programme a été exécuté avec succès
}
