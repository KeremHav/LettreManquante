=== MOTS CACHÉS - Jeu de lettres ===

Un jeu où vous devez deviner des lettres cachées dans des mots pour gagner des "fraises tagada" !

▶ FONCTIONNALITÉS
- 3 niveaux de difficulté (Facile, Moyen, Difficile)
- Système de scores avec classement
- Interface colorée dans le terminal
- Pseudonyme personnalisable
- Règles intégrées

INSTALLATION
1. Assurez-vous d'avoir GCC installé
2. Téléchargez ou clonez le projet
3. Compilez avec :
   make
COMMENT JOUER
1. Lancer le jeu :
   ./launchMe
2. Choisir un pseudonyme
3. Sélectionner "Nouvelle partie"
4. Deviner les lettres marquées [1], [2], etc.

COMMANDES UTILES
- make        : Compile le jeu
- make doc    : Génère la documentation
- make clean  : Nettoie les fichiers objets
- make mrproprer : Nettoie l'ensemble des fichiers créés
- ./launchMe  : Lance le jeu

GESTION DES SCORES
- Les meilleurs scores sont sauvegardés
- Visible via le menu principal
- Seuls les scores positifs sont enregistrés

FICHIERS IMPORTANTS
- easy.txt    : Mots pour le niveau facile
- medium.txt  : Mots pour le niveau moyen
- hard.txt    : Mots pour le niveau difficile
- scores.bin  : Fichier binaire des scores

CONTROLES
- Entrez une lettre pour deviner
- '1' : Passer une lettre (-3 points)
- '0' : Quitter la partie

ℹ POUR LES DÉVELOPPEURS
Documentation générée avec Doxygen :
1. Installer doxygen et graphviz
2. make doc
3. Ouvrir docs/html/index.html

© 2025 - Kerem Erkart
