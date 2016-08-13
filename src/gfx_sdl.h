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

/* TODO
- arrumar conversão de cores
- arrumar distância de ataque
*/

class classe_gfx_sdl {
	public:
	struct st_sprites *lista_sprites_CATAPULTA[10][2][8]; // [general][tipo][estado][sprite]
	struct st_sprites *lista_sprites_PEAO[10][2][8];
	struct st_sprites *lista_sprites_GUERREIROS[10][2][8];
	struct st_sprites *lista_sprites_ARQUEIROS[10][2][8];
	struct st_sprites *lista_sprites_CAVALEIROS[10][2][8];
	struct st_sprites *lista_sprites_PALADINOS[10][2][8];
	struct st_sprites *lista_sprites_MAGOS[10][2][8];
	struct st_sprites *lista_sprites_GRIFOS[10][2][8];
	struct st_sprites *lista_sprites_ESQUADRAO_DEMOLICAO[10][2][8];
	struct st_sprites *lista_sprites_MAQUINA_VOADORA[10][2][8];

		
	classe_gfx_sdl(int n_generais); // CONSTRUTOR
	int DrawImage(char *image_path, int x_pos, int y_pos, int x1, int y1, int x2, int y2, int id_general);
	void desenha_minas(classe_mapa *mapa);
	void desenha_money_box(classe_general *general, int i);
	void desenha_predios(classe_general *general);
	void desenha_tropas(classe_general *general);
	void apagar_tropas(classe_general *general);
	void fundo_tropas(classe_general *general);
	void copy_area(SDL_Surface *screen_source, int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y, SDL_Surface *screen_dest);
	void desenha_predio(classe_predio *aux);
	void move_screen();
	void desenha_fundo(int w, int h);
	void adiciona_sprite(struct st_sprites *lista_sprites[2][8], int j, int i, SDL_Surface *image);
	void carrega_graficos_tropa(char *img_file, struct st_sprites *lista_sprites[2][8], int id_general, int img_size, int n_quadros);
	int numero_sprites(struct st_sprites *lista_sprites);
	struct st_sprites* sprite_n(struct st_sprites *lista_sprites[2][8], int tipo, int direcao, int i);
	void converte_cores(SDL_Surface *optimizedImage, int id_general);
	void carrega_graficos_tropas(int n_generais);
	void copia_vetor_graficos(struct st_sprites *lista_sprites1[2][8], struct st_sprites *lista_sprites2[2][8]);
	
	};

	
	
classe_gfx_sdl::classe_gfx_sdl(int n_generais) {
	int i, j, k;
	for (i=0; i<10; i++) {
		for (j=0; j<2; j++) {
			for (k=0; k<8; k++) {
				lista_sprites_CATAPULTA[i][j][k] = NULL;
				lista_sprites_PEAO[i][j][k] = NULL;
				lista_sprites_GUERREIROS[i][j][k] = NULL;
				lista_sprites_ARQUEIROS[i][j][k] = NULL;
				lista_sprites_CAVALEIROS[i][j][k] = NULL;
				lista_sprites_PALADINOS[i][j][k] = NULL;
				lista_sprites_MAGOS[i][j][k] = NULL;
				lista_sprites_GRIFOS[i][j][k] = NULL;
				lista_sprites_ESQUADRAO_DEMOLICAO[i][j][k] = NULL;
				lista_sprites_MAQUINA_VOADORA[i][j][k] = NULL;
			}
		}
	}
	carrega_graficos_tropas(n_generais);
}

// copia um tile de um lugar para outro
void classe_gfx_sdl::copy_area(SDL_Surface *screen_source, int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y, SDL_Surface *screen_dest) {
	SDL_Rect src, dest;
	src.x = src_x;
	src.y = src_y;
	src.w = src_w;
	src.h = src_h;
	dest.x = dest_x;
	dest.y = dest_y;
	dest.w = src_w;
	dest.h = src_h;
	//printf("DEBUG - src.x: %d, src.y: %d, src.w: %d, src.h: %d\n", src.x, src.y, src.w, src.h);
	//printf("DEBUG - dest.x: %d, dest.y: %d, dest.w: %d, dest.h: %d\n", dest.x, dest.y, dest.w, dest.h);
	SDL_BlitSurface(screen_source, &src, screen_dest, &dest);
	//SDL_Flip(screen_dest);
}

