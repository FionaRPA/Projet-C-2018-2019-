
int tireDe(){

	int val = MLV_get_random_integer(0,100);
	return val;
}

int coup(Agent agent, int de){

	int coupAgent = 0;
	if (agent.genre == BARON){
		coupAgent = de*CBARON;
	}
	else if (agent.genre == GUERRIER){
		coupAgent = de*CGUERRIER;
	}
	return coupAgent;
}
/*
void deleteAgentInCastle(){

}
*/

void deleteAgent(Monde *world, AListe *clan, Agent *agent){

	AListe cursor = *clan;
	AListe tmp = NULL;
	AListe prec = NULL, suiv;
	for(;cursor != NULL; cursor = cursor->vsuiv){
		if(agent == cursor){
			if(cursor == agent && cursor->vprec == NULL){
				*clan = cursor->vsuiv;
				free(cursor);
			}
			if(cursor == agent && cursor->vsuiv != NULL && cursor->vprec != NULL){
				prec->vsuiv = cursor->vsuiv;
				suiv = cursor->vsuiv;
				suiv->vprec = prec;
				free(cursor);
			}
			if(cursor == agent && cursor->vsuiv == NULL && cursor->vprec != NULL ){
				prec->vsuiv = NULL;
				free(cursor);
			}
			world->plateau[agent->posx][agent->posy].habitant = NULL;
			world->plateau[agent->posx][agent->posy].chateau = NULL;

		}else{
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
}

void attack(Agent *agent, Agent *agent2, Monde *world){

	int de1, de2, coup1, coup2;
	printf("agent1: %c\n", agent->genre);
	printf("agent2: %c\n", agent2->genre);
	if((agent->genre == BARON || agent->genre == GUERRIER) && (agent2->genre == BARON || agent2->genre == GUERRIER || agent2->genre == CHATEAU) && (agent->clan != agent2->clan)) { 
		do{ 
			de1 = tireDe();
			de2 = tireDe();
			coup1 = coup(*agent, de1);
			coup2 = coup(*agent2, de2);
		}while(coup1 == coup2);
		printf("coup1: %d, coup2: %d\n\n", coup1, coup2);
		
		if (coup1 < coup2){
			world->plateau[agent->posx][agent->posy].habitant = NULL;
			if(agent->clan == ROUGE){
				deleteAgent(world, &(world->rouge), agent);
			}else if(agent->clan == BLEU){
				deleteAgent(world, &(world->bleu), agent);
			}
		}
		else{
			if(agent2->clan == ROUGE){
				deleteAgent(world, &(world->rouge), agent2);
			}else if(agent2->clan == BLEU){
				deleteAgent(world, &(world->bleu), agent2);				
			}
		}
	}
}

