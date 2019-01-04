#include "fichier.h/draw.h"
#include "fichier.h/menu.h"


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

	for (x = 0 ; x < NBLIG ; x++) 
		for (y = 0 ; y < NBCOL ; y++)
 			world->plateau[x][y]= cases;

	world->tour = 0;
	world->tresorBleu = world->tresorRouge = 50;
	world->rouge = NULL;
	world->bleu = NULL;
}

void addClan(Agent *agent, AListe *clan){
	
	AListe new = (Agent *) malloc(sizeof(Agent));
	new = agent;
	new->asuiv = NULL;

	AListe tmp = (*clan);
	while(tmp->asuiv != NULL){
		tmp = tmp->asuiv;
	}
	tmp->asuiv = new;
	tmp = *clan;
	*clan = tmp;
}

void addAgent(char couleur, Agent *agent, Monde *world){
	
	world->plateau[agent->posx][agent->posy].habitant = agent;
	if (couleur == ROUGE)
		addClan(agent, &(world->rouge));
	else if (couleur == BLEU)
		addClan(agent, &(world->bleu));
}


int castleInList(AListe clan){

	AListe tmp = clan;
	for(; tmp != NULL; tmp = tmp->asuiv){
		if(tmp->genre == CHATEAU)
			return 1;
	}
	return 0;
}


int checkPosition(Case plateau[NBLIG][NBCOL], Agent* chateau, int *x, int *y){

	if(chateau->posx == 0 && chateau->posy == 0){
		if(plateau[chateau->posx +1][chateau->posy].habitant == NULL){
			*x = chateau->posx+1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant ==NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posx == NBLIG-1 && chateau->posy == 0){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant ==NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posx == NBLIG-1 && chateau->posy == NBCOL-1){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy -1;
		}
		else if(plateau[chateau->posx -1][chateau->posy -1].habitant ==NULL){
			*x = chateau->posx -1;
			*y = chateau->posy -1;
			return 0;
		}
	}

	else if(chateau->posx == 0 && chateau->posy == NBCOL-1){
		if(plateau[chateau->posx +1][chateau->posy].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy -1].habitant ==NULL){
			*x = chateau->posx +1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy -1;
			return 0;
		}
	}

	else if(chateau->posx != 0 && chateau->posx != NBLIG-1 && chateau->posy == 0){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
	}

	else if(chateau->posx == 0 && chateau->posy != 0 && chateau->posy != NBCOL-1){
		if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL){
			*x = chateau->posx ;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy -1].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL){
			*x = chateau->posx ;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posx == NBLIG-1 && chateau->posy != 0 && chateau->posy == NBCOL-1){
		if(plateau[chateau->posx][chateau->posy -1].habitant == NULL){
			*x = chateau->posx ;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy -1].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL){
			*x = chateau->posx ;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posy == NBCOL-1 && chateau->posx != 0 && chateau->posx != NBLIG-1){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx][chateau->posy+1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy].habitant == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
	}	

	else if(chateau->posx != NBLIG-1 && chateau->posx != 0 && chateau->posy != NBCOL-1 && chateau->posy != 0){

		if(plateau[chateau->posx +1][chateau->posy].habitant == NULL){
			*x = chateau->posx+1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant ==NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy].habitant == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant ==NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy -1].habitant ==NULL){
			*x = chateau->posx -1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL){
			*x = chateau->posx;
			*y = chateau->posy -1;
			return 0;
		}	
		else if(plateau[chateau->posx +1][chateau->posy -1].habitant ==NULL){
			*x = chateau->posx +1;
			*y = chateau->posy -1;
			return 0;
		}
	}
	return 1;
}


