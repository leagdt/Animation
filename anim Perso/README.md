# Projet d'Animation de Tissu - Système Masses-Ressorts

Ce projet implémente une animation de tissu en utilisant le **système masses-ressorts**. Il simule un tissu flexible en prenant en compte les forces de gravité et de tension entre les points du tissu. Le projet utilise **SDL**, **GLEW**, et **SDL2_image** pour le rendu et la gestion des images. La compilation et la génération des Makefiles se font avec **Premake4**.

## Description

Ce projet implémente une animation de tissu en utilisant le **système masses-ressorts**. L'animation du tissu est réalisée à travers une grille de points reliés par des ressorts. Chaque point représente une masse, et les ressorts relient les masses voisines pour simuler l'élasticité du tissu. Le projet prend en compte :
- **Forces de gravité** qui attirent les masses vers le bas.
- **Forces de ressort** qui tentent de maintenir la forme du tissu en opposition à la gravité.

L'objectif est de simuler un comportement réaliste de déformation d'un tissu sous l'effet de la gravité et des interactions entre les ressorts.

### Fonctionnalités

1. **Simulation de la gravité et du vent** :
   - La simulation prend en compte les forces accumulées sur chaque particule. Pour chaque particule, la force du vent est ajoutée, et la force totale est divisée par la masse de la particule pour obtenir l'accélération, à laquelle s'ajoute la force de gravité.

2. **Déchirure du tissu** :
   - Le projet implémente une logique pour la déchirure du tissu. Si la longueur d'un ressort dépasse un seuil prédéfini, il est supprimé, simulant ainsi une déchirure. Pour observer ce phénomène, déplacez un point fixe en appuyant sur la touche **'m'** et utilisez les touches fléchées **'←'** et **'→'** ou **'Page Up'** et **'Page Down'**.

3. **Chute du tissu** :
   - Le tissu est maintenu par deux points fixes (particules de masse 0). En appuyant sur la touche **'p'**, vous pouvez donner une masse supérieure à 0 à ces points, permettant au tissu de tomber sous l'effet de la gravité.

4. **Gestion des collisions** :
   - **Avec le sol** : La simulation vérifie si une particule se trouve en dessous d'un certain seuil (y = -10). Si c'est le cas, sa vitesse est réglée à zéro pour simuler l'impact avec le sol.
   - **Avec une sphère** : La distance entre chaque particule et le centre de la sphère est calculée. Si cette distance est inférieure au rayon de la sphère, la particule est arrêtée et déplacée à la surface de la sphère.

## Prérequis

Avant de compiler et d'exécuter le projet, assurez-vous que les bibliothèques suivantes sont installées sur votre système :

- **SDL2**
- **GLEW** (OpenGL Extension Wrangler)
- **SDL2_image**
- **Premake4**

### Installer les dépendances sous Linux (Ubuntu/Debian)

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libglew-dev premake4
```


