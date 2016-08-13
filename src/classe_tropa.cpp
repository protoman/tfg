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

// construtor tropa
classe_tropa::classe_tropa(char *nome_tropa, classe_general *tgeneral) {
	int i, j;
	general = tgeneral;
	//printf("CRIANDO TROPA %s DE GENERAL %d\n", nome_tropa, general->id);
	next = NULL;
	estado = ESTADO_LIVRE;
	tipo = nome_tropa;
	pos.x = -1;
	pos.y = -1;
	ataque = 2;
	defesa = 1;
	HP = 10;
	timer_rearme = 0;
	tempo_rearme = 30;
	alvo_tropa = NULL;
	alvo_predio = NULL;
	alvo_local.x = 0;
	alvo_local.y = 0;
	// PARTE DE PEÃO
	ouro = 0;
	madeira = 0;
	alvo.x = 0;
	alvo.y = 0;
	alvo_base = NULL;
	alvo_mina = NULL;
	contador = 0;

	// TODO - variáveis de acordo com tipo
	old_x = -1;
	old_y = -1;
	raio_visao = 300;
	distancia_ataque = 72;
	//printf("DEBUG - nova tropa %s\n", nome_tropa);
	if (nome_tropa == "CATAPULTA") {
		gfx_file = "./graficos/units/ballista.png";
		gfx_dest.x = 0;
		gfx_dest.y = 0;
		gfx_dest.w = 96;
		gfx_dest.h = 96;
		distancia_ataque = 220;
		n_quadros_anim = 1;
		tempo_rearme = 80;
	} else if (nome_tropa == "PEAO") {
		gfx_file = "./graficos/units/peasant.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 56;
		gfx_dest.h = 56;
		n_quadros_anim = 5;
		ataque = 1;
		distancia_ataque = 32;
	} else if (nome_tropa == "GUERREIROS") {
		gfx_file = "./graficos/units/footman.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 64;
		gfx_dest.h = 64;
		n_quadros_anim = 5;
		ataque = 4;
		HP = 30;
	} else if (nome_tropa == "ARQUEIROS") {
		gfx_file = "./graficos/units/elven_archer.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 64;
		gfx_dest.h = 64;
		n_quadros_anim = 4;
		distancia_ataque = 80;
		ataque = 3;
		distancia_ataque = 150;
		tempo_rearme = 40;
		HP = 20;
	} else if (nome_tropa == "CAVALEIROS") {
		gfx_file = "./graficos/units/knight.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 64;
		gfx_dest.h = 64;
		n_quadros_anim = 5;
		ataque = 6;
		HP = 50;
	} else if (nome_tropa == "PALADINOS") {
		gfx_file = "./graficos/units/knight.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 64;
		gfx_dest.h = 64;
		n_quadros_anim = 5;
		ataque = 8;
		HP = 80;
	} else if (nome_tropa == "MAGOS") {
		gfx_file = "./graficos/units/mage.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 64;
		gfx_dest.h = 64;
		n_quadros_anim = 5;
		ataque = 7;
		tempo_rearme = 50;
		HP = 60;
	} else if (nome_tropa == "GRIFO") {
		gfx_file = "./graficos/units/gryphon_rider.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 128;
		gfx_dest.h = 128;
		n_quadros_anim = 2;
		ataque = 12;
		tempo_rearme = 100;
		HP = 120;
	} else if (nome_tropa == "ESQUADRAO DE DEMOLICAO") {
		gfx_file = "./graficos/units/dwarven_demolition_squad.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 64;
		gfx_dest.h = 64;
		n_quadros_anim = 5;
		ataque = 50;
		tempo_rearme = 50;
		HP = 3;
	} else if (nome_tropa == "MAQUINA VOADORA") {
		gfx_file = "./graficos/units/gnomish_flying_machine.png";
		gfx_dest.x = 194;
		gfx_dest.y = 0;
		gfx_dest.w = 128;
		gfx_dest.h = 128;
		n_quadros_anim = 3;
		ataque = 7;
		tempo_rearme = 50;
		HP = 60;
	} else {
		// default catapulta
		printf("DEBUG.classe_tropa::classe_tropa - ERRO: UNIDADE NÃO IDENTIFICADA %s\n", nome_tropa);
		getchar();
		gfx_file = "./graficos/units/ballista.png";
		gfx_dest.x = 0;
		gfx_dest.y = 0;
		gfx_dest.w = 63;
		gfx_dest.h = 63;
		n_quadros_anim = 1;
		HP = 100;
	}
	fundo = SDL_CreateRGBSurface (SDL_HWSURFACE , gfx_dest.w, gfx_dest.h, 16, 0, 0, 0, 0);
	for (i=0; i<2; i++) {
		for (j=0; j<8; j++) {
			lista_sprites[i][j] = NULL;
			lista_sprites_pos[i][j] = 0;
		}
	}
	direcao = 1;
}


