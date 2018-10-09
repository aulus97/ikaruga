#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
 
const float FPS = 60;
const int SCREEN_W = 940;
const int SCREEN_H = 540;
//variavel global que armazena o tamanho da nave (quadrado que vai navegar pela tela)
const int NAVE_SIZE = 20;
//variavel global que armazena o tamanho da nave inimiga
const int EN_GR_SIZE = 30;
const int EN_BL_SIZE = 30;

struct coordenada_nave{
		int nave_alive;
	//posicoes x e y iniciais da nave
	    float nave_x;
	    float nave_y;
	   //o quanto as posicoes x e y vao variar ao longo do tempo
	    float nave_dx, nave_dy;
	//movimentacao da nave
		bool nave_move_down;
	    bool nave_move_up;
	    bool nave_move_left;
	    bool nave_move_right;
	    bool nave_color;
	};
struct coordenada_enemygreen{
		int en_gr_alive;
		//int altera_g_speed;
	//posicoes iniciais en_gr:
	    float en_gr_x;
	    float en_gr_y;
	    float en_gr_dx, en_gr_dy;
	};
struct coordenada_enemyblue{
		int en_bl_alive;
		//int altera_b_speed;
	//posicoes iniciais en_bl:
	    float en_bl_x;
	    float en_bl_y;
	    float en_bl_dx, en_bl_dy;
	};

