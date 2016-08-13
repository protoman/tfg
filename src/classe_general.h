

class classe_general {
	public:
	// VÁRIÁVEIS PÚBLICAS
	struct st_lista_objetivos *lista_objetivos;
	classe_general *next;
	classe_predio *lista_predios;				// lista de bases deste general
	// VINDAS DA CLASSE PAI
	classe_tropa *lista_tropas;				// lista de bases deste general
	classe_mapa *mapa;
	classe_mina *lista_minas_encontradas;
	classe_general *lista_generais;
	int main_gold, main_wood;
	char *nome;
	int fila_criar_peao;
	int id;
	int derrotado;
	int agressividade;
	struct st_desejo *lista_desejos;
	struct st_contador contador_pouco_ouro;
	struct st_contador contador_tropas_ociosas;
	struct st_contador contador_derrota;
	int contagem_tropas_destruidas, contagem_predios_destruidos, contagem_tropas_perdidas, contagem_predios_perdidos;

	// FUNÇÕES PÚBLICAS
	classe_general(char *nome_general, int pos_x, int pos_y, int agress);		// construtor
	void define_objetivos();
	void novo_plano();
	void remove_objetivo(char *objetivo);
	void ordem_minerar();
	void executa_objetivo(void);
	int distancia_objetivo(char *objtv);
	int tem_objetivo(char *objtv);
	void agir();
	void adiciona_objetivo(char *objetivo, int inicio);
	int temPlano(char *nomePlano);
	int temPredio(char *nomePlano);
	int enviarOrdem(char *ordem);
	struct st_objetivos* objetivos_possiveis(int modo);
	struct st_lista_objetivos *lista_objetivos_possiveis;
	void define_pesos_objetivos(void);
	st_lista_objetivos* maior_objetivo(int modo);
	void lista_plano();
	int peaoOrdem(char *ordem);
	char* get_nome();
	int get_gold();
	int get_wood();
	int dependentes_objetivo(char *nome);
	
	// FUNCOES DE MAPA
	classe_mapa* get_mapa();
	void set_mapa(classe_mapa *mapa);

	// ----- FUNCOES DE PEAO ----- //
	void criar_peao(int x, int y);
	int busca_n_peoes(int estado);
	classe_tropa* busca_peao(int estado);

	// ----- FUNCOES DE PREDIOS ----- //
	classe_predio *get_predio_tipo(char *tipo);

	
	
	classe_tropa *get_tropas();

	// ----- FUNCOES DE MINA ----- //
	classe_mina* buscar_mina_livre(char *kind);
	void visao();
	void radar(int x, int y, int raio);
	int descobre_mina(classe_mina *mina);
	
	// VINDAS DA CLASSE PAI
	classe_base* get_lista_bases();
	classe_mina* get_lista_minas();
	void inc_gold(int x);

	// FUNCOES DE OBJETIVO
	st_objetivos *busca_objetivo(char *nome);
	classe_general* get_lista_generais();
	// COMBATE
	void alerta(void *alvo, char *tipo_alvo, char *tipo);
	int n_tropas_livres();
	
	int n_objetivo_lista(char *objtv);
	void construir_tropas();


	void adiciona_desejo(char *nome_desejo);
	struct coordenada define_canto_oposto();

};




classe_general::classe_general(char *nome_general, int pos_x, int pos_y, int agress) {
	int i;
	// CLASSE PAI
	lista_minas_encontradas = NULL;
	lista_tropas = NULL;
	lista_generais = NULL;
	nome = NULL;
	
	id = 0;
	next = NULL;
	main_gold = 1500;
	main_wood = 1000;
	agressividade = agress;
	derrotado = 0;
	contagem_tropas_destruidas=0;
	contagem_predios_destruidos=0;
	contagem_tropas_perdidas=0;
	contagem_predios_perdidos=0;
	// PREDIOS E TROPAS
	nome = nome_general;

	lista_minas_encontradas = NULL;
	lista_objetivos = NULL;
	lista_objetivos_possiveis = NULL;
	lista_tropas = NULL;
	lista_predios = NULL;
	lista_desejos = NULL;
	//printf("General acordou!\n");
	// cria um peão de brinde para estes poderem criar a primeira base
	criar_peao(pos_x, pos_y);
	//printf("DEBUG - definindo objetivos para general %s\n", nome);
	define_objetivos();
}






classe_mina* classe_general::get_lista_minas() {
	return lista_minas_encontradas;
}


void classe_general::inc_gold(int x) {
	main_gold+=x;
}

classe_general* classe_general::get_lista_generais() {
	return lista_generais;
}


st_objetivos *classe_general::busca_objetivo(char *nome) {
	struct st_objetivos *aux;
	aux = objetivos;
	if (aux) {
		// vai para o fim da lista
		while (aux) {
			//if (nome == "MAQUINA VOADORA") { printf("BUSCANDO %s em %s\n", nome, aux->nome); }
			if (aux->nome == nome) {
				return aux;
			}
			aux = aux->next;
		}
	}
	return NULL;
}








// aloca e adiciona na lista uma struct de rua
int classe_general::descobre_mina(classe_mina *mina) {
	classe_mina *aux1, *novo;
	//novo=(classe_mina*)malloc(sizeof(classe_mina));
	novo = new classe_mina(mina->tipo);
	novo->x = mina->x;
	novo->y = mina->y;
	novo->next = NULL;
	// vai para o fim da lista
	if (lista_minas_encontradas) {
		aux1 = lista_minas_encontradas;
		while (aux1->next) {
			// mina já está na lista
			if (aux1 == mina) {
				delete novo;
				return 0;
			}
			aux1 = aux1->next;
		}
		aux1->next = novo;
	} else {
		lista_minas_encontradas = novo;
	}
	return 1;
}

