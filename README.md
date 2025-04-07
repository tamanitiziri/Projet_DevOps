# Fasta API

## Description
Ce projet est une API conçue pour manipuler et analyser des fichiers biologiques au format FASTA et FASTQ. Elle inclut des fonctionnalités telles que la lecture de fichiers, la détection de formats, l'indexation de k-mers, et le mappage des séquences reads avec une autre séquence de génome de référence.

## Structure du Projet
- **`src/`** : Contient les fichiers source principaux en C++.
- **`include/`** : Contient les fichiers d'en-tête.
- **`build/`** : Répertoire pour les fichiers objets générés.
- **`data/`** : Contient des fichiers de données d'exemple (e.g., `mini_genome.fasta`, `test_reads.fastq`).
- **`test_unitaires/`** : Contient les tests unitaires.
  - **`fasta.cpp`** : Teste les fonctionnalités de lecture des fichiers FASTA. Aucun argument n'est requis, mais il faut changer le chemin du fichier dans le corps du programme.
  - **`fastaq.cpp`** : Teste les fonctionnalités de lecture des fichiers FASTQ avec des tests générés automatiquement sans arguments.
  - **`file.cpp`** : Teste les fichiers FASTA fournis dans le cadre de ce projet. Le fichier doit être passé en argument. Pour exécuter ce fichier, il faut passer un fichier FASTA/FASTQ en argument.
  - **`main.cpp`** : Valide les fonctionnalités de la classe de recherche d'un motif avec une table de suffixes de la classe `SuffixArray`, avec un exemple prêt dans le corps du programme.
  - **`makefile`** : Fichier permettant d'automatiser l'exécution des fichiers de test un à la fois. En tapant `make` seul, il affiche la bonne syntaxe d'exécution.
 
- **`doxyfile/`** : Fichier de Doxygen pour générer la documentation avec Doxygen.
- **`tests/`** : Contient le fichier principal nommé `mapper.cpp`, qui fait appel à l'ensemble des classes pour réaliser un mapping des reads.

## Prérequis
- **Compilateur** : GCC avec support pour C++20.
- **Outils** :
  - `make` pour la compilation (e.g., `make run ref=mini_genome.fasta reads=test_reads.fastq k=8 step=2` pour l'exécution) sinon tapez `make run` pour voir la bonne syntaxe d'exécution.
  - `doxygen` pour générer la documentation.

## Installation
1. Clonez le dépôt :
   ```bash
   git clone <url-du-repo>
   cd fasta_api