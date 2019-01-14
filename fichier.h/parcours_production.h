/*
-- On vérifie les positions libre autour du château, ensuite, 
	on prend les coorodonnées de la position valide qu'on 
	affecte à l'agent que l'on veut créer. Ce même agent est 
	ajouté dans la liste des habitants du même chateau 
	( ie : tous les agents que le château a produit).
	Pour finir on dessine notre monde.
*/
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

/*
--Fonction qui permet de vérifié en deux temps si le château a 
	assez d'argent pour produit un agent et par la suite si il y a une place disponible 
	pour pouvoir placer l'agent autour de son château.
*/
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

/*
--Si le château ne produit pas d'agent, on attend que l'utilisateur selectionne un choix.
	En fonction de son choix, on lance checkProduction seulement si il choisi de produire 
	un agent, sinon le bouton attendre lui fait passer son tour.
	Si le château produit un agent et que son temps de production n'est pas atteint, le joueur 
	a dans l'obligation de cliquer sur "ATTENDRE" pour désincrémenter le temps de production de -1
	pour pouvoir arriver à 1.
	Quand le temps vaut 1, l'agent est ensuite créé et affiché à la position récupéré grâce à la 
	fonction checkPosition.
*/
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
					MLV_draw_text(L_FENETRE-220, H_FENETRE-440, "Partie Sauvegardé ", MLV_COLOR_YELLOW);
					MLV_draw_text(L_FENETRE-220, H_FENETRE-425, "Cliquer sur Quitter ", MLV_COLOR_YELLOW);
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

/*
--Cette fonction permet d'effacer tous les liens entre le Baron et son château car il 
	sera lié au nouveau qu'il vient de construire. Sa position sera reprise par le nouveau chateau.
*/
void deleteBaron(Monde *world, AListe *clan, Agent *agent){

	AListe cursor = *clan;
	AListe tmp = NULL;
	AListe prec = NULL, suiv;
	for(;cursor != NULL; cursor = cursor->vsuiv){

		tmp = world->plateau[cursor->posx][cursor->posy].habitant;
		for(; tmp != NULL; tmp = tmp->asuiv){
			if(tmp == agent && tmp->aprec == NULL && tmp->asuiv != NULL ){
				world->plateau[cursor->posx][cursor->posy].habitant = tmp->asuiv;
				free(tmp);
			}
			if(tmp == agent && tmp->asuiv != NULL && tmp->aprec != NULL){
				prec->asuiv = tmp->asuiv;
				suiv = tmp->asuiv;
				suiv->aprec = prec;
				free(tmp);
			}
			if(tmp == agent && tmp->asuiv == NULL && tmp->aprec != NULL ){
				prec->asuiv = NULL;
				free(tmp);
			}
			prec = tmp;
			suiv = prec->asuiv;
		}
		world->plateau[agent->posx][agent->posy].habitant = NULL;
	}
}

/*
--Cette fonction permet de lancer les différentes actions de chaque agent obtenu grâce 
	à la liste des habitants d'un même château récupéré dans la fonction parcoursClan
*/
void parcoursCastle(char couleur, AListe agent, Monde *world, int *tresor, AListe equipe){

	int res = 0;
	AListe clan;
	AListe listeDeplacement;
	actuMonde(*world, couleur);
	MLV_draw_text(L_FENETRE-240, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, agent->genre, agent->clan);
	MLV_draw_text(L_FENETRE-235, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  agent->posx, agent->posy);
	MLV_draw_text(L_FENETRE-235, H_FENETRE-460, " du chateau : [%d,%d]", MLV_COLOR_WHITE,  equipe->posx, equipe->posy);

	MLV_actualise_window();

	//Si l'agent n'est pas encore arrivé à destination on continue de le bouger automatiquement
	if((agent->destx > -1 || agent->desty > -1 )){
		listeDeplacement = world->plateau[agent->posx][agent->posy].deplacement;
		moveAgent(agent, world, *tresor, &listeDeplacement);
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
			//Sauvegarder nos données
			else if(res == 4){
				save(world->rouge, world->bleu, *world);
				MLV_draw_text(L_FENETRE-220, H_FENETRE-440, "Partie Sauvegardé ", MLV_COLOR_YELLOW);
				MLV_draw_text(L_FENETRE-220, H_FENETRE-425, "Cliquer sur Quitter ", MLV_COLOR_YELLOW);
				MLV_actualise_window();

			}
			//Choix 5 correspond au bouton du BARON qui construit un CHATEAU			
			else if(res == 5){
				AListe listHab = NULL;
				int x, y;
				Agent * newchateau = createCastle(couleur, agent->posx, agent->posy);
				if(couleur == ROUGE){
					addInClan(newchateau, &(world->rouge));
					world->tresorRouge -= CCHATEAU;
					clan = (world->rouge);
				}
				else if(couleur == BLEU){
					addInClan(newchateau, &(world->bleu));
					world->tresorBleu -= CCHATEAU;
					clan = (world->bleu);
				}
				Agent *tmp=agent;
				deleteBaron(world, &clan, agent);
				world->plateau[newchateau->posx][newchateau->posy].chateau = newchateau;				
				checkPosition(world->plateau, &newchateau, &x, &y);
				Agent* baron = createAgent(couleur, BARON, x, y);
				addAndTri(baron, &listHab, world, couleur, baron->genre, x, y);
				world->plateau[newchateau->posx][newchateau->posy].habitant = listHab;
			}
		}while(res == 0 || res == 4);
	}

}

/*
-- Cette fonction parcours la liste du clan ("rouge"/"bleu") où est situé notre chateau de base
	Elle prend en paramètre la couleur de clan qui joue la liste equipe qui correspond a notre 
	liste clan, notre monde en pointeur car on le modifie et enfin le tresor du clan en 
	pointeur aussi pour pouvoir le modifier.
*/
void parcoursClan(char couleur, AListe equipe, Monde *world, int *tresor){

	AListe listHab;
	actuMonde(*world, couleur);
	drawInformation(couleur, *world);	
	for(;equipe != NULL; equipe = equipe->vsuiv){
		actuMonde(*world, couleur);
		if(equipe->genre == CHATEAU){
			drawBoxes();
			MLV_draw_text(L_FENETRE-275, H_FENETRE-500, "Tour de chateau %c  coordonnée : [%d,%d]", MLV_COLOR_WHITE, couleur, equipe->posx, equipe->posy);
			MLV_actualise_window();
			castleProduction(couleur, tresor, &equipe, world);
			listHab = world->plateau[equipe->posx][equipe->posy].habitant;

			for(;listHab != NULL; listHab = listHab->asuiv)
				parcoursCastle(couleur, listHab, world, tresor, equipe);
		}
	}
}