void classe_gfx_sdl::move_screen() {
	SDL_Rect src, dest;
	//printf("DEGUG - x: %d y: %d\n", dx, dy);
	src.x = scroll_x;
	src.y = scroll_y;
	src.w = VIDEO_W;
	src.h = VIDEO_H;
	//printf("DEBUG 3\n");
	// parte que vai ser colada
	dest.x = 0;
	dest.y = 0;
	dest.w = VIDEO_W;
	dest.h = VIDEO_H;
	//printf("DEBUG - src.x: %d, src.y: %d, src.w: %d, src.h: %d\n", src.x, src.y, src.w, src.h);
	//printf("DEBUG - dest.y: %d, dest.y: %d, dest.w: %d, dest.h: %d\n", dest.x, dest.y, dest.w, dest.h);
	SDL_BlitSurface (offscreen, &src, screen, &dest);
	SDL_Flip(screen);
}


void classe_gfx_sdl::carrega_graficos_tropas(int n_generais) {
	char *gfx_file;
	SDL_Rect gfx_dest;
	int n_quadros_anim, w;
	int i;
	//printf("DEBUG.classe_gfx_sdl::carrega_graficos_tropas - INICIO\n");

	gfx_file = "./graficos/units/ballista.png";
	w = 96;
	n_quadros_anim = 1;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_CATAPULTA[i], i, w, n_quadros_anim);
	}

	gfx_file = "./graficos/units/peasant.png";
	w = 56;
	n_quadros_anim = 5;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_PEAO[i], i, w, n_quadros_anim);
	}
	
	gfx_file = "./graficos/units/footman.png";
	w = 64;
	n_quadros_anim = 5;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_GUERREIROS[i], i, w, n_quadros_anim);
	}
	
	gfx_file = "./graficos/units/elven_archer.png";
	w = 64;
	n_quadros_anim = 4;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_ARQUEIROS[i], i, w, n_quadros_anim);
	}
		
	gfx_file = "./graficos/units/knight.png";
	w = 64;
	n_quadros_anim = 5;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_CAVALEIROS[i], i, w, n_quadros_anim);
	}
		
	gfx_file = "./graficos/units/knight.png";
	w = 64;
	n_quadros_anim = 5;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_PALADINOS[i], i, w, n_quadros_anim);
	}
		
	gfx_file = "./graficos/units/mage.png";
	w = 64;
	n_quadros_anim = 5;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_MAGOS[i], i, w, n_quadros_anim);
	}

	gfx_file = "./graficos/units/gryphon_rider.png";
	w = 128;
	n_quadros_anim = 2;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_GRIFOS[i], i, w, n_quadros_anim);
	}
	gfx_file = "./graficos/units/dwarven_demolition_squad.png";
	w = 64;
	n_quadros_anim = 4;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_ESQUADRAO_DEMOLICAO[i], i, w, n_quadros_anim);
	}
	gfx_file = "./graficos/units/gnomish_flying_machine.png";
	w = 128;
	n_quadros_anim = 3;
	for (i=0; i<n_generais; i++) {
		carrega_graficos_tropa(gfx_file, lista_sprites_MAQUINA_VOADORA[i], i, w, n_quadros_anim);
	}

}

// copia de 2 para 1
void classe_gfx_sdl::copia_vetor_graficos(struct st_sprites *lista_sprites1[2][8], struct st_sprites *lista_sprites2[2][8]) {
	int i, j;
	for (i=0; i<2; i++) {
		for (j=0; j<8; j++) {
			//printf("Copiando graficos em %d x %d\n", i, j);
			lista_sprites1[i][j] = lista_sprites2[i][j];
		}
	}
	
}


