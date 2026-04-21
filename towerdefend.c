#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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





//Initialise le chemin de manière aléatoire
int **initChemin(){
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = 18;  //et non 19
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int distanceMaxRestante = NBCOORDPARCOURS;
    bool chemin_fini = false;

    while (!chemin_fini) { //On entre dans la boucle de creation du chemin
                           //Si le chemin tombe sur une impasse, il recommence à 0

        int last_dir=-1;
        int dir;

        int cases_forcee = 0;

        chemin[0][0] = xdepart;
        chemin[0][1] = ydepart;

        bool impasse = false;

        for (int o = 1; o < NBCOORDPARCOURS; o++){ //Boucle pour determiner chaque cases du tableau

            bool moveValid = false;
            int nextX, nextY;

            int tentatives = 0;

            while (!moveValid && tentatives < 40) { //On entre dans la boucle pour decider du choix de la case indice o

                if (cases_forcee > 0){
                    dir = last_dir;
                }
                else {
                    dir = rand()%3;
                }

                tentatives++;
                bool DemiTour = false;

                if (dir == 0 && last_dir == 1) DemiTour = true;
                if (dir == 1 && last_dir == 0) DemiTour = true;
                // if (dir == 2 && last_dir == 3) DemiTour = true;
                // if (dir == 3 && last_dir == 2) DemiTour = true;

                if (!DemiTour){
                    nextX = chemin[o-1][0];
                    nextY = chemin[o-1][1];

                    switch (dir) {
                        case 0: nextX++; break;
                        case 1: nextX--; break;
                        case 2: nextY--; break;
                    }

                    if (nextX < LARGEURJEU && nextX >= 0 && nextY < HAUTEURJEU && nextY >= 0){
                        bool caseDejaUtilisee = false;
                        for (int q = 0; q < o; q++){
                            if (chemin[q][0] == nextX && chemin[q][1]== nextY) {caseDejaUtilisee = true; break;}
                        }

                        if (!caseDejaUtilisee){
                            moveValid = true;

                            chemin[o][0] = nextX;
                            chemin[o][1] = nextY;

                            if (cases_forcee > 0) {
                                cases_forcee--;
                            }

                            else if (last_dir != -1 && dir != last_dir) {
                                cases_forcee++;
                            }

                            last_dir = dir;
                        }
                    }
                }
            }

            if (!moveValid){
                impasse = true;
                break;
            }
        }

        if (!impasse){
            chemin_fini = true;
        }

    }

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

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur, int **chemin){
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[7]={"s", "a", "r", "A", "C", "D", "G"};

    bool chemin2D[hauteur][largeur];

    for (int i = 0; i < hauteur; i++){
        for (int j = 0; j < largeur; j++){
            chemin2D[i][j] = false;
        }
    }

    for (int i = 0; i < NBCOORDPARCOURS; i++){
        chemin2D[chemin[i][1]][chemin[i][0]] = true;
    }

    printf("\n");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){

            if (jeu[i][j] != NULL){
                    printf("%s",InitialeUnite[jeu[i][j]->nom]);
            }
            else if (chemin2D[j][i] == true) {
                printf(" ");
            }
            else {
                printf(".");
            };  //cad pas d'unit? sur cette case
        }
        printf("\n");
    }
    printf("\n");
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
    nouv->indChemin = 0;
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
    nouv->indChemin = -1.;

    //nouv->cible = NULL;
    return nouv;
}

Tunite *creeTourRoi(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 100;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 180;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->indChemin = -1.;

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
    nouv->vitessedeplacement = 1;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->indChemin = 0;

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
    nouv->portee = 5;
    nouv->vitessedeplacement = 1;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->indChemin = 0;

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
    nouv->vitessedeplacement = 1;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->indChemin = 0;

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
    nouv->indChemin = 0;

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
                player->pdata->pointsDeVie = 0;
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
            jeu[getptrData(tmp)->posX][getptrData(tmp)->posY] = tmp->pdata; //case = adresse de l'unité
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
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite){
    *player = ajoutEnFin(*player, nouvelleUnite);
}

/*False = la case est libre
  True = la case n'est pas libre
*/
bool verifCaseLibre(float ind, TListePlayer listeAtk, Tunite *uniteIgnoree){
    bool verif = false;
    TListePlayer temp = listeAtk;

    while (verif != true && temp != NULL)
    {
        if (temp->pdata != uniteIgnoree){
            if ((int)ind == (int)temp->pdata->indChemin) verif = true;
        }

        temp = temp->suiv;
    }

    return verif;
}


