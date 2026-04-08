#include "linked_list.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"


const char* nomUniteToString(TuniteDuJeu nom) {
    switch (nom) {
        case gargouille:  return "Gargouille";
        case archer:      return "Archer";
        case chevalier:   return "Chevalier";
        case dragon:      return "Dragon";
        case tourSol:     return "Tour Sol";
        case tourAir:     return "Tour Air";
        case tourRoi:     return "Tour Roi";

        default:          return "Inconnu";
    }
}

void initListe(TListePlayer *l){
    *l = NULL;
}


bool listeVide(TListePlayer l){
    return (l == NULL);
}

void afficheListe(TListePlayer l){
    TListePlayer tmp = l;
    int compteur = 0;

    if (listeVide(l)){
        printf("Liste vide\n");
    }

    while (tmp != NULL)
    {
        printf("%d : %s \n", compteur, nomUniteToString(tmp->pdata->nom));
        tmp = tmp->suiv;
        compteur += 1;
    }
}

TListePlayer ajoutEnTete(TListePlayer l, Tunite *data){

    struct T_cell *nouv = (struct T_cell*)malloc(sizeof(struct T_cell));
    nouv->pdata = data;

    if (listeVide(l)){
        nouv->suiv = NULL;
        return nouv;
    }

    else {
        nouv->suiv = l;
        return nouv;
    }
}


TListePlayer ajoutEnFin(TListePlayer l, Tunite *data){

    struct T_cell *nouv = (struct T_cell*)malloc(sizeof(struct T_cell));
    nouv->pdata = data;

    TListePlayer tmp = l;

    if (listeVide(l)){
        nouv->suiv = NULL;
        return nouv;
    }

    while (tmp->suiv != NULL)  
    {
        tmp = tmp->suiv;
    }

    tmp->suiv = nouv;
    nouv->suiv = NULL;

    return l;  

}


TListePlayer ajoutEnN(TListePlayer l, int pos, Tunite *data){

    if (listeVide(l) || pos == 0){
        l = ajoutEnTete(l, data);
        return l;
    }

    else {

        TListePlayer tmp = l;
        int compteur = 0;

        while (tmp->suiv != NULL && compteur < pos-1)
        {
            compteur ++;
            tmp = tmp->suiv;
        }

        if (compteur < pos-1)
        {
            printf("Erreur : Indice %d trop élevé\n", pos);
            return l;
        }
        else {
            struct T_cell* nouv = (struct T_cell*)malloc(sizeof(struct T_cell));
            nouv->pdata = data;
            nouv->suiv = tmp->suiv;
            tmp->suiv = nouv;
            return l;
        }     
    }
}


TListePlayer suppEnFin(TListePlayer l){

    if (listeVide(l)) return NULL; //Cas liste vide
    else if (l->suiv == NULL) {
        free(l->pdata); 
        free(l); 
        return NULL;
    }; //Cas liste à 1 élément

    TListePlayer tmp = l;
    
    while (tmp->suiv->suiv != NULL)
    {
        tmp = tmp->suiv;
    }

    free(tmp->suiv->pdata);
    free(tmp->suiv);
    tmp->suiv = NULL;


    return l;
}

TListePlayer suppEnTete(TListePlayer l){
    if (listeVide(l)) return NULL; //Cas liste vide

    TListePlayer tmp = l;

    l = l->suiv;
    free(tmp->pdata);
    free(tmp);

    return l;
}


TListePlayer suppEnN(TListePlayer l, int pos){

    if (listeVide(l)) {return NULL;}
    else if (l->suiv == NULL && pos > 0) {printf("Erreur : Indice %d trop élevé\n", pos); return l;}
    else if (pos == 0) {return suppEnTete(l);};

    TListePlayer tmp = l;
    int compteur = 0;

    while (tmp->suiv->suiv != NULL && compteur < pos-1)
    {
        tmp = tmp->suiv;
        compteur++;
    }
    
    if (compteur < pos-1) {printf("Erreur : Indice %d trop élevé\n", pos); return l;}

    else {

        struct T_cell *tmp2 = tmp->suiv;
        tmp->suiv = tmp->suiv->suiv;
        free(tmp2->pdata);
        free(tmp2);
        

        return l;
    }

}



TListePlayer getptrFirstCell(TListePlayer l){
    return l;
}

TListePlayer getptrNextCell(TListePlayer l){
    if (l->suiv != NULL){
        return l->suiv;
    }
    else {
        return NULL;
    }
}

TListePlayer getptrLastCell(TListePlayer l){
    while (l->suiv !=NULL)
    {
        l = l->suiv;
    }

    return l;    
}

Tunite *getptrData(TListePlayer l){
    return l->pdata;
}


int getNbreCell(TListePlayer l){
    int compteur = 0;

    while (l != NULL)
    {
        compteur++;
        l = l->suiv;
    }
    
    return compteur;
}



