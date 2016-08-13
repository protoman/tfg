class classe_predio {
	public:
	// VÁRIÁVEIS PÚBLICAS
		int x, y;
		classe_predio *next;
		classe_general *general;
		char *tipo;
		int drawn;
		int HP;
		int contador, estado;
		char *nome_tropa;
		int raio_visao;
		int ataque, timer_rearme, distancia_ataque, tempo_rearme;
		classe_tropa *alvo_tropa;
		classe_predio *alvo_predio;
		char *arquivo;
		int tamanho;
		struct coordenada gfx_pos;
		// ---------- FUNCOES ---------- //
		classe_predio(classe_general *gn, char *tipo);
		~classe_predio(); // DESTRUTOR
		int criar_tropa(char *nome);
		void agir();
		void visao();
};