void classe_gfx_sdl::converte_cores(SDL_Surface *optimizedImage, int id_general) {
	Uint32 cor1, cor2, cor3, cor4;
	Uint32 *pixels = (Uint32 *)optimizedImage->pixels;
	int i, j;
	//Convert the pixels to 32 bit 
	if (id_general == 0) {
		cor1 = SDL_MapRGB(optimizedImage->format, 164, 0, 0);
		cor2 = SDL_MapRGB(optimizedImage->format, 124, 0, 0);
		cor3 = SDL_MapRGB(optimizedImage->format, 92, 4, 0);
		cor4 = SDL_MapRGB(optimizedImage->format, 42, 4, 0);
	} else if (id_general == 1) {
		cor1 = SDL_MapRGB(optimizedImage->format, 0, 0, 164);
		cor2 = SDL_MapRGB(optimizedImage->format, 0, 0, 124);
		cor3 = SDL_MapRGB(optimizedImage->format, 0, 4, 92);
		cor4 = SDL_MapRGB(optimizedImage->format, 0, 0, 42);
	} else if (id_general == 2) {
		cor1 = SDL_MapRGB(optimizedImage->format, 0, 164, 0);
		cor2 = SDL_MapRGB(optimizedImage->format, 0, 124, 0);
		cor3 = SDL_MapRGB(optimizedImage->format, 4, 92, 0);
		cor4 = SDL_MapRGB(optimizedImage->format, 4, 42, 0);
	} else if (id_general == 3) {
		cor1 = SDL_MapRGB(optimizedImage->format, 164, 164, 0);
		cor2 = SDL_MapRGB(optimizedImage->format, 124, 124, 0);
		cor3 = SDL_MapRGB(optimizedImage->format, 96, 92, 0);
		cor4 = SDL_MapRGB(optimizedImage->format, 42, 42, 0);
	} else if (id_general == 4) {
		cor1 = SDL_MapRGB(optimizedImage->format, 0, 164, 164);
		cor2 = SDL_MapRGB(optimizedImage->format, 0, 124, 124);
		cor3 = SDL_MapRGB(optimizedImage->format, 4, 92, 92);
		cor4 = SDL_MapRGB(optimizedImage->format, 4, 42, 42);
	}
	//printf("DEBUG.classe_gfx_sdl::converte_cores - Convertendo imagem, w: %d, h: %d\n", optimizedImage->w, optimizedImage->h);
	for (i=0; i<optimizedImage->w; i++) {
		for (j=0; j<optimizedImage->h; j++) {
			//printf("DEBUG.classe_gfx_sdl::converte_cores - i: %d, j: %d\n", i, j);
			if (pixels[(j*optimizedImage->w)+i] == SDL_MapRGB(optimizedImage->format, 112, 255, 112) || pixels[(j*optimizedImage->w)+i] == SDL_MapRGB(optimizedImage->format, 222, 69, 69)) {
				//printf("DEBUG.classe_gfx_sdl::converte_cores - trocou cor1, i: %d, j: %d\n", i, j);
				pixels[(j*optimizedImage->w)+i] = cor1;
			} else if (pixels[(j*optimizedImage->w)+i] == SDL_MapRGB(optimizedImage->format, 42, 206, 38) || pixels[(j*optimizedImage->w)+i] == SDL_MapRGB(optimizedImage->format, 189, 35, 35)) {
				//printf("DEBUG.classe_gfx_sdl::converte_cores - trocou cor2, i: %d, j: %d\n", i, j);
				pixels[(j*optimizedImage->w)+i] = cor2;
			} else if (pixels[(j*optimizedImage->w)+i] == SDL_MapRGB(optimizedImage->format, 39, 147, 24)) {
				//printf("DEBUG.classe_gfx_sdl::converte_cores - trocou cor2, i: %d, j: %d\n", i, j);
				pixels[(j*optimizedImage->w)+i] = cor3;
			} else if (pixels[(j*optimizedImage->w)+i] == SDL_MapRGB(optimizedImage->format, 28, 91, 20)) {
				//printf("DEBUG.classe_gfx_sdl::converte_cores - trocou cor3, i: %d, j: %d\n", i, j);
				pixels[(j*optimizedImage->w)+i] = cor4;
			}
		}
	}
}

