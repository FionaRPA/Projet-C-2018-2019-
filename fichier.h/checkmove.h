
void choixDeplacement(char couleur, Agent *agent, Monde *world){

	int x, y, i, j;
	MLV_wait_mouse(&x, &y);

	while(1){
		int lig=10, col=10;
		for(i=0; i < NBLIG; i++){
			for(j=0; j < NBCOL; j++){
				if(( 0 <= x && x <= 1090) && (0 <= y && y <= 730)){
					if((lig <= y && lig + 60 >= y) && (col <= x && col + 60 >= x)){
						actuMonde(*world, couleur);
						MLV_actualise_window();
						agent->destx = i;
						agent->desty = j;
						return;
					}
				}
				else{
					actuMonde(*world, couleur);
					MLV_draw_text(L_FENETRE-250, H_FENETRE-500, "Tour de agent %c equipe %c", MLV_COLOR_WHITE, agent->genre, agent->clan);
					MLV_draw_text(L_FENETRE-250, H_FENETRE-480, " coordonnée : [%d,%d]", MLV_COLOR_WHITE,  agent->posx, agent->posy);
					MLV_draw_text(L_FENETRE-220, H_FENETRE-300, "Case non valide", MLV_COLOR_WHITE);
					MLV_actualise_window();
					MLV_wait_mouse(&x, &y);

				}
				col+=60;
			}
			lig+=60;
			col = 10;
		}
	}
}

int contourAgent(Agent *agent, Monde *world, int tresor){

	AListe agentdest = world->plateau[agent->destx][agent->desty].habitant;
	AListe caseChateau = world->plateau[agent->destx][agent->desty].chateau;
	if(agent->posx == agent->destx -1 && agent->posy == agent->desty && (agentdest != NULL)){
		if(caseChateau != NULL)
			agentdest = caseChateau;
		if(agentdest->clan != agent->clan)
			if(drawBoxesAttaque(agent, tresor) == 1)
				attack(agent, agentdest, world);
		agent->destx = -1;
		agent->desty = -1;
		return 0;
	}
	if(agent->posx == agent->destx +1 && agent->posy == agent->desty && (agentdest != NULL)){
		if(caseChateau != NULL)
			agentdest = caseChateau;
		if(agentdest->clan != agent->clan)
			if(drawBoxesAttaque(agent, tresor) == 1)
				attack(agent, agentdest, world);
		agent->destx = -1;
		agent->desty = -1;
		return 0;
	}
	if(agent->posx == agent->destx  && agent->posy == agent->desty -1 && (agentdest != NULL)){
		if(caseChateau != NULL)
			agentdest = caseChateau;
		if(agentdest->clan != agent->clan)
			if(drawBoxesAttaque(agent, tresor) == 1)
				attack(agent, agentdest, world);
		agent->destx = -1;
		agent->desty = -1;
		return 0;
	}
	if(agent->posx == agent->destx  && agent->posy == agent->desty +1 && (agentdest != NULL)){
		if(caseChateau != NULL)
			agentdest = caseChateau;
		if(agentdest->clan != agent->clan)
			if(drawBoxesAttaque(agent, tresor) == 1)
				attack(agent, agentdest, world);
		agent->destx = -1;
		agent->desty = -1;
		return 0;
	}
	return 1;
}

