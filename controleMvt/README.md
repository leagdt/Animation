# Projet de Contrôle de Mouvement

## Description

Ce projet est un programme en C++ qui implémente un système de contrôle de mouvement d'une entité. L'objectif est de maintenir cette entité en équilibre sur une plateforme en mouvement. Le joueur peut également faire sauter l'entité en appuyant sur la touche **Espace**.

### Fonctionnalités

- **Contrôle de l'équilibre :** Le joueur utilise les touches gauche et droite pour déplacer l'entité et compenser le mouvement de la plateforme.
- **Saut :** L'entité saute lorsque la touche **Espace** est pressée, permettant de repositionner l'entité sur la plateforme ou éviter des obstacles.
- **Plateforme en mouvement :** La plateforme se déplace horizontalement, ce qui rend le maintien de l'équilibre plus difficile.
- **Lancer de balles :** Le joueur peut appuyer sur la touche **B** pour lancer des balles sur l'entité. Ces balles ont pour effet de déstabiliser l'entité, rendant plus difficile le maintien de l'équilibre.

## Compilation et exécution

Le projet utilise un `Makefile` pour simplifier la compilation. Suivez les étapes ci-dessous pour compiler et exécuter le projet :

### Étapes

1. **Cloner le dépôt ou télécharger les fichiers**.

2. **Dépendances :**

   Vérifier que GLFW et GLEW sont installé ou installez-les avec la commande suivante :
   ```bash
   apt install libglfw3-dev libglew-dev
   ```

3. **Compiler le projet :**

   Dans le terminal, dans le répertoire du projet, exécutez la commande suivante pour compiler les librairies IMGUI et BOX2D :

  ```bash
   make install
   ```

  Puis exécuter la commande suivante pour compiler le projet : 
   ```bash
   make
   ```

4. **Executer le projet:**
  Dans le terminal, dans le répertoire du projet, exécutez la commande suivante pour exécuter le programme :

   ```bash
   ./TP_CONTROL_BIPED.out
   ```

5. **Nettoyer les fichiers compilés:**
  Dans le terminal, exécutez la commande suivante pour nettoyer les fichiers objets et l'éxécutable généré :
  
  ```bash
   make clean
   ```

  Pour supprimer les librairies exécuetez la commande suivante : 

  ```bash
   make superclean
   ```