// -------------------------------- FUNCOES GENERICAS -------------------------------- //
// image_path: arquivo de imagem; x_pos e y_pos: posição na tela onde vai ir a imagem
// x1, y1, w, h: área da imagem a ser copiada para a tela 
int classe_gfx_sdl::DrawImage(char *image_path, int x_pos, int y_pos, int x1, int y1, int w, int h, int id_general) {
	int i, j;
	Uint32 cor1, cor2, cor3;
	SDL_Surface* optimizedImage = NULL;
	SDL_Surface *image = IMG_Load (image_path);
	//printf("DEBUG - id_general: %d\n", id_general);
	if (!image) {
		printf ("IMG_Load: %s\n", IMG_GetError ());
		return 1;
	}
	optimizedImage = SDL_DisplayFormat(image);
	converte_cores(optimizedImage, id_general);
	//Get the requested pixel return pixels[ ( y * surface->w ) + x ];
	
   // Draws the image on the screen:
	SDL_Rect rcDest = {x_pos, y_pos, 0, 0};
	SDL_Rect rcSrc = {x1, y1, w, h};
	SDL_BlitSurface(optimizedImage, &rcSrc, offscreen, &rcDest);
	SDL_FreeSurface(image);
	SDL_FreeSurface(optimizedImage);
	SDL_UpdateRect(offscreen, x_pos, y_pos, w, h);
	return 0;
}




void classe_gfx_sdl::carrega_graficos_tropa(char *img_file, struct st_sprites *lista_sprites[2][8], int id_general, int img_size, int n_quadros) {
	SDL_Surface* optimizedImage = NULL;
	SDL_Surface *image = IMG_Load (img_file);
	int i, j;

	
	//printf("DEBUG.classe_gfx_sdl::carrega_graficos_tropa - CARREGANDO GRAFICOS - img_file: %s, general: %d, img_size: %d, n_quadros: %d\n", img_file, id_general, img_size, n_quadros);
	if (!image) {
		printf ("IMG_Load: %s\n", IMG_GetError ());
		return;
	}
	optimizedImage = SDL_DisplayFormat(image);
	//Convert the pixels to 32 bit 
	converte_cores(optimizedImage, id_general);
	// 1 - N
	for (j=1; j<=7; j=j+2) {
		for (i=0; i<n_quadros; i++) {
			SDL_Rect rcDest = {0, 0, img_size, img_size};
			SDL_Surface* area = SDL_CreateRGBSurface (SDL_HWSURFACE , img_size, img_size, 16, 0, 0, 0, 0);
			SDL_FillRect(area, &rcDest, SDL_MapRGB(screen->format, 0, 15, 0));
			SDL_SetColorKey(area, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0, 15, 0));
			// ANDANDO
			SDL_Rect rcSrc = {0, i*img_size, img_size, img_size};
			SDL_BlitSurface(optimizedImage, &rcSrc, area, &rcDest);
			if (j == 5 || j == 7) {
				area = rotozoomSurfaceXY(area, 180, 1, 1, 0);
			}
			//printf("DEBUG - adicionando sprite %d x %d da imagem %s\n", j, i, img_file);
			adiciona_sprite(lista_sprites, 0, j, area);

			// COMBATENDO - SPRITE UM É IGUAL AO SPRITE PARADO
			SDL_Surface* area2 = SDL_CreateRGBSurface (SDL_HWSURFACE , img_size, img_size, 16, 0, 0, 0, 0);
			SDL_FillRect(area2, &rcDest, SDL_MapRGB(screen->format, 0, 15, 0));
			SDL_SetColorKey(area2, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0, 15, 0));
			//printf("DEBUG.carrega_graficos_tropa - ADICIONANDO SPRITE %d em [1][%d]\n", i, j);
			if (i == 0) {
				SDL_Rect rcSrc = {0, i*img_size, img_size, img_size};
				SDL_BlitSurface(optimizedImage, &rcSrc, area2, &rcDest);
			} else {
				//printf("DEBUG.carrega_graficos_tropa - %d carregando área x: %d, y: %d, w: %d, h: %d\n", i, 0, ((i)*img_size)+(img_size*(n_quadros-1)), img_size, img_size);
				SDL_Rect rcSrc = {0, ((i)*img_size)+(img_size*(n_quadros-1)), img_size, img_size};
				SDL_BlitSurface(optimizedImage, &rcSrc, area2, &rcDest);
			}
			if (j == 5 || j == 7) {
				area2 = rotozoomSurfaceXY(area2, 180, 1, 1, 0);
			}
			adiciona_sprite(lista_sprites, 1, j, area2);
		}
	}
	if (lista_sprites[0][1] == NULL) {
		printf("DEBUG.carrega_graficos_tropa - não carregou gráficos MOVE em adiciona_sprite\n");
		exit(-1);
	}
	if (lista_sprites[1][1] == NULL) {
		printf("DEBUG.carrega_graficos_tropa - não carregou gráficos ATTACK em adiciona_sprite\n");
		exit(-1);
	}
	//paused = 1;
	//SDL_FreeSurface(image);
	//SDL_FreeSurface(optimizedImage);
}



