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
 -------------- URGENTE --------------
- envio de ataque por ociosidade de tropas (pesos e tal) - lista de desejos com contador
- quando n�o tiver ouro ou madeira, enviar mensagem para o general (pesos e tal)
- ir aumentando n� de pe�es minerando progressivamente cfe n� de msgs de falta de ouro/madeira (pesos e tal)
- optimizar um pouco, descobrir gargalos, tentar unificar fun��es que buscam em listas
- fuga
- buscar minas se n�o houver por perto
- grifos

- apagar_predios deve ser imediatamente ap�s apagar_tropas e antes de copiar fundo
- virar para a dire��o para onde est� atacando
- impedir constru��o de pr�dios com parte fora do mapa
- deletar todas as listas ao sair
GR�FICOS
- arrumar ataque torre -> pr�dio
- desenho de etapas de constru��o
- anima��o de morte/corpo
- anima��o de combate
- anima��o de proj�teis
*/

/*
CHANGELOG
- carregar s� uma vez gr�ficos de tropas (de todas elas)
- quando um pr�dio for destru�do, deve reconstru�-lo
- BUG: criando pe�es infinitamente sem mand�-los fazer nada (na verdade ficava parado construindo tropas)
- se for atacado e n�o tiver tropas livres, manda construir
- quando tentar construir uma tropa sem um pr�dio, constr�iu o pr�dio
- MADEIRA
- tempo para constru��o
- gr�fico de balista d� pau no windows
- vis�o e rea��o para: a) pe�o b) tropas (DONE) c) pr�dios
- rea��o quando enxerga: a) mina b) tropa inimiga (DONE) c) pr�dio inimigo
- raio de vis�o como propriedade da tropa/pr�dio
- mensagem de ataque recebido para general
- mensagem de inimigo atacado para general (manda tropas livres atacarem)
- custo de cria��o de tropas e pr�dios
- cria��o de torres por agressividade (negativa)
- fazer TORRES atirarem!
- tempo para criar tropa e pr�dio de acordo com seu n�vel
- tempo para criar tropa
- pe�o n�o pode atacar!!!
- poder construir tropas que j� existem (pr�dio n�o precisa planejar mais de um, tropa sim)
- se n�o tiver nenhum pe�o, mas tiver base, cria pe�o
- dist�ncia de ataque (torres, arqueiros, catapulstas)
- mirror do gr�fico do pe�o d� pau no windows
- transformar pe�o em tropa
- criar classes com new e n�o malloc
- destrutor de classe tropa: apaga gr�fico da tela e elimina-se da lista de tropas do general
- pontos de vida para tropas e pr�dios
- tempo entre ataques
- morte (delete tropa)
- arrumar constru��o de base sobre mina quando estiver embaixo/direita
- pe�o deve verificar novamente se local est� livre na hora que for criar pr�dio
- fazer generais como lista gen�rica de generais e fazer fun��es gfx_sdl usarem a lista inteira 
- cores de generais

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#ifdef WINDOWS
	#include <windows.h>
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_ttf.h>
	#include <SDL/SDL_rotozoom.h>
#else
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <SDL_rotozoom.h>
#endif

#define DEBUG_IA 1
#define VIDEO_W 1024
#define VIDEO_H 768
#define SCROLL_STEP 30
#define SEPARACAO_PREDIOS 128
#define TEMPO_CONSTRUCAO 10

#define QUALQUER_ESTADO -1
#define ESTADO_LIVRE 0
#define ESTADO_ATACANDO_TROPA 1
#define ESTADO_ATACANDO_PREDIO 2
#define ESTADO_FUGINDO 3
#define ESTADO_REARMANDO_TROPA 4
#define ESTADO_REARMANDO_PREDIO 5

#define ESTADO_BUSCANDO_OURO 10
#define ESTADO_RETORNANDO_OURO 11
#define ESTADO_BUSCANDO_MADEIRA 12
#define ESTADO_RETORNANDO_MADEIRA 13
#define ESTADO_MOVENDO 14
#define ESTADO_CONSTRUINDO 15
#define ESTADO_ATACANDO_LOCAL 16

using namespace std;

// fonte padr�o
TTF_Font *font=0;


struct st_objetivos {
	char *nome;
	int tipo;
	char *requisitos[3];
	int cria_tropas, tropa, nivel;
	struct st_objetivos *next;
	int custo_ouro, custo_madeira;
};

struct st_sprites {
	SDL_Surface *sprite;
	struct st_sprites *next;
};

// ---------------------------- VARI�VEIS GLOBAIS ---------------------------------- //
int fim = 0;
int paused = 0;
int scroll_x, scroll_y = 0;
int STEP=5;
int DELAY = 40;

struct st_objetivos *objetivos;


struct coordenada {
	int x, y;
};

struct st_mensagem {
	char *mensagem;
	struct st_mensagem *next;
};

struct st_desejo {
	char *desejo;
	int contador;
	struct st_desejo *next;
};

// defini��o da lista de objetivos
// cada objetivo � igual � mensagem de cria��o do objetivo
struct st_lista_objetivos {
	char *objetivo;
	int peso, nivel, tropa;
	struct st_lista_objetivos *next;
};


struct st_contador {
	int time1, time2, n;
};

// vari�vel tela
SDL_Surface *screen;
// tela completa, n�o totalmente vis�vel (para scrolling)
SDL_Surface *offscreen; 

class classe_peao;
class classe_base;
class classe_tropa;
class classe_mapa;
class classe_mina;
class classe_predio;
class classe_general;

