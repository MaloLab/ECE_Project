#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>

//DEFINE
#define MAX_TACHES 36


typedef struct {
    int tache;
    float temps;
    int prece;
    int station;

} Sommet;
typedef struct Noeud {
    int sommet;
    struct Noeud* suivant;
} Noeud;
typedef struct Graphe {
    int nbSommets;
    Noeud** listesAdj;
    int* degreEntrant;
} Graphe;

// Création d'un nœud
Noeud* creerNoeud(int v) {
    Noeud* nouveauNoeud = malloc(sizeof(Noeud));
    nouveauNoeud->sommet = v;
    nouveauNoeud->suivant = NULL;
    return nouveauNoeud;
}
// Création d'un graphe
Graphe* creerGraphe(int sommets) {
    Graphe* graphe = malloc(sizeof(Graphe));
    graphe->nbSommets = sommets;
    graphe->listesAdj = malloc(sommets * sizeof(Noeud*));
    graphe->degreEntrant = malloc(sommets * sizeof(int));

    for (int i = 0; i < sommets; i++) {
        graphe->listesAdj[i] = NULL;
        graphe->degreEntrant[i] = 0;
    }
    return graphe;
}
// Ajout d'une arête au graphe
void ajouterArete(Graphe* graphe, int src, int dest) {
    Noeud* nouveauNoeud = creerNoeud(dest);
    nouveauNoeud->suivant = graphe->listesAdj[src];
    graphe->listesAdj[src] = nouveauNoeud;
    graphe->degreEntrant[dest]++;
}
// Sous-programme pour la lecture du fichier
void lireFichier(Graphe* graphe, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    int src, dest;
    while (fscanf(fichier, "%d %d", &src, &dest) != EOF) {
        ajouterArete(graphe, src, dest);
    }
    fclose(fichier);
}

// Modification de la fonction triTopologique pour retourner l'ordre topologique
int* triTopologique(Graphe* graphe) {
    int* ordreTop = malloc(graphe->nbSommets * sizeof(int));
    int* degre_entrant = malloc(graphe->nbSommets * sizeof(int));
    int file[MAX_TACHES], debut = 0, fin = -1, compte = 0, v;

    for (v = 0; v < graphe->nbSommets; v++) {
        degre_entrant[v] = graphe->degreEntrant[v];
        if (degre_entrant[v] == 0) {
            file[++fin] = v;
        }
    }

    while (debut <= fin) {
        int u = file[debut++];
        ordreTop[compte++] = u;
        Noeud* temp = graphe->listesAdj[u];
        while (temp) {
            if (--degre_entrant[temp->sommet] == 0) {
                file[++fin] = temp->sommet;
            }
            temp = temp->suivant;
        }
    }

    if (compte != graphe->nbSommets) {
        printf("Le graphe contient un cycle. Le tri topologique n'est pas possible.\n");
        free(ordreTop);
        ordreTop = NULL;
    }

    free(degre_entrant);
    return ordreTop; // Retourner l'ordre topologique
}
void initSommets(Sommet sommets[], int nbSommets) {
    for (int i = 0; i < nbSommets; i++) {
        sommets[i].tache = i;
        sommets[i].temps = 0.0;
        sommets[i].station = -1;

    }
}

