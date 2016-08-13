class classe_mina {
	public:
	// VÁRIÁVEIS PÚBLICAS
		int ouro, madeira;
		int x, y;
		char *tipo;
		classe_mina *next;
		// ---------- FUNCOES ---------- //
		classe_mina(char *kind);
		int getX();
		int getY();
};

// construtor mina
classe_mina::classe_mina(char *kind) {
	tipo = kind;
	//printf("DEBUG.classe_mina::classe_mina - criando mina tipo %s\n", kind);
	if (tipo == "WOOD") {
		ouro = 0;
		madeira = 10000;
	} else {
		ouro = 10000;
		madeira = 0;
	}
	next = NULL;
}

int classe_mina::getX() {
	return x;
}

int classe_mina::getY() {
	return y;
}
