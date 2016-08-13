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

/*
TODO
- classe genérica para unidades, unidades-móveis e prédios
- criar_serraria
*/





// ------------------------------------ DEFINIÇÕES ------------------------------------ //

#include "main.h"

//#include "classe_pai.h"
#include "classe_mina.h"
#include "classe_tropa.h"
#include "classe_predio.h"
#include "classe_mapa.h"
#include "classe_general.h"

// CLASSE QUE CONTÉM FUNÇÕES DE DESENHO

#include "gfx_sdl.h"

#include "classe_tropa.cpp"
#include "classe_predio.cpp"

int n_generais;

// ------------------------------------ FUNÇÔES ------------------------------------ //

st_objetivos *cria_objetivo(char *nome, int tipo) {
	struct st_objetivos *novo;
	int i;
	//novo = (struct st_objetivos *)malloc(sizeof(struct st_objetivos));
	novo = new struct st_objetivos;
	novo->nome = nome;
	novo->tipo = tipo;
	novo->next = NULL;
	novo->tropa = 0;
	novo->cria_tropas = 0;
	novo->nivel = 0;
	novo->custo_ouro = 100;
	novo->custo_madeira = 0;
	for (i=0; i<3; i++) {
		novo->requisitos[i] = NULL;
	}
	//printf("DEBUG novo->nome: %s\n", novo->nome);
	return novo;
}




void adiciona_objetivo(struct st_objetivos *novo) {
	struct st_objetivos *aux;
	aux = objetivos;
	if (aux) {
		// vai para o fim da lista
		while (aux->next) {
			aux = aux->next;
		}
		// adiciona à lista de ramos
		aux->next = novo;
	} else {
		objetivos = novo;
	}
}

// TODO fazer função que calcula numero dedependentes do objetivo

void define_objetivos() {
	struct st_objetivos *aux;
	objetivos = cria_objetivo("BASE", 0);
	objetivos->custo_ouro = 120;
	objetivos->custo_madeira = 80;
	//printf("DEBUG - objetivos->none: %s\n", objetivos->nome);
	
	aux = cria_objetivo("FAZENDA", 0);
	aux->requisitos[0] = "BASE";
	aux->custo_ouro = 50;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("FORJA", 0);
	aux->requisitos[0] = "BASE";
	aux->cria_tropas = 1;
	aux->nivel = 1;
	aux->custo_ouro = 80;
	aux->custo_madeira = 45;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("SERRARIA", 0);
	aux->requisitos[0] = "BASE";
	aux->cria_tropas = 1;
	aux->nivel = 1;
	aux->custo_ouro = 60;
	aux->custo_madeira = 45;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("PEAO", 1);
	aux->requisitos[0] = "BASE";
	aux->nivel = 1;
	aux->custo_ouro = 40;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("GUERREIROS", 1);
	aux->requisitos[0] = "FORJA";
	aux->tropa = 1;
	aux->nivel = 2;
	aux->custo_ouro = 60;
	adiciona_objetivo(aux);

	
	aux = cria_objetivo("CATAPULTA", 1);
	aux->requisitos[0] = "FORJA";
	aux->requisitos[1] = "SERRARIA";
	aux->tropa = 1;
	aux->nivel = 2;
	aux->custo_ouro = 90;
	aux->custo_madeira = 30;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("TORRE", 0);
	aux->requisitos[0] = "FORJA";
	aux->requisitos[1] = "SERRARIA";
	// -1 quer dizer tropa de defesa
	aux->tropa = -1;
	aux->nivel = 2;
	aux->custo_ouro = 55;
	aux->custo_madeira = 200;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("ESTÁBULOS", 0);
	aux->requisitos[0] = "SERRARIA";
	aux->cria_tropas = 1;
	aux->nivel = 2;
	aux->custo_ouro = 100;
	aux->custo_madeira = 30;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("ARQUEIROS", 0);
	aux->requisitos[0] = "SERRARIA";
	aux->tropa = 1;
	aux->nivel = 2;
	aux->custo_ouro = 50;
	aux->custo_madeira = 50;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("CAVALEIROS", 0);
	aux->requisitos[0] = "ESTÁBULOS";
	aux->tropa = 1;
	aux->nivel = 3;
	aux->custo_ouro = 80;
	aux->custo_madeira = 10;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("CASTELO", 0);
	aux->requisitos[0] = "ESTÁBULOS";
	aux->cria_tropas = 0;
	aux->nivel = 3;
	aux->custo_ouro = 250;
	aux->custo_madeira = 120;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("IGREJA", 0);
	aux->requisitos[0] = "CASTELO";
	aux->cria_tropas = 1;
	aux->nivel = 4;
	aux->custo_ouro = 90;
	aux->custo_madeira = 50;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("TORRE MAGOS", 0);
	aux->requisitos[0] = "CASTELO";
	aux->cria_tropas = 1;
	aux->nivel = 4;
	aux->custo_ouro = 80;
	aux->custo_madeira = 60;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("PALADINOS", 0);
	aux->requisitos[0] = "IGREJA";
	aux->tropa = 1;
	aux->nivel = 5;
	aux->custo_ouro = 80;
	aux->custo_madeira = 10;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("MAGOS", 0);
	aux->requisitos[0] = "TORRE MAGOS";
	aux->tropa = 1;
	aux->nivel = 5;
	aux->custo_ouro = 100;
	aux->custo_madeira = 200;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("GRIFOÁRIO", 0);
	aux->requisitos[0] = "IGREJA";
	aux->requisitos[1] = "TORRE MAGOS";
	aux->custo_ouro = 100;
	aux->custo_madeira = 100;
	aux->cria_tropas = 1;
	aux->nivel = 5;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("INVENTOR", 0);
	aux->requisitos[0] = "IGREJA";
	aux->requisitos[1] = "TORRE MAGOS";
	aux->cria_tropas = 1;
	aux->nivel = 5;
	aux->custo_ouro = 100;
	aux->custo_madeira = 40;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("GRIFO", 0);
	aux->requisitos[0] = "GRIFOÁRIO";
	aux->tropa = 1;
	aux->nivel = 6;
	//aux->custo_ouro = 250;
	aux->custo_ouro = 1;
	//aux->custo_madeira = 400;
	aux->custo_madeira = 4;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("ESQUADRAO DE DEMOLICAO", 0);
	aux->requisitos[0] = "INVENTOR";
	aux->tropa = 1;
	aux->nivel = 6;
	aux->custo_ouro = 75;
	aux->custo_madeira = 25;
	adiciona_objetivo(aux);
	
	aux = cria_objetivo("MAQUINA VOADORA", 0);
	aux->requisitos[0] = "INVENTOR";
	aux->tropa = 1;
	aux->nivel = 6;
	aux->custo_ouro = 50;
	aux->custo_madeira = 10;
	adiciona_objetivo(aux);
}