int lireExclusions(const char* nomFichier, int (*exclusions)[2]) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    int nbPaires = 0;
    while (fscanf(fichier, "%d %d", &exclusions[nbPaires][0], &exclusions[nbPaires][1]) == 2) {
        nbPaires++;
    }

    fclose(fichier);
    return nbPaires;
}
bool estExclue(int tache, int station, Sommet sommets[], int exclusions[][2], int nbExclusions) {
    for (int i = 0; i < nbExclusions; i++) {
        if ((exclusions[i][0] == tache || exclusions[i][1] == tache) &&
            (sommets[exclusions[i][0]].station == station || sommets[exclusions[i][1]].station == station)) {
            return true; // L'exclusion est violée
        }
    }
    return false; // Aucune exclusion pour cette tâche et cette station
}
void assignerStationsE(Sommet sommets[], int nbSommets, float tempsMaxCycle, int exclusions[][2], int nbExclusions) {
    for (int j = 0; j < nbSommets; j++) { // Parcourir les stations

        for (int i = 0; i < nbSommets; i++) { // Essayer d'assigner chaque tâche à la station j
            if (sommets[i].temps == 0.0 || sommets[i].station != -1) continue; // Ignorer si la tâche n'a pas de temps ou est déjà assignée

            // Vérifier les contraintes
            if (!estExclue(i, j, sommets, exclusions, nbExclusions)) {
                sommets[i].station = j; // Assigner la tâche i à la station j
                //printf("temps: %f", tempsCumule);
            }
        }
    }
}
void assignerStationsET(Sommet sommets[], int nbSommets, float tempsMaxCycle, int exclusions[][2], int nbExclusions) {
    for (int j = 0; j < nbSommets; j++) { // Parcourir les stations
        float tempsCumule = 0.0;

        for (int i = 0; i < nbSommets; i++) { // Essayer d'assigner chaque tâche à la station j
            if (sommets[i].temps == 0.0 || sommets[i].station != -1) continue; // Ignorer si la tâche n'a pas de temps ou est déjà assignée

            // Vérifier les contraintes
            if (!estExclue(i, j, sommets, exclusions, nbExclusions) && tempsCumule + sommets[i].temps <= tempsMaxCycle) {
                sommets[i].station = j; // Assigner la tâche i à la station j
                tempsCumule += sommets[i].temps;
                printf("temps: %f\n", tempsCumule);
            }
        }
    }
}
void assignerStationsT(Sommet sommets[], int nbSommets, float tempsMaxCycle) {
    for (int j = 0; j < nbSommets; j++) { // Parcourir les stations
        float tempsCumule = 0.0;

        for (int i = 0; i < nbSommets; i++) { // Essayer d'assigner chaque tâche à la station j
            if (sommets[i].temps == 0.0 || sommets[i].station != -1) continue; // Ignorer si la tâche n'a pas de temps ou est déjà assignée
            if (tempsCumule + sommets[i].temps <= tempsMaxCycle) {
                sommets[i].station = j; // Assigner la tâche i à la station j
                tempsCumule += sommets[i].temps;
                //printf("temps: %f\n", tempsCumule);
            }
        }
    }
}

void lireTempsMaxCycle(const char* nomFichier, float *tempsMax) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    if (fscanf(fichier, "%f", tempsMax) != 1) {
        fprintf(stderr, "Erreur de format dans le fichier %s\n", nomFichier);
        exit(1);
    }

    fclose(fichier);
}
void lireTempsTaches(const char* nomFichier, Sommet sommets[], int taille) {
    FILE *fichier = fopen(nomFichier, "r");
    int tache;
    float temps;

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    while (fscanf(fichier, "%d %f", &tache, &temps) == 2) {
        if (tache >= 1 && tache < taille) {
            sommets[tache].temps = temps;
        }
    }
    fclose(fichier);
}


