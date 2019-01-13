
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

AListe parcoursClanAdverse(AListe clan, Agent *agent){

    AListe tmp = NULL;
    for(;clan != NULL; clan=clan->vsuiv){
        for(tmp=clan; tmp != NULL; tmp=tmp->asuiv){
            if(tmp == agent)
                return clan;
        }
    }
    return NULL;
}

void deleteAgentInCastle(AListe *listHab, Monde *world, Agent *agentKeep){

    AListe curs = *listHab;
    AListe clanAdv, hab;
    for(; curs != NULL; curs=curs->asuiv){
        //Supprime tout les agent de la liste mis à part les manants
        if(curs->genre != MANANT){
            world->plateau[curs->posx][curs->posy].habitant = NULL;
            (*listHab) = curs->asuiv;
            free(curs);
        }
        //On ajoute les manants dans la liste
        else{
            if(curs->clan == ROUGE){
                clanAdv = parcoursClanAdverse((*world).bleu, agentKeep);
                curs->clan = BLEU;
                hab = world->plateau[clanAdv->posx][clanAdv->posy].habitant;
                addAndTri(curs, &hab, world, BLEU, curs->genre, curs->posx, curs->posy);
            }
            else if(curs->clan == BLEU){
                clanAdv = parcoursClanAdverse((*world).rouge, agentKeep);
                curs->clan = ROUGE;
                hab = world->plateau[clanAdv->posx][clanAdv->posy].habitant;
                addAndTri(curs, &hab, world, ROUGE, curs->genre, curs->posx, curs->posy);
            }
        }
    }
}


void deleteAgent(Monde *world, AListe *clan, Agent *agentSupp, Agent *agentKeep){

	AListe cursor = *clan;
	AListe tmp = NULL, listHab;
	AListe prec = NULL, suiv;
	for(;cursor != NULL; cursor = cursor->vsuiv){
		if(agentSupp == cursor){
			if(cursor == agentSupp && cursor->vprec == NULL){
			    listHab = world->plateau[cursor->posx][cursor->posy].habitant;
                deleteAgentInCastle(&listHab, world, agentKeep);
                world->plateau[cursor->posx][cursor->posy].chateau = NULL;
                world->plateau[cursor->posx][cursor->posy].habitant = NULL;
				*clan = cursor->vsuiv;
				free(cursor);
			}
			if(cursor == agentSupp && cursor->vsuiv != NULL && cursor->vprec != NULL){
			    listHab = world->plateau[cursor->posx][cursor->posy].habitant;
                deleteAgentInCastle(&listHab, world, agentKeep);
                world->plateau[cursor->posx][cursor->posy].chateau = NULL;
                world->plateau[cursor->posx][cursor->posy].habitant = NULL;
				prec->vsuiv = cursor->vsuiv;
				suiv = cursor->vsuiv;
				suiv->vprec = prec;
				free(cursor);
			}
			if(cursor == agentSupp && cursor->vsuiv == NULL && cursor->vprec != NULL ){
			    listHab = world->plateau[cursor->posx][cursor->posy].habitant;
                deleteAgentInCastle(&listHab, world, agentKeep);
                world->plateau[cursor->posx][cursor->posy].chateau = NULL;
                world->plateau[cursor->posx][cursor->posy].habitant = NULL;
				prec->vsuiv = NULL;
				free(cursor);
			}
		}else{
			tmp = world->plateau[cursor->posx][cursor->posy].habitant;
			for(; tmp != NULL; tmp = tmp->asuiv){
				if(tmp == agentSupp && tmp->aprec == NULL && tmp->asuiv != NULL ){
					world->plateau[cursor->posx][cursor->posy].habitant = tmp->asuiv;
					free(tmp);
				}
				if(tmp == agentSupp && tmp->asuiv != NULL && tmp->aprec != NULL){
					prec->asuiv = tmp->asuiv;
					suiv = tmp->asuiv;
					suiv->aprec = prec;
					free(tmp);
				}
				if(tmp == agentSupp && tmp->asuiv == NULL && tmp->aprec != NULL ){
					prec->asuiv = NULL;
					free(tmp);
				}
				prec = tmp;
				suiv = prec->asuiv;
			}
			world->plateau[agentSupp->posx][agentSupp->posy].habitant = NULL;

		}
	}
}

void attack(Agent *agent1, Agent *agent2, Monde *world){

	int de1, de2, coupAgent1, coupAgent2;
	
	if((agent1->genre == BARON || agent1->genre == GUERRIER) && (agent2->genre == BARON || agent2->genre == GUERRIER || agent2->genre == CHATEAU) && (agent1->clan != agent2->clan)) { 
		do{ 
			de1 = tireDe();
			de2 = tireDe();
			coupAgent1 = coup(*agent1, de1);
			coupAgent2 = coup(*agent2, de2);
		}while(coupAgent1 == coupAgent2);
		
		if (coupAgent1 < coupAgent2){
			world->plateau[agent1->posx][agent1->posy].habitant = NULL;
			if(agent1->clan == ROUGE){
				deleteAgent(world, &(world->rouge), agent1, agent2);
			}else if(agent1->clan == BLEU){
				deleteAgent(world, &(world->bleu), agent1, agent2);
			}
		}
		else{
			if(agent2->clan == ROUGE){
				deleteAgent(world, &(world->rouge), agent2, agent1);
			}else if(agent2->clan == BLEU){
				deleteAgent(world, &(world->bleu), agent2, agent1);				
			}
		}
	}
}

