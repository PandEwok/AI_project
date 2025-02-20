# AI_project

## <ins>Introduction: </ins>  
Ce projet a pour but de mettre en valeur l'adaptabilité d'un algorithme d'IAs afin d'en comprendre le fonctionnement et la façon dont il est codé dans plusieurs jeux vidéos.
Notre travail à été conçu via C++ et visualisé avec la librairie SFML.

## <ins> Idée et concept: </ins>  
Similaire à une simulation, nous avons mis en place dans une carte pré-définie à l'avance des positions d'ennemis et d'alliés ainsi que du joueur.
Le but des alliés et de protéger le joueurs des ennemis qui chercheront le joueur et ainsi de l'éliminer au contact.
En utilisant plusieurs types d'IA telles que le Finite State Machine (FSM), du Pathfinding A*, des Behavior Trees et Goal-Oriented Action Planning (GOAP), les différents types d'entitées peuvent s'adapter et se déplacer en fonction des différents évènements actuels.

## <ins> Mécaniques & Contrôles </ins> 

<ins> L'Allié :</ins> L'allié a pour but de défendre le joueur ; pour cela, il s'imiscera entre le joueur et essayera de se déplacer en fonction des ennemis les plus proches. Si le joueur venait a mourir, alors l'allié ce mettra à ressuciter celui-ci, cependant, il sera ainsi vulnérable.

<ins> Les Ennemis :</ins>
- L'assassin (aka le petit) à pour rôle de chercher le joueur en se déplaçant rapidement sur le terrain, cependant, s'il détecte l'allié, alors il essayera de s'enfuir de quelques cases avant de réessayer.
- La brute (aka le gros) à pour rôle de chercher le joueur sans autres déplacements, si l'allié touche la brute pour l'en empêcher, alors la brute sera étourdi pendant 3 secondes avant de réessayer.
Les deux ennmemis peuvent éliminer l'allié si celui-ci est occupé à ressuciter le joueur.

## Contrôles
Z,Q,S,D pour se déplacer respectivement
