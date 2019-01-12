#include "fichier.h/draw.h"
#include "fichier.h/menu.h"
#include "fichier.h/checkmove.h"



//																	INITIALISATION

void afficheListeChateau(AListe L){
	AListe tmp = L;
	for(;tmp != NULL; tmp = tmp->asuiv){
		printf("chateau : genre %c, clan = %c\n", tmp->genre, tmp->clan);
	}
	printf("\n");
}

void afficheListe(Case plateau[NBLIG][NBCOL]){
	int i, j;
	for(i=0; i<NBLIG; i++){
		for(j=0;j < NBCOL; j++){
			if(plateau[i][j].habitant != NULL ){
				printf("hab: %c [%d;%d]\n",plateau[i][j].habitant->genre, plateau[i][j].habitant->posx, plateau[i][j].habitant->posy);
			}
			if(plateau[i][j].chateau != NULL )
				printf("chat: %c [%d;%d]\n",plateau[i][j].chateau->genre, plateau[i][j].chateau->posx, plateau[i][j].chateau->posy );
		}
	}
}

void afficheListec(AListe L, Monde *world){
	AListe tmp = L;
	AListe tmp2;
	for(;tmp != NULL; tmp = tmp->vsuiv){
		printf("chateau : genre %c, clan = %c\n", tmp->genre, tmp->clan);
		tmp2 = world->plateau[tmp->posx][tmp->posy].habitant;
		for(;tmp2 != NULL; tmp2 = tmp2->asuiv){
			printf("agent : genre %c, clan = %c\n", tmp2->genre, tmp2->clan);
		}
	}
	printf("\n");
}
/*
-- Cette fonction renvoie le chateau que l'on créé en initialisant son clan, sa destination (-2 car il est immobile), son genre,
-- ses positions, s'il est en production (-1 s'il ne l'est pas), et le temps restant pour une production (initialement à 0).
*/
AListe createCastle(char couleur, int x, int y){

	AListe chateau = (Agent *) malloc(sizeof(Agent));
	if(chateau != NULL){
		chateau->clan = couleur;
		chateau->destx = chateau->desty = -1;
		chateau->genre = CHATEAU;
		chateau->produit = -1;
		chateau->temps = 0;
		chateau->posx = x;
		chateau->posy = y;

		chateau->vsuiv = NULL;
		chateau->vprec = NULL;
	}
	return chateau;
}
/*
-- On renvoie l'agent que l'on créé en initialisant son clan, sa destination, son genre,
-- ses positions.
*/
Agent* createAgent(char couleur, char genre, int x, int y){

	Agent* tmp;
	tmp = (Agent *) malloc(sizeof(Agent));
	if (tmp != NULL){
		tmp->clan = couleur;
		tmp->genre = genre;
		tmp->posx = x;
		tmp->posy = y;
		tmp->destx = tmp->desty = -1;

		tmp->asuiv = NULL;
		tmp->aprec = NULL;
	}
	return tmp;
}

AListe createClan(Monde *world, AListe chateau){
	
	AListe tmp;
	tmp = (Agent *) malloc(sizeof(Agent));
	if (tmp != NULL){
		tmp = chateau;
		tmp->vsuiv = NULL;
		tmp->vprec = NULL;
		tmp->asuiv = NULL;
		tmp->aprec = NULL;
	}
	return tmp;
}

void createMonde(Monde *world){
	
	int x, y;
	Case cases;
	cases.chateau = cases.habitant = NULL;
	cases.clan = LIBRE;

	for (x = 0 ; x < NBLIG ; x++) 
		for (y = 0 ; y < NBCOL ; y++)
 			world->plateau[x][y]= cases;

	world->tour = 1;
	world->tresorBleu = world->tresorRouge = 50;
	world->rouge = NULL;
	world->bleu = NULL;
}

/*fonction qui ajoute un chateau dans son clan "rouge" ou "bleu"*/
void addInClan(Agent *chateau, AListe *clan){

	AListe cursor = *clan;
	if (cursor == NULL)
		*clan = chateau;
	else {
		while(cursor->vsuiv != NULL){
			cursor = cursor->vsuiv;
		}
		chateau->vprec = cursor;
		cursor->vsuiv = chateau;
	}
}

int addAndTri(Agent *agent, AListe *listHab, Monde *world, char couleur, char genre, int x, int y){

 	AListe cell = *listHab;
	agent = createAgent(couleur, genre, x, y);
	world->plateau[agent->posx][agent->posy].habitant = agent;
	AListe before = NULL;

	if(agent == NULL)
		return -1;
	while(cell != NULL && cell->genre < agent->genre){
		before = cell;
		cell = cell->asuiv;
	}
	agent->asuiv = cell;
	agent->aprec = before;
	if(before == NULL){
		*listHab = agent;
	}
	else{
		agent->aprec = before;
		before->asuiv = agent;
	}
	return 0;
}

