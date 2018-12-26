#include "draw.h"
#include "menu.h"


//																			INITIALISATION

Agent* createCastle(char couleur, int x, int y){
	Agent* chateau = (Agent *) malloc(sizeof(Agent));
	
	if(chateau != NULL){
		chateau->clan = couleur;
		chateau->destx = chateau->desty = -1;
		chateau->genre = CHATEAU;

		chateau->posx = x;
		chateau->posy = y;
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
		tmp->temps = 0;
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

	for (x = 0 ; x < NBCOL ; x++) 
		for (y = 0 ; y < NBLIG ; y++)
 			world->plateau[x][y]= cases;

	world->tour = 0;
	world->tresorBleu = world->tresorRouge = 50;
	world->rouge = NULL;
	world->bleu = NULL;
}


//																						MENU


int main(int argc, char const *argv[]){

	Monde m;
	char genreR, genreB;

	Agent* chateauR;
	Agent* chateauB;

	createMonde(&m);

	chateauR = createCastle(ROUGE, 0, 0);
	chateauB = createCastle(BLEU, NBCOL-1, NBLIG-1);

	m.plateau[NBCOL-1][NBLIG-1].chateau = chateauB;
	m.plateau[0][0].chateau = chateauR;

	do{
		printf("Quoi produire ? EQUIPE ROUGE\n");
		scanf(" %c",&genreR);
	}while(genreR != BARON && genreR != GUERRIER && genreR != CHATEAU && genreR != MANANT);

	do{
		printf("Quoi produire ? EQUIPE BLEU\n");
		scanf(" %c",&genreB);
	}while(genreB != BARON && genreB != GUERRIER && genreB != CHATEAU && genreB != MANANT);

	Agent* agentR = createAgent(ROUGE, genreR, 0, 1);
	Agent* agentB = createAgent(BLEU, genreB, NBCOL-1, NBLIG-2);
	
	m.plateau[agentB->posx][agentB->posy].habitant = agentB;
	m.plateau[agentR->posx][agentR->posy].habitant = agentR;

	MLV_create_window("Game Of Stools", "Projet", L_FENETRE, H_FENETRE);
	menu();
	MLV_clear_window(MLV_COLOR_BLACK);
	MLV_actualise_window();
	
	drawArray(m);
	drawBoxes();
	MLV_actualise_window();
	MLV_wait_seconds(20);
	MLV_free_window();

	return 0;
}
