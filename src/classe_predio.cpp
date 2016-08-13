/***************************************************************************
 *   Copyright (C) 2006 by Iuri Fiedoruk   *
 *   protomank@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



// construtor predio
classe_predio::classe_predio(classe_general *gn, char *tip) {
	//printf("DEBUG - CONSTRUTOR DE PRÉDIO\n");
	general = gn;
	tipo = tip;
	next = NULL;
	drawn = 0;
	HP = 150;
	contador = 0;
	estado = 0;
	nome_tropa = "";
	raio_visao = 500;
	distancia_ataque = 500;
	ataque = 0;
	timer_rearme = 0;
	estado == ESTADO_LIVRE;
	tempo_rearme = 40;
	if (tipo == "BASE") {
		arquivo = "./graficos/buildings/town_hall.png";
		tamanho = 190;
		gfx_pos.x = 18;
		gfx_pos.y = 422;
	} else if (tipo == "FORJA") {
		arquivo = "./graficos/buildings/blacksmith.png";
		tamanho = 232;
		gfx_pos.x = 18;
		gfx_pos.y = 758;
	} else if (tipo == "SERRARIA") {
		arquivo = "./graficos/buildings/elven_lumber_mill.png";
		tamanho = 126;
		gfx_pos.x = 3;
		gfx_pos.y = 379;
	} else if (tipo == "TORRE") {
		ataque = 20;
		tempo_rearme = 150;
		arquivo = "./graficos/buildings/scout_tower.png";
		tamanho = 100;
		gfx_pos.x = 140;
		gfx_pos.y = 276;
	} else if (tipo == "ESTÁBULOS") {
		arquivo = "./graficos/buildings/stables.png";
		tamanho = 252;
		gfx_pos.x = 0;
		gfx_pos.y = 768;
	} else if (tipo == "CASTELO") {
		arquivo = "./graficos/buildings/castle.png";
		tamanho = 128;
		gfx_pos.x = 0;
		gfx_pos.y = 0;
	} else if (tipo == "IGREJA") {
		arquivo = "./graficos/buildings/church.png";
		tamanho = 256;
		gfx_pos.x = 0;
		gfx_pos.y = 760;
	} else if (tipo == "TORRE MAGOS") {
		arquivo = "./graficos/buildings/mage_tower.png";
		tamanho = 250;
		gfx_pos.x = 6;
		gfx_pos.y = 504;
	} else if (tipo == "GRIFOÁRIO") {
		arquivo = "./graficos/buildings/gryphon_aviary.png";
		tamanho = 96;
		gfx_pos.x = 0;
		gfx_pos.y = 0;
	} else if (tipo == "INVENTOR") {
		arquivo = "./graficos/buildings/gnomish_inventor.png";
		tamanho = 226;
		gfx_pos.x = 16;
		gfx_pos.y = 768;
	} else if (tipo == "FAZENDA") {
		arquivo = "./graficos/buildings/farm.png";
		tamanho = 64;
		gfx_pos.x = 0;
		gfx_pos.y = 0;
	} else {
		printf("ERRO: Tipo de prédio desconhecido: %s em %d x %d\n", tipo, x, y);
		// S´O PARA N~AO DAR PAU
		arquivo = "./graficos/buildings/farm.png";
		tamanho = 64;
		gfx_pos.x = 0;
		gfx_pos.y = 0;
	}

}


// DESTRUTOR
classe_predio::~classe_predio() {
	int i, j;
	//printf("DEBUG - DESTRUTOR DE PRÉDIO\n");
	//paused = 1;
	// LIMPA GRÁFICO DO MAPA
	printf("DEBUG.classe_predio::~classe_predio - removendo predio %s (%d x %d), tamanho: %d\n", tipo, x, y, tamanho);
	//printf("DEBUG - desenhando fundo\n");
	SDL_Surface *image = IMG_Load ("./graficos/summer.png");
	if (!image) {
		printf ("IMG_Load: %s\n", IMG_GetError ());
		return;
	}
   // Draws the image on the screen:
	SDL_Rect rcSrc = {0, 32, 32, 32};
	for (i=x; i<tamanho+x; i+=32) {
		for (j=y; j<tamanho+y; j+=32) {
			SDL_Rect rcDest = {i, j, 32, 32};
			SDL_BlitSurface(image, &rcSrc, offscreen, &rcDest);
		}
	}
	SDL_UpdateRect(offscreen, 0,0,0,0);
	//SDL_FreeSurface(image);
	// REMOVE DA LISTA DE PREDIOS DO GENERAL
	
	classe_predio *aux;
	aux = general->lista_predios;
	if (aux == this) {
		if (aux->next != NULL) {
			//printf("DEBUG.classe_predio::~classe_predio - removendo do início 1\n");
			general->lista_predios = aux->next;
		} else {
			//printf("DEBUG.classe_predio::~classe_predio - removendo do início 2\n");
			general->lista_predios = NULL;
		}
	} else if (aux != NULL) {
		classe_predio *ant;
		ant = aux;
		aux = aux->next;
		while (aux) {
			if (aux == this) {
				//printf("DEBUG.classe_predio::~classe_predio - removendo do meio\n");
				ant->next = aux->next;
				break;
			}
			ant = aux;
			aux = aux->next;
		}
	}

	// REMOVE DO ALVO DE TODAS AS TROPAS E TORRES
	classe_general *aux_g;
	classe_tropa *aux_t;
	classe_predio *aux_p;
	aux_g = general->mapa->lista_generais;
	while (aux_g) {
		if (aux_g != general) {
			aux_t = aux_g->lista_tropas;
			while (aux_t) {
				if (aux_t->alvo_predio == this) {
					aux_t->alvo_predio = NULL;
					aux_t->estado = ESTADO_LIVRE;
				}
				aux_t = aux_t->next;
			}
			aux_p = aux_g->lista_predios;
			while (aux_p) {
				if (aux_p->alvo_predio == this) {
					aux_p->alvo_predio = NULL;
					aux_p->estado = ESTADO_LIVRE;
				}
				aux_p = aux_p->next;
			}
		}
		aux_g = aux_g->next;
	}
	general->adiciona_objetivo(tipo, 1);
}



// procura unidades inimigas ou neutras ainda não conhecidas no raio dado
void classe_predio::visao() {
	//printf("DEBUG.classe_predio::visao - INICIO\n");
	int dist=0, achou=0;
	// PROCURA LISTA DE PRÉDIOS E TROPAS INIMIGAS
	classe_general *aux_g = NULL;
	aux_g = general->mapa->lista_generais;
	classe_tropa *aux_t = NULL;
	classe_predio *aux_p = NULL;
	while (aux_g) {
		if (aux_g->id != general->id) {
			//printf("DEBUG.classe_tropa::visao - VISÃO PREDIO DE GENERAL %d, PREDIO: %s, GENERAL2: %d\n", general->id, tipo, aux_g->id);
			// TROPAS
			if (estado == ESTADO_LIVRE || estado == ESTADO_ATACANDO_PREDIO) {
				//printf("DEBUG.classe_tropa::visao - VISÃO DE PREDIO DE GENERAL-A %d, verificando tropas de GENERAL-B: %d\n", general->id, aux_g->id);
				aux_t = aux_g->lista_tropas;
				while (aux_t) {
					dist = abs((int) sqrt(pow((float)abs((x - aux_t->getPos().x)), 2)+pow((float)abs((y - aux_t->getPos().y)), 2)));
					//printf("DEBUG.classe_predio::visao - raio_visao: %d, dist: %d\n", raio_visao, dist);
					if (dist < raio_visao) {
						if (ataque >= 1 && estado == ESTADO_LIVRE) {
							//printf("DEBUG.classe_predio::visao - %s de GENERAL %d ACHOU INIMIGO %s de general %d, ataque: %d!\n", tipo, general->id, aux_t->tipo, aux_t->general->id, ataque);
							//printf("DEBUG.classe_predio::visao - indo atacar\n");
							alvo_tropa = aux_t;
							estado = ESTADO_ATACANDO_TROPA;
							//printf("DEBUG - alerta de %s RECEBENDO ATAQUE de tropa %s\n", alvo_tropa->tipo, tipo);
							alvo_tropa->general->alerta(this, "PREDIO", "RECEBENDO_ATAQUE");
							//general->alerta(alvo_tropa, "TROPA", "ATACANDO");
							break;
						}
					}
					aux_t = aux_t->next;
				}
			}
			/*
			// PREDIOS
			//printf("DEBUG.classe_predio::visao - GENERAL DIFERENTE, VERIFICANDO PREDIOS\n");
			if (estado == ESTADO_LIVRE) {
				printf("DEBUG.classe_predio::visao - GENERAL DIFERENTE (%d), VERIFICANDO PREDIOS\n", aux_g->id);
				aux_p = aux_g->lista_predios;
				while (aux_p) {
					printf("DEBUG.classe_tropa::visao - LOOP, predio: %s\n", aux_p->tipo);
					dist = (int) sqrt(pow((float)(x - aux_p->x), 2)+pow((float)(y - aux_p->y), 2));
					//printf("DEBUG.classe_tropa::visao - raio_visao: %d, dist: %d\n", raio_visao, dist);
					if (dist <= raio_visao) {
						if (ataque >= 1) {
							//printf("DEBUG.classe_predio::visao - %s ACHOU INIMIGO, ataque: %d! dist: %d, raio_visao: %d\n", tipo, ataque, dist, raio_visao);
							//printf("DEBUG - pressione enter\n");
							//getchar();
							printf("DEBUG.classe_predio::visao - indo atacar %s\n", aux_p->tipo);
							alvo_predio = aux_p;
							estado = ESTADO_ATACANDO_PREDIO;
							//printf("DEBUG - alerta de %s RECEBENDO ATAQUE de tropa %s\n", alvo_predio->tipo, tipo);
							alvo_predio->general->alerta(this, "PREDIO", "RECEBENDO_ATAQUE");
							general->alerta(alvo_predio, "PREDIO", "ATACANDO");
							break;
						}
					}
					aux_p = aux_p->next;
				}
				printf("DEBUG.classe_tropa::visao - FIM-LOOP\n");
			}
			*/
		} else {
			//printf("DEBUG.classe_tropa::visao - PROPRIO GENERAL, IGNORANDO\n");
		}
		aux_g = aux_g->next;
	}
}




