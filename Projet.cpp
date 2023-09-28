#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <unordered_map>
#include <list>
#include "struct.h"
#include "population_centre.h"
#include "entite_centre.h"

#define CAPACITY 100 // Size of the HashTable.
#define MAXEM 50
#define MAXR 100

using namespace std;
string type;
bool avance = true;
char c;
unordered_map<string, int> hashTable;
ifstream in;
float duree;
int ligne = 0;
EM indexEM[MAXEM];
int posIndexEM = 0;
Reaction indexR[MAXR];
int posIndexR = 0;

//Structure de lecture de fichier "Unite lexicaale"
typedef struct {
    short type;
    /*type de donnée
    1.PONCTUATION
    2.NOMBRE
    3.KEYWORDS
    4.IDENTIFIANT
    */
    float valeur;

} UL;

bool equal(UL ul1, UL ul2){
    return (ul1.type == ul2.type && ul1.valeur == ul2.valeur);
}

//rempli la hashtable avec les mots clefs
void remplir_KW(){
    hashTable["duree"] = 1;
    hashTable["diametre"] = 2;
    hashTable["vitesse"] = 3;
    hashTable["mM"] = 4;
    hashTable["uM"] = 5;
}

//renvoie la prochaine unitée lexicale du fichier ouvert
UL next_ul(){
    int etat = 1;
    string tmp = "";
    int num;
    /*liste etats
    1 -> etat initial/final
    2 -> etat C (précommentaire)
    3 -> etat Commentaire
    4 -> etat identificateur ""
    5 -> etat identificateur autre
    6 -> etat nombre
    7 -> etat fleche/tiret
    */

    UL res {-1,-1};
    while (!in.eof()) { 
        if (avance){
            c = in.get();
        }
        else {
            avance = true;
        }
        switch(etat){
            case 1:
                if ( c == '\n'){
                    ligne++;
                    continue;
                }
                else if (c == ' ' || c == '\t') {
                    continue;
                } 
                else if (c == '-'){
                    etat = 7;
                    continue;
                }
                else if (c == ';' || c == '[' || c == ']' || c == '=' || c == ':' || c == '+' || c == ','|| c == '(' || c == ')'){
                    res.type = 1;
                    res.valeur = c;
                    return res;
                }
                else if (c == '/'){
                    etat = 2;
                    continue;
                }
                else if (c == '\"'){
                    etat = 4;
                    continue;
                }
                else if (c == '0'|| c == '1' ||c == '2' || c == '3' ||c == '4' || c == '5' ||c == '6' || c == '7' ||c == '8' || c == '9'){
                    etat = 6;
                    tmp.push_back(c);
                    continue;
                }
                else if (isalpha(c)){
                    etat = 5;
                    tmp.push_back(c);
                    continue;
                }
                else if (in.eof()){
                    return res;
                }
            //etat pré commentaire
            case 2:
                if(c == '/'){
                    etat = 3;
                    continue;
                }
                else {
                    etat = 1;
                    break;
                }
            //etat commentaire
            case 3:
                while (c != '\n') { 
                    c = in.get();
                }       
                ligne ++;
                etat = 1;
                continue;
            //etat identifiant ""
            case 4:
                avance = false;
                while (c != '\"'){
                    if (c == '\n' || c == '\t'){
                        return res;
                    }
                    tmp.push_back(c);
                    c = in.get();
                }
                if (hashTable.count(tmp) > 0) {
                    res.type = 4;
                    res.valeur = hashTable[tmp];
                    return res;
                } 
                else {
                    res.type = 4;
                    num = hashTable.size() + 1;
                    hashTable[tmp] = num;
                    res.valeur = num;

                    return res;
                }
            //etat identifiant autre
            case 5:
                avance = false;
                while (c != ';' && c != '\n' && c != ' ' && c != '\t' && !in.eof() && c != '[' && c != ']' && c != '(' && c != ')' && c != ',' && c != '-'){
                    tmp.push_back(c);
                    c = in.get();
                }
                if (hashTable.count(tmp) > 0) {
                    if (hashTable[tmp] <= 5 ){
                        res.type = 3;
                    }
                    else {
                        res.type = 4;
                    }
                    res.valeur = hashTable[tmp];
                    return res;
                } else {
                    res.type = 4;
                    num = hashTable.size() + 1;
                    hashTable[tmp] = num;
                    res.valeur = num;
                    return res;
                }
                
            //etat nombre
            case 6:
                avance = false;
                while(c == '0'|| c == '1' ||c == '2' || c == '3' ||c == '4' || c == '5' ||c == '6' || c == '7' ||c == '8' || c == '9' || c == '.'){
                    tmp.push_back(c);
                    c = in.get();
                }
                res.type = 2;
                res.valeur = stof(tmp);
                return res;
            //etat fleche/tiret
            case 7:
                if (c == '>'){
                    res.type = 1;
                    res.valeur = c;
                    return res;
                }
                
                res.type = 1;
                res.valeur = '-';
                return res;
        }
    }
    res.type = -1;
    res.valeur = -1;
    return res;
    
}