void temps(){
    Sommet sommets[MAX_TACHES];
    float tempsMaxCycle;

    initSommets(sommets, MAX_TACHES);

    lireTempsMaxCycle("../rcycle.txt", &tempsMaxCycle);
    lireTempsTaches("../temps.txt", sommets, MAX_TACHES);

    assignerStationsT(sommets, MAX_TACHES, tempsMaxCycle);

    int stationMax = -1;
    // Trouver la station la plus élevée à laquelle une tâche est assignée
    for (int i = 0; i < MAX_TACHES; i++) {
        if (sommets[i].station > stationMax) {
            stationMax = sommets[i].station;
        }
    }

    // Afficher les tâches pour chaque station active
    int station = 0;
    do {
        printf("Station %d: ", station + 1);
        bool stationVide = true;

        for (int i = 0; i < MAX_TACHES; i++) {
            if (sommets[i].station == station) {
                printf("%d ", sommets[i].tache);
                stationVide = false;
            }
        }

        if (stationVide) {
            printf("Pas de tâches");
        }
        printf("\n");

        station++;
    } while (station <= stationMax);
};
void exclu(){

    Sommet sommets[MAX_TACHES];

    float tempsMaxCycle;

    initSommets(sommets, MAX_TACHES);

    lireTempsMaxCycle("../rcycle.txt", &tempsMaxCycle);
    lireTempsTaches("../temps.txt", sommets, MAX_TACHES);


    int exclusions[MAX_TACHES][2]; // Assurez-vous que cela a une taille suffisante
    int nbExclusions = lireExclusions("../exclu.txt", exclusions);


    ///ASSIGNER EXLCLSUION :
    assignerStationsE(sommets, MAX_TACHES, tempsMaxCycle, exclusions, nbExclusions);


    int stationMax = -1;
    // Trouver la station la plus élevée à laquelle une tâche est assignée
    for (int i = 0; i < MAX_TACHES; i++) {
        if (sommets[i].station > stationMax) {
            stationMax = sommets[i].station;
        }
    }

    // Afficher les tâches pour chaque station active
    int station = 0;
    do {
        printf("Station %d: ", station + 1);
        bool stationVide = true;

        for (int i = 0; i < MAX_TACHES; i++) {
            if (sommets[i].station == station) {
                printf("%d ", sommets[i].tache);
                stationVide = false;
            }
        }

        if (stationVide) {
            printf("Pas de tâches");
        }
        printf("\n");

        station++;
    } while (station <= stationMax);

};
void prece(){
    Graphe* graphe = creerGraphe(MAX_TACHES);
    lireFichier(graphe, "../prece.txt");

    Sommet sommets[MAX_TACHES];

// Récupérer l'ordre topologique
    int* ordreTopologique = triTopologique(graphe);
    if (ordreTopologique != NULL) {
        printf("Ordre topologique : ");
        for (int i = 0; i < graphe->nbSommets; i++) {
            printf("%d ", ordreTopologique[i]);

        }
        printf("\n");
    }

};

