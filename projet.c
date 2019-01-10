#include "fichier.h/draw.h"
#include "fichier.h/menu.h"
#include "fichier.h/checkmove.h"



//																	INITIALISATION


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
	for(;tmp != NULL; tmp = tmp->asuiv){
		printf("chateau : genre %c, clan = %c\n", tmp->genre, tmp->clan);
		tmp2 = world->plateau[tmp->posx][tmp->posy].habitant;
		for(;tmp2 != NULL; tmp2 = tmp2->asuiv){
			printf("agent : genre %c, clan = %c\n", tmp2->genre, tmp2->clan);
			}
	}
	printf("\n");
}

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

Agent* createAgent(char couleur, char genre, int x, int y){

	Agent* tmp;
	tmp = (Agent *) malloc(sizeof(Agent));
	if (tmp != NULL){
		tmp->clan = couleur;
		tmp->genre = genre;
		tmp->posx = x;
		tmp->posy = y;
		tmp->destx = tmp->desty = 0;

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


void addInClan(Agent *chateau, AListe *clan){	//	PAS POUR NIVEAU 1 JE PENSE !!!!
	
	AListe new = (Agent *) malloc(sizeof(Agent));
	new = chateau;
	new->asuiv = NULL;

	AListe tmp = (*clan);
	while(tmp->asuiv != NULL){
		tmp = tmp->asuiv;
	}
	tmp->asuiv = new;
	tmp = *clan;
	*clan = tmp;
}
/*
void trier(AListe *listHab){
	AListe cell = *listHab;
	AListe before = NULL;

	
	while(cell->asuiv != NULL && cell->genre < cell->asuiv->genre){
		before = cell;
		cell = cell->asuiv;
	}
	before = cell;
	before->aprec = cell;
	cell = cell->asuiv;
	cell->asuiv = before; 

}*/

void addInChateau(Agent *agent, AListe *listHab){

	AListe cursor = *listHab;
	if (cursor == NULL) 
		*listHab = agent;
	else {
		while(cursor->asuiv != NULL){
			cursor = cursor->asuiv;
		}
		agent->aprec = cursor;
		cursor->asuiv = agent;
	}
}
void addAgent(Agent *agent, Monde *world, AListe* listHab){
	world->plateau[agent->posx][agent->posy].habitant = agent;

	addInChateau(agent, listHab);
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
	addAgent( manant, world, &listHab);
	//afficheListec(listHab, world);

	checkPosition(world->plateau, &chateau, &x, &y);
	Agent* baron = createAgent(couleur, BARON, x, y);
	addAgent(baron, world, &listHab);
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
	addAgent( agent, world, &listHab);
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
	else if(tmp->temps == 0){
		castleAgent(couleur, &(tmp->produit), world, chateau);
		tmp->produit = -1;
		drawInformation(couleur, *world);
	}
	else if(tmp->temps > 0){
		do{
			MLV_draw_text(L_FENETRE-245, H_FENETRE-480, "Chateau %c  en production", MLV_COLOR_YELLOW, couleur);
			MLV_actualise_window();
			choix = clikBoxes();
		}while(choix != 'a');
		tmp->temps--;
	}
}

void choixDeplacement(char couleur, Agent *agent, Monde *world){

	int x, y, i, j;
	while(1){
		int lig=10, col=10;
		MLV_wait_mouse(&x, &y);
		for(i=0; i < NBLIG; i++){
			for(j=0; j < NBCOL; j++){
				if(( 0 <= x && x <= 1090) && (0 <= y && y <= 730)){
					if((lig <= y && lig + 60 >= y) && (col <= x && col + 60 >= x)){
						if (world->plateau[i][j].chateau == NULL ){
							actuMonde(*world, couleur);
							MLV_actualise_window();
							agent->destx = i;
							agent->desty = j;
							return;
						}
					}
				}
				else{
					actuMonde(*world, couleur);
					MLV_draw_text(L_FENETRE-250, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, agent->genre, agent->clan);
					MLV_draw_text(L_FENETRE-250, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  agent->posx, agent->posy);
					MLV_draw_text(L_FENETRE-220, H_FENETRE-300, "Case non valide", MLV_COLOR_WHITE);
					MLV_actualise_window();
				}
				col+=60;
			}
			lig+=60;
			col = 10;
		}
	}
}


void parcoursCastle(char couleur, AListe equipe, Monde *world, int *tresor){

	int res = 0;
	actuMonde(*world, couleur);
	MLV_draw_text(L_FENETRE-240, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, equipe->genre, equipe->clan);
	MLV_draw_text(L_FENETRE-235, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  equipe->posx, equipe->posy);
	MLV_actualise_window();

	if((equipe->destx > 0 || equipe->desty > 0 )){
		moveAgent(equipe, world);
		if(equipe->posx == equipe->destx && equipe->posy == equipe->desty){
			equipe->destx = 0;
			equipe->desty = 0;
		}
	}
	else if((equipe->destx == -1 && equipe->desty == -1) && equipe->genre == MANANT)
		(*tresor)++;
	
	else{
		do{
			if(couleur == ROUGE)
				res = drawBoxesAgent(equipe, world->tresorRouge);
			else if(couleur == BLEU)
				res = drawBoxesAgent(equipe, world->tresorBleu);
			//Choix 2 correspond au bouton déplacer
			if(res == 2){
				//si la destination n'est pas à 0 on ne choisi pas de case, l'agent se déplace tout seul
				
				MLV_draw_text(L_FENETRE-220, H_FENETRE-550, "Cliquer sur une case", MLV_COLOR_YELLOW);
				MLV_actualise_window();
				choixDeplacement(couleur, equipe, world);
				
			}
			else if(res == 1){
				if(equipe->genre == GUERRIER)
					world->plateau[equipe->posx][equipe->posy].clan = couleur;
				else if(equipe->genre == MANANT && world->plateau[equipe->posx][equipe->posy].clan != LIBRE){
					equipe->destx = -1;
					equipe->desty = -1;
				}
			}
			else if(res == 3){
				if(equipe->genre == MANANT){
					equipe->genre = GUERRIER;
					world->plateau[equipe->posx][equipe->posy].habitant = equipe;
					if(couleur == ROUGE)
						world->tresorRouge -= CGUERRIER;
					else if(couleur == BLEU)
						world->tresorBleu -= CGUERRIER;
				}
			}
		}while(res == 0);
	}
}


void parcoursClan(char couleur, AListe equipe, Monde *world, int *tresor){

	AListe tmp;
	actuMonde(*world, couleur);
	drawInformation(couleur, *world);	
	for(;equipe != NULL; equipe = equipe->asuiv){
		if(equipe->genre == CHATEAU){
			drawBoxes();
			MLV_draw_text(L_FENETRE-275, H_FENETRE-500, "Tour de chateau %c  coordonnée : [%d,%d]", MLV_COLOR_WHITE, couleur, equipe->posx, equipe->posy);
			MLV_actualise_window();
			castleProduction(couleur, tresor, &equipe, world);
			tmp = world->plateau[equipe->posx][equipe->posy].habitant;
			//trier(&tmp);
			afficheListec(tmp, world);

			for(;tmp != NULL; tmp=tmp->asuiv)
				parcoursCastle(couleur, tmp, world, tresor);
		}
	}
}

void jeu(Monde *world){
	
	int joueur = MLV_get_random_integer(0,100);
	while(1){
		if (castleInList(world->rouge) == 0 || castleInList(world->bleu) == 0)
			break;

		if (joueur % 2 == 0){

			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			afficheListec(world->rouge, world);
			afficheListec(world->bleu, world);

		}
		else{

			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			afficheListec(world->bleu, world);
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