// DESTRUTOR
classe_tropa::~classe_tropa() {
	// LIMPA GRÁFICO DO MAPA
	if (old_x != -1) {
		//printf("Apagar_tropas - Restaurando fundo, old_x: %d\n", aux->old_x);
		//copy_area(fundo, 0, 0, gfx_dest.w, gfx_dest.h, old_x, old_y, offscreen);
		SDL_Rect src, dest;
		src.x = 0;
		src.y = 0;
		src.w = gfx_dest.w;
		src.h = gfx_dest.h;
		dest.x = old_x;
		dest.y = old_y;
		dest.w = gfx_dest.w;
		dest.h = gfx_dest.w;
		//printf("DEBUG - src.x: %d, src.y: %d, src.w: %d, src.h: %d\n", src.x, src.y, src.w, src.h);
		//printf("DEBUG - dest.x: %d, dest.y: %d, dest.w: %d, dest.h: %d\n", dest.x, dest.y, dest.w, dest.h);
		// TODO - arrumar isso
		//SDL_BlitSurface(fundo, &src, offscreen, &dest);
	}
	//printf("DEBUG.classe_tropa::~classe_tropa - removendo tropa\n");
	// REMOVE DA LISTA DE TROPAS DO GENERAL
	classe_tropa *aux;
	aux = general->lista_tropas;
	if (aux == this) {
		if (aux->next) {
			general->lista_tropas = aux->next;
		} else {
			general->lista_tropas = NULL;
		}
	} else if (aux != NULL) {
		classe_tropa *ant;
		ant = aux;
		aux = aux->next;
		while (aux) {
			if (aux == this) {
				ant->next = aux->next;
			}
			ant = aux;
			aux = aux->next;
		}
	}
	// REMOVE DO ALVO DE TODAS AS TROPAS
	classe_general *aux_g;
	classe_predio *aux_p;
	aux_g = general->mapa->lista_generais;
	while (aux_g) {
		if (aux_g != general) {
			aux = aux_g->get_tropas();
			while (aux) {
				if (aux->alvo_tropa == this) {
					aux->alvo_tropa = NULL;
					aux->estado = ESTADO_LIVRE;
				}
				aux = aux->next;
			}
			while (aux_p) {
				if (aux_p->alvo_tropa == this) {
					aux_p->alvo_tropa = NULL;
					aux_p->estado = ESTADO_LIVRE;
				}
				aux_p = aux_p->next;
			}
		}
		aux_g = aux_g->next;
	}
}

struct coordenada classe_tropa::getPos() {
	return pos;
}

