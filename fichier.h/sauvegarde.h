/*
-- Cette fonction sauvegarde notre partie au point où on l'a laissé
*/
void save(AListe clanRouge, AListe clanBleu, Monde world){

	AListe clan = clanRouge;
	int i, j;

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
	for(i=0; i<NBLIG; i++){
		for(j=0;j < NBCOL; j++){
			if(world.plateau[i][j].clan != LIBRE){
				fprintf(fichier, "\n*");
				fprintf(fichier, "\n%c %d %d", world.plateau[i][j].clan, i, j);
			}
		}
	}
	fclose(fichier);
}

/*
-- Fonction pour commencer avec une partie déjà sauvegarder, on 
	récupére pour chaque agent son clan, son genre, sa position etc, 
	dans notre fichier de sauvegarde "save.txt". On reprend la 
	partie au tour du château qui jouait.
*/
void loadedGame(Monde *world){

	char chaine[100] = "";
	int largeur_txt, hauteur_txt, nbR=0, nbB=0, x, y;
	AListe chateau = (Agent *) malloc(sizeof(Agent));
	AListe listHab = NULL;
	AListe rouge = NULL;
	AListe bleu = NULL, tmpB = NULL, tmpR = NULL;
	char let, revendiquer;
	char *res;
	
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

		while(fgets(chaine, 100, fichier) != NULL && ret != 0){
			let = fgetc(fichier);
			if(let != '*'){
				Agent *agent = (Agent *) malloc(sizeof(Agent));
				agent->clan = let;
				agent->genre = fgetc(fichier);
				ret = fscanf(fichier,"%d %d", &(agent->posx), &(agent->posy));
				
				if(agent->genre == CHATEAU){
		
					listHab = NULL;
					res = fgets(chaine, 100, fichier);
					if(res != 0){
						agent->produit = fgetc(fichier);
						ret = fscanf(fichier, "%d", &(agent->temps));
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
							tmpR = rouge;
							nbR++;
						}
						else
							addInClan(chateau, &(rouge));
					}else if(chateau->clan == BLEU){
						if(nbB == 0){
							bleu = createClan(world, chateau);
							tmpB = bleu;
							nbB++;
						}
						else
							addInClan(chateau, &(bleu));
					}
				}
				else if(agent->genre != CHATEAU){
					ret = fscanf(fichier,"%d %d", &(agent->destx), &(agent->desty));
					agent->asuiv = NULL;
					agent->aprec = NULL;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					addInChateau(agent, &listHab);
					world->plateau[chateau->posx][chateau->posy].habitant = listHab;
				}
			}
			//On regarde les cases revendiqué.
			else if(let == '*'){
				res = fgets(chaine, 100, fichier);
				revendiquer = fgetc(fichier);
				ret = fscanf(fichier, "%d %d", &x, &y);
				world->plateau[x][y].clan = revendiquer;
			}
		}
		
	}
	world->rouge = tmpR;
	world->bleu = tmpB;
	fclose(fichier);
}