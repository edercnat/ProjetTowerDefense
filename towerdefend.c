#include <stdio.h>
#include <stdlib.h>
#include "towerdefend.h"
#include "linked_list.h"


//typedef Tunite* ** TplateauJeu;

TplateauJeu AlloueTab2D(int largeur, int hauteur){
    TplateauJeu jeu;
    jeu = (Tunite***)malloc(sizeof(Tunite**)*largeur);
    for (int i=0;i<largeur;i++){
        jeu[i] = (Tunite**)malloc(sizeof(Tunite*)*hauteur);
    }
    return jeu;  //tab2D contenant des pointeurs
}
void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur){
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }
}


float **AlloueTab2DBis(int hauteur, int largeur){
    float **tab = (float **)malloc(sizeof(float)*hauteur-1);

    for (int i = 0; i < hauteur; i++){
        tab[i] = (float *)malloc(sizeof(float)*largeur);
    }

    return tab;
}


void append(float **tab, float val, int NewTaille){

    float *newTab = realloc(*tab, sizeof(float)*(NewTaille));

    if (newTab != NULL){
        newTab[NewTaille-1] = val;
    }
    
    *tab = newTab;
}



/*
void ecritCheminVersleHaut  : permet d'initilaiser le tab chemin de distance cases (avec des coord x y) dans une direction, ? partir d'un point x y donn?

int **chemin  : tab de coordonn?es x y du chemin
int *ichemin  : indice de la case du chemin d'o? on part
int *xdepart, int *ydepart : valeur en x y de d?part pouri la premiere case
int distance  : distance sur laquelle on va ?crire des coordonn?es dans le tab chemin
int *distanceMaxRestante : securit? pour ne pas sortir de la plage d'indice de chemin
*/
void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int y;
        for (y=*ydepart;y>*ydepart-distance;y--){
            chemin[*ichemin][X]= *xdepart;
            chemin[*ichemin][Y]= y;
            (*ichemin)++;
        }
        *ydepart=y;
    }
    else printf("erreur longueur chemin\n");
}
void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x<*xdepart+distance;x++){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}
void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x>*xdepart-distance;x--){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}

int **initChemin(){
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = 18;  //et non 19
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int distanceMaxRestante = NBCOORDPARCOURS;

    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 5, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);

    return chemin;  //tab2D contenant des pointeurs
}

void afficheCoordonneesParcours(int **chemin, int nbcoord){
    printf("Liste coordonnees: ");
    for (int i=0; i<nbcoord; i++){
        printf("(%d, %d)",chemin[i][X], chemin[i][Y]);
    }
    printf("\nfin liste coordonn?es\n");
}

void freeChemin(int **tab){
    for (int j=0;j<NBCOORDPARCOURS;j++){
        free(tab[j]);  //libere chaque case, qui est un tableau de 2 cases
    }
    free(tab);
}

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur){
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[7]={"s", "a", "r", "A", "C", "D", "G"};

    printf("\n");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){

            if (jeu[i][j] != NULL){
                    printf("%s",InitialeUnite[jeu[i][j]->nom]);
            }
            else printf(".");  //cad pas d'unit? sur cette case
        }
        printf("\n");
    }
}

