#include "draw.h"
#include "menu.h"


//																	INITIALISATION

Agent* createCastle(char couleur, int x, int y){

	Agent* chateau = (Agent *) malloc(sizeof(Agent));
	if(chateau != NULL){
		chateau->clan = couleur;
		chateau->destx = chateau->desty = -1;
		chateau->genre = CHATEAU;
		chateau->produit = -1;
		chateau->temps = 0;
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


void initArray(Monde *world){

	Agent* chateauR, *chateauB;
	createMonde(world);

	chateauR = createCastle(ROUGE, 0, 0);
	world->rouge = createClan(world,chateauR);
	world->plateau[0][0].chateau = chateauR;

	chateauB = createCastle(BLEU, NBCOL-1, NBLIG-1);
	world->bleu = createClan(world,chateauB);
	world->plateau[NBCOL-1][NBLIG-1].chateau = chateauB;
}


void addClan(Agent *agent, AListe *couleur){
	
	AListe new = (Agent *) malloc(sizeof(Agent));
	new = agent;
	new->asuiv = NULL;

	AListe tmp = (*couleur);
	while(tmp->asuiv != NULL){
		tmp = tmp->asuiv;
	}
	tmp->asuiv = new;
	tmp = *couleur;
	*couleur = tmp;
}


void addAgent(char couleur, Agent *agent, Monde *world){
	
	world->plateau[agent->posx][agent->posy].habitant = agent;
	if (couleur == ROUGE)
		addClan(agent, &(world->rouge));
	else if (couleur == BLEU)
		addClan(agent, &(world->bleu));
}


int castleInList(AListe couleur){

	AListe tmp = couleur;
	for(; tmp != NULL; tmp = tmp->asuiv){
		if(tmp->genre == CHATEAU)
			return 1;
	}
	return 0;
}


void castleAgent(char couleur, char *genre, Monde *world){

	printf("genre %c\n", *genre);
	Agent* agent = createAgent(couleur, *genre, 0, 2);
	addAgent(couleur, agent, world);
	MLV_clear_window(MLV_COLOR_BLACK);
	MLV_actualise_window();
	drawArray(*world);
}


void castleProduction(char couleur, Agent *chateau, Monde *world){

	char choix;
	if(chateau->produit == -1){
		choix = clikBoxes();
		switch(choix){
			case 'a':
				break;
			case BARON:
				chateau->produit = BARON;
				chateau->temps = TBARON;
				break;
			case GUERRIER:
				chateau->produit = GUERRIER;
				chateau->temps = TGUERRIER;
				break;
			case MANANT:
				chateau->produit = MANANT;
				chateau->temps = TMANANT;
				break;
		}
	}
	else if(chateau->produit != -1 && chateau->temps == 0){
		castleAgent(couleur, &(chateau->produit), world);
		chateau->produit = -1;
	}
	else if(chateau->produit != -1 && chateau->temps > 0){
		do{
			choix = clikBoxes();
		}while(choix != 'a');
		chateau->temps--;
	}
}


void parcoursClan(char couleur, AListe clan, Monde *world){

	for(;clan != NULL; clan = clan->asuiv){
		if(clan->genre == CHATEAU)
			castleProduction(couleur, clan, world);
	}
}


void jeu(Monde *world){

	int nbtour;

	while(1){
		if (castleInList(world->rouge)==0 || castleInList(world->bleu)==0)
			break;
		parcoursClan(ROUGE, world->rouge, world);
		parcoursClan(BLEU, world->bleu, world);
		nbtour++;
	}
}


//																				MENU

int main(int argc, char const *argv[]){

	Monde m;
	initArray(&m);
	MLV_create_window("Game Of Stools", "Projet", L_FENETRE, H_FENETRE);

	menu();
	MLV_clear_window(MLV_COLOR_BLACK);
	drawArray(m);
	
	jeu(&m);

	MLV_actualise_window();
	MLV_wait_seconds(20);
	MLV_free_window();

	return 0;
}

