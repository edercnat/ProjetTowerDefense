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
        TListePlayer PlayerRoi, PlayerAtk;
        initListe(&PlayerRoi); initListe(&PlayerAtk);

        //Creation du roi positionné a la dernière case du chemin et ajout à la liste du joueur 
        Tunite *roi = creeTourRoi(chemin[(NBCOORDPARCOURS-1)][0], chemin[(NBCOORDPARCOURS-1)][1]);
        AjouterUnite(&PlayerRoi, roi);
        


        //-------------------------------------------------------------------------------------------------------------------------------
        //------------------------------VARIABLES DE TESTS ET AUTRES---------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        //Pour la tour random
        Tunite *tour = creeTourAir(3,3);

        AjouterUnite(&PlayerRoi, tour);

        //Ajout tour test
        
        TListePlayer testtest;


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
        PositionnePlayerOnPlateau(PlayerRoi, jeu);
        affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);

        //Boucle principale
        while (!tourRoiDetruite(PlayerRoi))
        {
                //Calculs des nouveaux indices dans le chemin et changements des coordonnées des unités sur le plateau
                calculNewInd(PlayerAtk, chemin);
                updateCoord(PlayerAtk, chemin, jeu);

                //Apparitions aléatoire des unités de la horde
                int spawn = rand()%2; //random pour faire apparaitre une unité
                if (spawn == 0){
                        AjouterUnite(&PlayerAtk, randomUnite(chemin));
                }

                //Positionnement des unités de la horde sur le plateau jeu
                PositionnePlayerOnPlateau(PlayerAtk, jeu);

                //Gestion des attaques de toutes les unités
                
                testtest = quiEstAPortee(jeu, tour, chemin, roi);
                print_TlistePlayer(testtest);
                triSelectionFcomp(testtest,moinsDePointsDeVies);
                print_TlistePlayer(testtest);


                //Attaque sur le Roi
                TListePlayer tmp = PlayerAtk;
                int compteur = 0;
                for (int i = 0; i < getNbreCell(PlayerAtk); i++){
                        atkKing(tmp->pdata, PlayerRoi, chemin);
                        if (canDamageKing(tmp->pdata, chemin)){
                                compteur++;
                        }
                        if (tourRoiDetruite(PlayerRoi)){
                                printf("Le Roi est mort !\n\n");
                                break;
                        }
                        tmp = tmp->suiv;
                }


                //Affichage du plateau
                affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);



                

                


                //Gestions de l'affichage et de rafraichissement de la fenêtre
                //if (compteur > 0 && !tourRoiDetruite(PlayerRoi)) printf("Touché x%d\n\n", compteur);
                printf("Vie du Roi : %d \n", PlayerRoi->pdata->pointsDeVie);
                
                sleep(1);
                if (!tourRoiDetruite(PlayerRoi)){
                        system("clear");
                }
        }


        freeChemin(tabParcours);
        return 0;

}