// procura unidades inimigas ou neutras ainda não conhecidas no raio dado
void classe_tropa::visao() {
	int dist, achou;
	classe_mina *aux_m;
	// vê se tem mina por perto
	aux_m = general->mapa->lista_minas;
	while (aux_m) {
		// calcula a distância entre os dois pontos
		dist = (int) sqrt(pow((float)(pos.x - aux_m->x), 2)+pow((float)(pos.y - aux_m->y), 2));
		//printf("DEBUG radar - dist: %d\n", dist);
		if (dist < raio_visao) {
			if (general->descobre_mina(aux_m)) {
				//printf("DEBUG - achou mina!\n");
			}
		}
		aux_m = aux_m->next;
	}
	// PROCURA LISTA DE PRÉDIOS E TROPAS INIMIGAS
	classe_general *aux_g;
	aux_g = general->mapa->lista_generais;
	classe_tropa *aux_t;
	classe_predio *aux_p;
	while (aux_g) {
		//printf("DEBUG.classe_tropa::visao - VISÃO TROPA DE GENERAL %d, TROPA: %s\n", general->id, tipo);
		if (aux_g->id != general->id) {
			//printf("DEBUG.classe_tropa::visao - GENERAL DIFERENTE, VERIFICANDO TROPAS\n");

			// TROPAS
			if (estado == ESTADO_LIVRE || estado == ESTADO_ATACANDO_PREDIO || estado == ESTADO_ATACANDO_LOCAL) {
				aux_t = aux_g->lista_tropas;
				while (aux_t) {
					dist = (int) sqrt(pow((float)(pos.x - aux_t->getPos().x), 2)+pow((float)(pos.y - aux_t->getPos().y), 2));
					//printf("DEBUG.classe_tropa::visao - dist: %d\n", dist);
					if (dist < raio_visao) {
						//printf("DEBUG.classe_tropa::visao - ACHOU INIMIGO, ataque: %d!\n", ataque);
						if (ataque >= 2) {
							if (estado == ESTADO_LIVRE) {
								//printf("DEBUG.classe_tropa::visao - indo atacar\n");
								alvo_tropa = aux_t;
								estado = ESTADO_ATACANDO_TROPA;
								//printf("DEBUG - alerta de ATACANDO tropa %s\n", alvo_tropa->tipo);
								general->alerta(alvo_tropa, "TROPA", "ATACANDO");
							}
						} else {
							//printf("DEBUG.classe_tropa::visao - fugindo\n");
							estado = ESTADO_FUGINDO;
						}
					}
					aux_t = aux_t->next;
				}
			}

			// PREDIOS
			if (estado == ESTADO_LIVRE || estado == ESTADO_ATACANDO_LOCAL) {
				aux_p = aux_g->lista_predios;
				while (aux_p) {
					dist = (int) sqrt(pow((float)(pos.x - aux_p->x), 2)+pow((float)(pos.y - aux_p->y), 2));
					//printf("DEBUG.classe_tropa::visao - dist: %d\n", dist);
					if (dist < raio_visao) {
						//printf("DEBUG.classe_tropa::visao - ACHOU INIMIGO, ataque: %d!\n", ataque);
						if (ataque >= 2) {
							//printf("DEBUG.classe_tropa::visao - indo atacar %s\n", aux_p->tipo);
							alvo_predio = aux_p;
							estado = ESTADO_ATACANDO_PREDIO;
							//printf("DEBUG - alerta de ATACANDO predio %s\n", alvo_predio->tipo);
							general->alerta(alvo_predio, "PREDIO", "ATACANDO");
						} else {
							//printf("DEBUG.classe_tropa::visao - fugindo\n");
							estado = ESTADO_FUGINDO;
						}
					}
					aux_p = aux_p->next;
				}
			}
		} else {
			//printf("DEBUG.classe_tropa::visao - PROPRIO GENERAL, IGNORANDO\n");
		}
		aux_g = aux_g->next;
	}
}