void classe_gfx_sdl::adiciona_sprite(struct st_sprites *lista_sprites[2][8], int tipo, int direcao, SDL_Surface *image) {
	struct st_sprites *novo, *aux1 = NULL;
	//novo=(struct st_sprites*)malloc(sizeof(struct st_sprites));
	novo = new struct st_sprites;
	novo->sprite = image;
	novo->next = NULL;
	//printf("DEBUG - classe_gfx_sdl::adiciona_sprite, tipo: %d, direcao: %d\n", tipo, direcao);
	if (lista_sprites[tipo][direcao] != NULL) {
		// vai para o fim da lista
		aux1 = lista_sprites[tipo][direcao];
		while (aux1->next) {
			aux1 = aux1->next;
		}
		aux1->next = novo;
	} else {
		//printf("DEBUG.classe_gfx_sdl::adiciona_sprite - NOVA LISTA DE SPRITES\n");
		lista_sprites[tipo][direcao] = novo;
	}
	if (lista_sprites == NULL) {
		printf("DEBUG.classe_gfx_sdl::adiciona_sprite - ERRO, lista de sprites vazia sendo retornada\n");
	}
}

int classe_gfx_sdl::numero_sprites(struct st_sprites *lista_sprites) {
	struct st_sprites *aux1 = NULL;
	int n = 0;
	// vai para o fim da lista
	if (lista_sprites != NULL) {
		aux1 = lista_sprites;
		while (aux1) {
			aux1 = aux1->next;
			n++;
		}
	} else {
		return 0;
	}
	return n;
}


struct st_sprites* classe_gfx_sdl::sprite_n(struct st_sprites *lista_sprites[2][8], int tipo, int direcao, int i) {
	struct st_sprites *aux1 = NULL;
	int n = 0;
	// vai para o fim da lista
	if (lista_sprites[tipo][direcao] != NULL) {
		aux1 = lista_sprites[tipo][direcao];
		while (aux1 && n < i) {
			aux1 = aux1->next;
			n++;
		}
	} else {
		return NULL;
	}
	return aux1;
}



void classe_gfx_sdl::desenha_fundo(int w, int h) {
	int i, j;
	//printf("DEBUG - desenhando fundo\n");
	SDL_Surface *image = IMG_Load ("./graficos/summer.png");
	if (!image) {
		printf ("IMG_Load: %s\n", IMG_GetError ());
		return;
	}
   // Draws the image on the screen:
	SDL_Rect rcSrc = {0, 32, 32, 32};
	for (i=0; i<w; i+=32) {
		for (j=0; j<h; j+=32) {
			SDL_Rect rcDest = {i, j, 32, 32};
			SDL_BlitSurface(image, &rcSrc, offscreen, &rcDest);
		}
	}
	SDL_FreeSurface(image);
	SDL_UpdateRect(offscreen, 0,0,0,0);
}


// -------------------------------- FUNCOES DE BASE -------------------------------- //
void classe_gfx_sdl::desenha_predios(classe_general *general) {
	classe_predio *aux;
	aux = general->lista_predios;
	while (aux != NULL) {
		if (aux->drawn == 0) {
			//printf("DESENHANDO PREDIO %s\n", aux->tipo);
			desenha_predio(aux);
			aux->drawn = 1;
		}
		aux = aux->next;
	}
}


