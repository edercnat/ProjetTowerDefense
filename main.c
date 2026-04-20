//PELE Raphael :
//PAPIN Tancrède :


#include "towerdefend.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



/*--------- Main ----------------------*/
int main(int argc, char* argv[])
{
        srand( time (NULL) );


    int** tabParcours=initChemin();  //tabParcours est un tableau de NBCOORDPARCOURS cases, chacune contenant un tableau � 2 cases (indice 0 pour X, indice 1 pour Y)


        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);

        //Initialistaion du chemin
        int **chemin = initChemin();

        //Initialisation des listes des joueurs
        TListePlayer PlayerRoi, PlayerAtk;
        initListe(&PlayerRoi); initListe(&PlayerAtk);




        //Ajout du roi
        Tunite *roi = creeTourRoi(chemin[(NBCOORDPARCOURS-1)][0], chemin[(NBCOORDPARCOURS-1)][1]); //Creation du roi positionné a la dernière case du chemin (ou premiere faudra verifier l'ordre du tableau)
        AjouterUnite(&PlayerRoi, roi);
        //Pour la tour random
        Tunite *tour = creeTourAir(3,3);

        AjouterUnite(&PlayerRoi, tour);

        PositionnePlayerOnPlateau(PlayerRoi, jeu);
        affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);

        //Ajout tour test
        



        while (!tourRoiDetruite(PlayerRoi))
        {
                calculNewInd(PlayerAtk, chemin);
                updateCoord(PlayerAtk, chemin, jeu);
                int spawn = rand()%2; //random pour faire apparaitre une unité

                if (spawn == 0){
                AjouterUnite(&PlayerAtk, randomUnite(chemin));
                }
                PositionnePlayerOnPlateau(PlayerAtk, jeu);
                quiEstAPortee(jeu, tour);
                affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);



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

                if (compteur > 0 && !tourRoiDetruite(PlayerRoi)) printf("Touché x%d\n\n", compteur);

                printf("Vie du Roi : %d \n", PlayerRoi->pdata->pointsDeVie);


                sleep(1);
                if (!tourRoiDetruite(PlayerRoi)){
                        system("clear");
                }
        }


        freeChemin(tabParcours);
        return 0;

}
