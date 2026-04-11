//PELE Raphael :
//PAPIN Tancrède :


#include "towerdefend.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



/*--------- Main ---------------------*/
int main(int argc, char* argv[])
{
        srand( time (NULL) );

        int** tabParcours = initChemin();  //tabParcours est un tableau de NBCOORDPARCOURS cases, chacune contenant un tableau � 2 cases (indice 0 pour X, indice 1 pour Y)


        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);

        //Initialistaion du chemin
        int **chemin = initChemin();

        //Initialisation des listes des joueurs
        TListePlayer PlayerRoi, PlayerAtk;
        initListe(&PlayerRoi); initListe(&PlayerAtk);

        float *posAtk = NULL;
        float *posRoi = NULL;



        
        //Ajout du roi
        Tunite *roi = creeTourRoi(chemin[(NBCOORDPARCOURS-1)][0], chemin[(NBCOORDPARCOURS-1)][1]); //Creation du roi positionné a la dernière case du chemin (ou premiere faudra verifier l'ordre du tableau)
        AjouterUnite(&PlayerRoi, roi, &posRoi);
        

        //Ajout d'une tour
        Tunite *tourTest = creeTourSol(6, 5);
        tourTest->portee = 4;
        PlayerRoi = ajoutEnFin(PlayerRoi, tourTest);

        PositionnePlayerOnPlateau(PlayerRoi, jeu);
        // AjouterUnite(&PlayerAtk, randomUnite(chemin), &posAtk);
        // calculNewInd(PlayerAtk, posAtk, chemin, jeu);
        // updateCoord(PlayerAtk, posAtk, chemin, jeu);


        //$$ recoder message pour faire le menu d'arrivée
        //message("Welcome in TowerDfend","Ceci est un point de depart pour votre future interface de votre jeu TowerDefend");
        //message("et fin","ECHAP->quitter, S/C ET D/V les gerer les sauvegardes");

        /**********************************************************************/
        /*                                                                    */
        /*              DEFINISSEZ/INITIALISER ICI VOS VARIABLES              */
        /*
        // FIN de vos variables                                                              */
        /**********************************************************************/

        while (!tourRoiDetruite(PlayerRoi))
        {
                


                int spawn = rand()%2; //random pour faire apparaitre une unité

                if (spawn == 0){
                        AjouterUnite(&PlayerAtk, randomUnite(chemin), &posAtk);
                }
                PositionnePlayerOnPlateau(PlayerAtk, jeu);
                affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);

                //vérifie si la tour peut attaquer quelqu'un
                quiEstAPortee(jeu, tourTest);
                
                calculNewInd(PlayerAtk, posAtk, chemin, jeu);
                updateCoord(PlayerAtk, posAtk, chemin, jeu);

                //Attaque sur le Roi

                TListePlayer tmp = PlayerAtk;
                int compteur = 0;
                for (int i = 0; i < getNbreCell(PlayerAtk); i++){
                        atkKing(tmp->pdata, (int)posAtk[i], PlayerRoi);
                        if (canDamageKing(tmp->pdata, (int)posAtk[i])){
                                compteur++;
                        }
                        if (tourRoiDetruite(PlayerRoi)){
                                printf("Le Roi est mort !\n\n");
                                break;
                        }
                        tmp = tmp->suiv;
                }

                if (compteur > 0 && !tourRoiDetruite(PlayerRoi)) printf("Touché x%d\n\n", compteur);

                printf("Vie du Roi : %d \n", PlayerRoi->pdata->pointsDeVie);

                
                
                sleep(1);
                if (!tourRoiDetruite(PlayerRoi)){
                        system("clear");
                }
        }
        


        // for( int i = 0; i < 7; i++){

        //         AjouterUnite(&PlayerAtk, randomUnite(chemin), &posAtk);


        //         print_list(posAtk, getNbreCell(PlayerAtk));
        //         afficheListe(PlayerAtk);
        //         printf("X : %d\n", drag->posX);
        //         printf("Y : %d\n", drag->posY);


        //         PositionnePlayerOnPlateau(PlayerAtk, jeu);
        //         // affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU);
        //         calculNewInd(PlayerAtk, posAtk);
        //         updateCoord(PlayerAtk, posAtk, chemin, jeu);
        // }

        // for (int i = 0; i < 10; i++)
        // {       
        //         print_list(posAtk, getNbreCell(PlayerAtk));
        //         afficheListe(PlayerAtk);
        //         printf("X : %d\n", drag->posX);
        //         printf("Y : %d\n", drag->posY);

        //         PositionnePlayerOnPlateau(PlayerAtk, jeu);
        //         // affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU);
        //         calculNewInd(PlayerAtk, posAtk);
        //         updateCoord(PlayerAtk, posAtk, chemin, jeu);
        // }
        
        



        freeChemin(tabParcours);
        return 0;

}
