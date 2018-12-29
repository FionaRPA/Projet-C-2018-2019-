#include "structure.h"


MLV_Color clanToColor(char clan){
	switch(clan){
		case ROUGE :
			return MLV_COLOR_RED;
		case BLEU :
			return MLV_COLOR_BLUE;
		default:
			printf("error clan : %c\n", clan);
			return MLV_COLOR_BLUE;
	}
}


void drawIn(char* file, int largeur, int hauteur, int x, int y, char clan, char genre){
	MLV_Image *image;
	image = MLV_load_image(file);
	MLV_resize_image_with_proportions( image, largeur/2, hauteur/2);
	switch(genre){
		case CHATEAU:
			MLV_draw_image( image, (x*largeur)+10, (y*hauteur)+10);
			MLV_draw_rectangle((x*largeur)+10, (y*hauteur)+10, largeur, hauteur, clanToColor(clan));
			break;
		case GUERRIER:
			MLV_draw_image( image, (x*largeur)+10, (y*hauteur)+(hauteur/2)+10);
			break;
		case MANANT:
			MLV_draw_image( image, (x*largeur)+(largeur/2)+10, (y*hauteur)+(hauteur/2)+10);	
			break;
		case BARON:
			MLV_draw_image( image, (x*largeur)+(largeur/2)+10, (y*hauteur)+10);
			break;
		default:
			printf("ERROR\n");
			break;
	}
}


void drawBoxes(){

	MLV_draw_text_box(L_FENETRE-200, H_FENETRE-700, 100, 30, "Attendre", 9, MLV_COLOR_VIOLET, MLV_COLOR_WHITE, MLV_COLOR_VIOLET, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(L_FENETRE-225, H_FENETRE-650, 150, 30, "Produire Baron", 9, MLV_COLOR_VIOLET, MLV_COLOR_WHITE, MLV_COLOR_VIOLET, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(L_FENETRE-225, H_FENETRE-600, 150, 30, "Produire Guerrier", 9, MLV_COLOR_VIOLET, MLV_COLOR_WHITE, MLV_COLOR_VIOLET, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(L_FENETRE-225, H_FENETRE-550, 150, 30, "Produire Manant" , 9, MLV_COLOR_VIOLET, MLV_COLOR_WHITE, MLV_COLOR_VIOLET, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(L_FENETRE-130, H_FENETRE-50, 100, 30, "Quitter" , 9, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_actualise_window();
}


void drawArray(Monde world){
	int x, y;
	int largeur = 60, hauteur = 60;
	
	for (x = 0 ; x < NBCOL ; x++) {
		for (y = 0 ; y < NBLIG ; y++) {
			Agent* chateau = world.plateau[x][y].chateau;
			Agent* habitant = world.plateau[x][y].habitant;
			//haut gauche
			MLV_draw_rectangle((x*largeur)+10, (y*hauteur)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//haut droit
			MLV_draw_rectangle((x*largeur)+(largeur/2)+10, (y*hauteur)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//bas gauche
			MLV_draw_rectangle((x*largeur)+10, (y*hauteur)+(hauteur/2)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//bas droit
			MLV_draw_rectangle((x*largeur)+(largeur/2)+10, (y*hauteur)+(hauteur/2)+10, largeur/2, hauteur/2, MLV_rgba(120,120,120,255));
			//gros rectangle
			MLV_draw_rectangle((x*largeur)+10, (y*hauteur)+10, largeur, hauteur, MLV_COLOR_WHITE);

			if (chateau != NULL){
				//CHATEAU
				if (chateau->clan == ROUGE)
					drawIn( "image/castleR.png", largeur, hauteur, chateau->posx, chateau->posy, chateau->clan, chateau->genre);
				else
					drawIn( "image/castleB.png", largeur, hauteur, chateau->posx, chateau->posy, chateau->clan, chateau->genre);
			}
			if (habitant != NULL){
				switch(habitant->genre){
					case GUERRIER:
						if (habitant->clan == ROUGE)
							drawIn("image/guerrierR.png", largeur, hauteur, habitant->posx, habitant->posy, habitant->clan, habitant->genre);
						else
							drawIn("image/guerrierB.png", largeur, hauteur, habitant->posx, habitant->posy, habitant->clan, habitant->genre);
						break;
					case MANANT:
						if (habitant->clan == ROUGE)
							drawIn("image/manantR.png", largeur, hauteur, habitant->posx, habitant->posy, habitant->clan, habitant->genre);
						else
							drawIn("image/manantB.png", largeur, hauteur, habitant->posx, habitant->posy, habitant->clan, habitant->genre);
						break;
					case BARON:
						if (habitant->clan == ROUGE)
							drawIn("image/baronR.png", largeur, hauteur, habitant->posx, habitant->posy, habitant->clan, habitant->genre);
						else
							drawIn("image/baronB.png", largeur, hauteur, habitant->posx, habitant->posy, habitant->clan, habitant->genre);
						break;
					default:
						printf("ERROR\n");
						break;
				}
			}
		}
	}
	drawBoxes();
}


int clikBoxes(){
	int x, y;

	while(1){
		MLV_wait_mouse(&x, &y);
		//Bouton Attendre
		if((x < L_FENETRE-100 && x > L_FENETRE-200) && (y < H_FENETRE-670 && y > H_FENETRE-700))
			return 'a';
		//Bouton Produire Baron
		else if((x < L_FENETRE-75 && x > L_FENETRE-225) && (y < H_FENETRE-620 && y > H_FENETRE-650))
			return BARON;
		//Bouton Produire Guerrier
		else if((x < L_FENETRE-75 && x > L_FENETRE-225) && (y < H_FENETRE-570 && y > H_FENETRE-600))
			return GUERRIER;
		//Bouton Produire Manant
		else if((x < L_FENETRE-75 && x > L_FENETRE-225) && (y < H_FENETRE-520 && y > H_FENETRE-550))
			return MANANT;
		//Bouton Quitter
		else if((x < L_FENETRE-30 && x > L_FENETRE-130) && (y < H_FENETRE-20 && y > H_FENETRE-50)){
			exit(EXIT_FAILURE);
		}
	}
}