void lista_objetivos() {
	int i;
	struct st_objetivos *aux;
	aux = objetivos;
	while(aux) {
		printf("Objetivo: %s\n", aux->nome);
		for (i=0; i<3 && aux->requisitos[i]; i++) {
			printf("Requisito %d: %s\n", i, aux->requisitos[i]);
		}	
		aux = aux->next;
	}
}

void eventos(classe_mapa *mapa) {
	SDL_Event event; //Events
	while(SDL_PollEvent(&event)){ //Poll events
		switch(event.type){ //Check event type
			case SDL_QUIT: //User hit the X (or equivelent)
				fim = 1; //Make the loop end
				break; //We handled the event
				case SDLK_SPACE: //User resized window
					if (paused) {
						paused = 0;
					} else {
						paused = 1;
					}
					break; //Event handled, fetch next :)
			case SDL_KEYDOWN:
				//printf("The %s key was pressed!\n", SDL_GetKeyName(event.key.keysym.sym));
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q) {
					printf("\nsaindo...\n");
					fim = 1;
					// a tecla de espaço foi pressionada, pausa ou sai do pause
				} else if (event.key.keysym.sym == SDLK_SPACE) {
					if (paused) {
						paused = 0;
					} else {
						paused = 1;
					}
					// pequeno tempo para evitar a taxa de repetição de teclado
					SDL_Delay(100);
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					if (scroll_y+SCROLL_STEP+VIDEO_H <= mapa->height) {
						scroll_y += SCROLL_STEP;
					} else {
						//scroll_y = mapa->height;
					}
				} else if (event.key.keysym.sym == SDLK_UP) {
					if (scroll_y-SCROLL_STEP > 0) {
						scroll_y -= SCROLL_STEP;
					} else {
						scroll_y = 0;
					}
				} else if (event.key.keysym.sym == SDLK_LEFT) {
					if (scroll_x-SCROLL_STEP > 0) {
						scroll_x -= SCROLL_STEP;
					} else {
						scroll_x = 0;
					}
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					if (scroll_x+SCROLL_STEP+VIDEO_W <= mapa->width) {
						scroll_x += SCROLL_STEP;
					} else {
						//scroll_x = mapa->width;
					}
				} else if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_KP_PLUS) {
					printf("AUMENTANDO DELAY %d, %d\n", DELAY, DELAY+5);
					DELAY += 5;
				} else if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS) {
					if (DELAY-5 >= 0) {
						printf("DIMINUINDO DELAY %d, %d\n", DELAY, DELAY-5);
						DELAY -= 5;
					}
				}
				break;			
		} //Finished with current event
	} //Done with all events for now
}