/*Recalcul les coordonées pour chaque unité pour une nouvelle boucle

La vitesse de déplacement est stockée en float (pas pratique pour un tableau) donc il faudra utiliser avec une fonction pour determiner sa case correspondante à l'arrondi

calculNewPos met a jour le champ indChemin avec un indice en float (qui devrait etre arrondi lors du placement des troupes pour correspondre a une case)

Ex : Dragon est en (0,0) et il a une vitesse de deplacement de 1.5 m/s
    - boucle 1 : PosInd [1.5] -> sera arrondi à 1 et donc placé case 1
    - boucle 2 : PosInd [3] -> sera placé à la case 3 (aucune vitesse n'est perdue)

*/
void calculNewInd(TListePlayer player, int **chemin){

    TListePlayer tmp = player;
    int indKing = NBCOORDPARCOURS - 1;
    int taille = getNbreCell(player);

    for (int i = 0; i < taille; i++){ //On calcul le nouvel indice de chaque joueur

        float new_ind = tmp->pdata->indChemin + getptrData(tmp)->vitessedeplacement;

        if (!canDamageKing(tmp->pdata, chemin) && (indKing - new_ind) >= tmp->pdata->portee ){
            //Si je ne peux pas encore frapper le Roi et que ma future position est encore trop loin de ma portée, alors j'avance

            if (!verifCaseLibre(new_ind, player, tmp->pdata)){
                //Si la case à distance (vitessedeplacement) est libre on se deplace
                tmp->pdata->indChemin = new_ind;
            }
            else {
                for( float j = new_ind-1; (int)j > (int)tmp->pdata->indChemin; j--){
                    //Sinon on traque toutes les cases avant (de la plus loin a la plus proche) pour trouver ou se mettre
                    if (!verifCaseLibre(j, player, tmp->pdata) && j <= indKing){
                        tmp->pdata->indChemin = j;
                        break;
                    }
                }
        }
        }

        else if (!canDamageKing(tmp->pdata,  chemin) && (indKing - new_ind) < tmp->pdata->portee){
        //Si je ne peux pas attaquer le roi et que ma futur position est a distance, je calcule la position idéale


            if (farestDist(tmp->pdata, chemin, player) == 0 ){ //Cela veut dire qu'aucune case à portée n'est disponible
                for( float j = new_ind-1; (int)j > (int)tmp->pdata->indChemin; j--){ //Alors on traque la plus proche du roi disponible qui ne sera pas a sa portée
                    if (!verifCaseLibre(j, player, tmp->pdata) && j < indKing){
                        tmp->pdata->indChemin = j;
                        break;
                    }
                }
            }

            else { //Cela veut dire qu'une case à portée du roi est disponible
                tmp->pdata->indChemin = tmp->pdata->indChemin + farestDist(tmp->pdata, chemin, player); //On avance sur cette case
            }
        }

        //Si une unité est déjà à portée du roi, elle ne bouge pas

        tmp = getptrNextCell(tmp);
    }
}



/*Met à jour toutes les coordonnées des unités par rapport à la indice de position dans le chemin*/
void updateCoord(TListePlayer player, int **chemin, TplateauJeu jeu){

    TListePlayer tmp = player;
    TListePlayer tmp2 = player;

    for (int i = 0; i < getNbreCell(player); i++){
        jeu[tmp2->pdata->posX][tmp2->pdata->posY] = NULL;
        tmp2 = tmp2->suiv;
    }

    for (int i = 0; i < getNbreCell(player); i++){
        tmp->pdata->posX = chemin[((int)tmp->pdata->indChemin)][0];
        tmp->pdata->posY = chemin[((int)tmp->pdata->indChemin)][1];

        tmp = tmp->suiv;
    }
}

/*Creer une unite de manière aleatoire
Return: type <Tunite *> -> poiteur vers une unité*/
Tunite *randomUnite(int** chemin){

    int result = rand()%4;

    Tunite* unite;
    switch (result)
    {
    case 0:
        unite = creeDragon(chemin[0][0],chemin[0][1]);
        break;
    case 1:
        unite = creeArcher(chemin[0][0],chemin[0][1]);
        break;
    case 2:
        unite = creeGargouille(chemin[0][0],chemin[0][1]);
        break;
    case 3:
        unite = creeChevalier(chemin[0][0],chemin[0][1]);
        break;
    default:
        break;
    }

    return unite;
}

/*L'unité est-elle a distance pour frapper le roi
Return:
type <bool> : true  -> elle peut taper
              false -> elle ne peut pas taper
*/
bool canDamageKing(Tunite *unite, int **chemin){
    int indKingX = chemin[NBCOORDPARCOURS-1][0];
    int indKingY = chemin[NBCOORDPARCOURS-1][1];

    int x1 = unite->posX + unite->portee;
    int y1 = unite->posY + unite->portee;

    int x2 = unite->posX - unite->portee;
    int y2 = unite->posY - unite->portee;

    if ((indKingX <= x1 && indKingX >= x2) && (indKingY <= y1 && indKingY >= y2)) {
        return true;
    }
    return false;
}


