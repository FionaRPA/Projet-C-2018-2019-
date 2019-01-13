#include "fichier.h/draw.h"
#include "fichier.h/createAndAdd.h"
#include "fichier.h/attack.h"
#include "fichier.h/checkmove.h"
#include "fichier.h/menu.h"



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

int castleInList(AListe clan){

	AListe tmp = clan;
	for(; tmp != NULL; tmp = tmp->asuiv){
		if(tmp->genre == CHATEAU)
			return 1;
	}
	return 0;
}

/*
-- Cette fonction sauvegarde notre partie au point où on l'a laissé
*/
void save(AListe clanRouge, AListe clanBleu, Monde world){

	AListe clan = clanRouge;

	FILE * fichier = NULL;
	fichier = fopen("save.txt", "w+");
	//Le Monde 
	fprintf(fichier, "%d %d %d", world.tour, world.tresorRouge, world.tresorBleu);
	//Parcours liste du clan Rouge
	for(; clan != NULL; clan=clan->vsuiv){
		//Chateau !!
		fprintf(fichier, "\n%c%c %d %d \n%c %d", clan->clan, clan->genre, clan->posx, clan->posy, clan->produit, clan->temps);
		AListe tmp = world.plateau[clan->posx][clan->posy].habitant;
		//Parcours la liste des habitants contenu dans ce chateau
		for(;tmp != NULL; tmp=tmp->asuiv){
			fprintf(fichier, "\n%c%c %d %d %d %d",tmp->clan, tmp->genre, tmp->posx, tmp->posy, tmp->destx, tmp->desty);
		}
	}
	clan = clanBleu;
	//Parcours liste du can Bleu
	for(;clan != NULL; clan=clan->vsuiv){
		//Chateau
		fprintf(fichier, "\n%c%c %d %d \n%c %d", clan->clan, clan->genre, clan->posx, clan->posy, clan->produit, clan->temps);
		AListe tmp = world.plateau[clan->posx][clan->posy].habitant;
		//Liste des habitants du chateau
		for(;tmp != NULL; tmp=tmp->asuiv){
			fprintf(fichier, "\n%c%c %d %d %d %d",tmp->clan, tmp->genre, tmp->posx, tmp->posy, tmp->destx, tmp->desty);
		}
	}
	fclose(fichier);
}

