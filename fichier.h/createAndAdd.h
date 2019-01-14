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
		chateau->produit = LIBRE;
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
		chateau->vsuiv = NULL;
	}
}

int addAndTri(Agent *agent, AListe *listHab, Monde *world, char couleur, char genre, int x, int y){

 	AListe cell = *listHab;
	agent = createAgent(couleur, genre, x, y);
	world->plateau[agent->posx][agent->posy].habitant = agent;
	AListe before = NULL;

	if(cell == NULL){
		*listHab = agent;
	}
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
		agent->asuiv = NULL;
	}
}