// procura unidades inimigas ou neutras ainda não conhecidas no raio dado
void classe_general::radar(int x, int y, int raio) {
	int dist, achou;
	classe_mina *aux_m;
	// procura pela lista de minas
	//printf("DEBUG radar - x: %d, y: %d, raio: %d\n", x, y, raio);
	aux_m = mapa->lista_minas;
	while (aux_m) {
		dist = (int) sqrt(pow((float)(x - aux_m->x), 2)+pow((float)(y - aux_m->y), 2));
		//printf("DEBUG radar - dist: %d\n", dist);
		if (dist<raio) {
			if (descobre_mina(aux_m)) {
				//printf("DEBUG - achou mina!\n");
			}
		}
		aux_m = aux_m->next;
	}
	// calcula a distância entre os dois pontos
}

void classe_general::visao() {
	// bases
	classe_predio *aux_b;
	aux_b = lista_predios;
	while (aux_b) {
		if (aux_b->tipo == "TORRE") {
			aux_b->visao();
		} else {
			//printf("DEBUG.classe_general::visao loop1 (PREDIOS), aux_b->tipo: %s\n", aux_b->tipo);
			radar(aux_b->x, aux_b->y, 100);
		}
		aux_b = aux_b->next;
	}
	// tropas
	classe_tropa *aux_t;
	aux_t = lista_tropas;
	while (aux_t) {
		//printf("DEBUG.classe_general::visao tropas\n");
		//radar(aux_t->getPos().x, aux_t->getPos().y, 50);
		aux_t->visao();
		aux_t = aux_t->next;
	}
}


int classe_general::n_tropas_livres() {
	classe_tropa *aux;
	int n = 0;
	if(lista_tropas) {
		aux = lista_tropas;
		while (aux) {
			if (aux->ataque > 1) {
				if (aux->estado == ESTADO_LIVRE) {
					n++;
				}
			}
			aux = aux->next;
		}
	}
	return n;
}

// TODO - se houver tropa livre no canto, escolhe outro
struct coordenada classe_general::define_canto_oposto() {
	classe_mina *mina = buscar_mina_livre("GOLD");
	int dist, dist_tropa;
	int maior = 0, i;
	int tropa_livre;
	struct coordenada cantos[4], maior_canto;
	cantos[0].x = 80;
	cantos[0].y = 80;
	cantos[1].x = mapa->width - 80;
	cantos[1].y = 80;
	cantos[2].x = 80;
	cantos[2].y = mapa->height - 80;
	cantos[3].x = mapa->width - 80;
	cantos[3].y = mapa->height - 80;
	classe_tropa *aux;
	for (i=0; i<4; i++) {
		tropa_livre = 0;
		dist = (int) sqrt(pow((float)(mina->x - cantos[i].x), 2)+pow((float)(mina->y - cantos[i].y), 2));
		if (dist > maior) {
			// verifica se tem alguma tropa livre por perto
			if(lista_tropas) {
				aux = lista_tropas;
				while (aux) {
					if (aux->ataque > 1 && aux->estado == ESTADO_LIVRE) {
						dist_tropa = (int) sqrt(pow((float)(aux->pos.x - cantos[i].x), 2)+pow((float)(aux->pos.y - cantos[i].y), 2));
						if (dist_tropa < 80) {
							tropa_livre = 1;
						}
					}
					aux = aux->next;
				}
			}
			if (tropa_livre==0) {
				maior = dist;
				maior_canto = cantos[i];
			}
		}
	}
	return maior_canto;
}

