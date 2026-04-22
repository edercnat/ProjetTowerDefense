//PELE Raphael :
//PAPIN Tancrède :


#include "towerdefend.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>




int main(int argc, char* argv[])
{
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-----------------------------------------------------VARIABLES GLOBALES--------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //Initialisation du random
        srand( time (NULL) );

        //tabParcours est un tableau de NBCOORDPARCOURS cases, chacune contenant un tableau � 2 cases (indice 0 pour X, indice 1 pour Y)
        int** tabParcours=initChemin();  

        //plateau du jeu
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);

        //Initialistaion du chemin (liste permettant de positionner et de gérer les unités de la horde)
        int **chemin = initChemin();

        //Initialisation des listes des joueurs
        TListePlayer PlayerRoi, PlayerHorde;
        initListe(&PlayerRoi); initListe(&PlayerHorde);

        //Creation du roi positionné a la dernière case du chemin et ajout à la liste du joueur 
        Tunite *roi = creeTourRoi(chemin[(NBCOORDPARCOURS-1)][0], chemin[(NBCOORDPARCOURS-1)][1]);
        AjouterUnite(&PlayerRoi, roi);
        
        //Positionnement des tours de manière optimisée
        PositionnePlayerOnPlateau(PlayerRoi, jeu);

        //Création du plateau simplifié
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
        TListePlayer tabPortee5 = listeMeilleuresCases(jeuBis, 5);
        TListePlayer tabPortee3 = listeMeilleuresCases(jeuBis, 3);
        TListePlayer *tabChoisi; //Variables pour le traitement
        TListePlayer tabASuppr;
        TListePlayer tabTemp;

        //Variable qui sert à garder en mémoire les unités
        

        //-------------------------------------------------------------------------------------------------------------------------------
        //--------------------------------------------VARIABLES DE TESTS ET AUTRES-------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        // //Pour la tour random
        // Tunite *tour = creeTourAir(3,3);
        // Tunite *tour1 = creeTourSol(5,3);
        // AjouterUnite(&PlayerRoi, tour);
        // AjouterUnite(&PlayerRoi, tour1);

        


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
                int spawn = rand()%2; //random pour faire apparaitre une unité
                if (spawn == 0){
                        AjouterUnite(&PlayerHorde, randomUnite(chemin));
                }


                //Apparition aléatoire des tours
                
                //50% de chance pour chaque
                // 0 : Tour sol 
                // 1 : Tour air

                //20% de chance qu'une tour apparaisse
                if(hasard%100 < 20){
                        if(hasard%2 == 0){
                                tabChoisi = &tabPortee3;
                                tabASuppr = tabPortee5;
                        }
                        else{
                                tabChoisi = &tabPortee5;
                                tabASuppr = tabPortee3;
                        }
                        tabTemp = *tabChoisi;
                        //Si la case est déjà occupée on supprime l'unité de la liste et on prend la suivante
                        //(cela arrive car on a deux tableaux pour les deux portées différentes)
                        while(jeu[(*tabChoisi)->pdata->posX][(*tabChoisi)->pdata->posY] != NULL){
                                *tabChoisi = (*tabChoisi)->suiv;
                                tabTemp = *tabChoisi;
                        }
                        AjouterUnite(&PlayerRoi, (*tabChoisi)->pdata);
                        

                        jeu[(*tabChoisi)->pdata->posX][(*tabChoisi)->pdata->posY] = (*tabChoisi)->pdata;
                }
                
                

                //Positionnement des unités sur le plateau jeu
                PositionnePlayerOnPlateau(PlayerHorde, jeu);


                //Gestion des attaques de toutes les unités
                attaquePlayer(jeu, chemin, &roi, PlayerHorde);
                attaquePlayer(jeu, chemin, &roi, PlayerRoi);
                
                print_TlistePlayer(PlayerHorde);
                
                

                //Supprime les unités mortes
                supprimerUnite(&PlayerHorde, jeu);
                




                //Affichage du plateau
                affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);

                //Gestions de l'affichage et de rafraichissement de la fenêtre
                //if (compteur > 0 && !tourRoiDetruite(PlayerRoi)) printf("Touché x%d\n\n", compteur);
                printf("Vie du Roi : %d \n", PlayerRoi->pdata->pointsDeVie);
                
                sleep(1);
                if (!tourRoiDetruite(PlayerRoi)){
                        //system("clear");
                }
        }

        affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);
        freeChemin(tabParcours);
        return 0;

}