//Cette fonction traite la prochaine expression lue en entrée et impléménte les données nessescaire pour la simulation.
int next_expr(){
    UL n = {1,1};
    UL erreur {-1,-1};
    UL fin_instr{1,59};
    UL tiret{1,45};
    UL deux_points{1,58};
    UL fleche{1,62};
    float r[8] = {0,0,0,0,0,0,0};//verification d'une expression
    int etat = 1;
    /*
    0.erreur
    1.instruction
    2.reaction
    3.duree
    4.diametre
    5.vitesse
    6.concentration
    */
    while( !equal(n,fin_instr) && !equal(n,erreur)){
        n = next_ul();
        if (equal(n,erreur)){
            return -1;
        }
        switch (etat){
        case 1:
            if (n.type == 1 && n.valeur == 91){
                etat = 6;
                continue;
            }
            else if (n.type == 3){
                switch((int)n.valeur){
                    case 1:
                        etat = 3;
                        continue;
                
                    case 2:
                        etat = 4;
                        continue;
                    
                    case 3:
                        etat = 5;
                        continue;

                    default: 
                        etat = 0;
                        continue;
                }

            }
            else if (n.type == 4){
                etat = 2;
                r[0] = n.valeur;
                continue;
            }
            else{
                etat = 0;
                continue;
            }
        
        //detection d'une expression
        case 2:
            if ( equal(n,deux_points) ){
                n = next_ul();
                if (n.type == 4){
                    r[1] = n.valeur;
                    n = next_ul();
                    if ( n.type == 1){
                        if (n.valeur == 43){
                            n = next_ul();
                            if (n.type == 4){
                                r[2] = n.valeur;
                                n = next_ul();
                            }
                            else{
                                etat = 0;
                                continue;
                            }
                        }
                        if (n.valeur == 62){
                            n = next_ul();
                            if (n.type == 4){
                                r[3] = n.valeur;
                                n = next_ul();
                                if (n.valeur == 43){
                                    n = next_ul();
                                    if (n.type == 4){
                                        r[4] = n.valeur;
                                        n = next_ul();
                                    }
                                    else{
                                        etat = 0;
                                        continue;
                                    }
                                }
                                if(n.type == 2){
                                    r[5] = n.valeur;
                                    n = next_ul();
                                    if(n.type == 3 && (n.valeur == 4 || n.valeur == 5)){
                                        //conversion en millimolaire
                                        if (n.valeur == 5){
                                            r[5] = r[5]/1000;
                                        }
                                        n = next_ul();
                                        if (n.type == 1){
                                            if (n.valeur == 44){
                                                n = next_ul();
                                                if (n.type == 2){
                                                    r[6] = n.valeur;
                                                    n = next_ul();
                                                    if (n.type == 3 && (n.valeur == 4 || n.valeur == 5)){
                                                        //conversion en millimolaire
                                                        if (n.valeur == 5){
                                                            r[6] = r[6]/1000;
                                                        }
                                                        n = next_ul();
                                                    }
                                                }
                                                else{
                                                    etat = 0;
                                                    continue;
                                                }
                                            }
                                            if ( equal(n,tiret)){
                                                n = next_ul();
                                                if (n.type == 2){
                                                    r[7] = n.valeur;
                                                    n = next_ul();
                                                    if ( equal(n,fin_instr)){
                                                        //verification si plusier substrat, plusier Km
                                                        if(r[2] != 0){
                                                            if(r[6] == 0){
                                                                etat = 0;
                                                                break;
                                                            }
                                                        }                                                                
                                                        
                                                        bool find = false;
                                                        int indx[5] = {-1,-1,-1,-1,-1};
                                                        //ajout des EM dans indexEM
                                                        for (int i = 0; i <= 4; i++){
                                                            find = false;
                                                            for(auto const& [key, value] : hashTable) {
                                                                if(value == r[i]) {
                                                                    for (int j = 0; j < MAXEM; j++){
                                                                        if (key == indexEM[j].nom){
                                                                            indx[i] = j;
                                                                            find = true;
                                                                            break;
                                                                        }
                                                                    }
                                                                    if(!find){
                                                                        indexEM[posIndexEM].nom = key;
                                                                        indx[i] = posIndexEM;
                                                                        posIndexEM++;
                                                                    }
                                                                    break;
                                                                }
                                                            }
                                                        }

                                                        //Enregeristrement des réactions si 1 substrat
                                                        if(indx[2] == -1){
                                                            //ajout de Es1 dans indexEM
                                                            int Es1;
                                                            indexEM[posIndexEM].nom = indexEM[indx[0]].nom + indexEM[indx[1]].nom;
                                                            Es1 = posIndexEM;
                                                            posIndexEM++;
                                                            
                                                            //set des proba
                                                            float p3 = r[7]/10000;
                                                            float p2 = r[7]/100000;
                                                            float p1 = (p2 + p3) / (0.448 * (1 + (p2+p3)*(p2+p3)) * r[5]);
                                                            
                                                            // E + s -> Es
                                                            indexR[posIndexR].s1 = indx[0];
                                                            indexR[posIndexR].s2 = indx[1];                                                           
                                                            indexR[posIndexR].p1 = Es1;
                                                            indexR[posIndexR].proba = p1;
                                                            posIndexR++;

                                                            // ES -> E + s
                                                            indexR[posIndexR].s1 = Es1;
                                                            indexR[posIndexR].p2 = indx[0];
                                                            indexR[posIndexR].p1 = indx[1];
                                                            indexR[posIndexR].proba = p2;
                                                            posIndexR++;
                                                            
                                                            //gestion du cas de 2 produits
                                                            if(indx[4] == -1){
                                                                // Es -> E + p
                                                                indexR[posIndexR].s1 = Es1;
                                                                indexR[posIndexR].p2 = indx[0];
                                                                indexR[posIndexR].p1 = indx[3];
                                                                indexR[posIndexR].proba = p3;
                                                                posIndexR++;
                                                            }
                                                            else{
                                                                int Ex;
                                                                indexEM[posIndexEM].nom = indexEM[indx[0]].nom + "x";
                                                                Ex = posIndexEM;
                                                                posIndexEM++;
                                                                //ES -> Ex + p1
                                                                indexR[posIndexR].s1 = Es1;
                                                                indexR[posIndexR].p2 = Ex;
                                                                indexR[posIndexR].p1 = indx[3];
                                                                indexR[posIndexR].proba = p3;
                                                                posIndexR++;

                                                                //Ex -> E + p
                                                                indexR[posIndexR].s1 = Ex;
                                                                indexR[posIndexR].p2 = indx[0];
                                                                indexR[posIndexR].p1 = indx[4];
                                                                indexR[posIndexR].proba = 1;
                                                                posIndexR++;
                                                            }
                                                        }
                                                        // ou 2 substrats
                                                        else{
                                                            //ajout des EMs : ES1, ES2, ES1S2
                                                            int Es1;
                                                            int Es2;
                                                            int Es1s2;
                                                            indexEM[posIndexEM].nom = indexEM[indx[0]].nom + indexEM[indx[1]].nom;
                                                            Es1 = posIndexEM;
                                                            posIndexEM++;
                                                            indexEM[posIndexEM].nom = indexEM[indx[0]].nom + indexEM[indx[2]].nom;
                                                            Es2 = posIndexEM;
                                                            posIndexEM++;
                                                            indexEM[posIndexEM].nom = indexEM[indx[0]].nom + indexEM[indx[1]].nom + indexEM[indx[2]].nom;
                                                            Es1s2 = posIndexEM;
                                                            posIndexEM++;

                                                            //set des proba
                                                            float p5 = r[7]/10000;
                                                            float p4 = r[7]/100000;
                                                            float p3 = (p5 + p4)/(0.448 * (1 + (p5+p4)*(p5+p4)) * r[6]);
                                                            float p2 = r[7]/100000;
                                                            float p1 = (p5 + p2) / (0.448 * (1 + (p2+p5)*(p2+p5)) * r[5]);

                                                            //ajout des réactions
                                                            //E + s1 -> Es1
                                                            indexR[posIndexR].s1 = indx[0];
                                                            indexR[posIndexR].s2 = indx[1];
                                                            indexR[posIndexR].p1 = Es1;
                                                            indexR[posIndexR].proba = p1;
                                                            posIndexR++;

                                                            //Es1 -> E + s1
                                                            indexR[posIndexR].s1 = Es1;
                                                            indexR[posIndexR].p1 = indx[0];
                                                            indexR[posIndexR].p2 = indx[1];
                                                            indexR[posIndexR].proba = p2;
                                                            posIndexR++;

                                                            //E + s2 -> Es2
                                                            indexR[posIndexR].s1 = indx[0];
                                                            indexR[posIndexR].s2 = indx[2];
                                                            indexR[posIndexR].p1 = Es2;
                                                            indexR[posIndexR].proba = p3;
                                                            posIndexR++;

                                                            //Es2 -> E + s2
                                                            indexR[posIndexR].s1 = Es2;
                                                            indexR[posIndexR].p1 = indx[0];
                                                            indexR[posIndexR].p2 = indx[2];
                                                            indexR[posIndexR].proba = p4;
                                                            posIndexR++;

                                                            //Es1 + s2 -> Es1s2
                                                            indexR[posIndexR].s1 = Es1;
                                                            indexR[posIndexR].s2 = indx[2];
                                                            indexR[posIndexR].p1 = Es1s2;
                                                            indexR[posIndexR].proba = p1;
                                                            posIndexR++;

                                                            //Es1s2 -> Es1 + s2
                                                            indexR[posIndexR].s1 = Es1s2;
                                                            indexR[posIndexR].p1 = Es1;
                                                            indexR[posIndexR].p2 = indx[2];
                                                            indexR[posIndexR].proba = p1;
                                                            posIndexR++;

                                                             //Es2 + s1 -> Es1s2
                                                            indexR[posIndexR].s1 = Es2;
                                                            indexR[posIndexR].s2 = indx[1];
                                                            indexR[posIndexR].p1 = Es1s2;
                                                            indexR[posIndexR].proba = p3;
                                                            posIndexR++;

                                                            //Es1s2 -> Es2 + s1
                                                            indexR[posIndexR].s1 = Es1s2;
                                                            indexR[posIndexR].p1 = Es2;
                                                            indexR[posIndexR].p2 = indx[1];
                                                            indexR[posIndexR].proba = p4;
                                                            posIndexR++;

                                                            //gestion des produits multiples
                                                            if(indx[4] == -1){
                                                                // Es -> E + p
                                                                indexR[posIndexR].s1 = Es1;
                                                                indexR[posIndexR].p2 = indx[0];
                                                                indexR[posIndexR].p1 = indx[3];
                                                                indexR[posIndexR].proba = p5;
                                                                posIndexR++;
                                                            }
                                                            else{
                                                                int Ex;
                                                                indexEM[posIndexEM].nom = indexEM[indx[0]].nom + "x";
                                                                Ex = posIndexEM;
                                                                posIndexEM++;
                                                                //ES -> Ex + p1
                                                                indexR[posIndexR].s1 = Es1;
                                                                indexR[posIndexR].p2 = Ex;
                                                                indexR[posIndexR].p1 = indx[3];
                                                                indexR[posIndexR].proba = p5;
                                                                posIndexR++;

                                                                //Ex -> E + p
                                                                indexR[posIndexR].s1 = Ex;
                                                                indexR[posIndexR].p2 = indx[0];
                                                                indexR[posIndexR].p1 = indx[4];
                                                                indexR[posIndexR].proba = 1;
                                                                posIndexR++;
                                                            }

                                                        }

                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            etat = 0;
            continue;
        
        //detection de la duree
        case 3:
            if (n.type == 1 && n.valeur == 61){
                n = next_ul();
                if (n.type == 2){
                    float Tmpduree = n.valeur;
                    n = next_ul();
                    if( n.type == 1 && n.valeur == 59){
                        duree = Tmpduree;
                        break;
                    }
                }
            }
            etat = 0;
            continue;

        //detection du diametre
        case 4:
            if (n.type == 1 && n.valeur == 40){
                n = next_ul();
                if (n.type == 4){
                    r[0] = n.valeur;
                    n = next_ul();
                    if (n.type == 1 && n.valeur == 41){
                        n = next_ul();
                        if (n.type == 1 && n.valeur == 61){
                            n = next_ul();
                            if (n.type == 2){
                                r[1] = n.valeur;
                                n = next_ul();
                                if( n.type == 1 && n.valeur == 59){
                                    bool find = false;
                                    for(auto const& [key, value] : hashTable) {
                                        if(value == r[0]) {
                                            for (int i = 0; i < MAXEM; i++){
                                                if (key == indexEM[i].nom){
                                                    indexEM[i].diametre = r[1];
                                                    find = true;
                                                    break;
                                                }
                                            }
                                            if(!find){
                                                indexEM[posIndexEM].nom = key;
                                                indexEM[posIndexEM].diametre = r[1];
                                                posIndexEM++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            etat = 0;
            continue;

        //detection de la vitesse
        case 5:
            if (n.type == 1 && n.valeur == 40){
                n = next_ul();
                if (n.type == 4){
                    r[0] = n.valeur;
                    n = next_ul();
                    if (n.type == 1 && n.valeur == 41){
                        n = next_ul();
                        if (n.type == 1 && n.valeur == 61){
                            n = next_ul();
                            if (n.type == 2){
                                r[1] = n.valeur;
                                n = next_ul();
                                if( n.type == 1 && n.valeur == 59){
                                    bool find = false;
                                    for(auto const& [key, value] : hashTable) {
                                        if(value == r[0]) {
                                            for (int i = 0; i < MAXEM; i++){
                                                if (key == indexEM[i].nom){
                                                    indexEM[i].vitesse = r[1];
                                                    find = true;
                                                    break;
                                                }
                                            }
                                            if(!find){
                                                indexEM[posIndexEM].nom = key;
                                                indexEM[posIndexEM].vitesse = r[1];
                                                posIndexEM++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            etat = 0;
            continue;

        //detection de la concentration
        case 6:
            if (n.type == 4){
                r[0] = n.valeur;
                n = next_ul();
                if (n.type == 1 && n.valeur == 93){
                    n = next_ul();
                    if (n.type == 1 && n.valeur == 61){
                        n = next_ul();
                        if (n.type == 2){
                            r[1] = n.valeur;
                            n = next_ul();
                            if(n.type == 3 && (n.valeur == 4 || n.valeur == 5)){
                                //conversion en mcromolaire
                                if (n.valeur == 4){
                                    r[1] = r[1]*1000;
                                }
                                n = next_ul();
                                if( n.type == 1 && n.valeur == 59){
                                    bool find = false;
                                    for(auto const& [key, value] : hashTable) {
                                        if(value == r[0]) {
                                            for (int i = 0; i < MAXEM; i++){
                                                if (key == indexEM[i].nom){
                                                    indexEM[i].nbCopie = r[1] * (602.252);//*100;
                                                    find = true;
                                                    break;
                                                }
                                            }
                                            if(!find){
                                                indexEM[posIndexEM].nom = key;
                                                indexEM[posIndexEM].nbCopie = r[1] * (602.252);//*100;
                                                posIndexEM++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            etat = 0;
            continue;
        default:
            n.type = erreur.type;
            n.valeur = erreur.valeur;
            continue;
        }
    }
    if ( equal(n,erreur) ){
        cout << "erreur ligne " << ligne << "\n";
        return -2;
    }
    
    return 0;
}

//cette fonction appelle next_expr()
void lecture(){
    int i = 0;
    while (i == 0 ){
        i = next_expr();
    }
}

void Population_centre(Reaction indexR[],EM indexEM[20],float duree){
    //initialisation
    fstream fout;
    bool pair = true;
    float calcDur = 0;
    float Att = duree/10;
    fout.open("evolution_reaction.csv", ios::out);
    
    //ajout dans le fichier les entrées
    fout << "temps,";
    for ( int i = 0; i < MAXEM; i++){
        if(!indexEM[i].nom.empty()){
            fout << indexEM[i].nom << ",";
        }
    }
    fout << "\n";
    fout << "0,";

    //ajout des concentration basiques
    for ( int i = 0; i < MAXEM; i++){
        if(!indexEM[i].nom.empty()){
            fout << indexEM[i].nbCopie << ",";
        }
    }
    fout << "\n";
    int compt = 0;

    //fonction qui applique gilles tartes
    while (duree > 0){
        GillesTarte(indexR,indexEM);
        duree -= 0.001;

        //toutles 0.1 secondes entrer les valeurs dans le fichier csv
        if( compt == 100){
            calcDur += 0.1;
            fout << fixed << setprecision(1) << calcDur << ",";
            for ( int i = 0; i < MAXEM; i++){
                if(!indexEM[i].nom.empty()){
                    fout << indexEM[i].nbCopie << ",";
                }
            }
            fout << "\n";
            compt = 0;
        }
        else{
            compt++;
        }
    }
}



int main(int argc, char* argv[]){
    // remplir la table avec les mots clefs basiques
    remplir_KW();
    //ouvrir le fichier à lire
    in.open(argv[1]);

    //affiche les 20 premieres unite lexicales
    /*UL n;
    for (int i = 1;i<=50;i++){
        n = next_ul();
        switch(n.type){
            case 1 : type = "ponctuation";break;
            case 2 : type = "nombre";break;
            case 3 : type = "keyword";break;
            case 4 : type = "identifiant";break;
        }
        cout << "Unité lexicale " << i << ": type -> "<< type << "; val -> " << n.valeur << "\n";
    }*/
    lecture();

    //afiche la hash table
    /*for (auto &entry : hashTable) {
        cout << entry.first << ": " << entry.second << endl;
    }*/

    //affiche le tableau ders EM
    cout << "\ntable des Especes Moléculaire : \n";
    int cpt = 0;
    for ( EM i : indexEM){
        if(!i.nom.empty()){
            cout << cpt << ". nom : " << i.nom << " | diametre : " << i.diametre << " | vitesse : " << i.vitesse << " | nb : " << i.nbCopie << "\n";
            cpt ++;
        }
    }

    //affiche le tableau des réactions
    cpt = 1;
    cout << "\ntable des réactions : \n";
    for ( Reaction i : indexR){
        if (i.proba != 0){
            cout << cpt << ". " << i.s1;
            if (i.s2 != -1){
                cout << " + " << i.s2;
            } 
            cout << " -> " << i.p1 ;
            if(i.p2 != -1){
                cout << " + " << i.p2;
            }
            cout << "  [" << i.proba <<"]\n";
            cpt++;
        }
    }
    /*----------Algo entite centree----------*/
    
    //entite(indexR,indexEM,duree);



    /*----------Algo population centree----------*/
    Population_centre(indexR,indexEM,duree);
    
    

    /*cout << "\ntable des Especes Moléculaire après gillou : \n";
    cpt = 0;
    for ( EM i : indexEM){
        if(!i.nom.empty()){
            cout << cpt << ". nom : " << i.nom << " | diametre : " << i.diametre << " | vitesse : " << i.vitesse << " | nb : " << i.nbCopie << "\n";
            cpt ++;
        }    
    }
    */
    //GillesTarte(indexR,indexEM);
    //GillesTarte(indexR,indexEM);

    return 0;
}