void classe_gfx_sdl::desenha_predio(classe_predio *aux) {
	SDL_Rect src, dest;
	char *arquivo;
	int tamanho;
	//printf("DEBUG - desenhando predio %s em %d, %d\n", aux->tipo, aux->x, aux->y);
	// desenha o sprite
	// TODO - mostrar construindo:
	//DrawImage("./graficos/buildings/town_hall.png", aux->x, aux->y, 0, 128, 128, 256);
	DrawImage(aux->arquivo, aux->x, aux->y, aux->gfx_pos.x, aux->gfx_pos.y, aux->tamanho, aux->tamanho, aux->general->id);
}




void classe_gfx_sdl::desenha_tropas(classe_general *general) {
	classe_tropa *aux;
	int estado;
	aux = general->get_tropas();
	while (aux) {
		//printf("DEBUG.classe_gfx_sdl::desenha_tropas - estado-tropa: %d\n", aux->estado);
		if (aux->estado == ESTADO_REARMANDO_TROPA || aux->estado == ESTADO_REARMANDO_PREDIO || aux->estado == ESTADO_CONSTRUINDO) {
			//printf("DEBUG.classe_gfx_sdl::desenha_tropas - estado 1\n");
			estado = 1;
		} else {
			//printf("DEBUG.classe_gfx_sdl::desenha_tropas - estado 0\n");
			estado = 0;
		}
		//printf("DEBUG - desenhando tropa\n");
		if (aux->lista_sprites[estado][aux->direcao] == NULL) {
			if (aux->tipo == "CATAPULTA") {
				//aux->lista_sprites = lista_sprites_CATAPULTA[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_CATAPULTA[aux->general->id]);
			} else if (aux->tipo == "PEAO") {
				//aux->lista_sprites = lista_sprites_PEAO[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_PEAO[aux->general->id]);
			} else if (aux->tipo == "GUERREIROS") {
				//aux->lista_sprites = lista_sprites_GUERREIROS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_GUERREIROS[aux->general->id]);
			} else if (aux->tipo == "ARQUEIROS") {
				//aux->lista_sprites = lista_sprites_ARQUEIROS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_ARQUEIROS[aux->general->id]);
			} else if (aux->tipo == "CAVALEIROS") {
				//aux->lista_sprites = lista_sprites_CAVALEIROS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_CAVALEIROS[aux->general->id]);
			} else if (aux->tipo == "PALADINOS") {
				//aux->lista_sprites = lista_sprites_PALADINOS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_PALADINOS[aux->general->id]);
			} else if (aux->tipo == "MAGOS") {
				//aux->lista_sprites = lista_sprites_MAGOS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_MAGOS[aux->general->id]);
			} else if (aux->tipo == "GRIFO") {
				//aux->lista_sprites = lista_sprites_MAGOS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_GRIFOS[aux->general->id]);
			} else if (aux->tipo == "ESQUADRAO DE DEMOLICAO") {
				//aux->lista_sprites = lista_sprites_MAGOS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_ESQUADRAO_DEMOLICAO[aux->general->id]);
			} else if (aux->tipo == "MAQUINA VOADORA") {
				//aux->lista_sprites = lista_sprites_MAGOS[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_MAQUINA_VOADORA[aux->general->id]);
			} else {
				//aux->lista_sprites = lista_sprites_PEAO[aux->general->id];
				copia_vetor_graficos(aux->lista_sprites, lista_sprites_PEAO[aux->general->id]);
			}
			//carrega_graficos_tropa(aux->gfx_file, aux->lista_sprites, aux->general->id, aux->gfx_dest.w, aux->n_quadros_anim);
		}
		if (aux->lista_sprites[estado][aux->direcao] == NULL) {
			printf("ERRO: Carregou gráfico mas este não existe ainda\n");
			exit(-1);
		}
		// copia para a tela
		SDL_Rect rcDest = {aux->getPos().x, aux->getPos().y, aux->gfx_dest.w, aux->gfx_dest.h};
		SDL_Rect rcSrc = {0, 0, aux->gfx_dest.w, aux->gfx_dest.h};
		//printf("DEBUG - desenhando sprite de tropa %d\n", aux->lista_sprites_pos[estado][0]); 
		SDL_BlitSurface(sprite_n(aux->lista_sprites, estado, aux->direcao, aux->lista_sprites_pos[estado][0])->sprite, &rcSrc, offscreen, &rcDest);
		int n_sprites = numero_sprites(aux->lista_sprites[0][aux->direcao]);
		//printf("DEBUG.classe_gfx_sdl::desenha_tropas - n_sprites: %d, pos_sprite: %d\n", n_sprites, aux->lista_sprites_pos[estado][0]);
		if (aux->lista_sprites_pos[estado][0]+1 < n_sprites) {
			aux->lista_sprites_pos[estado][0] = aux->lista_sprites_pos[estado][0]+1;
		} else {
			aux->lista_sprites_pos[estado][0] = 0;
		}
		if (aux->old_x != -1) {
			//printf("DEBUG - atualizando posicao da tropa\n");
			aux->old_x = aux->getPos().x;
			aux->old_y = aux->getPos().y;
		}
		aux = aux->next;
	}
}


