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

void afficheListec(AListe L){
	AListe tmp = L;
	for(;tmp != NULL; tmp = tmp->asuiv){
		printf("genre %c, c = %c\n", tmp->genre, tmp->clan);
	}
}

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

void addChateau(Agent *agent, AListe *chateau){
	
	AListe new = (Agent *) malloc(sizeof(Agent));
	new = agent;
	new->asuiv = NULL;

	AListe tmp = (*chateau);
	while(tmp->asuiv != NULL){
		tmp = tmp->asuiv;
	}
	tmp->asuiv = new;
	tmp = *chateau;
	*chateau = tmp;
}

void addClan(Agent *chateau, AListe *clan){
	
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
void trier(AListe *L){// ici on va ranger les elements dans l'ordre croisssant avant de les afficher.

	AListe cell, tmp1, tmp2;
	AListe min;
	for(cell= (*L)->asuiv ; cell!=NULL ; cell=cell->asuiv){
		tmp2 = cell;
		min = cell;
		for(tmp1 = cell->asuiv ; tmp1!=NULL ; tmp1 = tmp1->asuiv){
			if(min->genre > tmp1->genre){
				tmp2 = tmp1; // le 3è temporaire est l'adresse de l'élement où se trouve le minimum
				min = tmp2;
			}
			else if(min->genre == tmp1->genre){
				printf("le meme agent\n");
				tmp1->asuiv = min;

			}
		}
		tmp2 = cell; //echange des 2 elements...
		cell = min;
	}

}*/


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

void initArray(char couleur, AListe *clan, Monde *world, int x, int y){

	Agent* chateau;

	chateau = createCastle(couleur, x, y);
	*clan = createClan(world,chateau);
	world->plateau[x][y].chateau = chateau;

	checkPosition(world->plateau, chateau, &x, &y);
	Agent* baron = createAgent(couleur, BARON, x,y);
	addAgent(couleur, baron, world);

	checkPosition(world->plateau, chateau, &x, &y);
	Agent* manant = createAgent(couleur, MANANT, x,y);
	addAgent(couleur, manant, world);
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
	}
	else if(chateau->temps > 0){
		do{
			choix = clikBoxes();
		}while(choix != 'a');
		chateau->temps--;
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
					MLV_draw_text(L_FENETRE-270, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, agent->genre, agent->clan);
					MLV_draw_text(L_FENETRE-270, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  agent->posx, agent->posy);
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

void parcoursClan(char couleur, AListe equipe, Monde *world, int *tresor){

	int res = 0;
	actuMonde(*world, couleur);
	drawInformation(couleur, *world);	
	for(;equipe != NULL; equipe = equipe->asuiv){
		if(equipe->genre == CHATEAU){
			drawBoxes();
			MLV_draw_text(L_FENETRE-270, H_FENETRE-500, "Tour de chateau %c  coordonnée : [%d,%d]", MLV_COLOR_WHITE, couleur, equipe->posx, equipe->posy);
			MLV_actualise_window();
			castleProduction(couleur, tresor, equipe, world);
			actuMonde(*world, couleur);
		}
		else{
			actuMonde(*world, couleur);
			MLV_draw_text(L_FENETRE-270, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, equipe->genre, equipe->clan);
			MLV_draw_text(L_FENETRE-270, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  equipe->posx, equipe->posy);
			MLV_actualise_window();

			if((equipe->destx != 0 || equipe->desty != 0 )){
				moveAgent(equipe, world);
				if(equipe->posx == equipe->destx && equipe->posy == equipe->desty){
					equipe->destx = 0;
					equipe->desty = 0;
				}
			}
			
			else{
				do{
					if(couleur == ROUGE)
						res = drawBoxesAgent(equipe, world->tresorRouge);
					else if(couleur == BLEU)
						res = drawBoxesAgent(equipe, world->tresorBleu);
					//Choix 2 correspond au bouton déplacer
					if(res == 2){
						//si la destination n'est pas à 0 on ne choisi pas de case, l'agent se déplace tout seul
						
						MLV_draw_text(L_FENETRE-250, H_FENETRE-550, "Cliquer sur une case", MLV_COLOR_WHITE);
						MLV_actualise_window();
						choixDeplacement(couleur, equipe, world);
						
					}
					else if(res == 1){
						if(equipe->genre == GUERRIER)
							world->plateau[equipe->posx][equipe->posy].clan = couleur;
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
			//trier(&(world)->rouge);
			//trier(&(world)->bleu);
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			afficheListec(world->rouge);
			afficheListec(world->bleu);
		}
		else{
			//trier(&(world)->bleu);
			//trier(&(world)->rouge);
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			afficheListec(world->bleu);
			afficheListec(world->rouge);

		}
		afficheListe(world->plateau);
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

