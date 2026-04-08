#include "towerdefend.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
        printf("Test\n");

        Tunite test = {gargouille, sol, sol, 100, 1, 100, 1, 1, 5, 5, 0};
        Tunite test2 = {dragon, sol, sol, 100, 1, 100, 1, 1, 5, 5, 0};
        Tunite test3 = {archer, sol, sol, 100, 1, 100, 1, 1, 5, 5, 0};
        Tunite test4 = {chevalier, sol, sol, 100, 1, 100, 1, 1, 5, 5, 0};


        TListePlayer l1;
        initListe(&l1);
        l1 = ajoutEnTete(l1, test);
        l1 = ajoutEnTete(l1, test2);
        l1 = ajoutEnFin(l1, test3);
        l1 = ajoutEnFin(l1, test3);
        l1 = ajoutEnN(l1, 0, test4);
        afficheListe(l1);
        printf("Nouvelle liste : \n");
        l1 = suppEnN(l1, 1);
        afficheListe(l1);

        return 1;
}