int main(int argc, char **argv){
	   ALLEGRO_DISPLAY *display = NULL;
	   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	   ALLEGRO_TIMER *timer = NULL;
	   ALLEGRO_BITMAP *nave = NULL;
	   /*>>> RESOLVA O PROBLEMA!!
			 https://www.google.com.br/search?q=%27for%27+loop+initial+declarations+are+only+allowed&oq=%27for%27+loop+initial+declarations+are+only+allowed&aqs=chrome..69i57.809j0j1&sourceid=chrome&ie=UTF-8#q=allegro+bitmap+array
			 https://www.allegro.cc/forums/thread/584064/584064#target
			 http://stackoverflow.com/questions/10911033/an-array-of-allegro-bitmap-type-objects
			 http://liballeg.org/stabledocs/en/alleg010.html
			 http://fixbyproximity.com/2011/09/22/2d-game-dev-part-7-2-basic-animation-in-allegro-5/
			 https://metacpan.org/pod/release/COLINO/Allegro-0.02_0-ppm-MSWin32-x86-multi-thread-5.8/lib/Allegro/Bitmap.pod
			 */
	   /*COM MALLOC:
			https://www.google.com.br/search?q=%27for%27+loop+initial+declarations+are+only+allowed&oq=%27for%27+loop+initial+declarations+are+only+allowed&aqs=chrome..69i57.809j0j1&sourceid=chrome&ie=UTF-8#q=allegro+bitmap+array+vs+malloc
			https://groups.google.com/forum/#!topic/comp.os.msdos.djgpp/OTS9H5YA6Lw
			http://www.verycomputer.com/12_9f77e38f2db71924_1.htm
			https://sourceforge.net/p/alleg/mailman/alleg-main/thread/01bd447d$962a7e90$0100a8c0@stefan.schimanski.de/
			*/
	   /*
		AINDA NAO DESENHA? TA BUGANDO?
		https://www.allegro.cc/forums/thread/590734/660848
		http://liballeg.org/stabledocs/en/faq.html <<--
	   */
	   ALLEGRO_BITMAP **en_gr = NULL;
	   ALLEGRO_BITMAP **en_bl = NULL;
	   ALLEGRO_FONT *score_font = NULL;
	   
	void altera_color(int nave_color){
		if(nave_color){
		//avisa o allegro que eu quero modificar as propriedades da nave
			al_set_target_bitmap(nave);
		//altera a cor da nave para AZUL(0,0,255)
			al_clear_to_color(al_map_rgb(0, 0, 255));
			al_set_target_bitmap(al_get_backbuffer(display));//impede o bug (tela toda branca) quando a cor da nave eh alterada
		}
		else if(!nave_color){
			//avisa o allegro que eu quero modificar as propriedades da nave
				al_set_target_bitmap(nave);
			//altera a cor da nave para VERDE(0,255,0)
				al_clear_to_color(al_map_rgb(0, 255, 0));
				al_set_target_bitmap(al_get_backbuffer(display));//impede o bug (tela toda branca) quando a cor da nave eh alterada
		}
	}
	   
		struct coordenada_nave nave_coord;
		
			nave_coord.nave_alive = 1;
		//posicoes x e y iniciais da nave
			nave_coord.nave_x = SCREEN_W / 2.0 - NAVE_SIZE / 2.0;
			nave_coord.nave_y = SCREEN_H / 2.0 - NAVE_SIZE / 2.0;
		   //o quanto as posicoes x e y vao variar ao longo do tempo. OU ALTERA A VELOCIDADE AQUI OU LA EMBAIXO NOS COMANDOS DAS TECLAS!!!
			nave_coord.nave_dx = 10.0; //->meio da tela: 470 - 10=460 (eh divisivel por : 10,20)
			nave_coord.nave_dy = 10.0; //->meio da tela: 270 - 10=260 (eh divisivel por : 10,20)
		//movimentacao da nave
			nave_coord.nave_move_down = false;
			nave_coord.nave_move_up = false;
			nave_coord.nave_move_left = false;
			nave_coord.nave_move_right = false;
			nave_coord.nave_color = false;
			
		struct coordenada_enemygreen en_gr_coord[5];
			int i;
			for(i = 0; i<5;i++){
				srand(((unsigned)time(NULL)) + 2*i);
				float xg = 1 + rand()%8;
				xg /= 10;
				float yg = 30 + rand()%115;
				//-------
				en_gr_coord[i].en_gr_alive = 1;
				//en_gr_coord[i].altera_g_speed = 0;
			//posicoes iniciais en_gr:
				en_gr_coord[i].en_gr_x = SCREEN_W * xg - EN_GR_SIZE / 2.0;
				en_gr_coord[i].en_gr_y = -yg + EN_GR_SIZE;//-160.0 + EN_GR_SIZE / 2.0;
				en_gr_coord[i].en_gr_dx = 0.6;
				en_gr_coord[i].en_gr_dy = 0.5;
			}
					/*EXPLICACAO: 
					- Descubra o ponto inicial: SCREEN_W * 0.1 - EN_GR_SIZE / 2.0 --> 94 - 15= 79
					- Calcule a distancia horizontal retante ate o final da tela: tamanho da tela - ponto inicial --> 940 - 79= 861
					- Descubra quantos passos horizontais necessarios para percorrer os 861 pixels: 861 / en_gr_dx --> 861 / 0.6= 1435
					- Considerando a queda constante, calcule quantos pixels en_gr desce em 1435 passos: 1435 passos * en_gr_dy --> 1435 * 0.5= 717.5
					- Considerando o tamanho da tela, calcule a diferenca: 717 - 540= 177 --> logo, o inimigo deve iniciar na posicao -177 antes do 0 da SCREEN_H
					- Mas decidi colocar -160 para nao ficar muito roubado kkk --> o inimigo chega 17 pixels antes do ponto max da diagonal!
					- Mas ainda sim ele pega a nave! INCRIVEL!
					*/
					
		struct coordenada_enemyblue en_bl_coord[5];
			for(i = 0; i<5;i++){
				srand(((unsigned)time(NULL)) - i);
				float xb = 1 + rand()%8;
				xb /= 10;
				float yb = 30 + rand()%115;
				//-------
				en_bl_coord[i].en_bl_alive = 1;
				//en_bl_coord[i].altera_b_speed = 1;
			//posicoes iniciais en_bl:
				en_bl_coord[i].en_bl_x = SCREEN_W * xb - EN_BL_SIZE / 2.0;
				en_bl_coord[i].en_bl_y = -yb + EN_BL_SIZE;//-160.0 + EN_BL_SIZE / 2.0;
				en_bl_coord[i].en_bl_dx = 0.6;
				en_bl_coord[i].en_bl_dy = 0.51;//<<<<<------ COM 0.01 DE DIFERENÇA JA MUDA! VEJA SE DA PRA REMOVER O ESQUEMA DE TROCA DE VELOCIDADES (TELETRANS)!!
			}
			
		//variavel score
		int score = 0;		
		
		//variavel para liberar mudança da velocidade dos inimigos:
		//int key_speed = 1;
		
	   //condiçao de loop:
	   int playing = 1;
	   
	   bool redraw = true;
								//VEJA ISSO::::: http://equipe.nce.ufrj.br/adriano/c/apostila/estru.htm <<<<<-----------
								//outras possibilidades?: (*p[i]).jfje || (*p)[i].hrhre ||(*p+i).jhjdf  <MAS CUIDADO PARA NAO SE LIMITAR!!!
								void move_en_gr(struct coordenada_enemygreen *p, /*int v,*/ float nave_x){
										//teletrans_en_gr(&en_gr_coord, en_gr_coord.en_gr_alive);
									//int j;
									//SERA QUE PRECISA DESSE FOR? QUAL A LOGICA???
									//for(j=0; j<v; j++){
										//descendente
										p->en_gr_y += p->en_gr_dy;
										//following
										if(p->en_gr_x + EN_GR_SIZE < nave_x + NAVE_SIZE && p->en_gr_x + EN_GR_SIZE < SCREEN_W){
											p->en_gr_x += p->en_gr_dx;
										}
										else if(p->en_gr_x + EN_GR_SIZE > nave_x + NAVE_SIZE && p->en_gr_x + EN_GR_SIZE > EN_GR_SIZE){
											p->en_gr_x -= p->en_gr_dx;
										}											
									//}
								}
								void teletrans_en_gr(struct coordenada_enemygreen *g, int j/*, int key, int *altera_g_sp*/){
									//int j;
									//SERA QUE PRECISA DESSE FOR? QUAL A LOGICA???
									//for(j=0; j<v; j++){
									/*if(*position_g < 0.9){
										*position_g += 0.1;
									}*/
									srand((unsigned)g->en_gr_x);
									float z = j + (1 + rand()%8);//RESOLVA ISSO:::: 9 + 4 = 13!!! -->>>> ISSO EH FORA DA TELA!!!!
									//z /= 10;
										if(z > 9){
											if((int)z%2 == 0){z /= 2;}
											else{z = (z + 1)/2;}
											//z = 0.9 - (z - 0.9);
										}
									z /= 10;
									float y = j*10 + (30 + rand()%115);
									
										//que tal submeter as posiçoes de inicio com um if de modo que ora ela inicia
										//na posiçao do verde, ora na do azul? >> Assim vai ficar tudo misturado :)
										//aprox 6seg depois ele aparece!
										if(g->en_gr_alive == 0 || g->en_gr_y > SCREEN_H + EN_GR_SIZE){
											g->en_gr_x = SCREEN_W * z - EN_GR_SIZE / 2.0;
											g->en_gr_y = -y + EN_GR_SIZE;//-160.0 + EN_GR_SIZE / 2.0;
												/*if(key == 1){	
													if(*altera_g_sp == 0){
														g->en_gr_dx = 0.4;
														g->en_gr_dy = 0.7;
															*altera_g_sp = 1;
													}else{
														g->en_gr_dx = 0.6;
														g->en_gr_dy = 0.5;
															*altera_g_sp = 0;
													}
												}*/
											g->en_gr_alive = 1;
										}
									//}
								}
								void collision_en_gr(struct coordenada_enemygreen eg, /*int v,*/ int *en_gr_alive, struct coordenada_nave n, int *score, int *nave_alive){
									//int j;
									//SERA QUE PRECISA DESSE FOR? QUAL A LOGICA???
									//E SERA QUE PRECISA PASSAR POR REFERENCIA??
									//for(j=0; j<v; j++){
										//se alguma destas condicoes ocorrerem, entao NAO ha colisao, caso contrario HA colisao
										if( (eg.en_gr_x > n.nave_x + NAVE_SIZE) || (n.nave_x > eg.en_gr_x + EN_GR_SIZE) ||
											(eg.en_gr_y > n.nave_y + NAVE_SIZE) || (n.nave_y > eg.en_gr_y + EN_GR_SIZE) ){
												*en_gr_alive = 1;
										}
										else{
											if(n.nave_color){
												*nave_alive = 0;
											}
											else{
												*score += 1;
												*en_gr_alive = 0;
											}
										}
									//}
								}
								void move_en_bl(struct coordenada_enemyblue *c, /*int v,*/ float nave_x){
									//int j;
									//SERA QUE PRECISA DESSE FOR? QUAL A LOGICA???
									//for(j=0; j<v; j++){
										//descendente
										c->en_bl_y += c->en_bl_dy;
										//following
										if(c->en_bl_x + EN_BL_SIZE < nave_x + NAVE_SIZE && c->en_bl_x + EN_BL_SIZE < SCREEN_W){
											c->en_bl_x += c->en_bl_dx;
										}
										else if(c->en_bl_x + EN_BL_SIZE > nave_x + NAVE_SIZE && c->en_bl_x + EN_BL_SIZE > EN_BL_SIZE){
											c->en_bl_x -= c->en_bl_dx;
										}	
									//}
								}
								void teletrans_en_bl(struct coordenada_enemyblue *b, int j/*, int key, int *altera_b_sp*/){
									//int j;
									//SERA QUE PRECISA DESSE FOR? QUAL A LOGICA???
									//for(j=0; j<v; j++){
									/*if(*position_b > 0.1){
										*position_b -= 0.1;
									}*/
									srand((unsigned)b->en_bl_x);
									float k = j + (1 + rand()%8);
									//k /= 10;
										if(k > 9){
											if((int)k%2 == 0){k /= 2;}
											else{k = ( (k - 1)/2 ) - 1;}
											//k = 0.9 - (k - 0.9);
										}
									k /= 10;
									float y = j*10 + (30 + rand()%115);
										//aprox 6seg depois ele aparece!
										if(b->en_bl_alive == 0 || b->en_bl_y > SCREEN_H + EN_BL_SIZE){
											b->en_bl_x = SCREEN_W * k - EN_BL_SIZE / 2.0;
											b->en_bl_y = -y + EN_BL_SIZE;//-160.0 + EN_BL_SIZE / 2.0;
												/*if(key == 1){	
													if(*altera_b_sp == 0){
														b->en_bl_dx = 0.4;
														b->en_bl_dy = 0.7;
															*altera_b_sp = 1;
													}else{
														b->en_bl_dx = 0.6;
														b->en_bl_dy = 0.5;
															*altera_b_sp = 0;
													}
												}*/
											b->en_bl_alive = 1;
										}
									//}
								}
								void collision_en_bl(struct coordenada_enemyblue eb, /*int v,*/ int *en_bl_alive, struct coordenada_nave n, int *score, int *nave_alive){
									//int j;
									//SERA QUE PRECISA DESSE FOR? QUAL A LOGICA???
									//E SERA QUE PRECISA PASSAR POR REFERENCIA??
									//for(j=0; j<v; j++){	
										//se alguma destas condicoes ocorrerem, entao NAO ha colisao, caso contrario HA colisao
										if( (eb.en_bl_x > n.nave_x + NAVE_SIZE) || (n.nave_x > eb.en_bl_x + EN_BL_SIZE) ||
											(eb.en_bl_y > n.nave_y + NAVE_SIZE) || (n.nave_y > eb.en_bl_y + EN_BL_SIZE) ){
												*en_bl_alive = 1;
										}
										else{
											if(n.nave_color){
												*score += 1;
												*en_bl_alive = 0;
											}
											else{
												*nave_alive = 0;
											}
										}
									//}
								}
		//----------------------- rotinas de inicializacao ---------------------------------------
	   if(!al_init()) {
		  fprintf(stderr, "failed to initialize allegro!\n");
		  return -1;
	   }
	 
	   timer = al_create_timer(1.0 / FPS);
	   if(!timer) {
		  fprintf(stderr, "failed to create timer!\n");
		  return -1;
	   }
	 
	   display = al_create_display(SCREEN_W, SCREEN_H);
	   if(!display) {
		  fprintf(stderr, "failed to create display!\n");
		  al_destroy_timer(timer);
		  return -1;
	   }
	 
		//cria um bitmap quadrangular de tamanho NAVE_SIZE (variavel global declarada acima)
			nave = al_create_bitmap(NAVE_SIZE, NAVE_SIZE);
				if(!nave) {
					fprintf(stderr, "failed to create nave bitmap!\n");
					al_destroy_display(display);
					al_destroy_timer(timer);
					return -1;
				}
		//cria os bitmaps quadrangulares MANUALMENTE de tamanho EN_GR_SIZE (variavel global declarada acima)
			//SOBRE O ERRO: http://stackoverflow.com/questions/35614204/warning-assignment-from-incompatible-pointer-type
			en_gr = malloc(5*sizeof(BITMAP *));
			//for(i=0;i<5;i++){
			en_gr[0] = al_create_bitmap(EN_GR_SIZE, EN_GR_SIZE);//}
			en_gr[1] = al_create_bitmap(EN_GR_SIZE, EN_GR_SIZE);
			en_gr[2] = al_create_bitmap(EN_GR_SIZE, EN_GR_SIZE);
			en_gr[3] = al_create_bitmap(EN_GR_SIZE, EN_GR_SIZE);
			en_gr[4] = al_create_bitmap(EN_GR_SIZE, EN_GR_SIZE);
		
		//cria os bitmaps quadrangulares MANUALMENTE de tamanho EN_BL_SIZE (variavel global declarada acima)
			en_bl = malloc(5*sizeof(BITMAP *));
			//for(i=0;i<5;i++){
			en_bl[0] = al_create_bitmap(EN_BL_SIZE, EN_BL_SIZE);//}
			en_bl[1] = al_create_bitmap(EN_BL_SIZE, EN_BL_SIZE);
			en_bl[2] = al_create_bitmap(EN_BL_SIZE, EN_BL_SIZE);
			en_bl[3] = al_create_bitmap(EN_BL_SIZE, EN_BL_SIZE);
			en_bl[4] = al_create_bitmap(EN_BL_SIZE, EN_BL_SIZE);
				
			for(i=0;i<5;i++){
				if(!en_gr || !en_bl) {
					fprintf(stderr, "failed to create enemies bitmap!\n");
					free(en_gr);
					free(en_bl);
					al_destroy_display(display);
					al_destroy_timer(timer);
					return -1;
				}
			}
		
					//avisa o allegro que eu quero inicializar o teclado:
					al_install_keyboard();
					
					al_init_primitives_addon();
					//avisa o allegro que eu quero modificar as propriedades da nave
					al_set_target_bitmap(nave);
					//altera a cor da nave para VERDE(0,255,0)
					al_clear_to_color(al_map_rgb(0, 255, 0));
				
				for(i= 0; i<5;i++){
					//avisa o allegro que eu quero modificar as propriedades da en_gr
					al_set_target_bitmap(en_gr[i]);
					//altera a cor da en_gr para VERDE(0,255,0)
					al_clear_to_color(al_map_rgb(0, 150, 0));
					
					/*al_set_target_bitmap(en_gr[1]);
					//altera a cor da en_gr para VERDE(0,255,0)
					al_clear_to_color(al_map_rgb(0, 150, 0));
					
					al_set_target_bitmap(en_gr[2]);
					//altera a cor da en_gr para VERDE(0,255,0)
					al_clear_to_color(al_map_rgb(0, 150, 0));
					
					al_set_target_bitmap(en_gr[3]);
					//altera a cor da en_gr para VERDE(0,255,0)
					al_clear_to_color(al_map_rgb(0, 150, 0));
					
					al_set_target_bitmap(en_gr[4]);
					//altera a cor da en_gr para VERDE(0,255,0)
					al_clear_to_color(al_map_rgb(0, 150, 0));*/
				 
					//avisa o allegro que eu quero modificar as propriedades da en_bl
					al_set_target_bitmap(en_bl[i]);
					//altera a cor da en_bl para AZUL(0,50,255)
					al_clear_to_color(al_map_rgb(0, 0, 220));
					
					/*al_set_target_bitmap(en_bl[1]);
					//altera a cor da en_bl para AZUL(0,50,255)
					al_clear_to_color(al_map_rgb(0, 0, 220));
					
					al_set_target_bitmap(en_bl[2]);
					//altera a cor da en_bl para AZUL(0,50,255)
					al_clear_to_color(al_map_rgb(0, 0, 220));
					
					al_set_target_bitmap(en_bl[3]);
					//altera a cor da en_bl para AZUL(0,50,255)
					al_clear_to_color(al_map_rgb(0, 0, 220));
					
					al_set_target_bitmap(en_bl[4]);
					//altera a cor da en_bl para AZUL(0,50,255)
					al_clear_to_color(al_map_rgb(0, 0, 220));*/
				}
					//avisa o allegro que agora eu quero modificar as propriedades da tela
					al_set_target_bitmap(al_get_backbuffer(display));
					
				//colore a tela de branco (rgb(255,255,255))
				al_clear_to_color(al_map_rgb(255,255,255));   
		
		//inicializa o modulo allegro que carrega as fontes
			al_init_font_addon();
		//inicializa o modulo allegro que entende arquivos tff de fontes
			al_init_ttf_addon();
				if (!al_init_ttf_addon()){
					fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
					return -1;
				}
		//define a fonte a ser usada no score
			score_font = al_load_font("ARIAL.TTF", 48, 0);
				if (!score_font){
					fprintf(stderr, "Falha ao carregar fonte SCORE!.\n");
					return -1;
				}
		//define a cor da fonte usada no score
			//https://www.allegro.cc/manual/5/al_map_rgba_f
			float a = 0.5f;
			ALLEGRO_COLOR score_color = al_map_rgba_f(1.0*a, 0.4*a, 0.6*a, a);
			/*if (!score_color){
					fprintf(stderr, "Falha ao definir a cor da fonte SCORE!.\n");
					return -1;
				}*/
			
		//cria a fila de eventos
			event_queue = al_create_event_queue();
				if(!event_queue) {
					fprintf(stderr, "failed to create event_queue!\n");
					al_destroy_bitmap(nave);
					for(i=0;i<5;i++){
						al_destroy_bitmap(en_gr[i]);
						al_destroy_bitmap(en_bl[i]);
					}
					free(en_gr);
					free(en_bl);
					al_destroy_display(display);
					al_destroy_timer(timer);
					return -1;
			   }
	 
		//registra na fila de eventos que eu quero identificar quando a tela foi alterada
	   al_register_event_source(event_queue, al_get_display_event_source(display));
		//registra na fila de eventos que eu quero identificar quando o tempo alterou de t para t+1
	   al_register_event_source(event_queue, al_get_timer_event_source(timer));   
		//registra que a fila de eventos deve detectar quando uma tecla for pressionada no teclado
	   al_register_event_source(event_queue, al_get_keyboard_event_source());
	 

		//reinicializa a tela
	   al_flip_display();
		//inicia o temporizador
	   al_start_timer(timer);
	 
		//loop de jogo, ou seja, enquanto 'playing' for verdadeiro, faca:
	   while(playing)
	   {
		  ALLEGRO_EVENT ev;
		  //espera por um evento e o armazena na variavel de evento ev
		  al_wait_for_event(event_queue, &ev);
	 
			//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
				if(ev.type == ALLEGRO_EVENT_TIMER) {
					
					if(nave_coord.nave_alive){
						//verifica se a posicao x da nave passou dos limites da tela
						if(nave_coord.nave_x < 0 || nave_coord.nave_x > SCREEN_W - NAVE_SIZE) {
						//altera a direcao na qual o nave se move no eixo x
						nave_coord.nave_dx = -nave_coord.nave_dx;
						}	 
						//verifica se a posicao y da nave passou dos limites da tela
						if(nave_coord.nave_y < 0 || nave_coord.nave_y > SCREEN_H - NAVE_SIZE) {
						//altera a direcao na qual o nave se move no eixo y
						nave_coord.nave_dy = -nave_coord.nave_dy;
						} 		
					}
					//liberaçao da mudança de velocidade:
					//key_speed = 0;
					for(i= 0; i<5; i++){
						//teletransporte do en_gr?
							teletrans_en_gr( &(en_gr_coord[i]), i/*, key_speed, &(en_gr_coord[i].altera_g_speed)*/ );
						//analise de colisao do en_gr 
							collision_en_gr( en_gr_coord[i], /*5,*/ &(en_gr_coord[i].en_gr_alive),nave_coord,&score,&nave_coord.nave_alive);
						if(en_gr_coord[i].en_gr_alive == 1){	
							//movimento de en_gr
							move_en_gr(&(en_gr_coord[i]), /*5,*/ nave_coord.nave_x);
						}else{teletrans_en_gr( &(en_gr_coord[i]), i/*, key_speed, &(en_gr_coord[i].altera_g_speed)*/ );}
					}
					for(i= 4; i>=0; i--){
						//teletransporte do en_bl?
							teletrans_en_bl( &(en_bl_coord[i]), i/*, key_speed, &(en_bl_coord[i].altera_b_speed)*/ );
						//analise de colisao do en_bl 
							collision_en_bl( en_bl_coord[i], /*5,*/ &(en_bl_coord[i].en_bl_alive),nave_coord,&score,&nave_coord.nave_alive);
						if(en_bl_coord[i].en_bl_alive == 1){
							//movimento de en_bl
							move_en_bl(&(en_bl_coord[i]), /*5,*/ nave_coord.nave_x);
						}else{teletrans_en_bl( &(en_bl_coord[i]), i/*, key_speed, &(en_bl_coord[i].altera_b_speed)*/ );}
					}
						
				//VEJA ISSO!!!!------->>>>>>>>>   //
				redraw = true;
				}
		  
			//se o tipo do evento for uma tecla pressionada
				else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
					//verifica qual tecla foi
					switch(ev.keyboard.keycode) {
						//se a tecla for o W
						case ALLEGRO_KEY_W:
							nave_coord.nave_move_up = true;
								break;
						//se a tecla for o S
						case ALLEGRO_KEY_S:
							nave_coord.nave_move_down = true;
								break;
						//se a tecla for o A
						case ALLEGRO_KEY_A:
							nave_coord.nave_move_left = true;
								break;
						//se a tecla for o D
						case ALLEGRO_KEY_D:
							nave_coord.nave_move_right = true;
								break;
						//se a tecla for o SPACE		
						case ALLEGRO_KEY_SPACE:
							//FALSE: VERDE || TRUE: AZUL
							if(nave_coord.nave_color){
								nave_coord.nave_color = false;
							}else
								nave_coord.nave_color = true;
								break;
					}
				}
				else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
					//verifica qual tecla foi
					switch(ev.keyboard.keycode) {
						//se a tecla for o W
						case ALLEGRO_KEY_W:
							nave_coord.nave_move_up = false;
								break;
						//se a tecla for o S
						case ALLEGRO_KEY_S:
							nave_coord.nave_move_down = false;
								break;
						//se a tecla for o A
						case ALLEGRO_KEY_A:
							nave_coord.nave_move_left = false;
								break;
						//se a tecla for o D
						case ALLEGRO_KEY_D:
							nave_coord.nave_move_right = false;
								break;
						//se a tecla for o SPACE
						case ALLEGRO_KEY_SPACE:
								break;
					}
				}
			//se o tipo de evento for o fechamento da tela (clique no x da janela)
				else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				//interrompe o while(playing)
					playing = 0;
					//break; <<<<<----------
				}
	 
			//se eu alterei a posicao da nave, o redraw foi para true e eu nao tenho eventos na fila para ler
				if(redraw && al_is_event_queue_empty(event_queue)) {

					 redraw = false;
					//limpo a tela
					 al_clear_to_color(al_map_rgb(255,255,255));
					
					//desenha retangulo transparente para o score:
					 //al_draw_filled_rectangle(880 - 50, 50 - 30, 880 + 50, 50 + 30, al_map_rgba(255, 0, 255, -1));
					 //al_draw_tinted_bitmap(bitmap, al_map_rgba_f(1, 1, 1, 0.5), x, y, 0); //50% tranparency
					//desenha na tela o score:
					 al_draw_textf(score_font, /*score_color*/al_map_rgb(255, 0, 0), 880, 50, ALLEGRO_ALIGN_RIGHT, "%d", score);
					 //textprintf_ex(display,score_font,880,50,makecol(255,0,0),-1,"%d",score);
							/* inpiracao:
							https://www.google.com.br/search?q=texto+transparente+allegro&oq=texto+transparente+allegro&aqs=chrome..69i57.8513j0j1&sourceid=chrome&ie=UTF-8
							http://www.unidev.com.br/index.php?/topic/8536-efeitos-no-allegro/
							http://edcomjogos.dc.ufscar.br/tutoriais/tutorial_allegro.pdf
							https://www.google.com.br/search?q=text_mode+allegro+5&oq=text_mode+allegro+5&aqs=chrome..69i57.6337j0j1&sourceid=chrome&ie=UTF-8#q=bitmap+passing+inside+text+allegro+5
							https://www.allegro.cc/manual/5/al_draw_text
							https://www.allegro.cc/forums/thread/606127
							https://www.allegro.cc/forums/thread/615564
							https://www.google.com.br/search?q=texto+sobrepondo+bitmap+allegro&oq=texto+sobrepondo+bitmap+allegro&aqs=chrome..69i57.24248j0j4&sourceid=chrome&ie=UTF-8
							
							*/
					 
					if(nave_coord.nave_alive == 1){
						//desenho o nave nas novas posicoes x e y
						if(nave_coord.nave_y - nave_coord.nave_dy >= 0 && nave_coord.nave_move_up && !nave_coord.nave_move_down){//desta forma a tecla oposta apenas interrompe o movimento (quando ativada)
							nave_coord.nave_y -= nave_coord.nave_dy;
								//movimento diagonal
								if(nave_coord.nave_x > 0 && nave_coord.nave_move_left && !nave_coord.nave_move_right){
								nave_coord.nave_x -= nave_coord.nave_dx; 
									//barra de espaco funcionando com outras teclas apertadas (diagonal cima-esquerda)
										altera_color(nave_coord.nave_color);							
								}
								else if(nave_coord.nave_x < SCREEN_W - NAVE_SIZE && nave_coord.nave_move_right && !nave_coord.nave_move_left){
								nave_coord.nave_x += nave_coord.nave_dx; 									
									//barra de espaco funcionando com outras teclas apertadas (diagonal cima-direita)
										altera_color(nave_coord.nave_color);							
								}											
									//barra de espaco funcionando com outras teclas apertadas (para -> cima)
										altera_color(nave_coord.nave_color);
						}
						else if(nave_coord.nave_y < SCREEN_H - NAVE_SIZE && nave_coord.nave_move_down && !nave_coord.nave_move_up){//desta forma a tecla oposta apenas interrompe o movimento (quando ativada)
							nave_coord.nave_y += nave_coord.nave_dy; 
								//movimento diagonal
								if(nave_coord.nave_x > 0 && nave_coord.nave_move_left && !nave_coord.nave_move_right){
								nave_coord.nave_x -= nave_coord.nave_dx; 								
									//barra de espaco funcionando com outras teclas apertadas (diagonal esquerda-baixo)
										altera_color(nave_coord.nave_color);
								}
								else if(nave_coord.nave_x < SCREEN_W - NAVE_SIZE && nave_coord.nave_move_right && !nave_coord.nave_move_left){
								nave_coord.nave_x += nave_coord.nave_dx; 								
									//barra de espaco funcionando com outras teclas apertadas (diagonal direita-baixo)
										altera_color(nave_coord.nave_color);								
								}									
									//barra de espaco funcionando com outras teclas apertadas (para -> baixo)
										altera_color(nave_coord.nave_color);								
						}
						else if(nave_coord.nave_x > 0 && nave_coord.nave_move_left && !nave_coord.nave_move_right){//desta forma a tecla oposta apenas interrompe o movimento (quando ativada)
							nave_coord.nave_x -= nave_coord.nave_dx; 							
									//barra de espaco funcionando com outras teclas apertadas (para <- esquerda)
										altera_color(nave_coord.nave_color);								
						}
						else if(nave_coord.nave_x < SCREEN_W - NAVE_SIZE && nave_coord.nave_move_right && !nave_coord.nave_move_left){//desta forma a tecla oposta apenas interrompe o movimento (quando ativada)
							nave_coord.nave_x += nave_coord.nave_dx; 							
									//barra de espaco funcionando com outras teclas apertadas (para -> direita)
										altera_color(nave_coord.nave_color);								
						}
						//O COMANDO DA BARRA DE ESPACO PROPRIAMENTE DITO:
						altera_color(nave_coord.nave_color);
						//----------------------------------------------------
						al_draw_bitmap(nave, nave_coord.nave_x, nave_coord.nave_y, 0);
					}else{playing = 0;}
					//liberaçao da mudança de velocidade:
					//key_speed = 0;
					for(i=0;i<5;i++){
						//teletransporte do en_gr?
						teletrans_en_gr( &(en_gr_coord[i]), i/*, key_speed, &(en_gr_coord[i].altera_g_speed)*/ );
						//analise de colisao do en_gr 
						collision_en_gr( en_gr_coord[i], /*5,*/ &(en_gr_coord[i].en_gr_alive),nave_coord,&score,&nave_coord.nave_alive);
						//desenho en_gr nas posicoes x e y
							if(en_gr_coord[i].en_gr_alive){	
								//movimento de en_gr
								move_en_gr(&(en_gr_coord[i]), /*5,*/ nave_coord.nave_x);
													
							al_draw_bitmap(en_gr[i], en_gr_coord[i].en_gr_x, en_gr_coord[i].en_gr_y, 0);
							}else{teletrans_en_gr( &(en_gr_coord[i]), i/*, key_speed, &(en_gr_coord[i].altera_g_speed)*/ );}
					}
					for(i= 4; i>=0; i--){
						//teletransporte do en_bl?
						teletrans_en_bl( &(en_bl_coord[i]), i/*, key_speed, &(en_bl_coord[i].altera_b_speed)*/ );
						//analise de colisao do en_bl 
						collision_en_bl( en_bl_coord[i], /*5,*/ &(en_bl_coord[i].en_bl_alive),nave_coord,&score,&nave_coord.nave_alive);
						//desenho en_bl nas posicoes x e y
							if(en_bl_coord[i].en_bl_alive){
								//movimento de en_bl
								move_en_bl(&(en_bl_coord[i]), /*5,*/ nave_coord.nave_x);
								
							al_draw_bitmap(en_bl[i], en_bl_coord[i].en_bl_x, en_bl_coord[i].en_bl_y, 0);
							}else{teletrans_en_bl( &(en_bl_coord[i]), i/*, key_speed, &(en_bl_coord[i].altera_b_speed)*/ );}
					}		/*SE LIGA NISSO:
								http://cseweb.ucsd.edu/~j2lau/cs5a/week8.html
								http://www.cs.toronto.edu/~heap/270F02/node31.html
									http://stackoverflow.com/questions/14768230/malloc-for-struct-and-pointer-in-c
								MALLOC STRUCTS PESQUISA: https://www.google.com.br/search?q=malloc+structs&oq=malloc+structs&aqs=chrome..69i57.4336j0j3&sourceid=chrome&ie=UTF-8
							*/
					//reinicializa a tela
					 al_flip_display();
				}
			
		} //fim do while
	   
	   /*//inicializa o modulo allegro que carrega as fontes
			  al_init_font_addon();
			//inicializa o modulo allegro que entende arquivos tff de fontes
			  al_init_ttf_addon();*/
			//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
			  ALLEGRO_FONT *size_34 = al_load_font("arial.ttf", 34, 1);
			
			int record;
			//int rec = 0;
			//record = &rec;
			FILE *points = NULL;
			points = fopen("recorde.txt","rb");
				if(points == NULL){
					fprintf(stdout,"Erro1 ao acessar o arquivo de recorde!\n");
						al_destroy_bitmap(nave);
					   for(i = 0;i<5;i++){
						   al_destroy_bitmap(en_gr[i]);						   
						   al_destroy_bitmap(en_bl[i]);						   
						}
					   free(en_gr);
					   free(en_bl);
					   al_destroy_timer(timer);
					   al_destroy_font(score_font);
					   al_destroy_font(size_34);
					   al_destroy_display(display);
					   al_destroy_event_queue(event_queue);
					 
					   return 0;
				}
			fread(&record,sizeof(int),1,points);
			fclose(points);
			//retornar o recorde para 0:
			/*record = 0;
			points = fopen("recorde.txt","wb");
			fwrite(&record,sizeof(int),1, points);*/
				if(record < score){//mude a condiçao para maior que '>' e retorne o recorde para 0
					points = fopen("recorde.txt","wb");
						if(points == NULL){
							fprintf(stdout,"Erro2 ao acessar o arquivo de recorde!\n");
								al_destroy_bitmap(nave);
							   for(i = 0;i<5;i++){
								   al_destroy_bitmap(en_gr[i]);						   
								   al_destroy_bitmap(en_bl[i]);						   
								}
							   free(en_gr);
							   free(en_bl);
							   al_destroy_timer(timer);
							   al_destroy_font(score_font);
							   al_destroy_display(display);
							   al_destroy_event_queue(event_queue);
							 
							   return 0;
						}
					//record = &score;
					fwrite(&score,sizeof(int),1, points);
					fclose(points);
					
						//colore toda a tela de preto
						al_clear_to_color(al_map_rgb(225,225,255));
						
						al_draw_textf(size_34, al_map_rgb(20, 20, 220), SCREEN_W/4 , 2*SCREEN_H/3, 0, "Novo Recorde: %d Pontos!!", score);
				}
				else{
					//colore toda a tela de preto
					al_clear_to_color(al_map_rgb(225,225,255));
					
					al_draw_textf(size_34, al_map_rgb(255, 0, 0), SCREEN_W/3, SCREEN_H/2 + 45, 0, "Sua Pontuação: %d", score);
					al_draw_textf(size_34, al_map_rgb(0, 220, 0), SCREEN_W/3, SCREEN_H/2 + 125, 0, "Recorde: %d", record);
				}
			//fclose(points);
			//char pontuacao[20];	
		
			//colore toda a tela de preto
			//al_clear_to_color(al_map_rgb(0,255,255));
			
			//sprintf(pontuacao, "Sua Pontuação: %d", score);	
			//al_draw_text(size_32, al_map_rgb(15, 200, 30), SCREEN_W/3, SCREEN_H/2+100, 0, my_text);	
			//al_draw_textf(size_34, /*score_color*/al_map_rgb(255, 0, 255), SCREEN_W/3, 2*SCREEN_H/3, 0, "Sua Pontuação: %d", score);
			
	
		//reinicializa a tela
		al_flip_display();	
		al_rest(5);		  
		
		//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	   al_destroy_bitmap(nave);
	   for(i = 0;i<5;i++){
		   al_destroy_bitmap(en_gr[i]);
		   /*al_destroy_bitmap(en_gr[1]);
		   al_destroy_bitmap(en_gr[2]);
		   al_destroy_bitmap(en_gr[3]);
		   al_destroy_bitmap(en_gr[4]);*/
		   
		   al_destroy_bitmap(en_bl[i]);
		   /*al_destroy_bitmap(en_bl[1]);
		   al_destroy_bitmap(en_bl[2]);
		   al_destroy_bitmap(en_bl[3]);
		   al_destroy_bitmap(en_bl[4]);*/
		   
		}
	   free(en_gr);
	   free(en_bl);
	   al_destroy_timer(timer);
	   al_destroy_font(score_font);
	   al_destroy_font(size_34);
	   al_destroy_display(display);
	   al_destroy_event_queue(event_queue);
	 
	   return 0;
	}