void classe_tropa::agir() {
	int dist, nivel;
	if (estado == ESTADO_ATACANDO_LOCAL && alvo_local.x != 0 && alvo_local.y != 0) {
		//printf("DEBUG.classe_tropa::agir() - ATACANDO LOCAL\n");
		dist = (int) sqrt(pow((float)(pos.x - alvo_local.x), 2)+pow((float)(pos.y - alvo_local.y), 2));
		if (dist > distancia_ataque) {
			//printf("DEBUG.classe_tropa::agir() - ATACANDO LOCAL - MOVENDO-SE\n");
			ir(alvo_local.x, alvo_local.y);
		} else {
			estado = ESTADO_LIVRE;
		}
	} else if (estado == ESTADO_ATACANDO_TROPA) {
		if (alvo_tropa == NULL) {
			estado = ESTADO_LIVRE;
		} else if (alvo_tropa != NULL) {
			dist = (int) sqrt(pow((float)(pos.x - alvo_tropa->pos.x), 2)+pow((float)(pos.y - alvo_tropa->pos.y), 2));
			if (dist > distancia_ataque) {
				//printf("DEBUG.classe_tropa::agir - TROPA %s INDO PARA TROPA %dx%d\n", tipo, alvo_tropa->pos.x, alvo_tropa->pos.y);
				ir(alvo_tropa->pos.x, alvo_tropa->pos.y);
			} else {
				estado = ESTADO_REARMANDO_TROPA;
				timer_rearme = 0;
				// TODO - DAMAGE
				alvo_tropa->HP -= ataque;
				//printf("DEBUG - alerta de %s RECEBENDO ATAQUE de tropa %s\n", alvo_tropa->tipo, tipo);
				alvo_tropa->general->alerta(this, "TROPA", "RECEBENDO_ATAQUE");
				
				//printf("DEBUG.classe_tropa::agir.FAZ-DANO-TROPA - HP: %d, HP-INI: %d, tipo: %s\n", HP, alvo_tropa->HP, alvo_tropa->tipo);
				if (alvo_tropa->HP <= 0) {
					alvo_tropa->general->contagem_tropas_perdidas += 1;
					general->contagem_tropas_destruidas += 1;
					delete alvo_tropa;
					alvo_tropa = NULL;
					estado = ESTADO_LIVRE;
					//printf("DEBUG.classe_tropa::agir.FAZ-DANO-TROPA - inimigo morto!\n");
				}
			}
		}
	} else if (estado == ESTADO_ATACANDO_PREDIO) {
		if (alvo_predio == NULL) {
			estado = ESTADO_LIVRE;
		} else if (alvo_predio != NULL) {
			if (pos.x != alvo_predio->x || pos.y != alvo_predio->y) {
				//printf("DEBUG.classe_tropa::agir - TROPA %s INDO PARA PREDIO %s EM %dx%d\n", tipo, alvo_predio->tipo, alvo_predio->x, alvo_predio->y);
				ir(alvo_predio->x, alvo_predio->y);
			} else {
				timer_rearme = 0;
				// TODO - DAMAGE
				alvo_predio->HP -= ataque;
				//printf("DEBUG - alerta de %s RECEBENDO ATAQUE de tropa %s\n", alvo_predio->tipo, tipo);
				alvo_predio->general->alerta(this, "TROPA", "RECEBENDO_ATAQUE");
				//printf("DEBUG.classe_tropa::agir.FAZ-DANO-PREDIO - HP: %d, HP-INI: %d\n", HP, alvo_predio->HP);
				if (alvo_predio->HP <= 0) {
					//printf("DEBUG.classe_tropa::agir.FAZ-DANO - predio destruído!\n");
					alvo_predio->general->contagem_predios_perdidos += 1;
					general->contagem_predios_destruidos += 1;
					delete alvo_predio;
					alvo_predio = NULL;
					estado = ESTADO_LIVRE;
				} else {
					estado = ESTADO_REARMANDO_PREDIO;
				}
			}
			
		}
	} else if (estado == ESTADO_REARMANDO_TROPA) {
		if (timer_rearme <= tempo_rearme) {
			timer_rearme++;
		} else {
			timer_rearme = 0;
			estado = ESTADO_ATACANDO_TROPA;
		}
	} else if (estado == ESTADO_REARMANDO_PREDIO) {
		if (timer_rearme <= tempo_rearme) {
			timer_rearme++;
		} else {
			timer_rearme = 0;
			//printf("MUDANDO ESTADO PARA ESTADO_ATACANDO_PREDIO 2\n");
			estado = ESTADO_ATACANDO_PREDIO;
		}
	// PARTE DE PEÃO
	} else if (estado == ESTADO_BUSCANDO_OURO || estado == ESTADO_BUSCANDO_MADEIRA) {
		//printf("DEBUG - peao indo minerar\n");
		int goods;
		if (estado == ESTADO_BUSCANDO_MADEIRA) {
			goods = madeira;
		} else {
			goods = ouro;
		}
		if (goods <= 10) {
			//printf("DEBUG - nao tem ouro\n");
			if (alvo_mina) {
				// esta fora da mina
				if (pos.x != alvo_mina->x || pos.y != alvo_mina->y) {
					ir(alvo_mina->x, alvo_mina->y);
					// chegou na mina - aumenta ocupacao dela
					//printf("FORA-MINA\n");
					// esta dentro da mina
				} else {
					//printf("DENTRO-MINA\n");
					if (estado == ESTADO_BUSCANDO_MADEIRA) {
						madeira++;
					} else {
						ouro++;
					}
				}
			}
		} else {
			if (estado == ESTADO_BUSCANDO_MADEIRA) {
				estado = ESTADO_RETORNANDO_MADEIRA;
			} else {
				estado = ESTADO_RETORNANDO_OURO;
			}
		}
	} else if (estado == ESTADO_RETORNANDO_OURO || estado == ESTADO_RETORNANDO_MADEIRA) {
		//printf("DEBUG - tem ouro\n");
		int goods;
		if (estado == ESTADO_RETORNANDO_MADEIRA) {
			goods = madeira;
		} else {
			goods = ouro;
		}
		if (goods > 0) {
			if (!alvo_base) {
				//printf("DEBUG - nao tem base-alvo\n");
				alvo_base = procuraBaseMaisProxima();
				//printf("DEBUG2\n");
			}
			if (alvo_base) {
				//printf("DEBUG - fora base\n");
				// esta fora da base
				if (pos.x != alvo_base->x || pos.y != alvo_base->y) {
					ir(alvo_base->x, alvo_base->y);
					// dentro da base
				} else {
					//printf("DEBUG - dentro base\n");
					if (estado == ESTADO_RETORNANDO_MADEIRA) {
						madeira-=1;
						general->main_wood = general->main_wood+1;
					} else {
						ouro-=1;
						general->inc_gold(1);
					}
				}
			} else {
				//printf("DEBUG - procurar base\n");
				// PROCURAR BASE
			}
		} else {
			if (estado == ESTADO_RETORNANDO_MADEIRA) {
				estado = ESTADO_BUSCANDO_MADEIRA;
			} else {
				estado = ESTADO_BUSCANDO_OURO;
			}
		}
	} else if (estado == ESTADO_MOVENDO) {
		if (alvo.x == -1 && alvo.y == -1) {
			//printf("DEBUG - procurando alvo\n");
			defineLocalConstrucao();
			//printf("DEBUG - novo alvo: (%d, %d)\n", alvo.x, alvo.y);
		}
		// não chegou ainda no lugar definido
		if (pos.x != alvo.x || pos.y != alvo.y) {
			ir(alvo.x, alvo.y);
			// chegou no local
		} else {
			estado = ESTADO_CONSTRUINDO;
			//general->main_gold = general->main_gold - general->busca_objetivo(ordem)->custo_ouro;
		}
		// TODO - tempo de construção
	} else if (estado == ESTADO_CONSTRUINDO) {
		nivel = general->busca_objetivo(ordem)->nivel;
		if (contador <= (nivel+1)*TEMPO_CONSTRUCAO) {
			contador++;
		} else {
			// verifica se local AINDA está livre
			//printf("Local (%dx%d) livre: %d\n", pos.x, pos.y, localLivre(pos.x, pos.y));
			if (localLivre(pos.x, pos.y) != 1) {
				defineLocalConstrucao();
				//printf("NOVO alvo: (%dx%d)\n", alvo.x, alvo.y);
				estado = ESTADO_MOVENDO;
				contador = 0;
			} else {
				// paga o preço, se tiver dinheiro, senão, dá alerta ao general
				//printf("DEBUG.classe_peao.agir.1 - ouro: %d, custo: %d\n", general->main_gold, general->busca_objetivo(ordem)->custo_ouro);
				if (general->main_gold < general->busca_objetivo(ordem)->custo_ouro) { 
					//printf("DEBUG.classe_peao.agir - NAO TEM OURO PARA CONSTRUIR\n");
					estado = ESTADO_LIVRE;
					ordem = "";
				} else if (general->main_wood < general->busca_objetivo(ordem)->custo_madeira) {
					//printf("DEBUG.classe_peao.agir - NAO TEM MADEIRA PARA CONSTRUIR\n");
					estado = ESTADO_LIVRE;
					ordem = "";
				} else {
					//printf("PEAO CONSTRUINDO %s\n", ordem);
					general->main_gold = general->main_gold - general->busca_objetivo(ordem)->custo_ouro;
					general->main_wood = general->main_wood - general->busca_objetivo(ordem)->custo_madeira;
					if (ordem == "BASE" || ordem == "FORJA" || ordem == "SERRARIA" || ordem == "TORRE" || ordem == "ESTÁBULOS" || ordem == "CASTELO" || ordem == "IGREJA" || ordem == "TORRE MAGOS" || ordem == "GRIFOÁRIO" || ordem == "INVENTOR" || ordem == "FAZENDA") {
						criar_predio(ordem);
						// remove plano do general e passa ao próximo
						estado = ESTADO_LIVRE;
						//printf("DEBUG.classe_tropa::agir - remove_objetivo(%s)\n", ordem);
						general->remove_objetivo(ordem);
						estado = ESTADO_LIVRE;
						ordem = "";
						// evita colar o fundo velho sobre o pré
						ordem = "";
					} else {
						printf("Peao não sabe cumprir ordem %s\n", ordem);
					}
				}
				contador = 0;
			}
		}
	}
}