void criar_general(char *nome, int x, int y, int agress, classe_mapa *mapa) {
	int id;
	//printf("DEBUG - criar_general() inicio\n");
	classe_general *aux;
	classe_general *novo = new classe_general(nome, x, y, agress);
	novo->set_mapa(mapa);
	novo->visao();
	if(!mapa->lista_generais) {
		mapa->lista_generais = novo;
	} else {
		aux = mapa->lista_generais;
		id = 1;
		while (aux->next) {
			aux = aux->next;
			id++;
		}
		novo->id = id;
		aux->next = novo;
	}
	printf("CRIANDO GENERAL %d COM AGRESSIVIDADE %d\n", novo->id, novo->agressividade);
	//printf("DEBUG - criar_peao() fim\n");
	n_generais += 1;
}


void inicializa_contador(struct st_contador *contador) {
	contador->n = 0;
	contador->time1 = SDL_GetTicks();
}

void contadores(classe_general *aux_g) {
	// CONTADOR DE FALTA DE OURO
	aux_g->contador_pouco_ouro.time2 = SDL_GetTicks();
	if (aux_g->contador_pouco_ouro.time2 - aux_g->contador_pouco_ouro.time1 >= 5000) {
		//printf("TICK OURO\n");
		if (aux_g->main_gold < 500) {
			aux_g->contador_pouco_ouro.n++;
		} else {
			if (aux_g->contador_pouco_ouro.n > 0) {
				aux_g->contador_pouco_ouro.n--;
			}
		}
		if (aux_g->contador_pouco_ouro.n >= 5) {
			if (!aux_g->temPlano("MINERAR")) {
				printf("DEBUG.tfgcpp - CONTADOR POUCO OURO - ARRECADAR MAIS!\n");
				aux_g->enviarOrdem("MINERAR");
			}
			aux_g->contador_pouco_ouro.n = 0;
		}
		//printf("TICK GENERAL %d, n: %d\n", aux_g->id, aux_g->contador_pouco_ouro.n);
		aux_g->contador_pouco_ouro.time1 = SDL_GetTicks();
	}
	// CONTADOR DE TROPAS OCIOSAS
	aux_g->contador_tropas_ociosas.time2 = SDL_GetTicks();
	if (aux_g->contador_tropas_ociosas.time2 - aux_g->contador_tropas_ociosas.time1 >= 5000) {
		//printf("TICK TROPAS\n");
		if (aux_g->n_tropas_livres() >= 10) {
			//printf("Tem tropas livres sobrando\n");
			aux_g->contador_tropas_ociosas.n++;
		} else {
			if (aux_g->contador_tropas_ociosas.n > 0) {
				aux_g->contador_tropas_ociosas.n--;
			}
		}
		if (aux_g->contador_tropas_ociosas.n >= 5) {
			printf("DEBUG.tfgcpp - CONTADOR DE TROPAS LIVRES - ENVIAR ATAQUE!\n");
			aux_g->adiciona_objetivo("ATACAR CANTO", 1);
			aux_g->contador_tropas_ociosas.n = 0;
		}
		//printf("TICK GENERAL %d, n: %d\n", aux_g->id, aux_g->contador_tropas_ociosas.n);
		aux_g->contador_tropas_ociosas.time1 = SDL_GetTicks();
	}
}


