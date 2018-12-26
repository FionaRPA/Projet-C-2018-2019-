#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>


#define H_FENETRE 800 
#define L_FENETRE  1400

/* dimension du monde en nombre de cases*/
#define NBCOL 18
#define NBLIG 12

/* l’origine est en haut a gauche*/
/* les deux clans */
#define ROUGE 'R'
#define BLEU  'B'
#define LIBRE '-'

/* les genres d’agents */
#define MANANT 'm'
#define BARON 'b'
#define GUERRIER 'g'
#define CHATEAU 'c'

/* les temps de production */
#define TMANANT 2
#define TGUERRIER 4
#define TBARON 6

/* les couts */
#define CMANANT 1
#define CGUERRIER 5
#define CBARON 10
#define CCHATEAU 30



typedef struct Bouton {
	int x1;
	int y1;
	int x2;
	int y2;
	int numero_bouton;
} Bouton;


typedef struct agent{
	char clan; /* ROUGE ou BLEU */
	char genre; /* BARON, MANANT, GUERRIER, CHATEAU */
	char produit; /* production actuelle d’un chateau*/ //  PRODUCTION = AGENT ????????????
	int temps; /* tours restant pour cette production */
	int posx, posy; /* position actuelle */
	int destx, desty; /* destination (negatif pour manant immobile) */
	struct agent *asuiv, *aprec; /* liste des agents liees a un chateau */
	struct agent *vsuiv, *vprec; /* liste des voisins */
}Agent,*AListe;


typedef struct{
	Agent *chateau; /* s’il y a un chateau sur la case */
	AListe habitant; /* les autres occupants */
	char clan;/* couleur du clan ayant revendique*/
}Case;


typedef struct{
	Case plateau[NBCOL][NBLIG];
	AListe rouge, bleu;
	int tour; /* Numero du tour */
	int tresorRouge, tresorBleu;
}Monde;