// envia uma ordem para as tropas
// TODO - dar quantidade de tropas afetadas? (tipo, manda só 10 pra um ataque)
// TODO - identificar unidades para o qual a ordem vai?
// TODO - se tiver base e peões livres, mandar minerar
int classe_general::enviarOrdem(char *ordem) {
	int tempX, tempY;
	st_objetivos *aux_objetivo;
	classe_predio *aux_p;
	// ordem minerar
	// TODO - fazer peões procurarem mina
	//printf("DEBUG.classe_general.enviar_ordem - ordem: %s\n", ordem);
	if (!strncmp("MINERAR", ordem, strlen(ordem)) || !strncmp("DESMATAR", ordem, strlen(ordem))) {
		//printf("DEBUG.classe_general::enviarOrdem - ORDEM %s\n", ordem);
		int estado2;
		int estado1;
		char *tipo_mina;
		if (!strncmp("DESMATAR", ordem, strlen(ordem))) {
			estado2 = ESTADO_RETORNANDO_MADEIRA;
			estado1 = ESTADO_BUSCANDO_MADEIRA;
			tipo_mina = "WOOD";
		} else {
			estado2 = ESTADO_RETORNANDO_OURO;
			estado1 = ESTADO_BUSCANDO_OURO;
			tipo_mina = "GOLD";
		}
		classe_tropa *peao;
		peao = busca_peao(ESTADO_LIVRE);
		if (!peao) {
			// por hora pega a base nº 1
			// TODO - verificar se tem peão minerando
			if ((busca_n_peoes(estado1)+busca_n_peoes(estado2)) <= 1) {
				//printf("Poucos peões para ordem: %s, tipo de mina: %s, %d\n", ordem, tipo_mina, (busca_n_peoes(estado1)+busca_n_peoes(estado2)) <= 1);
				aux_p = get_predio_tipo("BASE");
				if (aux_p) {
					//if (n_objetivo_lista("PEAO") <= 0) {
						//printf("DEBUG CRIAR PEAO 2 - GENERAL %d\n", id);
						fila_criar_peao = 1;
					//}
				} else {
					//TODO - se não tiver mais nenhuma tropa ou prédio, perdeu
					if (lista_predios == NULL && lista_tropas == NULL) {
						derrotado = 1;
						//printf("DEBUG.classe_general::enviarOrdem - GENERAL %d PERDEU!\n", id);
					}
					//printf("DEBUG.classe_general::enviarOrdem - ERRO: Tentando criar peão sem ter base\n");
				}
			}
			return 0;
		} else {
			classe_mina *mina = buscar_mina_livre(tipo_mina);
			// colocar aqui verificação para caso não haja nenhum peao livre, aumenta peso de coonstruir peão
			if (mina) {
				//printf("DEBUG.classe_general::enviarOrdem - peao achou mina tipo %s\n", mina->tipo);
				while (peao) {
					peao->alvo_mina = mina;
					peao->estado = estado1;
					peao = busca_peao(ESTADO_LIVRE);
				}
				return 1;
			} else {
				// TODO - procurar mina
				//printf("General nao conhece nenhuma mina - TODO mandar peao procurar\n");
				return 0;
			}
		}
		// todos os outros são tipo construções
	} else if (!strncmp("ATACAR CANTO", ordem, strlen(ordem))) {
		//printf("DEBUG.classe_general::enviarOrdem(%s)\n", ordem);
		struct coordenada pos_canto = define_canto_oposto();
		// MANDA TROPAS LIVRES ATACAREM CANTO
		classe_tropa *aux_t;
		aux_t = lista_tropas;
		while (aux_t) {
			//printf("DEBUG.classe_general::alerta - ATACANDO verificando estado: %d, ataque: %d\n", aux_t->estado, aux_t->ataque);
			if (aux_t->estado == ESTADO_LIVRE && aux_t->ataque >= 2) {
				aux_t->estado = ESTADO_ATACANDO_LOCAL;
				aux_t->alvo_local = pos_canto;
			}
			aux_t = aux_t->next;
		}
	} else {
		// não tem dinheiro, manda ordem para minerar
		// ORDENS DE TROPAS, ENVIA PARA O PRÉDIO CORRESPONDENTE
		//printf("DEBUG.classe_general::enviarOrdem - ORDEM: %s\n", ordem);
		aux_objetivo = busca_objetivo(ordem);
		if (!aux_objetivo) {
			printf("ERRO GRAVE!! Não achou objetivo %s\n", ordem);
			exit(-1);
		}
		// TODO - pesos para aumenta nº de mineradores
		if (aux_objetivo->custo_ouro > main_gold && (busca_n_peoes(ESTADO_BUSCANDO_OURO)+busca_n_peoes(ESTADO_RETORNANDO_OURO)) <= 1) {
			//printf("GENERAL.enviarOrdem - FALTOU DINHEIRO\n");
			enviarOrdem("MINERAR");
			classe_tropa *peao;
			peao = busca_peao(ESTADO_LIVRE);
			if (!peao) {
				peao = busca_peao(QUALQUER_ESTADO);
			}
			if (!peao) {
				// TRATAR REATIVAMENTE
				//printf("TODO - construir mais peões ou liberar peões minerando\n");
				//printf("DEBUG CRIAR PEAO 3 - GENERAL %d\n", id);
				fila_criar_peao=1;
			}
			return 0;
		} else if (aux_objetivo->custo_madeira > main_wood && (busca_n_peoes(ESTADO_BUSCANDO_MADEIRA)+busca_n_peoes(ESTADO_RETORNANDO_MADEIRA)) <= 1) {
			//printf("GENERAL.enviarOrdem - FALTOU DINHEIRO\n");
			enviarOrdem("DESMATAR");
			classe_tropa *peao;
			peao = busca_peao(ESTADO_LIVRE);
			if (!peao) {
				peao = busca_peao(QUALQUER_ESTADO);
			}
			if (!peao) {
				// TRATAR REATIVAMENTE
				//printf("TODO - construir mais peões ou liberar peões minerando\n");
				//printf("DEBUG CRIAR PEAO 4 - GENERAL %d\n", id);
				fila_criar_peao=1;
			}
			return 0;
		}
		if (ordem == "CATAPULTA") {
			aux_p = get_predio_tipo("FORJA");
			if (!aux_p) {
				//printf("ERRO GRAVE!! General %d tentando criar CATAPULTA sem FORJA\n", id); 
				adiciona_objetivo("FORJA", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "PEAO") {
			aux_p = get_predio_tipo("BASE");
			if (!aux_p) {
				//printf("ERRO GRAVE!! General %d tentando criar PEÃO sem BASE\n", id); 
				adiciona_objetivo("BASE", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "GUERREIROS") {
			aux_p = get_predio_tipo("FORJA");
			if (!aux_p) {
				adiciona_objetivo("FORJA", 1);
				//printf("ERRO GRAVE!! General %d tentando criar GUERREIROS sem FORJA\n", id); 
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "ARQUEIROS") {
			aux_p = get_predio_tipo("SERRARIA");
			if (!aux_p) {
				//printf("ERRO GRAVE!! General %d tentando criar ARQUEIROS sem SERRARIA\n", id); 
				adiciona_objetivo("SERRARIA", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "CAVALEIROS") {
			aux_p = get_predio_tipo("ESTÁBULOS");
			if (!aux_p) {
				//printf("ERRO GRAVE!! General %d tentando criar cavaleiros sem ESTÁBULOS\n", id); 
				adiciona_objetivo("ESTÁBULOS", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "PALADINOS") {
			aux_p = get_predio_tipo("IGREJA");
			if (!aux_p) {
				//printf("ERRO GRAVE!! General %d tentando criar PALADINOS sem IGREJA\n", id); 
				adiciona_objetivo("IGREJA", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "MAGOS") {
			aux_p = get_predio_tipo("TORRE MAGOS");
			if (!aux_p) {
				//printf("ERRO GRAVE!! General %d tentando criar MAGOS sem TORRE MAGOS\n", id); 
				adiciona_objetivo("TORRE MAGOS", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "GRIFO") {
			aux_p = get_predio_tipo("GRIFOÁRIO");
			if (!aux_p) {
				printf("ERRO GRAVE!! General %d tentando criar GRIFO sem GRIFOÁRIO\n", id);
				adiciona_objetivo("GRIFOÁRIO", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "ESQUADRAO DE DEMOLICAO") {
			aux_p = get_predio_tipo("INVENTOR");
			if (!aux_p) {
				printf("ERRO GRAVE!! General %d tentando criar ESQUADRAO DE DEMOLICAO sem INVENTOR\n", id);
				adiciona_objetivo("INVENTOR", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else if (ordem == "MAQUINA VOADORA") {
			aux_p = get_predio_tipo("INVENTOR");
			if (!aux_p) {
				printf("ERRO GRAVE!! General %d tentando criar MAQUINA VOADORA sem INVENTOR\n", id);
				adiciona_objetivo("INVENTOR", 1);
				return 0;
			} else {
				return aux_p->criar_tropa(ordem);
			}
		} else {
			// ORDENS DE CONSTRUÇÃO DE PRÉDIOS
			//printf("DEBUG.classe_general::enviarOrdem - PEÃO INDO CRIAR %s\n", ordem);
			// verifica se já tem alguém com aquela ordem
			if (!peaoOrdem(ordem)) {
				aux_objetivo = busca_objetivo(ordem);
				if (!aux_objetivo) {
					printf("ERRO GRAVE!! Não achou objetivo %s\n", ordem);
					exit(-1);
				}
				// TODO - pesos para aumenta nº de mineradores
				if (aux_objetivo->custo_ouro > main_gold && (busca_n_peoes(ESTADO_BUSCANDO_OURO)+busca_n_peoes(ESTADO_RETORNANDO_OURO)) <= 1) {
					//printf("GENERAL.enviarOrdem - FALTOU DINHEIRO\n");
					enviarOrdem("MINERAR");
					return 0;
				} else if (aux_objetivo->custo_madeira > main_wood && (busca_n_peoes(ESTADO_BUSCANDO_MADEIRA)+busca_n_peoes(ESTADO_RETORNANDO_MADEIRA)) <= 1) {
					//printf("GENERAL.enviarOrdem - FALTOU DINHEIRO\n");
					enviarOrdem("DESMATAR");
					return 0;
				}
				classe_tropa *peao;
				peao = busca_peao(ESTADO_LIVRE);
				if (!peao) {
					peao = busca_peao(QUALQUER_ESTADO);
				}
				if (!peao) {
					// TRATAR REATIVAMENTE
					//printf("TODO - construir mais peões ou liberar peões minerando\n");
					//printf("DEBUG CRIAR PEAO 1 - GENERAL %d\n", id);
					fila_criar_peao=1;
					return 0;
				} else {
					// manda construir base
					peao->estado = ESTADO_MOVENDO;
					// procura um lugar livre por perto
					//printf("peao->alvo.x: %d, peao->alvo.y: %d\n", peao->alvo.x, peao->alvo.y); 
					// TODO - deixar por conta do peão descobrir o melhor lugar para construir
					// DESCOBRE SE X OU Y ESTÁ MAIS PERTO DE ESTAR LIVRE PARA CONSTRUIR
					peao->alvo.y = -1;
					peao->alvo.x = -1;
					//printf("peao->alvo.x: %d, peao->alvo.y: %d\n", peao->alvo.x, peao->alvo.y);
					peao->ordem = ordem;
					return 1;
				}
			}
		}
	}
	// conseguiu alguém para cumprar a ordem
	return 1;
}

struct st_objetivos* classe_general::objetivos_possiveis(int modo) {
	int i, inclui;
	struct st_objetivos *aux1;
	struct st_lista_objetivos *aux2=NULL, *novo;
	aux1 = objetivos;
	while (aux1) {
		// MODO ZERO - PLANEJAMENTO PRÉ-JOGO
		if (modo == 0) {
			// VERIFICA SE O PLANO É POSSÍVEL, ISTO É, SE POSSUI TODOS OS PRÉ-REQUISITOS
			//printf("DEBUG 5 - verificando %s\n", aux1->nome);
			inclui = 1;
			for (i=0; i<3 && aux1->requisitos[i]; i++) {
				if (!temPlano(aux1->requisitos[i])) {
					inclui = 0;
				}
			}
			if (inclui) {
				//printf("POSSIVEL - aux1->nome: %s\n", aux1->nome);
				//novo =(struct st_lista_objetivos*)malloc(sizeof(struct st_lista_objetivos));
				novo = new struct st_lista_objetivos;
				novo->objetivo = aux1->nome;
				novo->nivel = aux1->nivel;
				novo->tropa = aux1->tropa;
				novo->peso = 0;
				novo->next = NULL;
				if (lista_objetivos_possiveis == NULL) {
					lista_objetivos_possiveis = novo;
				} else {
					aux2 = lista_objetivos_possiveis;
					while (aux2->next) {
						//printf("loop 3\n");
						aux2 = aux2->next;
					}
					aux2->next = novo;
				}
			} else {
				//printf("NÃO-POSSIVEL - aux1->nome: %s\n", aux1->nome);
			}
			aux1 = aux1->next;
		} else {
			// VERIFICA SE O PLANO É POSSÍVEL, ISTO É, SE POSSUI TODOS OS PRÉ-REQUISITOS
			//printf("DEBUG 5 - verificando %s\n", aux1->nome);
			inclui = 1;
			for (i=0; i<3 && aux1->requisitos[i]; i++) {
				if (!temPredio(aux1->requisitos[i])) {
					inclui = 0;
				}
			}
			if (temPredio(aux1->nome)) {
				inclui = 0;
			}
			if (inclui) {
				//printf("NOVO PLANO - POSSIVEL - aux1->nome: %s\n", aux1->nome);
				//novo =(struct st_lista_objetivos*)malloc(sizeof(struct st_lista_objetivos));
				novo = new struct st_lista_objetivos;
				novo->objetivo = aux1->nome;
				novo->nivel = aux1->nivel;
				novo->tropa = aux1->tropa;
				novo->peso = 0;
				novo->next = NULL;
				if (lista_objetivos_possiveis == NULL) {
					lista_objetivos_possiveis = novo;
				} else {
					aux2 = lista_objetivos_possiveis;
					while (aux2->next) {
						//printf("loop 3\n");
						aux2 = aux2->next;
					}
					aux2->next = novo;
				}
			} else {
				//printf("NÃO-POSSIVEL - aux1->nome: %s\n", aux1->nome);
			}
			aux1 = aux1->next;
		}
	}
}


int classe_general::dependentes_objetivo(char *nome) {
	int i, inclui;
	struct st_objetivos *aux1;
	aux1 = objetivos;
	int n = 0;
	while (aux1) {
		for (i=0; i<3 && aux1->requisitos[i]; i++) {
			if (aux1->requisitos[i] == nome) {
				inclui = 0;
			}
		}
		aux1 = aux1->next;
	}
	return n;
}


void classe_general::define_pesos_objetivos() {
	struct st_lista_objetivos *aux;
	aux = lista_objetivos_possiveis;
	while (aux) {
		aux->peso = 0;
		// 1. nível de profundidade na árvore
		aux->peso += aux->nivel;
		// 2. número de construções de disponibiliza (que tem requisito dele)
		aux->peso += dependentes_objetivo(aux->objetivo);
		// 3. nível de agressividade
		if (agressividade < 0 && aux->objetivo == "TORRE") {
			aux->peso += (agressividade*-1);
		} else if (agressividade > 0 && (aux->objetivo == "GUERREIROS" || aux->objetivo == "ARQUEIROS" || aux->objetivo == "CATAPULTA")) {
			aux->peso += agressividade;
		}
		//printf("DEBUG.classe_general::define_pesos_objetivos() - aux->objetivo: %s, aux->peso: %d\n", aux->objetivo, aux->peso);
		aux = aux->next;
	}
	//printf("saindo função define_pesos_objetivos()\n");
}


void classe_general::lista_plano() {
	struct st_lista_objetivos *aux;
	int i=1;
	aux = lista_objetivos;
	while (aux) {
		printf("Plano %d: %s\n", i, aux->objetivo);
		aux = aux->next;
		i++;
	}
}


// conta quantas vezes o objetivo já está na lista
int classe_general::n_objetivo_lista(char *objtv) {
	int n=0;
	struct st_lista_objetivos *aux;
	aux = lista_objetivos;
	while (aux) {
		//printf("DEBUG.classe_general::distancia_objetivo - LOOP\n");
		//printf("Buscando %s, achou %s\n", objtv, aux->objetivo);
		if (aux->objetivo == objtv) {
			//printf("ACHOU %s NA LISTA\n", aux->objetivo);
			n++;
		}
		aux = aux->next;
	}
	//printf("Nº DE OCORRÊNCIAS DE %s: %d\n", objtv, n);
	return n;
}


// calcula quando foi a última vez q um dado objetivo apareceu, do fim para o início
int classe_general::distancia_objetivo(char *objtv) {
	int n=0;
	struct st_lista_objetivos *aux;
	aux = lista_objetivos;
	while (aux) {
		//printf("DEBUG.classe_general::distancia_objetivo - LOOP\n");
		if (aux->objetivo == objtv) {
			//printf("ACHOU %s NA LISTA\n", aux->objetivo);
			n = 0;
		} else {
			n++;
		}
		aux = aux->next;
	}
	//printf("DISTANCIA DE %s: %d\n", objtv, n);
	return n;
}

st_lista_objetivos* classe_general::maior_objetivo(int modo) {
	struct st_lista_objetivos *aux;
	int maior = -1;
	st_lista_objetivos *maior_objetivo=NULL;
	aux = lista_objetivos_possiveis;
	while (aux) {
		// verifica peso e impede repetição de plano
		// TODO - não pode impedir repetição de tropas/unidades, se não for 2 vezes seguidas
		//printf("DEBUG.classe_general::maior_objetivo - aux->objetivo: %s, aux->tropa: %d, distancia: %d, abs(agressividade): %d\n", aux->objetivo, aux->tropa, distancia_objetivo(aux->objetivo), 11-abs(agressividade));
		if (aux->peso > maior && ((aux->tropa != 1 && modo == 0) || modo == 1) && !temPlano(aux->objetivo)) {
			//if (aux->peso > maior && !temPlano(aux->objetivo)) {
			//printf("objetivo: %s, templano: %d\n", aux->objetivo, temPlano(aux->objetivo));
			maior = aux->peso;
			maior_objetivo = aux;
		} else if (agressividade > 0 && aux->peso > maior && aux->tropa == 1 && distancia_objetivo(aux->objetivo)>= 11-abs(agressividade) && n_objetivo_lista(aux->objetivo)<=abs(agressividade)) {
			//printf("DEBUG 3\n");
			maior = aux->peso;
			maior_objetivo = aux;
		} else if (agressividade < 0 && aux->peso > maior && aux->objetivo == "TORRE" && distancia_objetivo(aux->objetivo)>= 11-abs(agressividade) && n_objetivo_lista(aux->objetivo)<=abs(agressividade)) {
			//printf("DEBUG 4\n");
			maior = aux->peso;
			maior_objetivo = aux;
		}
		aux = aux->next;
	}
	if (maior_objetivo != NULL) {
		//printf("Maior_objetivo: %s\n", maior_objetivo->objetivo);
		return maior_objetivo;
	} else {
		//printf("ERRO: Maior objetivo não encontrado 2\n");
		return NULL;
	}
}


void classe_general::adiciona_objetivo(char *objetivo, int inicio) {
	//printf("DEBUG - adicionando objetivo: %s\n", objetivo);
	struct st_lista_objetivos *novo, *aux1;
	// se já está no iníciuo da lista, não precisa adicionar de novo
	if (lista_objetivos != NULL && lista_objetivos->objetivo == objetivo) {
		return;
	}
	//novo =(struct st_lista_objetivos*)malloc(sizeof(struct st_lista_objetivos));
	novo = new struct st_lista_objetivos;
	novo->objetivo = objetivo;
	novo->next = NULL;
	if (inicio) {
		aux1 = lista_objetivos;
		novo->next = aux1;
		lista_objetivos = novo;
	} else {
		// vai para o fim da lista
		if (lista_objetivos) {
			aux1 = lista_objetivos;
			while (aux1->next) {
				aux1 = aux1->next;
			}
			aux1->next = novo;
		} else {
			lista_objetivos = novo;
		}
	}
}

int classe_general::tem_objetivo(char *objtv) {
	struct st_lista_objetivos *aux1;
	aux1 = lista_objetivos;
	while (aux1) {
		if (aux1->objetivo == objtv) {
			return 1;
		}
		aux1 = aux1->next;
	}
	return 0;
}





// TODO - tirar do buffer de completos tb
void classe_general::remove_objetivo(char *objetivo) {
	struct st_lista_objetivos *aux1, *aux2;
	int fim1=0;
	aux1 = lista_objetivos;
	//printf("3. Removendo objetivo %s, general: %d\n", objetivo, id);
	if (aux1) {
		//printf("INICIO: %s\n", lista_objetivos->objetivo);
		if (lista_objetivos->objetivo == objetivo) {
			//printf("DO INICIO DA LISTA\n");
			aux2 = lista_objetivos;
			lista_objetivos = lista_objetivos->next;
		} else {
			while (aux1->next && fim1 == 0) {
				//printf("DEBUG - general: %d, aux1->next->objetivo: %s\n", id, aux1->next->objetivo);
				if (aux1->next->objetivo == objetivo) {
					//printf("DE DENTRO DA LISTA\n");
					aux2 = aux1;
					aux1->next = aux1->next->next;
					aux1 = aux1->next;
					// sai do loop para não remover as repetidas depois
					fim1 = 1;
				}
				aux1 = aux1->next;
			}
		}
	} else {
		printf("DEBUG.classe_general::remove_objetivo - lista de objetivos vazia\n");
	}
}


// verifica se um predio está na lista ou não
int classe_general::temPredio(char *nomePlano) {
	int res;
	classe_predio *aux_p = lista_predios;
	while (aux_p) {
		if (aux_p->tipo == nomePlano) {
			return 1;
		}
		aux_p = aux_p->next;
	}
	return 0;
}


// verifica se um plano está na lista ou não
int classe_general::temPlano(char *nomePlano) {
	int res;
	struct st_lista_objetivos *aux1;
	if (lista_objetivos) {
		aux1 = lista_objetivos;
		while (aux1) {
			res = strncmp(nomePlano, aux1->objetivo, strlen(nomePlano));
			if (res == 0) {
				//printf("Já tem %s\n", nomePlano);
				return 1;
			}
			aux1 = aux1->next;
		}
		return 0;
	} else {
		return 0;
	}
}

// OBS: pegar ouro/madeira vão funcionar de mandeira reativa
void classe_general::define_objetivos(void) {
	int i, fim=0;
	st_lista_objetivos *maior_obj;
	objetivos_possiveis(0);
	// TODO - trocar por quando chegar em uma tarefa de nível máximo
	while (fim ==0) {
		// não tem base, prioridade #1
		define_pesos_objetivos();
		maior_obj = maior_objetivo(0);
		if (maior_obj) {
			//printf("Adicionando objetivo maior: %s de nível: %d\n", maior_obj->objetivo, maior_obj->nivel);
			adiciona_objetivo(maior_obj->objetivo, 0);
			if (maior_obj->nivel < 6) {
				objetivos_possiveis(0);
			} else {
				fim = 1;
			}
		} else {
			//printf("ERRO: Maior objetivo não encontrado 1\n");
			fim = 1;
		}
	}
	//lista_plano();
	// não achou nada, manda peões minerarem, ou cria peões se não tiver
	//executa_objetivo();
}




// OBS: pegar ouro/madeira vão funcionar de mandeira reativa
void classe_general::novo_plano(void) {
	int i, fim=0, n=0;
	st_lista_objetivos *maior_obj;
	lista_objetivos_possiveis = NULL;
	objetivos_possiveis(1);
	// TODO - trocar por quando chegar em uma tarefa de nível máximo
	while (lista_objetivos_possiveis != NULL && fim ==0) {
		// não tem base, prioridade #1
		define_pesos_objetivos();
		maior_obj = maior_objetivo(1);
		if (maior_obj) {
			printf("DEBUG.classe_general::novo_plano - GENERAL: %d - Adicionando objetivo maior: %s de nível: %d\n", id, maior_obj->objetivo, maior_obj->nivel);
			adiciona_objetivo(maior_obj->objetivo, 0);
			n++;
			if (n > 10) {
				fim = 1;
			}
		} else {
			//printf("ERRO: Maior objetivo não encontrado 1\n");
			fim = 1;
		}
	}
	lista_plano();
	// não achou nada, manda peões minerarem, ou cria peões se não tiver
	//executa_objetivo();
}


void classe_general::executa_objetivo(void) {
	int res;
	// fecha todos os objetivos já completados
	if (lista_objetivos == NULL) {
		printf("DEBUG.classe_general::executa_objetivo - lista de objetivos vazia, criando novo plano\n");
		novo_plano();
		//enviarOrdem("MINERAR");
	} else {
		//printf("DEBUG - enviando ORDEM objetivo: %s\n", lista_objetivos->objetivo);
		if (!lista_objetivos->objetivo && lista_objetivos->next != NULL) {
			lista_objetivos = lista_objetivos->next;
		}
		if (enviarOrdem(lista_objetivos->objetivo) == 0 && lista_objetivos->objetivo != "MINERAR") {
			//printf("ERRO: faltou algo para cumprir objetivo %s\n", lista_objetivos->objetivo);
			// TODO - gerenciar erros de execução
		}
	}
}


void classe_general::agir() {
	int n = 0;
	classe_predio *aux;
	classe_tropa *aux_t;
	classe_predio *aux_p;
	//printf("DEBUG.classe_general::agir - GENERAL %d agindo\n", id);
	if (fila_criar_peao==1) {
		//printf("CRIANDO PEAO\n");
		aux = get_predio_tipo("BASE");
		if (!aux) {
			//printf("NAO TEM BASE\n");
			if (lista_predios == NULL && lista_tropas == NULL) {
				derrotado = 1;
				contador_derrota.time2 = SDL_GetTicks();
				printf("DEBUG.classe_general::agir - 1.GENERAL %d PERDEU! Tempo: %d (seg)\n", id, (contador_derrota.time2-contador_derrota.time1)/1000);
			} else if (lista_predios == NULL) {
				// se não tem mais nenhum peão, não tem o que fazer e não ser chorar
				if (busca_n_peoes(QUALQUER_ESTADO) <= 0) {
					contador_derrota.time2 = SDL_GetTicks();
					printf("DEBUG.classe_general::agir - 2.GENERAL %d PERDEU! Tempo: %d (seg)\n", id, (contador_derrota.time2-contador_derrota.time1)/1000);
					derrotado = 1;
				// tem peão mas não tem grana para criar ele
				} else if ((busca_n_peoes(QUALQUER_ESTADO) > 0) && (main_gold < 120 || main_wood < 80)) {
					contador_derrota.time2 = SDL_GetTicks();
					printf("DEBUG.classe_general::agir - 3.GENERAL %d PERDEU! Tempo: %d (seg)\n", id, (contador_derrota.time2-contador_derrota.time1)/1000);
					derrotado = 1;
				} else {
					//printf("ERRO: Tentando criar peão sem ter base\n");
					// manda peão construir BASE
					adiciona_objetivo("BASE", 1);
				}
			}
			return;
		} else {
			//printf("DEBUG.classe_general::agir - CRIANDO PEAO\n");
			adiciona_objetivo("PEAO", 1);
			//criar_peao(aux->x+50, aux->y+50);
			//enviarOrdem("PEAO");
			fila_criar_peao = 0;
		}
	}
	// TROPAS
	aux_t = lista_tropas;
	while (aux_t) {
		aux_t->agir();
		aux_t = aux_t->next;
	}
	// PREDIOS
	aux_p = lista_predios;
	while (aux_p) {
		aux_p->agir();
		aux_p = aux_p->next;
	}
	//printf("n peoes: %d\n", n);
}

// -------------------------------- FUNCOES DE PEAO -------------------------------- //
// aloca e adiciona a  lista uma struct de rua
void classe_general::criar_peao(int x, int y) {
	//printf("DEBUG - criar_peao() inicio\n");
	classe_tropa *aux;
	classe_tropa *novo = new classe_tropa("PEAO", this);
	novo->pos.x = x;
	novo->pos.y = y;
	if(!lista_tropas) {
		lista_tropas = novo;
	} else {
		aux = lista_tropas;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = novo;
	}
	//printf("DEBUG - criar_peao() fim\n");
}


int classe_general::peaoOrdem(char *ordem) {
	classe_tropa *aux;
	if (lista_tropas) {
		aux = lista_tropas;
		while (aux) {
			if (aux->tipo == "PEAO") {
				if (aux->ordem == ordem) {
					return 1;
				}
			}
			aux = aux->next;
		}
	}
	return 0;
}


// TODO - receber coordenadas para encontrar o peão mais próximo destas
	int classe_general::busca_n_peoes(int estado) {
	classe_tropa *aux;
	int n = 0;
	if (lista_tropas) {
		aux = lista_tropas;
		while (aux) {
			if (aux->tipo == "PEAO") {
				if (estado == QUALQUER_ESTADO || aux->estado == estado) {
					n++;
				}
			}
			aux = aux->next;
		}
	}
	return n;
}


// TODO - receber coordenadas para encontrar o peão mais próximo destas
classe_tropa* classe_general::busca_peao(int estado) {
	classe_tropa *aux;
	if(!lista_tropas) {
		return NULL;
	} else {
		aux = lista_tropas;
		while (aux) {
			if (aux->tipo == "PEAO") {
				if (estado == QUALQUER_ESTADO || aux->estado == estado) {
					return aux;
				}
			}
			aux = aux->next;
		}
		return NULL;
	}
}



// aloca e adiciona na lista uma struct de rua
classe_mina* classe_general::buscar_mina_livre(char *kind) {
	//printf("DEBUG.classe_general::buscar_mina_livre - BUSCANDO MINA TIPO %s\n", kind);
	classe_mina *aux1;
	if (lista_minas_encontradas) {
		aux1 = lista_minas_encontradas;
		while (aux1) {
			//printf("DEBUG.classe_general::buscar_mina_livre - kind: %s, tipo: %s\n", kind, aux1->tipo);
			if (aux1->tipo == kind) {
				return aux1;
			}
			aux1 = aux1->next;
		}
		return NULL;
		// TODO - ver quantidade de peoes minerando nela
	} else {
		//printf("buscar_mina_livre() - nao achou\n");
		return NULL;
	}
}



// -------------------------------- FUNCOES DE PREDIOS -------------------------------- //


classe_predio *classe_general::get_predio_tipo(char *tipo) {
	classe_predio *aux;
	aux = lista_predios;
	while (aux) {
		if (aux->tipo == tipo) {
			return aux;
		}
		aux = aux->next;
	}
}


// -------------------------------- FUNCOES DE TROPAS -------------------------------- //

classe_tropa *classe_general::get_tropas() {
	return lista_tropas;
}



// -------------------------------- FUNCOES DE MAPA -------------------------------- //

classe_mapa *classe_general::get_mapa() {
	return mapa;
}

void classe_general::set_mapa(classe_mapa *map) {
	mapa = map;
}

char* classe_general::get_nome() {
	return nome;
}

int classe_general::get_gold() {
	return main_gold;
}

int classe_general::get_wood() {
	return main_wood;
}


// -------------------------------- FUNCOES DE COMBATE -------------------------------- //



// tipo indica se é atacando ou recebendo ataque, etc
// tipo_alvo indica se alvo é prédio ou tropa, etc
void classe_general::alerta(void *alvo, char *tipo_alvo, char *tipo) {
	//printf("RECEBIDO ALERTA - tipo_alvo: %s, tipo: %s\n", tipo_alvo, tipo);
	classe_tropa *aux_t;
	int n=0;
	if (tipo == "ATACANDO") {
		aux_t = lista_tropas;
		// MANDA TROPAS LIVRES ATACAREM 
		while (aux_t) {
			//printf("DEBUG.classe_general::alerta - ATACANDO verificando estado: %d, ataque: %d\n", aux_t->estado, aux_t->ataque);
			if ((aux_t->estado == ESTADO_LIVRE || aux_t->estado == ESTADO_ATACANDO_LOCAL) && aux_t->ataque >= 2) {
				if (tipo_alvo == "TROPA") {
					aux_t->estado = ESTADO_ATACANDO_TROPA;
					aux_t->alvo_tropa = (classe_tropa *) alvo;
					//printf("DEBUG.classe_general::alerta - enviando tropa para ajudar a atacar %s, %s\n", tipo_alvo, aux_t->alvo_tropa->tipo);
				} else if (tipo_alvo == "PREDIO") {
					aux_t->estado = ESTADO_ATACANDO_PREDIO;
					aux_t->alvo_predio = (classe_predio *) alvo;
					//printf("DEBUG.classe_general::alerta - enviando tropa para ajudar a atacar %s, %s\n", tipo_alvo, aux_t->alvo_tropa->tipo);
				}
			}
			aux_t = aux_t->next;
		}
	} else if (tipo == "RECEBENDO_ATAQUE") {
		//printf("WE ARE UNDER ATTACK!! tipo_alvo: %s\n", tipo_alvo);
		aux_t = lista_tropas;
		n = 0;
		while (aux_t) {
			//printf("DEBUG.classe_general::alerta - RECEBENDO_ATAQUE verificando estado: %d, ataque: %d\n", aux_t->estado, aux_t->ataque);
			if ((aux_t->estado == ESTADO_LIVRE || aux_t->estado == ESTADO_ATACANDO_LOCAL) && aux_t->ataque >= 2) {
				//printf("DEBUG.classe_general::alerta - enviando tropa para ajudar a defender\n");
				n++;
				if (tipo_alvo == "TROPA") {
					aux_t->estado = ESTADO_ATACANDO_TROPA;
					aux_t->alvo_tropa = (classe_tropa *) alvo;
					//printf("DEBUG.classe_general::alerta - enviando tropa para defender ataque de TROPA %s, %s\n", tipo_alvo, aux_t->alvo_tropa->tipo);
				} else if (tipo_alvo == "PREDIO") {
					aux_t->estado = ESTADO_ATACANDO_PREDIO;
					aux_t->alvo_predio = (classe_predio *) alvo;
					//printf("DEBUG.classe_general::alerta - enviando tropa para defender ataque de %s, %s\n", tipo_alvo, aux_t->alvo_predio->tipo);
				}
			}
			// não tinha ninguém para mandar, então construa!
			if (n <= 0) {
				//printf("DEBUG.classe_general::alerta - NENHUMA TROPA, CONSTRUIR\n");
				construir_tropas();
			}
			aux_t = aux_t->next;
		}
	}
}





void classe_general::construir_tropas() {
	// verifica tropas que pode construir, da mais forte para a mais fraca, e se possível, constrói
	struct st_objetivos *aux;
	char *maior;
	int n_maior, requisitos_ok, i;
	classe_predio *aux_p;

	aux = objetivos;
	n_maior = 0;
	maior = NULL;
	while (aux) {
		//printf("DEBUG.classe_general::construir_tropas - aux->tropa: %d, aux->nivel: %d\n", aux->tropa, aux->nivel);
		if (aux->tropa) {
			requisitos_ok = 1;
			for (i=0; i<3; i++) {
				if (aux->requisitos[i] != NULL) {
					aux_p = get_predio_tipo(aux->requisitos[i]);
					if (!aux_p) {
						//printf("Faltou requisito %s\n", aux->requisitos[i]);
						requisitos_ok = 0;
					}
				}
			}
			if ((aux->nivel > n_maior) && (requisitos_ok == 1)) {
				maior = aux->nome;
			}
		}
		aux = aux->next;
	}
	if (maior != NULL) {
		//printf("DEBUG.classe_general::construir_tropas - GENERAL %d CONSTRUINDO TROPA %s\n", id, maior);
		// só constrói se já não for o próximo item da lista de tarefas
		if (lista_objetivos->objetivo != maior) {
			adiciona_objetivo(maior, 1);
		}
	} else {
		//printf("DEBUG.classe_general::construir_tropas - NÃO PODE CONSTRUIR NENHUMA TROPA\n");
	}
}


// -------------------------------- FUNCOES DE DESEJOS -------------------------------- //


// aloca e adiciona na lista uma struct de rua
void classe_general::adiciona_desejo(char *nome_desejo) {
	struct st_desejo *aux1, *novo;
	int achou = 0;
	// vai para o fim da lista
	if (lista_desejos) {
		aux1 = lista_desejos;
		while (aux1) {
			// mina já está na lista
			if (aux1->desejo == nome_desejo) {
				achou = 1;
				aux1->contador = aux1->contador + 1;
				return;
			}
			aux1 = aux1->next;
		}
		// não achou, insere no fim
		if (achou == 0) {
			novo = new struct st_desejo;
			novo->desejo = nome_desejo;
			novo->contador = 0;
			novo->next = NULL;
			aux1->next = novo;
		}
	} else {
		novo = new struct st_desejo;
		novo->desejo = nome_desejo;
		novo->contador = 0;
		novo->next = NULL;
		lista_desejos = novo;
	}
	return;
}
