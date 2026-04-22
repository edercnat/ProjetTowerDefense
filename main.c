//PELE Raphael :
//PAPIN Tancrède :


#include "towerdefend.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define SAVE_SEQ "sauvegarde.txt"
#define SAVE_BIN "sauvegarde.bin"




int main()
{
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-----------------------------------------------------VARIABLES GLOBALES--------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        system("clear");
        //Initialisation du random
        srand( time (NULL) );


        //plateau du jeu
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);

        //Initialisation des listes des joueurs
        TListePlayer PlayerRoi, PlayerHorde;
        initListe(&PlayerRoi); initListe(&PlayerHorde);


        //Initialisation du roi
        Tunite *roi = NULL;


        //Initialisation du chemin
        int **chemin;


        int choix;
        int choix_save_reprise;
        int choix_save;


        printf("<-------- Menu -------->\n\n");
        printf("1. Lancer une partie\n");
        printf("2. Reprendre une sauvegarde\n");
        scanf("%d", &choix);
        printf("\n\n");

        if (choix == 1){

            printf("<-------- Type de sauvegarde -------->\n");
            printf("1. Séquentielle\n");
            printf("2. Binaire\n");
            scanf("%d", &choix_save);

            chemin = initChemin();

            //Ajout du roi
            roi = creeTourRoi(chemin[(NBCOORDPARCOURS-1)][0], chemin[(NBCOORDPARCOURS-1)][1]); //Creation du roi positionné a la dernière case du chemin (ou premiere faudra verifier l'ordre du tableau)
            AjouterUnite(&PlayerRoi, roi);
            PositionnePlayerOnPlateau(PlayerRoi, jeu);
            system("clear");
        }


        else {
            printf("<-------- Type de sauvegarde -------->\n");
            printf("Séquentielle : 1\n");
            printf("Binaire : 2\n");
            scanf("%d", &choix_save_reprise);

            if (choix_save_reprise == 1){
                chemin = repriseSave(SAVE_SEQ, &PlayerHorde, &PlayerRoi);
            }
            else {
                chemin = repriseSaveBin(SAVE_BIN, &PlayerHorde, &PlayerRoi);
            }
            roi = PlayerRoi->pdata;
            PositionnePlayerOnPlateau(PlayerRoi, jeu);
        }


        //Création du plateau simplifié (ou le chemin apparaît)
        //Cela sert à trouver les meilleures cases
        int jeuBis[LARGEURJEU][HAUTEURJEU];
        for(int i = 0; i < LARGEURJEU ; i++){
                for(int j = 0 ; j < HAUTEURJEU ; j++){
                        jeuBis[i][j] = 0;
                }
        }
        for(int i = 0; i < NBCOORDPARCOURS ; i ++){
                jeuBis[chemin[i][0]][chemin[i][1]] = 1;
        }
        //On récupère la liste croissante des meilleures positions
        TListePlayer tabPorteeSol = listeMeilleuresCases(jeuBis, 5);
        TListePlayer tabPorteeAir = listeMeilleuresCases(jeuBis, 3);
        TListePlayer *tabChoisi; //Variables pour le traitement
        TListePlayer tabTemp;


        //-------------------------------------------------------------------------------------------------------------------------------
        //--------------------------------------------VARIABLES DE TESTS ET AUTRES-------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------



        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-----------------------------------------------------PROGRAMME ET BOUCLE PRINCIPALE--------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //Recherche des meilleures cases pour les tours

        //On positionne toutes unités du joueur du côté roi
        //(Elles ne se déplacent jamais)
        affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);
        system("clear");
        //Boucle principale
        while (!tourRoiDetruite(PlayerRoi))
        {
                //Nouveau variable aléatoire pour le tour
                int hasard = rand();


                printf("\n");
                //Calculs des nouveaux indices dans le chemin et changements des coordonnées des unités sur le plateau
                calculNewInd(PlayerHorde, chemin);
                updateCoord(PlayerHorde, chemin, jeu);

                //Apparitions aléatoire des unités de la horde

                if(hasard%100 < 75){
                        AjouterUnite(&PlayerHorde, randomUnite(chemin));
                }

                //Apparition aléatoire des tours

                //50% de chance pour chaque
                // 0 : Tour sol
                // 1 : Tour air

                //20% de chance qu'une tour apparaisse
                if(hasard%100 < 20){
                        if(hasard%2 == 0){
                                tabChoisi = &tabPorteeSol;
                        }
                        else{
                                tabChoisi = &tabPorteeAir;
                        }

                        tabTemp = *tabChoisi;
                        //Si la case est déjà occupée on supprime l'unité de la liste et on prend la suivante
                        //(cela arrive car on a deux tableaux pour les deux portées différentes)
                        while(jeu[(*tabChoisi)->pdata->posX][(*tabChoisi)->pdata->posY] != NULL){
                                tabTemp = *tabChoisi;
                                *tabChoisi = (*tabChoisi)->suiv;
                                free(tabTemp->pdata);
                                free(tabTemp);
                        }

                        if (*tabChoisi != NULL){
                            AjouterUnite(&PlayerRoi, (*tabChoisi)->pdata);
                            jeu[(*tabChoisi)->pdata->posX][(*tabChoisi)->pdata->posY] = (*tabChoisi)->pdata;
                            tabTemp = *tabChoisi;
                            *tabChoisi = (*tabChoisi)->suiv;
                            free(tabTemp);
                        }
                }


                //Positionnement des unités sur le plateau jeu
                PositionnePlayerOnPlateau(PlayerHorde, jeu);


                //Gestion des attaques de toutes les unités
                attaquePlayer(jeu, chemin, &roi, PlayerHorde);
                attaquePlayer(jeu, chemin, &roi, PlayerRoi);



                //Supprime les unités mortes
                supprimerUnite(&PlayerHorde, jeu);


                //Affichage du plateau
                affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);

                if (tourRoiDetruite(PlayerRoi)){
                    printf("Vie du Roi : 0\n\n");
                    printf("Le Roi est mort !\n");
                }
                else {
                    printf("Vie du Roi : %d PV\n", roi->pointsDeVie);
                }

                //Gestions de l'affichage et de rafraichissement de la fenêtre
                //if (compteur > 0 && !tourRoiDetruite(PlayerRoi)) printf("Touché x%d\n\n", compteur);

                sleep(1);
                if (!tourRoiDetruite(PlayerRoi)){
                        system("clear");
                }

                if (choix_save_reprise == 1 || choix_save == 1){
                    SaveState(PlayerHorde, PlayerRoi, SAVE_SEQ, chemin);
                }
                else {
                    SaveStateBin(PlayerHorde, PlayerRoi, SAVE_BIN, chemin);
                }
        }

        //Libération de la mémoire
        freeListePlayer(&tabPorteeSol);
        freeListePlayer(&tabPorteeAir);
        freeListePlayer(&PlayerRoi);
        freeListePlayer(&PlayerHorde);

        freeChemin(chemin);


        return 0;

}