void classe_predio::agir() {
	int nivel, dist;
	st_objetivos *aux_objetivo;
	//printf("DEBUG.classe_predio::agir\n");
	if (estado == ESTADO_CONSTRUINDO) {
		aux_objetivo = general->busca_objetivo(nome_tropa);
		if (aux_objetivo) {
			nivel = aux_objetivo->nivel;
		}
		//printf("DEBUG.classe_predio::agir - OBJETIVO: %s, NIVEL: %d\n", nome_tropa, nivel);
		if (contador <= (nivel+1)*(TEMPO_CONSTRUCAO/2)) {
			contador++;
		} else {
			if (general->main_gold < general->busca_objetivo(nome_tropa)->custo_ouro) {
				//if (nome_tropa=="GRIFO") { printf("DEBUG.classe_predio::agir - NAO TEM OURO PARA CONSTRUIR\n"); }
				estado = ESTADO_LIVRE;
				nome_tropa = "";
			} else if (general->main_wood < general->busca_objetivo(nome_tropa)->custo_madeira) {
				//if (nome_tropa=="GRIFO") { printf("DEBUG.classe_predio::agir - NAO TEM MADEIRA PARA CONSTRUIR\n"); }
				estado = ESTADO_LIVRE;
				nome_tropa = "";
			} else {
				general->main_gold = general->main_gold - general->busca_objetivo(nome_tropa)->custo_ouro;
				general->main_wood = general->main_wood - general->busca_objetivo(nome_tropa)->custo_madeira;
				//if (nome_tropa=="GRIFO") { printf("DEBUG.classe_predio::agir - criando nova tropa: %s\n", nome_tropa); }
				classe_tropa *aux;
				classe_tropa *novo = new classe_tropa(nome_tropa, general);
				novo->pos.x = x+10;
				novo->pos.y = y+10;
				novo->tipo = nome_tropa;
				if(!general->lista_tropas) {
					general->lista_tropas = novo;
				} else {
					aux = general->lista_tropas;
					while (aux->next) {
						aux = aux->next;
					}
					aux->next = novo;
				}
				//if (nome_tropa=="GRIFO") { printf("DEBUG.classe_predio::agir - remove_objetivo(%s)\n", nome_tropa); }
				general->remove_objetivo(nome_tropa);
				contador = 0;
				estado = ESTADO_LIVRE;
				nome_tropa = "";
			}
		}
	} else if (estado == ESTADO_ATACANDO_TROPA) {
		if (alvo_tropa == NULL) {
			estado = ESTADO_LIVRE;
		} else if (alvo_tropa != NULL) {
			dist = (int) sqrt(pow((float)(x - alvo_tropa->pos.x), 2)+pow((float)(y - alvo_tropa->pos.y), 2));
			if (dist > distancia_ataque) {
				//printf("DEBUG.classe_predio::agir - LONGE DEMAIS %dx%d\n", alvo_tropa->pos.x, alvo_tropa->pos.y);
				//paused = 1;
				alvo_tropa = NULL;
				estado = ESTADO_LIVRE;
			} else {
				//printf("DEBUG.classe_predio::agir - PREDIO %s ATACANDO TROPA %s\n", tipo, alvo_tropa->tipo);
				//paused = 1;
				estado = ESTADO_REARMANDO_TROPA;
				timer_rearme = 0;
				alvo_tropa->HP -= ataque;
				//alvo_tropa->general->alerta(this, "TROPA", "RECEBENDO_ATAQUE");
				
				//printf("DEBUG.classe_predio::agir.FAZ-DANO-TROPA - HP: %d, HP-INI: %d\n", HP, alvo_tropa->HP);
				if (alvo_tropa->HP <= 0) {
					//printf("DEBUG.classe_predio::agir.FAZ-DANO-TROPA %s - inimigo morto!\n", alvo_tropa->tipo);
					alvo_tropa->general->contagem_tropas_perdidas += 1;
					general->contagem_tropas_destruidas += 1;
					delete alvo_tropa;
					alvo_tropa = NULL;
					estado = ESTADO_LIVRE;
				}
			}
		}
	/*
	} else if (estado == ESTADO_ATACANDO_PREDIO && alvo_predio != NULL) {
		printf("DEBUG.classe_tropa::agir - LOOP, alvo_predio: %s\n", alvo_predio->tipo);
		dist = (int) sqrt(pow((float)(x - alvo_predio->x), 2)+pow((float)(y - alvo_predio->y), 2));
		if (dist > distancia_ataque) {
			printf("DEBUG.classe_predio::agir - LONGE DEMAIS dist: %d, alcance: %d\n", dist, distancia_ataque);
			alvo_predio = NULL;
			estado = ESTADO_LIVRE;
		} else {
			//printf("PREDIO ATACANDO PREDIO\n");
			timer_rearme = 0;
			// TODO - DAMAGE
			alvo_predio->HP -= ataque;
			alvo_predio->general->alerta(this, "PREDIO", "RECEBENDO_ATAQUE");
			//printf("DEBUG.classe_predio::agir.FAZ-DANO-PREDIO - HP: %d, HP-INI: %d\n", HP, alvo_predio->HP);
			if (alvo_predio->HP <= 0) {
				alvo_predio->general->contagem_predios_perdidos += 1;
				general->contagem_predios_destruidos += 1;
				printf("DEBUG.classe_predio::agir - DELETANDO PREDIO\n");
				//delete alvo_predio;
				alvo_predio = NULL;
				estado = ESTADO_LIVRE;
				//printf("DEBUG.classe_predio::agir.FAZ-DANO - predio destruído!\n");
			} else {
				estado = ESTADO_REARMANDO_PREDIO;
			}
			
		}
	*/
	} else if (estado == ESTADO_REARMANDO_TROPA) {
		if (timer_rearme <= tempo_rearme) {
			timer_rearme++;
		} else {
			timer_rearme = 0;
			//printf("DEBUG - PREDIO %s REARMADO\n", tipo);
			//getchar();
			estado = ESTADO_ATACANDO_TROPA;
		}
	} else if (estado == ESTADO_REARMANDO_PREDIO) {
		if (timer_rearme <= tempo_rearme) {
			timer_rearme++;
		} else {
			timer_rearme = 0;
			//printf("MUDANDO ESTADO PARA ESTADO_ATACANDO_PREDIO\n");
			estado = ESTADO_ATACANDO_PREDIO;
		}
	}
}

int classe_predio::criar_tropa(char *nome_t) {
	//if (nome_tropa == "GRIFO") { printf("DEBUG - criando tropa %s\n", nome_tropa); }
	estado = ESTADO_CONSTRUINDO;
	nome_tropa = nome_t;
	return 1;
}