Tunite *creeTourSol(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourSol;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 120;
    nouv->portee = 5;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourAir(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourAir;
    nouv->cibleAttaquable = air;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 100;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourRoi(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 800;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 180;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeDragon(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 1.1;
    nouv->degats = 180;
    nouv->portee = 2;
    nouv->vitessedeplacement = 4;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

/*Stats à revoir*/
Tunite *creeArcher(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 150;
    nouv->vitesseAttaque = 0.75;
    nouv->degats = 110;
    nouv->portee = 2;
    nouv->vitessedeplacement = 5.5;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

/*Stats à revoir*/
Tunite *creeGargouille(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 100;
    nouv->vitesseAttaque = 0.60;
    nouv->degats = 80;
    nouv->portee = 1;
    nouv->vitessedeplacement = 3.5;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

Tunite *creeChevalier(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 250;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 80;
    nouv->portee = 1;
    nouv->vitessedeplacement = 1;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}



/*
Cherche la cellule du roi et regarde si elle est détruite

Params : 
        TListePlayer player -> Liste du joueur avec le roi

Returns: 
        true  -> la tour du roi est détruite
        false -> la tour n'est pas détruite
        NULL  -> si la liste ne contient pas de roi
(Testée et 100% fonctionnelle (normallement))
*/
bool tourRoiDetruite(TListePlayer player){

    TListePlayer tmp = player;

    for (int i = 0; i < getNbreCell(player); i++){

        if (getptrData(player)->nom == tourRoi){ //Condition validée si la cellule pointée est celle du roi

            if (getptrData(player)->pointsDeVie > 0){ //Si le roi a plus de 0hp return false (Tour du roi non détruite)
                return false;
            }
            else { //Tour du roi détruite
                return true;
            }
        }
        else { //On avance dans la liste pour trouver le roi
            tmp = getptrNextCell(tmp);
        }
    }

    printf("Erreur: le roi n'est pas présent dans cette liste \n");
    return NULL;
}

/*Positionne les cellules d'un joueur sur le tableau de jeu
Params: 
    TListePlayer player -> Liste du joueur
    TplateauJeu jeu     -> plateau du jeu

Returns:
    Met a jour le tableau jeu avec les adresses

*/
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu){
    TListePlayer tmp = player;

    for (int i = 0; i < getNbreCell(player); i++){
        if (jeu[getptrData(tmp)->posX][getptrData(tmp)->posY] == NULL){
            jeu[getptrData(tmp)->posX][getptrData(tmp)->posY] = getptrData(tmp); //case = adresse de l'unité
        }
        else {printf("Erreur: positionnement %s ; case (%d, %d) déjà occupée par %s\n",
            nomUniteToString(getptrData(tmp)->nom), getptrData(tmp)->posX, getptrData(tmp)->posY, nomUniteToString((jeu[getptrData(tmp)->posX][getptrData(tmp)->posY])->nom));
        };

        tmp = getptrNextCell(tmp);
    }
}


/*
!!!Fonctionnement pas sûr!!! 
J'ai du modifier la signature pour avoir un truc qui marche bien.
Je ne sais pas si elle doit modifier elle meme le tableau de jeu mais sinon il y a une erreur avec l'appelle d'affichage car il ne sait pas comment afficher une adresse free.
Donc fonction a vérifier
*/
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu){
    if (*player == NULL) {printf("Erreur : Liste déjà vide\n"); return;};
    TListePlayer tmp = *player;

    int compteur = 0;
    while (tmp->pdata != UniteDetruite && tmp->suiv != NULL)
    {
        tmp = tmp->suiv;
        compteur++;
    }

    if (tmp->pdata == UniteDetruite){
        jeu[getptrData(tmp)->posX][getptrData(tmp)->posY] = NULL;
        *player = suppEnN(*player, compteur);
    }
    else printf("Erreur: l'unité n'est pas présente dans la liste\n");
}

/*Ajoute simplement une unité a la fin de la liste d'un joueur
Objectif: etre combiné à PositionnePlayerOnPlateau à chaque appel de boucle pour mettre à jour

Ajouter l'indice de l'unité créée dans la liste des indices
*/
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite, float **posInd){
    *player = ajoutEnFin(*player, nouvelleUnite);
    append(posInd, 0., getNbreCell(*player));
}

/*False = la case est libre
  True = la case n'est pas libre
*/
bool verifCaseLibre(float ind, float* posInd, int taille){
    bool verif = false;
    int compteur = 0;

    while (verif != true && compteur < taille)
    {
        if ((int)ind == (int)posInd[compteur]) verif = true;
        else {
            compteur++;
        }
    }
    
    return verif;
}


/*Recalcul les coordonées pour chaque unité pour une nouvelle boucle

La vitesse de déplacement est stockée en float (pas pratique pour un tableau) donc il faudra utiliser avec une fonction pour determiner sa case correspondante à l'arrondi

calculNewPosRaw renvoit seulement un tableau avec un indice en float (qui devrait etre arrondi lors du placement des troupes pour correspondre a une case)

Ex : Dragon est en (0,0) et il a une vitesse de deplacement de 1.5 m/s 
    - boucle 1 : PosInd [1.5] -> sera arrondi à 1 et donc placé case 1
    - boucle 2 : PosInd [3] -> sera placé à la case 3 (aucune vitesse n'est perdue)

*/
void calculNewInd(TListePlayer player, float *posInd){

    TListePlayer tmp = player;
    int taille = getNbreCell(player);
    for (int i = 0; i < taille; i++){

        float new_ind = posInd[i] + getptrData(tmp)->vitessedeplacement;
        if (!verifCaseLibre(new_ind, posInd, taille)){
            posInd[i] = new_ind;
        }
        else {
            for( float j = new_ind-1; (int)j > (int)posInd[i]; j--){
                if (!verifCaseLibre(j, posInd, taille)){
                    posInd[i] = j;
                }
            }
        }

        tmp = getptrNextCell(tmp);
    }
}







void print_list(float *l, int taille){
    for (int i = 0; i < taille; i++){
        printf("%f ; ", l[i]);
    }
    printf("\n");
}