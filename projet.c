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

	world->tour = 1;
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


int checkProduction(int cout, int temps, char choix, int *tresor, Agent *chateau, Monde *world){

	int x, y;
	if(*tresor >= cout && (checkPosition(world->plateau, chateau, &x, &y)==0)){
		chateau->produit = choix;
		chateau->temps = temps;
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


void castleProduction(char couleur, int *tresor, Agent *chateau, Monde *world){
	
	int solve;
	char choix;
	if(chateau->produit == -1){
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
	else if(chateau->temps == 0){
		castleAgent(couleur, &(chateau->produit), world, chateau);
		chateau->produit = -1;
		drawInformation(couleur, *world);
		MLV_wait_mouse(NULL,NULL);

	}
	else if(chateau->temps > 0){
		do{
			choix = clikBoxes();
		}while(choix != 'a');
		chateau->temps--;
	}

}

void moveAgent(Agent *agent, Monde *world){

	int tmpx, tmpy;
	tmpx = agent->posx;
	tmpy = agent->posy;

	if((agent->posx != agent->destx) || (agent->posy != agent->desty)){
		if (tmpx < agent->destx){
			agent->posx += 1;
			world->plateau[agent->posx][agent->posy].habitant = agent;
			world->plateau[tmpx][tmpy].habitant = NULL;
			return;
		}
		if (tmpx > agent->destx){
			agent->posx -= 1;
			world->plateau[agent->posx][agent->posy].habitant = agent;
			world->plateau[tmpx][tmpy].habitant = NULL;
			return;
		}
		if (tmpy < agent->desty){
			agent->posy += 1;
			world->plateau[agent->posx][agent->posy].habitant = agent;
			world->plateau[tmpx][tmpy].habitant = NULL;
			return;
		}
		if (tmpy > agent->desty){
			agent->posy -= 1;
			world->plateau[agent->posx][agent->posy].habitant = agent;
			world->plateau[tmpx][tmpy].habitant = NULL;
			return;
		}
	}
	agent->destx = 0;
	agent->desty = 0;
}


void deplaceAgent(char couleur, Agent *agent, Monde *world){

	int x, y, i, j;
	while(1){
		int lig=10, col=10;
		MLV_wait_mouse(&x, &y);
		for(i=0; i < NBLIG; i++){
			for(j=0; j < NBCOL; j++){
				if(( 0 <= x && x <= 1090) && (0 <= y && y <= 730)){
					if((lig <= y && lig + 60 >= y) && (col <= x && col + 60 >= x)){
						if (world->plateau[i][j].chateau == NULL && world->plateau[i][j].habitant == NULL ){
							actuMonde(*world, couleur);
							MLV_draw_text(L_FENETRE-220, H_FENETRE-200, "Case valide", MLV_COLOR_WHITE);
							MLV_actualise_window();
							agent->destx = i;
							agent->desty = j;
							return;
						}
					}
				}
				else{
					actuMonde(*world, couleur);
					MLV_draw_text(L_FENETRE-270, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, agent->genre, agent->clan);
					MLV_draw_text(L_FENETRE-270, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  agent->posx, agent->posy);
					MLV_draw_text(L_FENETRE-220, H_FENETRE-200, "Case non valide", MLV_COLOR_WHITE);
					MLV_actualise_window();
				}
				col+=60;
				
			}
			lig+=60;
			col = 10;

			
		}
	}
}

void parcoursClan(char couleur, AListe equipe, Monde *world, int *tresor){
	
	actuMonde(*world, couleur);
	drawInformation(couleur, *world);	
	for(;equipe != NULL; equipe = equipe->asuiv){
		if(equipe->genre == CHATEAU){
			MLV_draw_text(L_FENETRE-270, H_FENETRE-500, "Tour de chateau %c  coordonnée : [%d,%d]", MLV_COLOR_WHITE, couleur, equipe->posx, equipe->posy);
			MLV_actualise_window();
			castleProduction(couleur, tresor, equipe, world);
			actuMonde(*world, couleur);
		}
		else{
			MLV_draw_text(L_FENETRE-270, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, equipe->genre, equipe->clan);
			MLV_draw_text(L_FENETRE-270, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  equipe->posx, equipe->posy);

			MLV_actualise_window();
			if(equipe->destx != 0 || equipe->desty != 0)

				moveAgent(equipe, world);
			else
				deplaceAgent(couleur, equipe, world);
			actuMonde(*world, couleur);

		}
	}
}


void jeu(Monde *world){
	
	int joueur = MLV_get_random_integer(0,100);
	while(1){
		if (castleInList(world->rouge)==0 || castleInList(world->bleu)==0)
			break;

		if (joueur % 2 == 0){
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
		}
		else{
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
		}
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

