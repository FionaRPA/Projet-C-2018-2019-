/*
--Affiche le plateau (non utilisée)
*/
void afficheListe(Case plateau[NBLIG][NBCOL]){
	int i, j;
	for(i=0; i<NBLIG; i++){
		for(j=0;j < NBCOL; j++){
			if(plateau[i][j].habitant != NULL ){
				printf("habitant : %c [%d;%d]\n",plateau[i][j].habitant->genre, plateau[i][j].habitant->posx, plateau[i][j].habitant->posy);
			}
			if(plateau[i][j].chateau != NULL )
				printf("château : %c [%d;%d]\n",plateau[i][j].chateau->genre, plateau[i][j].chateau->posx, plateau[i][j].chateau->posy );
		}
	}
}

/*
--Affiche la liste de chaque château d'un même clan
*/
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

/*
--Vérfifie si l'équipe (rouge ou bleu) posséde toujours un château dans sa liste
*/
int castleInList(AListe clan){

	AListe tmp = clan;
	for(; tmp != NULL; tmp = tmp->asuiv){
		if(tmp->genre == CHATEAU)
			return 1;
	}
	return 0;
}

/*
-- Initialise notre plateau de départ avec les châteaux bleu et rouge ainsi 
	que les agents baron et manant respectifs à chaque château initialisés
*/
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



/*
Fonction qui affiche lorsque l'une des deux equipe à perdu
*/
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
		AListe rouge =world->rouge;
		AListe bleu = world->bleu;
		joueur = MLV_get_random_integer(0,100);
		if (castleInList(rouge) == 0){
			perdu("L'EQUIPE ROUGE");
			break;
		}
		if(castleInList(bleu) == 0){
			perdu("L'EQUIPE BLEU");			
			break;
		}

		if (joueur % 2 == 0){
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			afficheListec(world->rouge, world);
			afficheListec(world->bleu, world);
		}
		else{
			parcoursClan(BLEU, world->bleu, world, &world->tresorBleu);
			parcoursClan(ROUGE, world->rouge, world, &world->tresorRouge);
			afficheListec(world->bleu, world);
			afficheListec(world->rouge, world);

		}
		//afficheListe(world->plateau);
		world->tour++;
	}
}