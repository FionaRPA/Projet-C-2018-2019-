#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>


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
#define BARON  'b'
#define GUERRIER  'g'
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


typedef struct agent{
	char clan; /* ROUGE ou BLEU */
	char genre; /* BARON, MANANT, GUERRIER, CHATEAU */
	char produit; /* production actuelle d’un chateau*/
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



void initCastle(char couleur, Agent *chateau){
	chateau->clan = couleur;

	chateau->genre = CHATEAU;
	chateau->destx = chateau->desty = -1; //-1 pcq chateau immobile ?!

	if (couleur == 'R'){
		chateau->posx = 0;
		chateau->posy = 0;
	}

	else{
		chateau->posx = NBCOL - 1;
		chateau->posy = NBLIG -1;
	}
}


void initProduction(char couleur, Agent* agent, char genre){
	agent->clan = couleur;

	agent->genre = genre;

	if (couleur == 'R'){
		agent->posx = 0;
		agent->posy = 0;
	}

	else{
		agent->posx = NBCOL - 1;
		agent->posy = NBLIG -1;
	}

	/*
	//pour mannant immobile ?!
	if (genre == 'm')
		agent->destx = agent->desty = -1;
	else{
	*/
	agent->destx = agent->desty = 0;
	//}

	agent->temps = 0;
}



void initMonde( Monde *world){
/*
	//initialisation des chateaux rouge et bleu;
	Agent *chateauR = initcastle(ROUGE, chateauR), *chateauB;
	world->plateau[0][0]->chateau = chateauR;
	//world->plateau->initcastle(BLEU, &chateauB);

	world->tour = 0;

	world->tresorRouge = world->tresorBleu = 50;
*/
	int x, y;
	Case cases;
	cases.chateau = NULL;
	cases.habitant = NULL;
	cases.clan = LIBRE;

	for (x = 0 ; x < NBCOL ; x++) 
		for (y = 0 ; y < NBLIG ; y++) 
			world->plateau[x][y] = cases;

	world->tour = 0;
	world->tresorBleu = world->tresorRouge = 50;
	world->rouge = NULL;
	world->bleu = NULL;
}

/*
void afficheListe(Monde world){
	int x, y;
	for (x = 0 ; x < NBCOL ; x++){
		for (y = 0 ; y < NBLIG ; y++){
			if (world.plateau[x][y].chateau != NULL)
				printf(" %c\n", world.plateau[x][y].chateau->clan);
		}
	}
}
*/


void drawArray(Monde world, MLV_Image *image){
	int x, y;
	int largeur = 30, hauteur = 30;
	int width = 30, height = 30;
	
	MLV_resize_image( image, width/2, height/2);

	for (x = 0 ; x < NBCOL ; x++) {
		for (y = 0 ; y < NBLIG ; y++) {

			if (world.plateau[x][y].chateau != NULL){
				if (world.plateau[x][y].chateau->clan == BLEU)
					//MLV_draw_filled_circle((x*largeur)+(largeur/4)+10, (y*hauteur)+(hauteur/4)+10, 5, MLV_COLOR_BLUE);
					MLV_draw_image(image, (x*largeur)+10, (y*hauteur)+10);

				else if (world.plateau[x][y].chateau->clan == ROUGE)
					MLV_draw_filled_circle((x*largeur)+(largeur/4)+10, (y*hauteur)+(hauteur/4)+10, 5, MLV_COLOR_RED);
				
			}
			//haut gauche
			MLV_draw_rectangle((x*largeur)+10, (y*hauteur)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//haut droit
			MLV_draw_rectangle((x*largeur)+(largeur/2)+10, (y*hauteur)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//bas gauche
			MLV_draw_rectangle((x*largeur)+10, (y*hauteur)+(hauteur/2)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//bas droit
			MLV_draw_rectangle((x*largeur)+(largeur/2)+10, (y*hauteur)+(hauteur/2)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//gros rectangle
			MLV_draw_rectangle((x*largeur)+10, (y*hauteur)+10, largeur, hauteur, /*(x == 10 && y == 10) ? MLV_COLOR_BLUE :*/ MLV_COLOR_WHITE);

		}
	}
	MLV_actualise_window();
}



int main(int argc, char const *argv[]){
	int h_fenetre = 400, l_fenetre = 800;
	
	MLV_create_window("Game of Stools", "Projet", l_fenetre, h_fenetre);
	
	MLV_Image *image;
	image = MLV_load_image( "ChateauB.jpg" );

	Monde m;
	Agent chateauR, chateauB;

	initMonde(&m);
	initCastle(ROUGE, &chateauR);
	initCastle(BLEU, &chateauB);

	m.plateau[0][0].chateau = &chateauR;
	m.plateau[NBCOL-1][NBLIG-1].chateau = &chateauB;
	//afficheListe(m);
	
	drawArray(m, image);


	MLV_wait_seconds(105000);
	MLV_free_image(image);
	MLV_free_window();

	return 0;
}
