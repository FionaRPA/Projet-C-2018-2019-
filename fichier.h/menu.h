
void recupereDimensionBouton(char nom[], int taille, int *largeur, int *hauteur) {
	
	MLV_Font* font = MLV_load_font("image/KeepCalm-Medium.ttf", taille);
	MLV_get_size_of_adapted_text_box_with_font(nom, font, 0, largeur, hauteur);
}


Bouton placeBouton(char nom[], int x, int y, float taille, MLV_Color couleur) {

	int largeur, hauteur;

	MLV_Font* font = MLV_load_font("image/KeepCalm-Medium.ttf", taille);
	MLV_get_size_of_adapted_text_box_with_font(nom, font, 0, &largeur, &hauteur);

	MLV_draw_text_with_font(x, y, nom, font, couleur);
	MLV_draw_rectangle(x - taille/10, y - taille/10, largeur + 2*taille/10, hauteur + taille/10, couleur);

	MLV_free_font(font);
	MLV_actualise_window();

	Bouton zone = {x - taille/10, y - taille/10, x + largeur + taille/10, y + hauteur};

	return zone;
}

int menu() {

	int largeur, hauteur, taille;
	int largeur_txt, hauteur_txt;

	MLV_clear_window(MLV_COLOR_DARKSEAGREEN1);

	MLV_Font* font = MLV_load_font("image/A Box For.ttf", L_FENETRE*0.2);

	MLV_get_size_of_text_with_font("Game of stools", &largeur_txt, &hauteur_txt, font);
	MLV_draw_text_with_font((L_FENETRE-largeur_txt)/2, H_FENETRE*0.01, "Game of stools", font, MLV_COLOR_GREEN4);
	MLV_free_font(font);

	MLV_draw_filled_rectangle(L_FENETRE*0.1, H_FENETRE*0.01 + hauteur_txt, L_FENETRE*0.8, H_FENETRE*0.03, MLV_COLOR_DARK_GREEN);

	taille = L_FENETRE*0.07;
	recupereDimensionBouton("Nouvelle Partie", taille, &largeur, &hauteur);
	Bouton jouer = placeBouton("Nouvelle Partie", L_FENETRE/2 - largeur/2, H_FENETRE*0.5, taille, MLV_COLOR_GREEN3);

	recupereDimensionBouton("Partie Sauvegardé", taille, &largeur, &hauteur);
	Bouton sauvegarder = placeBouton("Partie Sauvegardé", L_FENETRE/2 - largeur/2, jouer.y2 + 2*taille/10, taille, MLV_COLOR_GREEN3);

	recupereDimensionBouton("Quitter", taille, &largeur, &hauteur);
	Bouton quitter = placeBouton("Quitter", L_FENETRE/2 - largeur/2, sauvegarder.y2 + 2*taille/10, taille, MLV_COLOR_GREEN3);

	MLV_actualise_window();

	int x, y;
	MLV_Event event;

	while (1) {
		event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &x, &y, NULL, NULL);
		if ( event == MLV_MOUSE_BUTTON && x > jouer.x1 && x < jouer.x2 && y > jouer.y1 && y < jouer.y2 )
			return 0;
		if ( event == MLV_MOUSE_BUTTON && x > sauvegarder.x1 && x < sauvegarder.x2 && y > sauvegarder.y1 && y < sauvegarder.y2 )
			exit(EXIT_FAILURE);
		if ( event == MLV_MOUSE_BUTTON && x > quitter.x1 && x < quitter.x2 && y > quitter.y1 && y < quitter.y2 )
			exit(EXIT_FAILURE);
	}
	return 0;
}