/*Determine la case disponible la plus éloignée du Roi tout en restant à la plus grande distance possible pour attaquer
(à combiner avec calculNewInd);

Return: type <int> -> nbre de cases à avancer (entre 0 et vitessedeplacement)
*/
int farestDist(Tunite *unite, int** chemin, TListePlayer playerAtk){
    int indKing = NBCOORDPARCOURS-1;
    int distance = indKing - unite->indChemin;


    if (distance <= unite->portee){
        return 0;
    }

    for (int i = 1; i <= unite->vitessedeplacement; i++){ //On test toutes les cases dans la range du deplacement

        int targetId = unite->indChemin + i;

        if (targetId >= NBCOORDPARCOURS-1) break; //Si la case a tester est superieure ou egale à la case du roi
        if (distance-i <= unite->portee) //Si on est a distance de tir
        {
            int x = chemin[targetId][0];
            int y = chemin[targetId][1];

            if ( x>= 0 && x < LARGEURJEU && y >= 0 && y < HAUTEURJEU){
                if (!verifCaseLibre(targetId, playerAtk, unite)){
                    return i;
                }
            }
        }
    }
    return 0;
}

void atkKing(Tunite * unite, TListePlayer playerKing, int **chemin){

    float degats = unite->degats / unite->vitesseAttaque; //degats par seconde
    // int vieKing = playerKing->pdata->pointsDeVie; //Le roi est forcement la tete

    if (canDamageKing(unite, chemin))
    {
        playerKing->pdata->pointsDeVie = playerKing->pdata->pointsDeVie - (int)degats;
        // printf("Touché\n");
    };
}


void print_list(float *l, int taille){
    for (int i = 0; i < taille; i++){
        printf("%f ; ", l[i]);
    }
    printf("\n");
}


void printOutUnite(FILE *pointeur, Tunite *unite){

    fprintf(pointeur,"%d ", unite->nom);
    fprintf(pointeur,"%d ", unite->pointsDeVie);
    fprintf(pointeur,"%d ", unite->posX);
    fprintf(pointeur,"%d ", unite->posY);
    fprintf(pointeur,"%f ", unite->indChemin);

};

void SaveState(TListePlayer PlayerAtk, TListePlayer PlayerRoi, char *file, int **chemin){
    FILE *f_out;
    int nbAtk = getNbreCell(PlayerAtk);
    int nbDef = getNbreCell(PlayerRoi);

    if ((f_out = fopen(file,"w")) == NULL)
      {
        fprintf(stderr, "\nErreur: Impossible de sauvegarder %s\n",file);
        return;
      }

    TListePlayer tmpAtk = PlayerAtk;
    TListePlayer tmpRoi = PlayerRoi;

    fprintf(f_out, "%d \n", nbAtk);

    while (tmpAtk != NULL) {

        printOutUnite(f_out, tmpAtk->pdata);

        tmpAtk = tmpAtk->suiv;
    }
    fprintf(f_out, "\n\n");
    fprintf(f_out, "%d \n", nbDef);

    while (tmpRoi != NULL){
        printOutUnite(f_out, tmpRoi->pdata);

        tmpRoi = tmpRoi->suiv;
    }

    fprintf(f_out, "\n\n");

    for (int i = 0; i < NBCOORDPARCOURS; i++){
        for (int j = 0; j < 2; j++){
            fprintf(f_out, "%d ", chemin[i][j]);
        }
    }

    fclose(f_out);

}



