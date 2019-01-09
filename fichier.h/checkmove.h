int tireDe(){
	int val = MLV_get_random_integer(0,100);
	return val;
}

int coup(Agent agent, int de){
	int cou;
	if (agent.genre == BARON){
		cou = de*CBARON;
	}
	if (agent.genre == GUERRIER){
		cou = de*CGUERRIER;
	}
}

void attaque(Agent *agent, Agent *agent2, Monde *world){
	int de1, de2, coup1, coup2;
	AListe tmp, prec, suiv;
	if((agent->genre == BARON || agent->genre == GUERRIER) && (agent2->genre == BARON || agent2->genre == GUERRIER) && (agent->clan != agent2->clan)) { 
		do{ 
			de1 = tireDe();
			de2 = tireDe();
		}while(de1 == de2);
		coup1 = coup(*agent, de1);
		coup2 = coup(*agent2, de2);
		if (coup1 < coup2){

			world->plateau[agent->posx][agent->posy].habitant = NULL;
			if(agent->clan == ROUGE){
				tmp = world->rouge;
				for(; tmp != NULL; tmp = tmp->asuiv){
					if(tmp == agent && tmp->aprec == NULL && tmp->asuiv != NULL ){
						world->rouge = tmp->asuiv;
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

			}
		}
	}
}







int contourAgent(Agent *agent, Monde *world){
	AListe agentdest = world->plateau[agent->destx][agent->desty].habitant;
	if(agent->posx == agent->destx -1 && agent->posy == agent->desty  && agentdest != NULL){
		attaque(agent, agentdest, world);
		agent->destx = 0;
		agent->desty = 0;
		return 0;
	}
	if(agent->posx == agent->destx +1 && agent->posy == agent->desty  && agentdest != NULL){
		attaque(agent, agentdest, world);
		agent->destx = 0;
		agent->desty = 0;
		return 0;
	}
	if(agent->posx == agent->destx  && agent->posy == agent->desty -1  && agentdest != NULL){
		attaque(agent, agentdest, world);
		agent->destx = 0;
		agent->desty = 0;
		return 0;
	}
	if(agent->posx == agent->destx  && agent->posy == agent->desty +1  && agentdest != NULL){
		attaque(agent, agentdest, world);
		agent->destx = 0;
		agent->desty = 0;
		return 0;
	}
	return 1;
}



int checkPosition(Case plateau[NBLIG][NBCOL], Agent* chateau, int *x, int *y){

	if(chateau->posx == 0 && chateau->posy == 0){
		if(plateau[chateau->posx +1][chateau->posy].habitant == NULL && plateau[chateau->posx +1][chateau->posy].chateau == NULL){
			*x = chateau->posx+1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL && plateau[chateau->posx ][chateau->posy +1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant == NULL  && plateau[chateau->posx +1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posx == NBLIG-1 && chateau->posy == 0){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL  && plateau[chateau->posx -1][chateau->posy].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant == NULL  && plateau[chateau->posx -1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL && plateau[chateau->posx ][chateau->posy +1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posx == NBLIG-1 && chateau->posy == NBCOL-1){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL && plateau[chateau->posx -1][chateau->posy].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL  && plateau[chateau->posx ][chateau->posy -1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy -1;
		}
		else if(plateau[chateau->posx -1][chateau->posy -1].habitant == NULL  && plateau[chateau->posx -1][chateau->posy -1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy -1;
			return 0;
		}
	}

	else if(chateau->posx == 0 && chateau->posy == NBCOL-1){
		if(plateau[chateau->posx +1][chateau->posy].habitant == NULL  && plateau[chateau->posx +1][chateau->posy].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy -1].habitant == NULL  && plateau[chateau->posx +1][chateau->posy -1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL && plateau[chateau->posx ][chateau->posy -1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy -1;
			return 0;
		}
	}

	else if(chateau->posx != 0 && chateau->posx != NBLIG-1 && chateau->posy == 0){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL && plateau[chateau->posx -1][chateau->posy].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant == NULL && plateau[chateau->posx -1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL && plateau[chateau->posx ][chateau->posy +1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant == NULL && plateau[chateau->posx +1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy].habitant == NULL && plateau[chateau->posx +1][chateau->posy].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
	}

	else if(chateau->posx == 0 && chateau->posy != 0 && chateau->posy != NBCOL-1){
		if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL && plateau[chateau->posx ][chateau->posy -1].chateau == NULL){
			*x = chateau->posx ;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy -1].habitant == NULL && plateau[chateau->posx +1][chateau->posy -1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy].habitant == NULL && plateau[chateau->posx +1][chateau->posy].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant == NULL && plateau[chateau->posx +1][chateau->posy -1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL && plateau[chateau->posx ][chateau->posy +1].chateau == NULL){
			*x = chateau->posx ;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posx == NBLIG-1 && chateau->posy != 0 && chateau->posy == NBCOL-1){
		if(plateau[chateau->posx][chateau->posy -1].habitant == NULL && plateau[chateau->posx ][chateau->posy -1].chateau == NULL){
			*x = chateau->posx ;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy -1].habitant == NULL  && plateau[chateau->posx -1][chateau->posy -1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy].habitant == NULL && plateau[chateau->posx -1][chateau->posy].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant == NULL && plateau[chateau->posx -1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL && plateau[chateau->posx ][chateau->posy +1].chateau == NULL){
			*x = chateau->posx ;
			*y = chateau->posy +1;
			return 0;
		}
	}

	else if(chateau->posy == NBCOL-1 && chateau->posx != 0 && chateau->posx != NBLIG-1){
		if(plateau[chateau->posx -1][chateau->posy].habitant == NULL && plateau[chateau->posx -1][chateau->posy].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant == NULL && plateau[chateau->posx -1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx][chateau->posy +1].habitant == NULL && plateau[chateau->posx ][chateau->posy +1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant == NULL && plateau[chateau->posx +1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy].habitant == NULL && plateau[chateau->posx +1][chateau->posy].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy;
			return 0;
		}
	}	

	else if(chateau->posx != NBLIG-1 && chateau->posx != 0 && chateau->posy != NBCOL-1 && chateau->posy != 0){

		if(plateau[chateau->posx +1][chateau->posy].habitant == NULL && plateau[chateau->posx +1][chateau->posy].chateau == NULL){
			*x = chateau->posx+1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx +1][chateau->posy +1].habitant ==NULL && plateau[chateau->posx +1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy +1].habitant == NULL && plateau[chateau->posx ][chateau->posy +1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy].habitant == NULL && plateau[chateau->posx -1][chateau->posy].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy +1].habitant ==NULL && plateau[chateau->posx -1][chateau->posy +1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy +1;
			return 0;
		}
		else if(plateau[chateau->posx -1][chateau->posy -1].habitant ==NULL && plateau[chateau->posx -1][chateau->posy -1].chateau == NULL){
			*x = chateau->posx -1;
			*y = chateau->posy -1;
			return 0;
		}
		else if(plateau[chateau->posx ][chateau->posy -1].habitant == NULL && plateau[chateau->posx ][chateau->posy -1].chateau == NULL){
			*x = chateau->posx;
			*y = chateau->posy -1;
			return 0;
		}	
		else if(plateau[chateau->posx +1][chateau->posy -1].habitant ==NULL && plateau[chateau->posx +1][chateau->posy -1].chateau == NULL){
			*x = chateau->posx +1;
			*y = chateau->posy -1;
			return 0;
		}
	}
	return 1;
}


void moveAgent(Agent *agent, Monde *world){

	int tmpx, tmpy;
	tmpx = agent->posx;
	tmpy = agent->posy;

	if (contourAgent(agent, world) == 0)
		return;


	if((agent->posx != agent->destx) || (agent->posy != agent->desty)){

		if(agent->destx < tmpx && agent->desty < tmpy){

			if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){
				if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0){
					if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){
						if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							agent->posy += 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;

					}
				}else{
					agent->posy -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posx -= 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}

		}

		else if(agent->destx < tmpx && agent->desty == tmpy){
				
			if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){
				if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0){
					if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){
						if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							agent->posy += 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;
					}
				}else{
					agent->posy -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posx -= 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}

		}

		else if(agent->destx < tmpx && agent->desty > tmpy){
				
			if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){
				if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1){
					if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){
						if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							agent->posy -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;
					}
				}else{
					agent->posy += 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posx -= 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}

		}

		else if(agent->destx == tmpx && agent->desty > tmpy){
				
			if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1){
				
				if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){

					if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){

						if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							agent->posy -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx += 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;
					}
				}else{
					agent->posx -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posy += 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}

		}

		else if(agent->destx > tmpx && agent->desty > tmpy){
				
			if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1){

				if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){
				
					if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){


						if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);							
						else{
							agent->posy -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;
					}
				}else{
					agent->posx += 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posy += 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}
		}

		else if(agent->destx > tmpx && agent->desty == tmpy){
				
			if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){
	
				if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1){
				
					if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){


						if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							agent->posy -= 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;
					}
				}else{
					agent->posy += 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posx += 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}
		}

		else if(agent->destx > tmpx && agent->desty < tmpy){
				
			if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){
	
				if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0){
				
					if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){


						if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							agent->posy += 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;
					}
				}else{
					agent->posy -= 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posx += 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}
		}

		else if(agent->destx == tmpx && agent->desty < tmpy){
				
			if((world->plateau[tmpx ][tmpy -1].habitant != NULL || world->plateau[tmpx ][tmpy -1].chateau != NULL) && tmpy >= 0){

				if((world->plateau[tmpx +1][tmpy].habitant != NULL || world->plateau[tmpx +1][tmpy].chateau != NULL) && tmpx <= NBLIG-1){
					
					if((world->plateau[tmpx -1][tmpy].habitant != NULL || world->plateau[tmpx -1][tmpy].chateau != NULL) && tmpx >= 0){

						if((world->plateau[tmpx ][tmpy +1].habitant != NULL || world->plateau[tmpx ][tmpy +1].chateau != NULL) && tmpy <= NBCOL-1)
							MLV_draw_text(L_FENETRE-250, H_FENETRE-600, "Aucune case libre autour", MLV_COLOR_WHITE);
						else{
							agent->posy += 1;
							world->plateau[agent->posx][agent->posy].habitant = agent;
							world->plateau[tmpx][tmpy].habitant = NULL;
							return;
						}
					}else{
						agent->posx -= 1;
						world->plateau[agent->posx][agent->posy].habitant = agent;
						world->plateau[tmpx][tmpy].habitant = NULL;
						return;
					}
				}else{
					agent->posx += 1;
					world->plateau[agent->posx][agent->posy].habitant = agent;
					world->plateau[tmpx][tmpy].habitant = NULL;
					return;
				}
			}else{
				agent->posy -= 1;
				world->plateau[agent->posx][agent->posy].habitant = agent;
				world->plateau[tmpx][tmpy].habitant = NULL;
				return;
			}
		}

	}
}