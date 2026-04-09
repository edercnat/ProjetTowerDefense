#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "stdbool.h"
#include "towerdefend.h"




typedef struct T_cell{
    struct T_cell *suiv;
    Tunite *pdata; //pointeur vers une unit�
} *TListePlayer;


const char* nomUniteToString(TuniteDuJeu nom);
void initListe(TListePlayer *l);
void afficheListe(TListePlayer l);
bool listeVide(TListePlayer l);
TListePlayer ajoutEnTete(TListePlayer l, Tunite *data);
TListePlayer ajoutEnFin(TListePlayer l, Tunite *data);
TListePlayer ajoutEnN(TListePlayer l, int pos, Tunite *data);
TListePlayer suppEnFin(TListePlayer l);
TListePlayer suppEnTete(TListePlayer l);
TListePlayer suppEnN(TListePlayer l, int);
TListePlayer getptrFirstCell(TListePlayer l);
TListePlayer getptrNextCell(TListePlayer l);
TListePlayer getptrLastCell(TListePlayer l);
Tunite *getptrData(TListePlayer l);
int getNbreCell(TListePlayer l);


#endif

