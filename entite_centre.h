#ifndef ENTITE_CENTRE_H
#define ENTITE_CENTRE_H

#include <vector>
#include <random>
#include <cmath>

#define DIAMETRE 1000 // diametre de la sphere
#define DUREE_DIFFUSION 60 //duree de la phase de diffusion

void entite(Reaction TabRea[],EM TabEM[50], float duree){

    //taille du deplacement normal

    int dist = 10;
    float alpha;
    float beta;
    float x2,y2,z2;
    float pR;

    //generateur de chiffre aléatoire pour l'angle de déplacement
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> distr(0,360);

    //generateur de chiffre aléatoire pour la probabilité de se déplacer
    random_device rd2;
    default_random_engine eng2(rd2());
    uniform_real_distribution<float> distr2(0,1);

    //creation du vecteur de molecules
        vector<Mol> listMol= {};
        for(int i = 0; i<20;i++){
            for(int j = 0; j< TabEM[i].nbCopie;j++){
                listMol.push_back(Mol{type: i});
            }
        }

    cout<< "taille du tab de molecule : " << listMol.size() << endl;

    //Phase de diffusion
    /*for (float j = 0; j < DUREE_DIFFUSION; j = j+0.001){
        for (int i = 0 ; i<listMol.size(); i++){
            alpha = distr(eng) * M_PI/180;
            beta = distr(eng) * M_PI/180;
            float p = distr2(eng2);

            //application du mouvement en fonction des angle alpha et beta generer aleatoirement
            if (p <= TabEM[listMol[i].type].vitesse){
                //cout << "p : " << p << " | vitesse : " << TabEM[listMol[i].type].vitesse << endl;
                listMol[i].x = listMol[i].x + 10 * cos(alpha) * cos(beta);
                listMol[i].y = listMol[i].y + 10 * sin(alpha) * sin(beta);
                listMol[i].z = listMol[i].z + 10 * sin(beta);
            }
        }
    }
    */


    //affichage des molecules a la Fin de la phase de diffusion
    cout << "Fin de la phase de diffusion " << endl ;

    for (float t = 0; t < duree; t = t+0.01){
        //mouvement des molecules
        for (int i = 0 ; i<listMol.size(); i++){
            alpha = 10;//distr(eng) * M_PI/180;
            beta = 10;//distr(eng) * M_PI/180;

            //application du mouvement en fonction des angle alpha et beta generer aleatoirement
            if (distr2(eng2) <= TabEM[listMol[i].type].vitesse){
                x2 = listMol[i].x + 10 * cos(alpha) * cos(beta);
                y2 = listMol[i].y + 10 * sin(alpha) * sin(beta);
                z2 = listMol[i].z + 10 * sin(beta);

                //vérifie que l'on ne se trouve pas hors de la sphere
                if(sqrt(pow(x2,2)+pow(y2,2)+pow(z2,2)) <= DIAMETRE/2){

                    //gestion des collision avec les autres molecules
                    for (int j = 0 ; j<listMol.size(); j++){
                        if (i != j){
                            if ( x2 >= listMol[j].x - (TabEM[listMol[i].type].diametre/2) && x2 <= listMol[j].x + (TabEM[listMol[i].type].diametre/2)){
                                if ( y2 >= listMol[j].y - (TabEM[listMol[i].type].diametre/2) && y2 <= listMol[j].y + (TabEM[listMol[i].type].diametre/2)){
                                    if ( z2 >= listMol[j].z - (TabEM[listMol[i].type].diametre/2) && z2 <= listMol[j].z + (TabEM[listMol[i].type].diametre/2)){
                                        //gestion des reaction de type s1 + s2 = ...

                                        for (int k = 0; k < 50; k++){
                                            if (TabRea[k].s1 == -1){
                                                break;
                                            }
                                            cout << "i :" << i << " | j : " << j << endl;

                                            if((TabRea[k].s1 == listMol[i].type && TabRea[k].s2 == listMol[j].type) || (TabRea[k].s2 == listMol[i].type && TabRea[k].s1 == listMol[j].type)){
                                                pR = distr2(eng2);
                                                cout <<"pR : " << TabRea[j].p1;
                                                if (pR <= TabRea[k].proba){
                                                    cout << i << " : " << TabRea[j].p1;
                                                    listMol[i].type = TabRea[k].p1;
                                                    if (TabRea[k].p2 == -1){
                                                        listMol.erase(listMol.begin()+j);
                                                    }
                                                    else{
                                                        listMol[j].type = TabRea[k].p2;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //gestion des réaction de type s1 = ...
                    for (int j = 0; j < 50; j++){
                        if( TabRea[j].s2 == -1){
                            if (TabRea[j].s1 == listMol[i].type){
                                if (pR <= TabRea[j].proba){
                                    listMol[i].type = TabRea[j].p1;
                                    if (TabRea[j].p2 != -1){
                                        listMol.push_back(Mol{type: i, x:x2, y:y2, z:z2});
                                    }
                                }
                                else{
                                    pR -= TabRea[j].proba;
                                }
                            }
                        }
                    }
                
                    listMol[i].x = x2;
                    listMol[i].y = y2;
                    listMol[i].z = z2;
                }

            }

        }
    }

    for (int i = 0 ; i<50; i++){
        cout << "type : " << TabEM[listMol[i].type].nom << "      ->      x : " << listMol[i].x << "; y : " << listMol[i].y << "; z : " << listMol[i].z << endl;
    }
}

#endif