int castleInList(AListe clan){

	AListe tmp = clan;
	for(; tmp != NULL; tmp = tmp->asuiv){
		if(tmp->genre == CHATEAU)
			return 1;
	}
	return 0;
}

void initArray(char couleur, AListe *clan, Monde *world, int x, int y){

	AListe chateau; 
	AListe listHab = NULL;

	chateau = createCastle(couleur, x, y);
	*clan = createClan(world,chateau);
	world->plateau[x][y].chateau = chateau;

	checkPosition(world->plateau, &chateau, &x, &y);
	Agent* manant = createAgent(couleur, MANANT, x, y);
	addAndTri( manant, &listHab, world, couleur, MANANT, x , y);
	//afficheListec(listHab, world);

	checkPosition(world->plateau, &chateau, &x, &y);
	Agent* baron = createAgent(couleur, BARON, x, y);
	addAndTri( baron, &listHab, world, couleur, BARON, x , y);	
	world->plateau[chateau->posx][chateau->posy].habitant = listHab;
	//printf("-------Liste Chateau---------\n");
	//afficheListec(chateau, world);
	//afficheListec(listHab);
	//printf("\n");
}

void castleAgent(char couleur, char *genre, Monde *world, AListe* chateau){

	int x, y;
	AListe listHab = world->plateau[(*chateau)->posx][(*chateau)->posy].habitant;
	checkPosition(world->plateau, chateau, &x, &y);
	Agent* agent = createAgent(couleur, *genre, x, y);
	addAndTri( agent, &listHab, world, couleur, *genre, x, y);
	world->plateau[(*chateau)->posx][(*chateau)->posy].habitant = listHab;
	MLV_clear_window(MLV_COLOR_BLACK);
	MLV_actualise_window();
	drawArray(*world);
}

int checkProduction(int cout, int temps, char choix, int *tresor, AListe *chateau, Monde *world){

	int x, y;
	if(*tresor >= cout && (checkPosition(world->plateau, chateau, &x, &y)==0)){
		(*chateau)->produit = choix;
		(*chateau)->temps = temps;
		*tresor-=cout;
	}
	else if(checkPosition(world->plateau, chateau, &x, &y) == 1){
		MLV_draw_text(L_FENETRE-280, H_FENETRE-480, "Plus de place pour placer l'agent",MLV_COLOR_RED);
		MLV_draw_text(L_FENETRE-220, H_FENETRE-450, "Autre choix",MLV_COLOR_WHITE);
		MLV_actualise_window();
		return -1;
	}
	else{
		MLV_draw_text(L_FENETRE-280, H_FENETRE-480, "Pas assez d'argent",MLV_COLOR_GREEN);
		MLV_draw_text(L_FENETRE-220, H_FENETRE-450, "Autre choix",MLV_COLOR_WHITE);
		MLV_actualise_window();
		return -1;
	}
	return 0;
} 

void castleProduction(char couleur, int *tresor, AListe *chateau, Monde *world){
	
	int solve;
	char choix;
	AListe tmp = *chateau;
	if(tmp->produit == -1){
		do{
			choix = clikBoxes();
			switch(choix){
				case 'a':
					solve = 0;
					break;
				case BARON:
					solve = checkProduction(CBARON, TBARON, BARON, tresor, chateau, world);
					break;
				case GUERRIER:
					solve = checkProduction(CGUERRIER, TGUERRIER, GUERRIER, tresor, chateau, world);
					break;
				case MANANT:
					solve = checkProduction(CMANANT, TMANANT, MANANT, tresor, chateau, world);
					break;
			}
		}while(solve == -1);
	}
	else if(tmp->temps == 1){
		castleAgent(couleur, &(tmp->produit), world, chateau);
		tmp->produit = -1;
		actuMonde(*world, couleur);
	}
	else if(tmp->temps > 1){
		do{
			MLV_draw_text(L_FENETRE-245, H_FENETRE-480, "Chateau %c en production", MLV_COLOR_YELLOW, couleur);
			MLV_actualise_window();
			choix = clikBoxes();
		}while(choix != 'a');
		tmp->temps--;
	}
}