/*
-- Fonction pour commencer avec une partie déjà sauvegarder 
*/
void loadedGame(Monde *world){

	char chaine[100] = "";
	int largeur_txt, hauteur_txt, nbR=0, nbB=0;
	Agent *agent = (Agent *) malloc(sizeof(Agent));
	AListe chateau = (Agent *) malloc(sizeof(Agent));
	AListe listHab = NULL;
	AListe rouge = world->rouge;
	AListe bleu = world->bleu;
	
	FILE* fichier = NULL;
	fichier = fopen("save.txt", "r");

	if(fichier == NULL){

		MLV_clear_window(MLV_COLOR_DARKSEAGREEN1);
		MLV_Font* font = MLV_load_font("image/A Box For.ttf", L_FENETRE*0.2);

		MLV_get_size_of_text_with_font("Aucun Fichier", &largeur_txt, &hauteur_txt, font);
		MLV_draw_text_with_font((L_FENETRE-largeur_txt-50)/2, H_FENETRE*0.01, "Aucun Fichier", font, MLV_COLOR_GREEN4);

		MLV_get_size_of_text_with_font("en Sauvegarde", &largeur_txt, &hauteur_txt, font);
		MLV_draw_text_with_font((L_FENETRE-largeur_txt)/2, H_FENETRE/2, "en Sauvegarde", font, MLV_COLOR_GREEN4);
		MLV_free_font(font);
		MLV_actualise_window();
		MLV_wait_mouse(NULL, NULL);
	}
	else{
		int ret = fscanf(fichier, "%d %d %d", &(world->tour), &(world->tresorRouge), &(world->tresorBleu));
		//printf("t:%d, r:%d, b:%d\n", world->tour, world->tresorRouge, world->tresorBleu);

		while(fgets(chaine, 100, fichier) != NULL && ret != 0){

			agent->clan = fgetc(fichier);

			agent->genre = fgetc(fichier);
			//printf("g: %c\n", agent->genre);

			ret = fscanf(fichier,"%d %d", &(agent->posx), &(agent->posy));
			//printf("x: %d, y: %d\n", agent->posx, agent->posy);

			if(agent->genre == CHATEAU){

				listHab = NULL;
				char *res = fgets(chaine, 100, fichier);
				if(res != 0){
					agent->produit = fgetc(fichier);
					//printf("p: %c\n", agent->produit);

					ret = fscanf(fichier, "%d", &(agent->temps));
					//printf("t: %d\n", agent->temps);
				}

				chateau = agent;

				chateau->vsuiv = NULL;
				chateau->vprec = NULL;
				chateau->asuiv = NULL;
				chateau->aprec = NULL;

				world->plateau[chateau->posx][chateau->posy].chateau = chateau;
				if(chateau->clan == ROUGE){
					if(nbR == 0){
						rouge = createClan(world, chateau);
						nbR++;
					}
					else
						addInClan(chateau, &(rouge));
				}else if(chateau->clan == BLEU){
					if(nbB == 0){
						bleu = createClan(world, chateau);
						nbB++;
					}
					else
						addInClan(chateau, &(bleu));
				}
			}
			else if(agent->genre != CHATEAU){
				ret = fscanf(fichier,"%d %d", &(agent->destx), &(agent->desty));
				//printf("dx: %d, dy: %d\n", agent->destx, agent->desty);

				agent->asuiv = NULL;
				agent->aprec = NULL;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				addInChateau(agent, &listHab);
				//afficheListeChateau(listHab);
				world->plateau[chateau->posx][chateau->posy].habitant = listHab;
				//afficheListec(world->rouge,world);
			}
		}
	}
	fclose(fichier);
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

	checkPosition(world->plateau, &chateau, &x, &y);
	Agent* baron = createAgent(couleur, BARON, x, y);
	addAndTri( baron, &listHab, world, couleur, BARON, x , y);	
	world->plateau[chateau->posx][chateau->posy].habitant = listHab;
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
	if(tmp->produit == LIBRE){
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
				case 's':
					save(world->rouge, world->bleu, *world);
					MLV_draw_text(L_FENETRE-220, H_FENETRE-500, "Partie Sauvegardé ", MLV_COLOR_YELLOW);
					MLV_draw_text(L_FENETRE-220, H_FENETRE-480, "Cliquer sur Quitter ", MLV_COLOR_YELLOW);
					MLV_actualise_window();
					castleProduction(couleur, tresor, chateau, world);
			}
		}while(solve == -1);
	}
	else if(tmp->temps == 1){
		castleAgent(couleur, &(tmp->produit), world, chateau);
		tmp->produit = LIBRE;
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

	//Si l'agent n'est pas encore arrivé à destination on continue de le bouger automatiquement
	if((agent->destx > -1 || agent->desty > -1 )){
		moveAgent(agent, world, *tresor);
		actuMonde(*world, couleur);
		if(agent->posx == agent->destx && agent->posy == agent->desty){
			if(world->plateau[agent->posx][agent->posy].clan != agent->clan && world->plateau[agent->posx][agent->posy].clan != LIBRE)
				world->plateau[agent->posx][agent->posy].clan = LIBRE;
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
			else if(res == 4){
				save(world->rouge, world->bleu, *world);
				MLV_draw_text(L_FENETRE-220, H_FENETRE-450, "Partie Sauvegardé ", MLV_COLOR_YELLOW);
				MLV_draw_text(L_FENETRE-220, H_FENETRE-435, "Cliquer sur Quitter ", MLV_COLOR_YELLOW);
				MLV_actualise_window();

			}
		}while(res == 0 || res == 4);
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

void perdu(char* message){

	int largeur_txt, hauteur_txt;

	MLV_clear_window(MLV_COLOR_DARKSEAGREEN1);

	MLV_Font* font = MLV_load_font("image/A Box For.ttf", L_FENETRE*0.2);

	MLV_get_size_of_text_with_font(message, &largeur_txt, &hauteur_txt, font);
	MLV_draw_text_with_font((L_FENETRE-largeur_txt-50)/2, H_FENETRE*0.01, message, font, MLV_COLOR_GREEN4);

	MLV_get_size_of_text_with_font("A PERDU!", &largeur_txt, &hauteur_txt, font);
	MLV_draw_text_with_font((L_FENETRE-largeur_txt)/2, H_FENETRE/2, "A PERDU!", font, MLV_COLOR_GREEN4);
	MLV_free_font(font);
	MLV_actualise_window();
	MLV_wait_mouse(NULL, NULL);
}

/*
-- C'est une fonction principale, qui gère le jeu.
-- 
*/
void jeu(Monde *world){
	
	int joueur;

	while(1){
		joueur = MLV_get_random_integer(0,100);
		if (castleInList(world->rouge) == 0){
			printf("hey\n");
			perdu("L'EQUIPE ROUGE");
			break;
		}
		printf("weshhh\n");
		if(castleInList(world->bleu) == 0){
			printf("heyb\n");			
			perdu("L'EQUIPE BLEU");			
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
	
	MLV_create_window("Game Of Stools", "Projet", L_FENETRE, H_FENETRE);

	if(menu() == 1)
		loadedGame(&m);
	else{
		initArray(ROUGE, &m.rouge, &m, 0, 0);
		initArray(BLEU, &m.bleu, &m, NBLIG-1, NBCOL-1);
	}
	
	jeu(&m);
	MLV_actualise_window();
	MLV_free_window();

	return 0;
}

