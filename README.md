# Projet_simulation

**Tables des matières :**  
&nbsp;&nbsp;1.Contexte  
&nbsp;&nbsp;2.Explication de l’expérience  
**Description du projet :**  
&nbsp;&nbsp;1.Les deux approches  
&nbsp;&nbsp;2.L’approche population centrée  
&nbsp;&nbsp;3.L’approche entité centrée  
**Implémentation :**  
&nbsp;&nbsp;1.Introduction  
&nbsp;&nbsp;2.Structure de données  
&nbsp;&nbsp;3.Lecture du fichier   
&nbsp;&nbsp;4.Analyseur lexical   
&nbsp;&nbsp;5.Analyseur syntaxique  
&nbsp;&nbsp;6.Simulateur population centré    
&nbsp;&nbsp;7.Simulateur entité centrée   
**Résultats**   
**Utilisation**  
**Discussions**
  

## Contexte :
Notre projet de TER consiste à simuler les réactions produites entre des molécules
dans une vésicule dans l’objectif de tester les biomarqueurs correspondant à certaines
maladies. Le but est de tester les biomarqueurs de manière booléenne afin de déterminer si
une personne est atteinte d’une certaine maladie selon les taux des différents biomarqueurs.
Cette technique consiste à mesurer la concentration d’un biomarqueur , si celle-ci est
supérieure à un seuil fixé alors la personne peut être déclarée malade.

## Explication de l’expérience :
Dans une vésicule, un récipient en forme de capsule mesurant de 50 à 100 microns,
on met des enzymes , dans notre cas des protéines de 5 à 10 nm, ainsi que des produits
chimiques, ici des métabolites de 1 nm. Certains métabolites vont passer à travers de la
membrane de vésicule, par contre rien ne pourra en sortir. Sur la membrane, on place des
grosses protéines (des protéines transmembranaires) qui sont composés de 3 domaines,
ces protéines sont appelées des récepteurs, ils peuvent fixer et reconnaître certains
biomarqueurs, modifier leur structure et ils agissent comme des enzymes des 2 côtés de la
protéine, à l’intérieur et à l’extérieur.  
A l’intérieur de la vésicule, il y a un réseau métabolique, c’est-à-dire un grand
nombre de protéines et de métabolites. On essaye donc de synthétiser un produit qui
change de couleur ou qui est bioluminescent en réaction à certains biomarqueurs.
L’enzyme transforme les substrats ( les métabolites) en produit ( nouvelle molécule
avec la même composition chimique).

## Description du projet :
Dans notre cas, on souhaite, à partir d’un document décrivant les molécules
contenues dans la vésicule et leurs réactions, faire fonctionner notre projet afin de simuler
l’expérience décrite ci-dessus. On souhaite donc avoir des molécules qui réagissent entre
elles pendant une certaine durée et obtenir en résultat les courbes des concentrations de
chaque type de molécules présentes à l’origine et des nouvelles apparues. Ce projet sera
implémenté de deux façons différentes qui possèdent chacune leurs avantages. Celles-ci
seront expliquées dans la prochaine partie.

## Les deux approches
Notre projet prend en compte deux approches différentes, qui sont représentées par
deux implémentations différentes pour ce même problème. La première approche est
appelée l’approche population-centrée ,et la seconde l’approche entité-centrée. Des
explications pour les deux approches sont faites dans les parties qui suivent.

### L’approche population centrée
L’approche population centrée est celle qui sera utilisée pour implémenter le premier
simulateur, un simulateur population centrée, c’est-à-dire que pour ce simulateur, on ne
prend pas en compte chaque molécule individuellement, mais on détermine la probabilité
qu’un certain nombre de réaction, parmi les réactions possibles suivant les molécules
présentes dans la vésicule, ait lieu pendant un certain intervalle de temps.  
Cet intervalle de temps se nomme “pas de temps” , il correspond au temps qui
s’écoule entre chaque itération de l’algorithme du simulateur, autrement dit à chaque pas de
temps on calcule le nombre de réactions qui peuvent se produire et on l’applique aux
concentrations des molécules, i.e on diminue ou on augmente les concentrations des
molécules qui sont utilisés dans les réactions. Les réactions peuvent être effectuées
plusieurs fois suivant la durée sélectionnée pour le pas de temps, c’est pour cela que le pas
de temps ne doit ni être trop long, ni être trop court.  
Cette approche part du principe que les molécules sont toutes réparties de manière
homogène dans la vésicule, même si cela ne représente pas les conditions réelles dans
lesquelles les molécules ne sont pas toutes réparties de manière équitable dans le
contenant.

### L’approche entité centrée
L’approche entité centrée est, contrairement à l’approche précédente, une
représentation plus fidèle de la réalité, on ne suppose plus que les molécules sont réparties
de manière homogène dans la vésicule.  
Pour cette approche, on en vient à considérer les molécules une à une, en prenant
en compte leur vitesse et leur position ainsi que les collisions qu’elles effectuent pendant
leur déplacement. Cela nous permet d’obtenir des résultats plus fiables vis-à -vis de la
réalité mais cela apporte l’inconvénient de prendre plus de temps que l’approche
précédente.  
Nous calculons les déplacements en trois dimensions dans la vésicule pour chaque
molécule puis les collisions possibles avec d’autres molécules lors de ce déplacement. Plus
de détails seront donnés dans la partie implémentation.

## Implémentation :