void TE(){
    Sommet sommets[MAX_TACHES];

    float tempsMaxCycle;

    initSommets(sommets, MAX_TACHES);

    lireTempsMaxCycle("../rcycle.txt", &tempsMaxCycle);
    lireTempsTaches("../temps.txt", sommets, MAX_TACHES);


    int exclusions[MAX_TACHES][2]; // Assurez-vous que cela a une taille suffisante
    int nbExclusions = lireExclusions("../exclu.txt", exclusions);

    assignerStationsET(sommets, MAX_TACHES, tempsMaxCycle, exclusions, nbExclusions);

    int stationMax = -1;
    // Trouver la station la plus élevée à laquelle une tâche est assignée
    for (int i = 0; i < MAX_TACHES; i++) {
        if (sommets[i].station > stationMax) {
            stationMax = sommets[i].station;
        }
    }

    // Afficher les tâches pour chaque station active
    int station = 0;
    do {
        printf("Station %d: ", station + 1);
        bool stationVide = true;

        for (int i = 0; i < MAX_TACHES; i++) {
            if (sommets[i].station == station) {
                printf("%d ", sommets[i].tache);
                stationVide = false;
            }
        }

        if (stationVide) {
            printf("Pas de tâches");
        }
        printf("\n");

        station++;
    } while (station <= stationMax);

};
void TP(){
    Graphe* graphe = creerGraphe(MAX_TACHES);
    lireFichier(graphe, "../prece.txt");

    Sommet sommets[MAX_TACHES];

    float tempsMaxCycle;

    initSommets(sommets, MAX_TACHES);

    lireTempsMaxCycle("../rcycle.txt", &tempsMaxCycle);
    lireTempsTaches("../temps.txt", sommets, MAX_TACHES);

// Récupérer l'ordre topologique
    int* ordreTopologique = triTopologique(graphe);
    if (ordreTopologique != NULL) {
        printf("Ordre topologique : ");
        for (int i = 0; i < graphe->nbSommets; i++) {
            printf("%d ", ordreTopologique[i]);

        }
        printf("\n");
    }

    assignerStationsT(sommets, MAX_TACHES, tempsMaxCycle);

    int stationMax = -1;
    // Trouver la station la plus élevée à laquelle une tâche est assignée
    for (int i = 0; i < MAX_TACHES; i++) {
        if (sommets[i].station > stationMax) {
            stationMax = sommets[i].station;
        }
    }

    // Afficher les tâches pour chaque station active
    int station = 0;
    do {
        printf("Station %d: ", station + 1);
        bool stationVide = true;

        for (int i = 0; i < MAX_TACHES; i++) {
            if (sommets[ordreTopologique[i]].station == station) {
                printf("%d ", sommets[ordreTopologique[i]].tache);
                stationVide = false;
            }
        }

        if (stationVide) {
            printf("Pas de tâches");
        }
        printf("\n");

        station++;
    } while (station <= stationMax);

};
void EP(){Graphe* graphe = creerGraphe(MAX_TACHES);
    lireFichier(graphe, "../prece.txt");

    Sommet sommets[MAX_TACHES];

    float tempsMaxCycle;



    initSommets(sommets, MAX_TACHES);

    lireTempsMaxCycle("../rcycle.txt", &tempsMaxCycle);
    lireTempsTaches("../temps.txt", sommets, MAX_TACHES);


    int exclusions[MAX_TACHES][2]; // Assurez-vous que cela a une taille suffisante
    int nbExclusions = lireExclusions("../exclu.txt", exclusions);


// Récupérer l'ordre topologique
    int* ordreTopologique = triTopologique(graphe);
    if (ordreTopologique != NULL) {
        printf("Ordre topologique : ");
        for (int i = 0; i < graphe->nbSommets; i++) {
            printf("%d ", ordreTopologique[i]);

        }
        printf("\n");
    }


    ///ASSIGNER EXLCLSUION :
    assignerStationsE(sommets, MAX_TACHES, tempsMaxCycle, exclusions, nbExclusions);

    int stationMax = -1;
    // Trouver la station la plus élevée à laquelle une tâche est assignée
    for (int i = 0; i < MAX_TACHES; i++) {
        if (sommets[i].station > stationMax) {
            stationMax = sommets[i].station;
        }
    }

    // Afficher les tâches pour chaque station active
    int station = 0;
    do {
        printf("Station %d: ", station + 1);
        bool stationVide = true;

        for (int i = 0; i < MAX_TACHES; i++) {
            if (sommets[ordreTopologique[i]].station == station) {
                printf("%d ", sommets[ordreTopologique[i]].tache);
                stationVide = false;
            }
        }

        if (stationVide) {
            printf("Pas de tâches");
        }
        printf("\n");

        station++;
    } while (station <= stationMax);
};

void TPE(){
    Graphe* graphe = creerGraphe(MAX_TACHES);
    lireFichier(graphe, "../prece.txt");

    Sommet sommets[MAX_TACHES];

    float tempsMaxCycle;



    initSommets(sommets, MAX_TACHES);

    lireTempsMaxCycle("../rcycle.txt", &tempsMaxCycle);
    lireTempsTaches("../temps.txt", sommets, MAX_TACHES);


    int exclusions[MAX_TACHES][2]; // Assurez-vous que cela a une taille suffisante
    int nbExclusions = lireExclusions("../exclu.txt", exclusions);


// Récupérer l'ordre topologique
    int* ordreTopologique = triTopologique(graphe);
    if (ordreTopologique != NULL) {
        printf("Ordre topologique : ");
        for (int i = 0; i < graphe->nbSommets; i++) {
            printf("%d ", ordreTopologique[i]);

        }
        printf("\n");
    }

    assignerStationsET(sommets, MAX_TACHES, tempsMaxCycle, exclusions, nbExclusions);



    int stationMax = -1;
    // Trouver la station la plus élevée à laquelle une tâche est assignée
    for (int i = 0; i < MAX_TACHES; i++) {
        if (sommets[i].station > stationMax) {
            stationMax = sommets[i].station;
        }
    }

    // Afficher les tâches pour chaque station active
    int station = 0;
    do {
        printf("Station %d: ", station + 1);
        bool stationVide = true;

        for (int i = 0; i < MAX_TACHES; i++) {
            if (sommets[ordreTopologique[i]].station == station) {
                printf("%d ", sommets[ordreTopologique[i]].tache);
                stationVide = false;
            }
        }

        if (stationVide) {
            printf("Pas de tâches");
        }
        printf("\n");

        station++;
    } while (station <= stationMax);

};