int** repriseSave(char *file, TListePlayer *playerAtk, TListePlayer *playerRoi){
    int nbAtk;
    int nbRoi;
    FILE *f_in;

    if ((f_in = fopen(file,"r")) == NULL)
      {
        fprintf(stderr, "\nErreur: Impossible de sauvegarder %s\n",file);
      };

    fscanf(f_in, "%d", &nbAtk);

    for (int i = 0; i < nbAtk; i++){
        int typeUnite;
        int posX, posY;
        int pointDeVie;
        float indChemin;

        fscanf(f_in, "%d %d %d %d %f", &typeUnite,&pointDeVie, &posX, &posY, &indChemin);

        Tunite *nouvelleUnite = NULL;

        switch (typeUnite) {
            case dragon: nouvelleUnite = creeDragon(posX, posY); break;
            case gargouille : nouvelleUnite = creeGargouille(posX, posY); break;
            case archer: nouvelleUnite = creeArcher(posX, posY); break;
            case chevalier: nouvelleUnite = creeChevalier(posX, posY); break;
            case tourAir: nouvelleUnite = creeTourAir(posX, posY); break;
            case tourSol: nouvelleUnite = creeTourSol(posX, posY); break;
            case tourRoi: nouvelleUnite = creeTourRoi(posX, posY); break;
            default: printf("Erreur: unité inexistante\n"); break;
        }

        AjouterUnite(playerAtk, nouvelleUnite);
        nouvelleUnite->indChemin = indChemin;
        nouvelleUnite->pointsDeVie = pointDeVie;
    }

    fscanf(f_in, "%d", &nbRoi);

    for (int i = 0; i < nbRoi; i++){
        int typeUnite;
        int posX, posY;
        int pointDeVie;
        float indChemin;

        fscanf(f_in, "%d %d %d %d %f", &typeUnite,&pointDeVie, &posX, &posY, &indChemin);

        Tunite *nouvelleUnite = NULL;

        switch (typeUnite) {
            case tourAir: nouvelleUnite = creeTourAir(posX, posY); break;
            case tourSol: nouvelleUnite = creeTourSol(posX, posY); break;
            case tourRoi: nouvelleUnite = creeTourRoi(posX, posY); break;
            default: printf("Erreur: unité inexistante\n");
        }

        AjouterUnite(playerRoi, nouvelleUnite);
        nouvelleUnite->pointsDeVie = pointDeVie;
    }

    int **chemin = (int **)malloc(sizeof(int *)*NBCOORDPARCOURS);

    for (int i = 0; i < NBCOORDPARCOURS; i++){

        chemin[i] = (int*)malloc(sizeof(int)*2);

        for (int j = 0; j < 2; j++){
            fscanf(f_in, "%d", &chemin[i][j]);
        }
    }

    fclose(f_in);

    return chemin;
}



void SaveStateBin(TListePlayer PlayerAtk, TListePlayer PlayerRoi, char *file, int **chemin){
    FILE *f_out = fopen(file, "wb");

    if (f_out == NULL) {
        fprintf(stderr, "\nErreur: Impossible de sauvegarder %s\n", file);
        return;
    }

    int nbAtk = getNbreCell(PlayerAtk);
    fwrite(&nbAtk, sizeof(int), 1, f_out);

    TListePlayer tmpAtk = PlayerAtk;
    while (tmpAtk != NULL) {
        fwrite(tmpAtk->pdata, sizeof(Tunite), 1, f_out);
        tmpAtk = tmpAtk->suiv;
    }

    int nbRoi = getNbreCell(PlayerRoi);
    fwrite(&nbRoi, sizeof(int), 1, f_out);

    TListePlayer tmpRoi = PlayerRoi;
    while (tmpRoi != NULL) {
        fwrite(tmpRoi->pdata, sizeof(Tunite), 1, f_out);
        tmpRoi = tmpRoi->suiv;
    }

    for (int i = 0; i < NBCOORDPARCOURS; i++){
        fwrite(chemin[i], sizeof(int), 2, f_out);
    }

    fclose(f_out);
}



int** repriseSaveBin(char *file, TListePlayer *playerAtk, TListePlayer *playerRoi){
    FILE *f_in = fopen(file, "rb");
    if (f_in == NULL) {
        fprintf(stderr, "\nErreur: Impossible de charger %s\n", file);
        return NULL;
    }


    //Lecture liste Attaque
    int nbAtk;
    fread(&nbAtk, sizeof(int), 1, f_in);

    for (int i = 0; i < nbAtk; i++){
            Tunite *nouvelleUnite = (Tunite*)malloc(sizeof(Tunite));
            fread(nouvelleUnite, sizeof(Tunite), 1, f_in);
            AjouterUnite(playerAtk, nouvelleUnite);
        }


    //Lecture liste Roi
    int nbDef;
    fread(&nbDef, sizeof(int), 1, f_in);

    for (int i = 0; i < nbDef; i++){
        Tunite *nouvelleUnite = (Tunite*)malloc(sizeof(Tunite));
        fread(nouvelleUnite, sizeof(Tunite), 1, f_in);
        AjouterUnite(playerRoi, nouvelleUnite);
    }

    //Lecture chemin
    int **chemin = (int **)malloc(sizeof(int *)*NBCOORDPARCOURS);
    for (int i = 0; i < NBCOORDPARCOURS; i++){
        chemin[i] = (int*)malloc(sizeof(int)*2);

        // On lit les 2 cases (X et Y) d'un coup pour cette ligne
        fread(chemin[i], sizeof(int), 2, f_in);
    }

    fclose(f_in);
    return chemin;
}