void parcoursCastle(char couleur, AListe agent, Monde *world, int *tresor){

	int res = 0;
	actuMonde(*world, couleur);
	MLV_draw_text(L_FENETRE-240, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, agent->genre, agent->clan);
	MLV_draw_text(L_FENETRE-235, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  agent->posx, agent->posy);
	MLV_actualise_window();

	//Si l'agent n'est pas encore arrivé à destination on continue de la bouger automatiquement
	if((agent->destx > -1 || agent->desty > -1 )){
		moveAgent(agent, world, *tresor);
		if(agent->posx == agent->destx && agent->posy == agent->desty){
			// ou egale a LIBRE ?????
				agent->destx = -1;
				agent->desty = -1;
		}
	}
	//Production de richesse par mannant
	else if((agent->destx == -2 && agent->desty == -2) && agent->genre == MANANT)
		(*tresor)++;
	
	else{
		do{
			if(couleur == ROUGE)
				res = drawBoxesAgent(agent, world->tresorRouge);
			else if(couleur == BLEU)
				res = drawBoxesAgent(agent, world->tresorBleu);
			//Choix 2 correspond au bouton déplacer
			if(res == 2){
				//si la destination n'est pas à 0 on ne choisi pas de case, l'agent se déplace tout seul
				
				MLV_draw_text(L_FENETRE-220, H_FENETRE-550, "Cliquer sur une case", MLV_COLOR_YELLOW);
				MLV_actualise_window();
				choixDeplacement(couleur, agent, world);
				
			}
			//Choix 1 correspond au bouton Immobile
			else if(res == 1){
				if(agent->genre == GUERRIER)
					world->plateau[agent->posx][agent->posy].clan = couleur;
				else if(agent->genre == MANANT && world->plateau[agent->posx][agent->posy].clan != LIBRE){
					agent->destx = -2;
					agent->desty = -2;
				}
			}
			//Choix 3 correspond au bouton du MANANT qui devient un GUERRIER
			else if(res == 3){
				if(agent->genre == MANANT){
					agent->genre = GUERRIER;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					if(couleur == ROUGE)
						world->tresorRouge -= CGUERRIER;
					else if(couleur == BLEU)
						world->tresorBleu -= CGUERRIER;
				}
			}
		}while(res == 0 && res == 4);
	}
}

/*
-- Cette fonction parcours la liste du clan ("rouge"/"bleu") où est situé notre chateau de base
-- Elle prend en paramètre la couleur de clan qui joue la liste equipe qui correspond a notre liste clan, notre monde en pointeur car on le modifie
-- et enfin le tresor du clan en pointeur aussi pour pouvoir le modifier.
*/
void parcoursClan(char couleur, AListe equipe, Monde *world, int *tresor){

	AListe listHab;
	actuMonde(*world, couleur);
	drawInformation(couleur, *world);	
	for(;equipe != NULL; equipe = equipe->asuiv){
		if(equipe->genre == CHATEAU){
			drawBoxes();
			MLV_draw_text(L_FENETRE-275, H_FENETRE-500, "Tour de chateau %c  coordonnée : [%d,%d]", MLV_COLOR_WHITE, couleur, equipe->posx, equipe->posy);
			MLV_actualise_window();
			castleProduction(couleur, tresor, &equipe, world);
			listHab = world->plateau[equipe->posx][equipe->posy].habitant;

			for(;listHab != NULL; listHab = listHab->asuiv)
				parcoursCastle(couleur, listHab, world, tresor);
		}
	}
}
/*
void sauvegarde(ALitse clanRouge, AListe clanBleu, Monde world){

	AListe rouge = clanRouge;
	AListe bleu = clanBleu;

	FILE * fichier = NULL;
	fichier = fopen("save.txt", "w+");

	for(; rouge != NULL; rouge=rouge->vsuiv){
		//Chateau !!
		fprintf(fichier, "%c %d %d\n", rouge->genre, rouge->posx, rouge->posy);
		
		AListe tmp = world.plateau[rouge->posx][rouge->posy].habitant;
		for(;tmp != NULL; tmp=tmp->asuiv){

		}
	}
}
*/

/*
-- C'est une fonction principale, qui gère le jeu.
-- 
*/
void jeu(Monde *world){
	
	int joueur;
	while(1){
		joueur = MLV_get_random_integer(0,100);
		if (castleInList(world->rouge) == 0 || castleInList(world->bleu) == 0){
			printf("perdu\n");
			break;
		}

		if (joueur % 2 == 0){
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			afficheListec(world->rouge, world);
			//afficheListec(world->bleu, world);
		}
		else{
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			//afficheListec(world->bleu, world);
			afficheListec(world->rouge, world);

		}
		//afficheListe(world->plateau);
		world->tour++;
	}
}

//																				MAIN

int main(int argc, char const *argv[]){

	Monde m;
	createMonde(&m);	
	initArray(ROUGE, &m.rouge, &m, 0, 0);
	initArray(BLEU, &m.bleu, &m, NBLIG-1, NBCOL-1);

	MLV_create_window("Game Of Stools", "Projet", L_FENETRE, H_FENETRE);

	menu();
	jeu(&m);

	MLV_actualise_window();
	MLV_free_window();

	return 0;
}