void initArray(char couleur, AListe *clan, Monde *world, int x, int y){

	Agent* chateau;

	chateau = createCastle(couleur, x, y);
	*clan = createClan(world,chateau);
	world->plateau[x][y].chateau = chateau;

	checkPosition(world->plateau, chateau, &x, &y);
	Agent* manant = createAgent(couleur, MANANT, x,y);
	addAgent(couleur, manant, world);

	checkPosition(world->plateau, chateau, &x, &y);
	Agent* baron = createAgent(couleur, BARON, x,y);
	addAgent(couleur, baron, world);
}


void castleAgent(char couleur, char *genre, Monde *world, Agent* chateau){

	int x, y;
	checkPosition(world->plateau, chateau, &x, &y);
	Agent* agent = createAgent(couleur, *genre, x, y);
	addAgent(couleur, agent, world);
	MLV_clear_window(MLV_COLOR_BLACK);
	MLV_actualise_window();
	drawArray(*world);
}


void checkProduction(int cout, int temps, char choix, int *tresor, Agent *chateau, Monde *world){

	int x, y;
	if(*tresor >= CBARON && (checkPosition(world->plateau, chateau, &x, &y)==0)){
		chateau->produit = BARON;
		chateau->temps = TBARON;
		*tresor-=CBARON;
	}
	else if(checkPosition(world->plateau, chateau, &x, &y) == 1){
		MLV_draw_text(L_FENETRE-280, H_FENETRE-480, "Plus de place pour placer l'agent",MLV_COLOR_RED);
		MLV_draw_text(L_FENETRE-220, H_FENETRE-450, "Cliquer pour passer",MLV_COLOR_WHITE);
		MLV_actualise_window();
		MLV_wait_mouse(&x,&y);
	}
	else{
		MLV_draw_text(L_FENETRE-280, H_FENETRE-480, "Pas assez d'argent",MLV_COLOR_GREEN);
		MLV_draw_text(L_FENETRE-220, H_FENETRE-450, "Cliquer pour passer",MLV_COLOR_WHITE);
		MLV_actualise_window();
		MLV_wait_mouse(&x,&y);
	}
}


void castleProduction(char couleur, int *tresor, Agent *chateau, Monde *world){

	char choix;
	if(chateau->produit == -1){
		choix = clikBoxes();
		switch(choix){
			case 'a':
				break;
				
			case BARON:
				checkProduction(CBARON, TBARON, BARON, tresor, chateau, world);
				break;

			case GUERRIER:
				checkProduction(CGUERRIER, TGUERRIER, GUERRIER, tresor, chateau, world);
				break;

			case MANANT:
				checkProduction(CMANANT, TMANANT, MANANT, tresor, chateau, world);
				break;
		}
	}
	else if(chateau->produit != -1 && chateau->temps == 0){
		castleAgent(couleur, &(chateau->produit), world, chateau);
		chateau->produit = -1;
	}
	else if(chateau->produit != -1 && chateau->temps > 0){
		do{
			choix = clikBoxes();
		}while(choix != 'a');
		chateau->temps--;
	}
}


void parcoursClan(char couleur, AListe clan, Monde *world, int *tresor){
	
	drawInformation(couleur, *world);
	for(;clan != NULL; clan = clan->asuiv){
		if(clan->genre == CHATEAU){
			MLV_draw_text(L_FENETRE-220, H_FENETRE-500, "Tour de chateau %c", MLV_COLOR_WHITE, couleur);
			MLV_actualise_window();
			castleProduction(couleur, tresor, clan, world);
			MLV_clear_window(MLV_COLOR_BLACK);
			MLV_actualise_window();
			drawArray(*world);
		}
	}
}


void jeu(Monde *world){

	while(1){
		if (castleInList(world->rouge)==0 || castleInList(world->bleu)==0)
			break;
		parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
		parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
		world->tour++;
	}
}


//																				MENU

int main(int argc, char const *argv[]){

	Monde m;
	createMonde(&m);	
	initArray(ROUGE, &m.rouge, &m, 0, 0);
	initArray(BLEU, &m.bleu, &m, NBLIG-1, NBCOL-1);

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