int main(int argc, char *argv[]) {
	SDL_Rect src, dest;
	objetivos = NULL;
	classe_general *aux_g;
	n_generais = 0;
	
	define_objetivos();
	//lista_objetivos();
	if(SDL_Init(SDL_INIT_VIDEO) < 0){ //Could we start SDL_VIDEO?
		std::cerr << "Couldn't init SDL"; //Nope, output to stderr and quit
		exit(1);
	}
	// inicializa SDL_ttf
	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	font = TTF_OpenFont("./data/fonts/freesans.ttf", 10);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(3);
	} else {
		printf("Font loaded\n");
	}
	// inicializa TELA
	screen = SDL_SetVideoMode (VIDEO_W, VIDEO_H+24, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	// nao conseguiu criar tela, retorna erro
	if (screen == NULL) {
		fprintf(stderr, "Unable to set %dx%d video: %s\n", VIDEO_W, VIDEO_H, SDL_GetError());
		exit(1);
	}
	
	// cria mapa
	classe_mapa* mapa = new classe_mapa(3000, 2000);
	//printf("DEBUG - offscreen_width: %d, offscreen_height: %d\n", mapa->width, mapa->height);
	offscreen = SDL_CreateRGBSurface (SDL_HWSURFACE , mapa->width, mapa->height, 16, 0, 0, 0, 0);
	if (offscreen == NULL) {
		fprintf(stderr, "Unable create offscreen %dx%d video: %s\n", mapa->width, mapa->height, SDL_GetError());
		exit(1);
	}
	mapa->cria_mina(10, 10, 100, "GOLD");
	mapa->cria_mina(120, 120, 100, "WOOD");

	mapa->cria_mina(2700, 10, 100, "GOLD");
	mapa->cria_mina(2600, 120, 100, "WOOD");

	mapa->cria_mina(10, 1700, 100, "GOLD");
	mapa->cria_mina(120, 1800, 100, "WOOD");
	
	mapa->cria_mina(2700, 1700, 100, "GOLD");
	mapa->cria_mina(2600, 1800, 100, "WOOD");
	
	//mapa->cria_mina(1600, 1000, 100, "GOLD");
	//mapa->cria_mina(1000, 1000, 100, "WOOD");

	// cria agente
	criar_general("GENERAL 1", 20, 20, 9, mapa);
	criar_general("GENERAL 2", 2350, 40, -9, mapa);
	criar_general("GENERAL 3", 20, 1550, 0, mapa);
	criar_general("GENERAL 4", 2350, 1750, 5, mapa);
	//criar_general("GENERAL 5", 950, 950, 5, mapa);

	classe_gfx_sdl *gfx_sdl = new classe_gfx_sdl(n_generais);
	// cria a tela
	SDL_WM_SetCaption("DragonSpirits RTS v0.7.1 - 11/12/2006", "MAXIMIZE ME");
	
	dest.x = 0;
	dest.y = 0;
	dest.w = mapa->width;
	dest.h = mapa->height;
	//SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));
	gfx_sdl->desenha_fundo(mapa->width, mapa->height);
	SDL_UpdateRect(screen,0,0,0,0);

	gfx_sdl->desenha_minas(mapa);
	SDL_EnableKeyRepeat(200, 20);

	
	// inicializa contadores dos generais
	aux_g = mapa->lista_generais;
	while (aux_g) {
		inicializa_contador(&aux_g->contador_tropas_ociosas);
		inicializa_contador(&aux_g->contador_pouco_ouro);
		inicializa_contador(&aux_g->contador_derrota);
		aux_g = aux_g->next;
	}

	while (fim == 0) {
		// define o primeiro objetivo
		// TODO - usar checa objetivos ou checa reatividade
		// TODO - flag que indica que já está executando um objetivo para não ficar chamando a função
		
		//Checa por eventos quaisquer
		eventos(mapa);
		gfx_sdl->move_screen();
		if (!paused) {
			aux_g = mapa->lista_generais;
			while (aux_g) {
				if (aux_g->derrotado == 0) {
					gfx_sdl->apagar_tropas(aux_g);
					gfx_sdl->desenha_predios(aux_g);
				}
				aux_g = aux_g->next;
			}
			aux_g = mapa->lista_generais;
			while (aux_g) {
				if (aux_g->derrotado == 0) {
					aux_g->agir();
					aux_g->executa_objetivo();
					gfx_sdl->fundo_tropas(aux_g);
				}
				aux_g = aux_g->next;
			}
			aux_g = mapa->lista_generais;
			while (aux_g) {
				if (aux_g->derrotado == 0) {
					aux_g->visao();
					gfx_sdl->desenha_tropas(aux_g);
					//printf("POS desenha_bases()\n");
					gfx_sdl->desenha_money_box(aux_g, aux_g->id);
					//printf("POS agir()\n");
				}
				contadores(aux_g);
				aux_g = aux_g->next;
			}
			SDL_Delay(DELAY);
		}
	}
	// TODO - free nas variáveis alocadas - construir destrutor para as classes que elimine as variáveis alocadas
	atexit(SDL_Quit); //Now that we're enabled, make sure we cleanup
	// finaliza SDL_ttf
	TTF_Quit();
	// finaliza SDL
	SDL_Quit();
}