int main() {
///DECLARER LES VARIABLES
    premierpage();
    int choix, choixquit;
    while (1) {
        //nettoyer l'ecran
        system("cls");
        do {
            ///----------------BOUCLE POUR AFFICHER MENU ET LA SAISIE SI INCORRECTE-------------------------
            do {
                color(9, 0);
                ///AFFICHER MENU
                printf("\n\n\t\t\t\t\t\t                                         \n"
                       "\t\t\t\t\t                                       \n"
                       "\t\t\t\t\t  _ __ ___     ___   _ __    _   _    \n"
                       "\t\t\t\t\t | '_ ` _ \\   / _ \\ | '_ \\  | | | |      \n"
                       "\t\t\t\t\t | | | | | | |  __/ | | | | | |_| |    \n"
                       "\t\t\t\t\t |_| |_| |_|  \\___| |_| |_|  \\__,_|   \n");
                printf("\t\t\t\t\t\t                                         \n\n");

                printf("\n\t\t\t\t\t 1- Temps\n\n");
                printf("\t\t\t\t\t 2- Exclusion\n\n");
                printf("\t\t\t\t\t 3- Precedence\n\n");
                printf("\t\t\t\t\t 4- Temps + Exclusion\n\n");
                printf("\t\t\t\t\t 5- Exclusion + Precedence\n\n");
                printf("\t\t\t\t\t 6- Precedence + Temps\n\n");
                printf("\t\t\t\t\t 7- Assemblage optimise !\n\n");
                printf("\t\t\t\t\t 8- Quitter\n\n");

                ///SASIR LE MENU VOULU
                scanf("%d", &choix);


            } while (choix != 0 && choix != 1 && choix != 2 && choix != 3 && choix != 4 && choix != 5 && choix != 6 &&
                     choix != 7 && choix != 8);
            color(9, 0);
            printf("vous avez choisi %d\n\n", choix);

            switch (choix) {
                case 1:///1
                    color(0, 9);
                    printf("\n 1- Temps\n\n");
                    color(9, 0);
                    temps();
                    Sleep(3000);
                    break;

                case 2:///2
                    color(0, 9);
                    printf("\n 2- Exclusion\n\n");
                    color(9, 0);
                    exclu();
                    Sleep(3000);
                    break;

                case 3:///3
                    color(0, 9);
                    printf("\n 3- Precedence\n\n");
                    color(9, 0);
                    prece();
                    Sleep(3000);
                    break;

                case 4:///4
                    color(0, 9);
                    printf("\n 4-Temps + Exclusion\n\n");
                    color(9, 0);
                    TE();
                    Sleep(3000);
                    break;

                case 5:///5
                    color(0, 9);
                    printf("\n 5- Exclusion + Precedence\n\n");
                    color(9, 0);
                    EP();
                    Sleep(3000);
                    break;

                case 6:///6
                    color(0, 9);
                    printf("\n 6- Precedece + Temps\n\n");
                    color(9, 0);
                    TP();
                    Sleep(3000);
                    break;

                case 7: ///7
                    color(0, 9);
                    printf("\n 7- Assemblage optimise !\n\n");
                    color(9, 0);
                    TPE();
                    Sleep(3000);
                    break;

                case 8:///quitter
                    color(9, 1);
                    printf("\t\t\t\t\t 8- MERCI !\n\n");
                    Sleep(3000);
                    exit(0);

                default:
                    printf("ERREUR DE SAISIE\n");
                    system("CLS");

                    break;
            }
        } while (choixquit == 2);
        system("PAUSE");
    }
}