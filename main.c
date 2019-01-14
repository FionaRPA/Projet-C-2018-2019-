#include "fichier.h/structure.h"
#include "fichier.h/draw.h"
#include "fichier.h/createAndAdd.h"
#include "fichier.h/attack.h"
#include "fichier.h/checkmove.h"
#include "fichier.h/sauvegarde.h"
#include "fichier.h/menu.h"
#include "fichier.h/parcours_production.h"
#include "fichier.h/jeu.h"



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

