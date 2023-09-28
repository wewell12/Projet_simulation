#ifndef STRUCT_H
#define STRUCT_H

#include <string>

using namespace std;

//Structure "Espece moléculaire" -> elle permet de stocker tous les types 
typedef struct {
    string nom;
    float diametre = 1;
    float vitesse = 1;
    long int nbCopie = 0;
} EM;

typedef struct {
    int s1 = -1;
    int s2 = -1;
    int p1 = -1;
    int p2 = -1;
    float proba = 0;
} Reaction;

typedef struct {
    int type;
    float x=0;
    float y=0;
    float z=0;
} Mol;

typedef struct {
    int id;
    //coordonnées du point "initial"
    float x,y,z;
    //liste des molécules
    Mol list_mol[];
}Cube;

#endif