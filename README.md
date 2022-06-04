# Simulation d'un parc d'attraction
Projet en C réalisé au sein de Polytech Paris Saclay

## Sujet
Dans ce projet, nous considérons un parc d’attraction comportant les zones suivantes :

- Une entrée par laquelle les visiteurs passent obligatoirement pour entrer et sortir du parc. Au
moment d’entrer dans le parc, chaque visiteur doit régler la somme de 30€. Nous précisons que
toute sortie est définitive.

- Des allées permettant aux visiteurs de se rendre aux différentes attractions. Les visiteurs
peuvent rester autant de temps qu’ils le souhaitent dans les allées du parc.

- Les attractions dites « libres » auxquelles les visiteurs peuvent accéder librement, sans faire la
queue, et où ils peuvent rester le temps qu’ils souhaitent. Ces attractions peuvent être, par
exemple, des animations. Nous considérons, dans cet exercice, que ce type d’attractions ne
possède pas de limite au niveau du nombre de ses visiteurs.

- Les attractions dites « contrôlées » pour lesquelles les visiteurs doivent faire la queue. Nous
précisons que les visiteurs sortent de la queue pour entrer dans l’attraction dans le même ordre
qu’ils y sont entrés. Bien sûr, une attraction de ce type ne peut admettre qu’un nombre limité
de visiteurs. Contrairement aux attractions « libres », une fois entrés dans une attraction
« contrôlée », les visiteurs sont contraints d’y rester le temps que dure celle-ci. Cependant,
chaque visiteur ayant un certain niveau de patience, si le temps passé à faire la queue est trop
long cela affectera son niveau de satisfaction et les chances qu’il revienne dans le parc.

Nous concevrons donc un programme qui simule une journée de
fonctionnement pour un tel parc d’attraction. Celui-ci permettra de voir les recettes réalisées ainsi que
le niveau de satisfaction des visiteurs.
