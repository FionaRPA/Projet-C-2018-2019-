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


AListe createClan(Monde *world, Agent* chateau){

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

	for (x = 0 ; x < NBCOL ; x++) 
		for (y = 0 ; y < NBLIG ; y++)
 			world->plateau[x][y]= cases;

	world->tour = 0;
	world->tresorBleu = world->tresorRouge = 50;
	world->rouge = NULL;
	world->bleu = NULL;
}


void addClan(AListe *couleur, Agent *agent){

	AListe tmp = (*couleur);
	while(tmp->asuiv != NULL){
		tmp = tmp->asuiv;
	}
	tmp->asuiv = agent;
	tmp->asuiv->asuiv = NULL;
	(*couleur) = tmp;
}


void addAgent(char couleur, Agent *agent, Monde *world){

	world->plateau[agent->posx][agent->posy].habitant = agent;
	if (couleur == ROUGE){
		//printf("chateau %c\n", world->rouge->clan);
		addClan(&(world->rouge), agent);
	}
	else if (couleur == BLEU)
		addClan(&(world->bleu), agent);
}


int castleInList(AListe couleur){

	for(; couleur != NULL; couleur = couleur->asuiv){
		if(couleur->genre == CHATEAU)
			return 1;
	}
	return 0;
}



void jeu(Monde *world){

	int first = 0;
	int last = NBCOL-1;

	while(1){
		MLV_clear_window(MLV_COLOR_BLACK);
		MLV_actualise_window();
		drawArray(*world);

		char genreR = clikBoxes();
		Agent* agentR = createAgent(ROUGE, genreR, first, 1);
		addAgent(ROUGE, agentR, world);

		MLV_clear_window(MLV_COLOR_BLACK);
		MLV_actualise_window();
		drawArray(*world);

		char genreB = clikBoxes();
		Agent* agentB = createAgent(BLEU, genreB, last, NBLIG-2);
		addAgent(BLEU, agentB, world);

		if (castleInList(world->rouge)==0 || castleInList(world->bleu)==0)
			break;

		first++;
		last--;
		//printf("R : %c\n", m.plateau[agentR->posx][agentR->posy].habitant->clan);
	}
}


//																						MENU


int main(int argc, char const *argv[]){

	Monde m;

	Agent* chateauR, *chateauB;

	createMonde(&m);

	chateauR = createCastle(ROUGE, 0, 0);
	m.rouge = createClan(&m,chateauR);
	m.plateau[0][0].chateau = chateauR;

	chateauB = createCastle(BLEU, NBCOL-1, NBLIG-1);
	m.bleu = createClan(&m,chateauB);
	m.plateau[NBCOL-1][NBLIG-1].chateau = chateauB;
	
	MLV_create_window("Game Of Stools", "Projet", L_FENETRE, H_FENETRE);

	menu();
	
	jeu(&m);

	MLV_actualise_window();
	MLV_wait_seconds(20);
	MLV_free_window();

	return 0;
}
