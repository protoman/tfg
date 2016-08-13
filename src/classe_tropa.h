

class classe_tropa {
	public:
	// VÁRIÁVEIS PÚBLICAS
		struct coordenada pos;
		classe_tropa *next;
		classe_general *general;
		char *tipo;
		char *ordem;
		// COMBATE
		int ataque, defesa, HP, timer_rearme, tempo_rearme, distancia_ataque;
		int contador; // para tarefas que levam um tempo
		char *gfx_file;
		int old_x, old_y;
		int n_quadros_anim;
		SDL_Surface *fundo;
		// GRÁFICOS
		SDL_Rect gfx_dest;
		struct st_sprites *lista_sprites[2][8]; // [0][n] = andando, [1][n] = batendo
		int lista_sprites_pos[2][8];
		int direcao;
		int raio_visao;
		// ESTADO
		int estado;
		classe_tropa *alvo_tropa;
		classe_predio *alvo_predio;
		struct coordenada alvo_local;
		// PEÃO
		int ouro, madeira;
		classe_mina *alvo_mina;
		classe_predio *alvo_base;
		struct coordenada alvo;

		
		// ---------- FUNCOES ---------- //
		classe_tropa(char *nome_tropa, classe_general *tgeneral); // CONSTRUTOR
		~classe_tropa(); // DESTRUTOR
		struct coordenada getPos();
		void visao();
		void agir();
		void ir(int x1, int y1);
		// FUNÇÕES DE PEÃO
		int localLivre(int x, int y);
		void defineLocalConstrucao();
		void criar_predio(char *tipo);
		classe_predio* procuraBaseMaisProxima(void);
		classe_mina* procuraMinaMaisProxima(void);

};

