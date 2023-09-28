#ifndef POPULATION_CENTRE_H
#define POPULATION_CENTRE_H

#include <stdio.h>
#include<list> 
#include <math.h>
#include <stdlib.h> 
#include <ctime>
#include "struct.h"
#include <iomanip>
#include <random>
#include <vector>
#include <gmpxx.h>
using namespace std;

bool par = true;

// concentration vers le nombre de copies (volume de 1µ³)

#define uM2cn(c) (int)(c*(602.252*1e21))		// c en µM
#define mM2cn(c) uM2cn(c*1000.)		        	// c en mM

// nombre de copies vers concentration (volume de 1µ³)

#define cn2uM(n) (((float)n)/602.252)	// résultat en µM 

/*algo de Gilles tarte
 1- choisir une réaction aléatoire dans la liste des réactions
 2- calcul du nombre de collisions de R
 3 - n = partie entière de c * p(R) * 7.4*10-7
 4 - appliquer R n fois sur la population (retire les substrats en entrée et ajoute ceux en sortie)
 5 - m = partie fractionnaire de de c*p(R)
 6 - on tire un nombre aléatoire float U [0,1]:
        -si U < m alors on applique R une fois de plus
        -sinon on passe à la réaction suivante.

*/

//calcul de c -> nombre de copies de s1 * nombres de copies de s2 ou nombre de copies de s1

//code de GillesTarte qui permet de faire le code de l'algo population centrée

void GillesTarte(Reaction TabRea[],EM TabEM[20]){
    float pasTemps = 0.001;
    random_device rd;
    default_random_engine eng(rd());
    uniform_real_distribution<float> distr(0,1);
    if (par){
        for(int i = 0; i < 50;i++){
            //cout << i << ".s1 : " << TabEM[TabRea[i].s1].nbCopie << endl;
            if(TabRea[i].s1 == -1){
                continue;          //permet de quitter la boucle si le tableau contient moins de 50 réactions
            }else{
                long double calcul;

                if(TabRea[i].s2 == -1){ //si un seul substrat alors c = concentration de c1 
                    double c = TabEM[TabRea[i].s1].nbCopie;//mM2cn(TabEM[TabRea[i].s1].concentration);
                    calcul = c * TabRea[i].proba;
                    //cout << "calcul : " << calcul << endl;
                }else{ //sinon c = concentration 1 * concentration 2
                    double c1 = TabEM[TabRea[i].s1].nbCopie;
                    double c2 = TabEM[TabRea[i].s2].nbCopie;
                    double c = c1 * c2;
                    calcul = c * TabRea[i].proba * (7.4*1e-7); // c = c1 ou c1 * c2 sont les concentrations respectives des substrats s1 et s2
                }
                long double n = floor(calcul);

                long double m = calcul - n;
                long double u = distr(eng);
                if( u < m ){
                n += 1; // ajoute 1 à n si m tombe plus grand que u
                }

                //appliquer R n fois sur les concentrations i.e la concentration en substrat diminue et la concentration en produit augmente 
                TabEM[TabRea[i].s1].nbCopie -= n;
                TabEM[TabRea[i].p1].nbCopie += n;

                //cout << TabEM[TabRea[i].s1].concentration << "\n" ;
            
                if(TabRea[i].s2 != -1){
                    TabEM[TabRea[i].s2].nbCopie -= n;                          
                }
                if(TabRea[i].p2 != -1){
                    TabEM[TabRea[i].p2].nbCopie += n;
                }
            }
            //cout << i << ".s1 : " << TabEM[TabRea[i].s1].nbCopie << endl;
        }
        
        par = false;
    }
    else{
        for(int i = 50; i > 0;i--){
            if(TabRea[i].s1 == -1){
                continue;          //permet de quitter la boucle si le tableau contient moins de 50 réactions
            }else{
                long double calcul;

                if(TabRea[i].s2 == -1){ //si un seul substrat alors c = concentration de c1 
                    double c = TabEM[TabRea[i].s1].nbCopie;//mM2cn(TabEM[TabRea[i].s1].concentration);
                    calcul = c * TabRea[i].proba;
                }else{ //sinon c = concentration 1 * concentration 2
                    double c1 = TabEM[TabRea[i].s1].nbCopie;
                    double c2 = TabEM[TabRea[i].s2].nbCopie;
                    double c = c1 * c2;
                    calcul = c * TabRea[i].proba * (7.4*1e-7); // c = c1 ou c1 * c2 sont les concentrations respectives des substrats s1 et s2
                }
                long double n = floor(calcul);

                long double m = calcul - n;
                long double u = distr(eng);
                if( u < m ){
                n += 1; // ajoute 1 à n si m tombe plus grand que u
                }

                //appliquer R n fois sur les concentrations i.e la concentration en substrat diminue et la concentration en produit augmente 
                TabEM[TabRea[i].s1].nbCopie -= n;
                TabEM[TabRea[i].p1].nbCopie += n;

                //cout << TabEM[TabRea[i].s1].concentration << "\n" ;
            
                if(TabRea[i].s2 != -1){
                    TabEM[TabRea[i].s2].nbCopie -= n;                          
                }
                if(TabRea[i].p2 != -1){
                    TabEM[TabRea[i].p2].nbCopie += n;
                }
            }
        }
        par = true;
    }

}

#endif