void classe_tropa::ir(int x1, int y1) {
	if (pos.x > x1) {
		if (pos.x-STEP > x1) {
			pos.x-= STEP;
		} else {
			pos.x = x1;
		}
		direcao = 7;
	} else if (pos.x < x1) {
		if (pos.x+STEP < x1) {
			pos.x+= STEP;
		} else {
			pos.x = x1;
		}
		direcao = 3;
	}
	if (pos.y > y1) {
		if (pos.y-STEP > y1) {
			pos.y-= STEP;
		} else {
			pos.y = y1;
		}
		direcao = 1;
	} else if (pos.y < y1) {
		if (pos.y+STEP < y1) {
			pos.y+= STEP;
		} else {
			pos.y = y1;
		}
		direcao = 5;
	}
}

// FUNÇÕES DE PEÃO
// verifica se não existe nenhum prédio bloqueando esta posição
int classe_tropa::localLivre(int x, int y) {
	if (x <= 0 || y <= 0) {
		//printf("NEGATIVO NAO PODE\n");
		return 0;
	}
	//printf("DEBUG - classe_peao::localLivre(%d, %d)\n", x, y);
	// 1. minas
	classe_mina *aux1;
	aux1 = general->mapa->getMinas();
	while (aux1) {
		//printf("DEBUG.classe_peao.localLivre A - x: %d, y: %d, aux2->x: %d, aux2->y: %d\n", x, y, aux1->x, aux1->y);
		if ((x >= aux1->x-128-SEPARACAO_PREDIOS && x <= aux1->x+128+SEPARACAO_PREDIOS) || (x <= aux1->x-128-SEPARACAO_PREDIOS && x >= aux1->x+128+SEPARACAO_PREDIOS)) {
			if ((y >= aux1->y-128-SEPARACAO_PREDIOS && y <= aux1->y+128+SEPARACAO_PREDIOS) || (y <= aux1->y-128-SEPARACAO_PREDIOS && y >= aux1->y+128+SEPARACAO_PREDIOS)) {
				return 0;
			}
		}
		aux1 = aux1->next;
	}
	// 2. predios
	classe_general *aux_g;
	aux_g = general->mapa->lista_generais;
	while (aux_g) {
		classe_predio *aux2;
		aux2 = aux_g->lista_predios;
		while (aux2) {
			//printf("DEBUG.classe_peao.localLivre B - x: %d, y: %d, aux2->x: %d, aux2->y: %d\n", x, y, aux2->x, aux2->y);
			if ((x >= aux2->x-128-SEPARACAO_PREDIOS && x <= aux2->x+128+SEPARACAO_PREDIOS) || (x <= aux2->x-128-SEPARACAO_PREDIOS && x >= aux2->x+128+SEPARACAO_PREDIOS)) {
				if ((y >= aux2->y-128-SEPARACAO_PREDIOS && y <= aux2->y+128+SEPARACAO_PREDIOS) || (y <= aux2->y-128-SEPARACAO_PREDIOS && y >= aux2->y+128+SEPARACAO_PREDIOS)) {
					return 0;
				}
			}
			aux2 = aux2->next;
		}
		aux_g = aux_g->next;
	}
	//printf("DEBUG.classe_peao.localLivre ALVO - x: %d, y: %d\n", x, y);
	return 1;
}

