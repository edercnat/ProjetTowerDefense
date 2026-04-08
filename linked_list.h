#include "towerdefend.h"
#include "stdbool.h"

typedef struct T_cell{
    struct T_cell *suiv;
    Tunite *pdata; //pointeur vers une unit�
} *TListePlayer;


void initListe(TListePlayer *l);
void afficheListe(TListePlayer l);
bool listeVide(TListePlayer l);
TListePlayer ajoutEnTete(TListePlayer l, Tunite data);
TListePlayer ajoutEnFin(TListePlayer l, Tunite data);
TListePlayer ajoutEnN(TListePlayer l, int pos, Tunite data);
TListePlayer suppEnFin(TListePlayer l);
TListePlayer suppEnTete(TListePlayer l);
TListePlayer suppEnN(TListePlayer l, int);