int checkPosition(Case plateau[NBLIG][NBCOL], AListe* chateau, int *x, int *y){

	if((*chateau)->posx == 0 && (*chateau)->posy == 0){
		if(plateau[(*chateau)->posx +1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx+1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy +1].habitant == NULL  && plateau[(*chateau)->posx +1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy +1;
			return 0;
		}
	}

	else if((*chateau)->posx == NBLIG-1 && (*chateau)->posy == 0){
		if(plateau[(*chateau)->posx -1][(*chateau)->posy].habitant == NULL  && plateau[(*chateau)->posx -1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy +1].habitant == NULL  && plateau[(*chateau)->posx -1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy +1;
			return 0;
		}
	}

	else if((*chateau)->posx == NBLIG-1 && (*chateau)->posy == NBCOL-1){
		if(plateau[(*chateau)->posx -1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy -1].habitant == NULL  && plateau[(*chateau)->posx ][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy -1;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy -1].habitant == NULL  && plateau[(*chateau)->posx -1][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy -1;
			return 0;
		}
	}

	else if((*chateau)->posx == 0 && (*chateau)->posy == NBCOL-1){
		if(plateau[(*chateau)->posx +1][(*chateau)->posy].habitant == NULL  && plateau[(*chateau)->posx +1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy -1].habitant == NULL  && plateau[(*chateau)->posx +1][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy -1;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy -1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy -1;
			return 0;
		}
	}

	else if((*chateau)->posx != 0 && (*chateau)->posx != NBLIG-1 && (*chateau)->posy == 0){
		if(plateau[(*chateau)->posx -1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy;
			return 0;
		}
	}

	else if((*chateau)->posx == 0 && (*chateau)->posy != 0 && (*chateau)->posy != NBCOL-1){
		if(plateau[(*chateau)->posx ][(*chateau)->posy -1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx ;
			*y = (*chateau)->posy -1;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy -1].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy -1;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx ;
			*y = (*chateau)->posy +1;
			return 0;
		}
	}

	else if((*chateau)->posx == NBLIG-1 && (*chateau)->posy != 0 && (*chateau)->posy != NBCOL-1){
		if(plateau[(*chateau)->posx][(*chateau)->posy -1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx ;
			*y = (*chateau)->posy -1;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy -1].habitant == NULL  && plateau[(*chateau)->posx -1][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy -1;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx ;
			*y = (*chateau)->posy +1;
			return 0;
		}
	}

	else if((*chateau)->posy == NBCOL-1 && (*chateau)->posx != 0 && (*chateau)->posx != NBLIG-1){
		if(plateau[(*chateau)->posx -1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy;
			return 0;
		}
	}	

	else if((*chateau)->posx != NBLIG-1 && (*chateau)->posx != 0 && (*chateau)->posy != NBCOL-1 && (*chateau)->posy != 0){

		if(plateau[(*chateau)->posx +1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx +1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx+1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy +1].habitant ==NULL && plateau[(*chateau)->posx +1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy +1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy].habitant == NULL && plateau[(*chateau)->posx -1][(*chateau)->posy].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy +1].habitant ==NULL && plateau[(*chateau)->posx -1][(*chateau)->posy +1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy +1;
			return 0;
		}
		else if(plateau[(*chateau)->posx -1][(*chateau)->posy -1].habitant ==NULL && plateau[(*chateau)->posx -1][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx -1;
			*y = (*chateau)->posy -1;
			return 0;
		}
		else if(plateau[(*chateau)->posx ][(*chateau)->posy -1].habitant == NULL && plateau[(*chateau)->posx ][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx;
			*y = (*chateau)->posy -1;
			return 0;
		}	
		else if(plateau[(*chateau)->posx +1][(*chateau)->posy -1].habitant ==NULL && plateau[(*chateau)->posx +1][(*chateau)->posy -1].chateau == NULL){
			*x = (*chateau)->posx +1;
			*y = (*chateau)->posy -1;
			return 0;
		}
	}
	return 1;
}

Agent copieAgent(Agent agent){
	Agent genre = agent;
	return genre;
}

void  listDplmt(Agent *agent, AListe *listeDeplacement){
	/*AListe tmp = *listeDeplacement;
	Agent genre = copieAgent(*agent);
	genre.asuiv = NULL;
	genre.aprec = NULL;
	if(tmp == NULL){
		*listeDeplacement = genre;
	}
	else {
		while(tmp->asuiv != NULL){
			tmp = tmp->asuiv;
		}
		genre->aprec = tmp;
		tmp->asuiv = genre;
		genre->asuiv = NULL;
	
	}*/
	return;
}


int moveAgent(Agent *agent, Monde *world, int tresor, AListe *listeDeplacement){
	int tmpx, tmpy, last_posx, last_posy;
	
	last_posx = agent->posx;
	last_posy = agent->posy;
	tmpx = agent->posx;
	tmpy = agent->posy;
	
	if (contourAgent(agent, world, tresor) == 0)
		return 1;
	if(*listeDeplacement != NULL){
		while((*listeDeplacement)->asuiv != NULL){
			(*listeDeplacement) = (*listeDeplacement)->asuiv;
		}

		if((*listeDeplacement)->aprec != NULL){

			last_posx = (*listeDeplacement)->aprec->posx;
			last_posy = (*listeDeplacement)->aprec->posy;

		}

		else{

			last_posx = (*listeDeplacement)->posx;
			last_posy = (*listeDeplacement)->posy;
		}
	}

	if((tmpx != agent->destx) || (tmpy != agent->desty)){

		if(agent->destx < tmpx && agent->desty < tmpy){

			if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy == tmpy -1){

				if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1){
				
					if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1){
					
						if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){
						
								agent->posx += 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;
								
								return 0;
							}
						}
					}else{
						if(agent->posy +1 >= 0 && agent->posy +1 <= NBCOL-1){

							agent->posy += 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;							
							return 0;
						}

					}
				}else{								
					if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){
					
						agent->posx -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						
						return 0;
					}
				}
			}else{
				if(agent->posy -1 >= 0 && agent->posy -1 <= NBCOL-1){

					agent->posy -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					
					return 0;
				}
			}
		}

		else if(agent->destx < tmpx && agent->desty == tmpy){
				
			if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1){

				if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy != tmpy -1){

					if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1){
				
						if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){
				
								agent->posx += 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;
								
								return 0;
							}
						}
					}else{
						if(agent->posy +1 >= 0 && agent->posy +1 <= NBCOL-1){
			
							agent->posy += 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							
							return 0;
						}
					}
				}else{
					if(agent->posy -1 >= 0 && agent->posy -1 <= NBCOL-1){
	
						agent->posy -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						
						return 0;
					}
				}
			}else{
				if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){

					agent->posx -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					
					return 0;
				}
			}
		}

		else if(agent->destx < tmpx && agent->desty > tmpy){
				
			if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1){

				if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1){
						
					if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy == tmpy -1){

						if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){

								agent->posx += 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;
								
								return 0;
							}
						}
					}else{
						if(agent->posy -1 >= 0 && agent->posy -1 <= NBCOL-1){

							agent->posy -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
															
							return 0;
						}
					}
				}else{
					if(agent->posy +1 >= 0 && agent->posy+1 <= NBCOL-1){	

						agent->posy += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
												
						return 0;
					}
				}
			}else{
				if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){

					agent->posx -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
													
					return 0;
				}
			}

		}

		else if(agent->destx == tmpx && agent->desty > tmpy){
				
			if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1){
				
				if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1){
						
					if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1){

						if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy == tmpy -1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							if(agent->posy -1 >= 0 && agent->posy -1 <= NBCOL-1){

								agent->posy -= 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;
								
								return 0;
							}
						}
					}else{
						if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){

							agent->posx -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
															
							return 0;
						}
					}
				}else{
					if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){

						agent->posx -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
								
						return 0;
					}
				}
			}else{
				if(agent->posy +1 >= 0 && agent->posy+1 <= NBCOL-1){	

					agent->posy += 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
													
					return 0;
				}
			}

		}

		else if(agent->destx > tmpx && agent->desty > tmpy){
				
			if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1){

				if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1){

					if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1){

				
						if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy == tmpy -1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);							
						else{
							if(agent->posy -1 >= 0 && agent->posy -1 <= NBCOL-1){

								agent->posy -= 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;
								
								return 0;
							}
						}
					}else{
						if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){

							agent->posx -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
															
							return 0;
						}
					}
				}else{
					if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){

						agent->posx += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
								
						return 0;
					}
				}
			}else{
				if(agent->posy +1 >= 0 && agent->posy+1 <= NBCOL-1){
					
					agent->posy += 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					
					return 0;
				}
			}
		}

		else if(agent->destx > tmpx && agent->desty == tmpy){
				
			if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1){
	
				if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy == tmpy -1){

					if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1){
				
						if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){
  
								agent->posx -= 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;
								return 0;
							}
						}
					}else{
						if(agent->posy +1 >= 0 && agent->posy +1 <= NBCOL-1){
					
							agent->posy += 1;	
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return 0;
						}
					}
				}else{
					if(agent->posy -1 >= 0 && agent->posy -1 <= NBCOL-1){
			
						agent->posy -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
								
						return 0;
					}
				}
			}else{
				if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){
		
					agent->posx += 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
													
					return 0;
				}
			}
		}

		else if(agent->destx > tmpx && agent->desty < tmpy){
			
			if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy == tmpy -1){
			
				if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1){
					
					if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1){
				
						if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							if(agent->posy +1 >= 0 && agent->posy +1 <= NBCOL-1){

								agent->posy += 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;
								
								return 0;
							}
						}
					}else{
						if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){

							agent->posx -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
															
							return 0;
						}
					}
				}else{
					if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){

						agent->posx += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
								
						return 0;
					}
				}
			}else{
				if(agent->posy -1 >= 0 && agent->posy -1 <= NBCOL-1){

					agent->posy -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
								
					return 0;
				}
			}
		}
		
		else if(agent->destx == tmpx && agent->desty < tmpy){
				
			if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) || tmpy == 0 || last_posy == tmpy -1){

				if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) || tmpx == NBLIG-1 || last_posx == tmpx +1){
						
					if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) || tmpx == 0 || last_posx == tmpx -1){
					
						if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) || tmpy == NBCOL-1 || last_posy == tmpy +1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							if(agent->posy +1 >= 0 && agent->posy +1 <= NBCOL-1){

								agent->posy += 1;
								world->plateau[agent->posx][agent->posy].habitant = agent;
								world->plateau[tmpx][tmpy].habitant = NULL;

								return 0;
							}
						}
					}else{
						if(agent->posx -1 >= 0 && agent->posx -1 <= NBLIG-1){

							agent->posx -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
															
							return 0;
						}
					}
				}else{
					if(agent->posx +1 >= 0 && agent->posx +1 <= NBLIG-1){
	   
						agent->posx += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
												
						return 0;
					}
				}
			}else{
				if(agent->posy -1 >= 0 && agent->posy-1 <= NBCOL-1){   

					agent->posy -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
								
					return 0;
				}
			}
		}

	}
    return 0;
}