void classe_tropa::defineLocalConstrucao() {
	classe_predio *aux_b;
	classe_mina *aux_m;
	int fim, dist;
	int origX, origY, tempX, tempY;

	aux_b = procuraBaseMaisProxima();
	if (aux_b) {
		//printf("POSIÇÃO-BASE DA BASE\n");
		origX = aux_b->x;
		origY = aux_b->y;
	} else if (aux_m = procuraMinaMaisProxima()) {
		//printf("POSIÇÃO-BASE DA MINA\n");
		origX = aux_m->getX();
		origY = aux_m->getY();
	} else {
		//printf("POSIÇÃO-BASE DO PEÃO\n");
		origX = pos.x;
		origY = pos.y;
	}
	//printf("DEBUG 2 - origX: %d, origX: %d\n", origX, origY);
	dist = 10;
	fim = 0;
	// verifica quadrantes ao redor
	while (fim == 0) {
		if (localLivre(origX-dist, origY-dist) == 1) {
			alvo.x = origX-dist;
			alvo.y = origY-dist;
			fim = 1;
		} else if (localLivre(origX, origY-dist) == 1) {
			alvo.x = origX;
			alvo.y = origY-dist;
			fim = 1;
		} else if (localLivre(origX+dist, origY-dist) == 1) {
			alvo.x = origX+dist;
			alvo.y = origY-dist;
			fim = 1;
		} else if (localLivre(origX+dist, origY) == 1) {
			alvo.x = origX+dist;
			alvo.y = origY;
			fim = 1;
		} else if (localLivre(origX+dist, origY+dist) == 1) {
			alvo.x = origX+dist;
			alvo.y = origY+dist;
			fim = 1;
		} else if (localLivre(origX, origY+dist) == 1) {
			alvo.x = origX;
			alvo.y = origY+dist;
			fim = 1;
		} else if (localLivre(origX-dist, origY+dist) == 1) {
			alvo.x = origX-dist;
			alvo.y = origY+dist;
			fim = 1;
		} else if (localLivre(origX-dist, origY) == 1) {
			alvo.x = origX-dist;
			alvo.y = origY;
			fim = 1;
		}
		dist = dist+10;
	}
	//printf("DEBUG 3 - alvo.X: %d, alvo.y: %d\n", alvo.x, alvo.y);
}

