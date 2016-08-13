class classe_mapa {
	public:
	// VÁRIÁVEIS PÚBLICAS
		int width, height;
		classe_mina *lista_minas;
		classe_general *lista_generais;
		// ---------- FUNCOES ---------- //
		classe_mapa(int w, int h);
		void cria_mina(int x, int y, int o1, char *tipo);
		void desenha_minas();
		classe_mina *getMinas();
};


// construtor mapa
classe_mapa::classe_mapa(int w, int h) {
	width=w;
	height=h;
	lista_minas = NULL;
	lista_generais = NULL;
}

classe_mina* classe_mapa::getMinas() {
	return lista_minas;
}

// aloca e adiciona na lista uma struct de rua
void classe_mapa::cria_mina(int x1, int y1, int o1, char *tipo) {
	//printf("DEBUG - criar_mina() inicio\n");
	classe_mina *aux;
	classe_mina *novo = new classe_mina(tipo);
	novo->x = x1;
	novo->y = y1;
	novo->ouro = o1;
	if(!lista_minas) {
		lista_minas = novo;
	} else {
		aux = lista_minas;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = novo;
	}
	//printf("DEBUG - criar_mina() fim\n");
}





