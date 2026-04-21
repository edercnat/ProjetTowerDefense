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


/*--------- Main ----------------------*/
int main(int argc, char* argv[])
{
        srand( time (NULL) );

        int **chemin;
        TListePlayer PlayerAtk;
        TListePlayer PlayerRoi;
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);

        //Initialisation des listes des joueurs
        initListe(&PlayerRoi); initListe(&PlayerAtk);

        int choix;
        int choix_save_reprise;
        int choix_save;

        printf("Lancer une partie : 1\n");
        printf("Relancer depuis une sauvegarde : 2\n");

        scanf("%d", &choix);


        if (choix == 2){

            printf("Choisissez votre type de sauvegarde :\n");
            printf("Séquentielle : 1\n");
            printf("Binaire : 2\n");
            scanf("%d", &choix_save_reprise);

            if (choix_save_reprise == 1){
                chemin = repriseSave(SAVE_SEQ, &PlayerAtk, &PlayerRoi);
            }
            else {
                chemin = repriseSaveBin(SAVE_BIN, &PlayerAtk, &PlayerRoi);
            }

            PositionnePlayerOnPlateau(PlayerRoi, jeu);
        }

        else {
            printf("Choisissez votre type de sauvegarde :\n");
            printf("Séquentielle : 1\n");
            printf("Binaire : 2\n");
            scanf("%d", &choix_save);
            //Initialistaion du chemin
            chemin = initChemin();

            //Ajout du roi
            Tunite *roi = creeTourRoi(chemin[(NBCOORDPARCOURS-1)][0], chemin[(NBCOORDPARCOURS-1)][1]); //Creation du roi positionné a la dernière case du chemin (ou premiere faudra verifier l'ordre du tableau)
            AjouterUnite(&PlayerRoi, roi);
            PositionnePlayerOnPlateau(PlayerRoi, jeu);

            affichePlateauConsole(jeu, LARGEURJEU, HAUTEURJEU, chemin);

        }
            // AjouterUnite(&PlayerAtk, randomUnite(chemin), &posAtk);
            // calculNewInd(PlayerAtk, posAtk, chemin, jeu);
            // updateCoord(PlayerAtk, posAtk, chemin, jeu);
            while (!tourRoiDetruite(PlayerRoi))
            {
                    calculNewInd(PlayerAtk, chemin);
                    updateCoord(PlayerAtk, chemin, jeu);
                    int spawn = rand()%2; //random pour faire apparaitre une unité

                    if (spawn == 0){
                    AjouterUnite(&PlayerAtk, randomUnite(chemin));
                    }
                    PositionnePlayerOnPlateau(PlayerAtk, jeu);
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

                    if (choix_save == 1){
                        SaveState(PlayerAtk, PlayerRoi, SAVE_SEQ, chemin);
                    }
                    else {
                        SaveStateBin(PlayerAtk, PlayerRoi, SAVE_BIN, chemin);
                    }

            }


        freeChemin(chemin);
        return 0;

}