classe_predio* classe_tropa::procuraBaseMaisProxima(void) {
	classe_predio *aux_b, *menor_b;

	menor_b = NULL;
	int menor=-1, dist=0;
	//printf("DEBUG procuraBase\n");
	aux_b = general->lista_predios;
	if (!aux_b) {
		//printf("DEBUG - general->lista_bases = NULL\n");
		return NULL;
	} else {
		//printf("TEM LISTA\n");
		while(aux_b) {
			//printf("DEBUG1\n");
			//printf("DEBUG x: %d, y: %d\n", x, y);
			//printf("DEBUG aux_x: %d, aux_y: %d\n", aux_b->x, aux_b->y);
			if (aux_b->tipo == "BASE") {
				dist = (int) sqrt(pow((float)(pos.x - aux_b->x), 2)+pow((float)(pos.y - aux_b->y), 2));
				//printf("DEBUG - dist: [%d]\n", dist);
				if (dist > menor) {
					menor = dist;
					menor_b = aux_b;
				}
			}
			aux_b = aux_b->next;
		}
	}
	return menor_b;
}


classe_mina* classe_tropa::procuraMinaMaisProxima(void) {
	classe_mina *aux_b, *menor_b;

	int menor=-1, dist=0;
	//printf("DEBUG procuraBase\n");
	aux_b = general->get_lista_minas();
	if (!aux_b) {
		//printf("DEBUG - general->lista_bases = NULL\n");
		return NULL;
	} else {
		//printf("TEM LISTA\n");
		while(aux_b) {
			//printf("DEBUG1\n");
			//printf("DEBUG x: %d, y: %d\n", x, y);
			//printf("DEBUG aux_x: %d, aux_y: %d\n", aux_b->x, aux_b->y);
			dist = (int) sqrt(pow((float)(pos.x - aux_b->x), 2)+pow((float)(pos.y - aux_b->y), 2));
			//printf("DEBUG - dist: [%d]\n", dist);
			if (dist > menor) {
				menor = dist;
				menor_b = aux_b;
			}
			aux_b=aux_b->next;
		}
	}
	return menor_b;
}


void classe_tropa::criar_predio(char *tipo) {
	printf("CRIANDO NOVO PREDIO %s\n", tipo);
	classe_predio *aux;
	classe_predio *novo = new classe_predio(general, tipo);
	novo->x = alvo.x;
	novo->y = alvo.y;
	novo->next = NULL;

	if (general->lista_predios == NULL) {
		general->lista_predios = novo;
	} else {
		aux = general->lista_predios;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = novo;
	}
}