void classe_gfx_sdl::apagar_tropas(classe_general *general) {
	classe_tropa *aux;
	aux = general->get_tropas();
	while (aux) {
		// restaura o backup da tela na posição velha
		if (aux->old_x != -1) {
			//printf("Apagar_tropas - Restaurando fundo, old_x: %d\n", aux->old_x);
			copy_area(aux->fundo, 0, 0, aux->gfx_dest.w, aux->gfx_dest.h, aux->old_x, aux->old_y, offscreen);
		}
		aux = aux->next;
	}
}

void classe_gfx_sdl::fundo_tropas(classe_general *general) {
	classe_tropa *aux;
	//struct st_sprites *aux_sp;
	aux = general->get_tropas();
	while (aux) {
		//printf("Copiando fundo de tropa\n");
		copy_area(offscreen, aux->getPos().x, aux->getPos().y, aux->gfx_dest.w, aux->gfx_dest.h, 0, 0, aux->fundo);
		if (aux->old_x == -1) {
			aux->old_x = 0;
		}
		aux = aux->next;
	}
}

void classe_gfx_sdl::desenha_minas(classe_mapa *mapa) {
	SDL_Rect src, dest;
	classe_mina *aux;
	aux = mapa->getMinas();
	while (aux) {
		//printf("DEBUG - desenhando mina em %d, %d, ocupação: %d\n", aux->x, aux->y, aux->ocupacao);
		//printf("DEBUG - desenhando mina livre: %d\n", aux->ocupacao);
		if (aux->tipo == "WOOD") {
			DrawImage("./graficos/buildings/forest.png", aux->getX(), aux->getY(), 0, 0, 96, 96, 0);//96x96
		} else {
			DrawImage("./graficos/buildings/gold_mine.png", aux->getX(), aux->getY(), 0, 0, 32, 32, 0);//96x96
		}
		aux = aux->next;
	}
}


void classe_gfx_sdl::desenha_money_box(classe_general *general, int i) {
	char buffer [80];
	int n;
	int SEPARADOR_TEXTO = 340;
	SDL_Surface *text_surface;
	SDL_Rect text_pos={SEPARADOR_TEXTO*i+5,VIDEO_H+5,0,0};
	SDL_Color font_color={255,255,255};
	SDL_Rect dest = {SEPARADOR_TEXTO*i, VIDEO_H+2, 600, 20};
	//n = sprintf(buffer, "%s - ouro: %d madeira: %d", general->get_nome(), general->get_gold(), general->get_wood(), i);
	n = sprintf(buffer, "ID:%d_OURO:%d_MADEIRA:%d,_TD:%d,_TP:_%d,_PD:_%d,_PP:_%d", general->id, general->get_gold(), general->get_wood(), i, general->contagem_tropas_destruidas, general->contagem_predios_destruidos, general->contagem_tropas_perdidas, general->contagem_predios_perdidos);
	//printf("texto: %s\n", buffer);
	SDL_FillRect(screen, &dest, SDL_MapRGB(offscreen->format, 0, 0, 0));
	if(!(text_surface=TTF_RenderText_Solid(font, buffer, font_color))) {
        //handle error here, perhaps print TTF_GetError at least
		printf("ERRO: Não foi possível renderizar o texto\n");
	} else {
		//printf("Fonte carregada ok\n");
		SDL_BlitSurface(text_surface, 0, screen, &text_pos);
		SDL_UpdateRect(screen, SEPARADOR_TEXTO*i, VIDEO_H-50, 200, 24);
	}
}
