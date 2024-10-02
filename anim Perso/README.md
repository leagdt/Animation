# Projet de Visualisation et Animation de Personnage

## Description

Ce projet consiste à créer une application de visualisation et d'animation de personnages en utilisant une interface graphique. Le projet se divise en plusieurs parties, couvrant l'affichage de squelettes, le contrôle d'un personnage via le clavier, la gestion des transitions entre animations, et l'interaction avec des objets physiques.

## Structure du Projet

- **Skeleton.h / Skeleton.cpp** : Implémente la classe `Skeleton` pour gérer les articulations du squelette et leurs transformations.
- **CharAnimViewer** : Gère l'affichage et la mise à jour du squelette.
- **CharacterController** : Contrôle le déplacement du personnage via le clavier.
- **PhysicalWorld / Particle** : Gère la physique des particules et leur interaction avec le personnage.

## Fonctionnalités

### Partie 1 : Affichage

- Implémentation de la classe `Skeleton` pour gérer un tableau d'articulations (`SkeletonJoint`).
- Affichage des squelettes en utilisant les transformations appropriées.
- Initialisation et mise à jour des positions des articulations.

### Partie 2 : Contrôleur d'Animation

- Implémentation de la classe `CharacterController` pour contrôler le déplacement du personnage.
- Déplacement d'une sphère en utilisant les touches du clavier pour accélérer, freiner, tourner et sauter.

### Partie 3 : Transition et Graphe d'Animation

- Calcul de distance entre 2 poses afin de construire le graphe d'animation.
- Construction automatique d'un graphe d'animation pour gérer les transitions entre animations (4 animations :Idle, Walk, Run et Kick).
- Implémentation d'une machine à état (classe `FiniteStateMachine`), qui nous permet de passer dans différent états, comme Idle, walk, Jump, Backflip...
- Gestion du temps et interpolation entre 2 frames. 

### Partie 4 : Interaction entre le Personnage et des Sphères Physiques

- Implémentation de la physique des particules.
- Gestion des collisions entre les particules et le sol. (Effet de balle qui rebondit)
- Le personnage peut intéragir avec les particules s'il y a collision. 

## Instructions de Compilation et d'Exécution

### Compilation

Pour compiler le programme, ouvrez un terminal dans le répertoire du projet et exécutez les commandes suivantes :

```sh
premake4 --file=master_CharAnim.lua gmake ou premake5 --file=master_CharAnim.lua gmake
make
```

### Exécution

Pour exécutez le programme, dans le même terminal, exécutez une des commandes suivantes : 

- Si vous voulez lancer le mode avec le graphe d'animation automatique : 

```sh
./bin/master_CharAnim mg
```

- Si vous voulez lancer le mode avec la machine à état : 

```sh
./bin/master_CharAnim
