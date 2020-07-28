#include <stdio.h> <stdbool.h> <string.h> <math.h> <unistd.h> <stdarg.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define pi 3.141592654
#define width 800
#define height 600
#define bgcolor al_map_rgb(1, 5, 15)
#define FPS 60
#define mouseOverInit     	 x > 315 && x < 490 && y > 410 && y < 472
#define mouseOverRanking 	 x > 35  && x < 191 && y > 528 && y < 572
#define mouseOverAjuda 		 x > 340 && x < 455 && y > 528 && y < 572
#define mouseOverSair 		 x > 682 && x < 761 && y > 528 && y < 572
#define mouseOverSound		 x > 226 && x < 296 && y > 220 && y < 274
#define mouseOverContinuar   x > 300 && x < 500 && y > 544 && y < 588
#define mouseOverMenu        x > 215 && x < 315 && y > 415 && y < 465
#define mouseOverReturn      x > 166 && x < 338 && y > 390 && y < 447
#define mouseOverReiniciar   x > 420 && x < 635 && y > 390 && y < 447
#define mouseOverContinue    x > 335 && x < 470 && y > 505 && y < 535
#define mouseOverSairEndGame x > 495 && x < 580 && y > 415 && y < 465
#define mouseOverVolumeBar   x > width/2.55 && x < width/2+150 && y > height/2-70 && y < height/2-30 && escapeBool
#define soundPixelSize 		 75
#define branco       		 al_map_rgba_f(1, 1, 1, 1)
#define bgGrid				 al_map_rgba(59, 60, 61, 190)

#define returnToDisplay      al_set_target_bitmap(al_get_backbuffer(display))


typedef struct anim {

 float destBlockX;
 float destBlockY;
 int destValue;

 float b1X;
 float b1Y;
 int b1Value;

 float b2X;
 float b2Y;
 int b2Value;

 float b3X;
 float b3Y;
 int b3Value;

} Animation;

typedef struct rank {
	char username[20];
	unsigned long points;
} Rank;


float tR ( char XOrY, float angle, float size, float y, float x, short largura, short altura ) {
	if ( largura == 400 ) return XOrY == 'x' ? cos(x+angle*pi/180)*size+(largura/2) : sin(y+angle*pi/180)*size+(altura/2);
	y -= size/800;
	angle -= size/20;
	angle += 1000/size;
	float bb = XOrY == 'x' ? cos(x+angle*pi/180)*size+(width/2) : sin(y+angle*pi/180)*size+(height/2);
	y += size/800;
	return bb;
}


void  resetMatrix ( unsigned int matrix[99][8][8] ) {
    for ( int i = 0; i <= 7; i++ ) {
	    for ( int j = 0; j <= 7; j++ ) {
			matrix[0][i][j] = 0;
			matrix[0][7][j] = 1;
			
        }
	} puts("Matrix initialized");
}


void  createRanking ( ) {

	FILE *rankFile = fopen("ranking.txt", "w");
	
	Rank rankingsN[10];

	puts("Rankfile cound not load, generating new....");
	rankingsN[0].points = 800000;
	rankingsN[1].points = 600000;
	rankingsN[2].points = 550000;
	rankingsN[3].points = 500000;
	rankingsN[4].points = 65000;
	rankingsN[5].points = 4500;
	rankingsN[6].points = 4000;
	rankingsN[7].points = 3000;
	rankingsN[8].points = 1500;
	rankingsN[9].points = 1000;
	sprintf(rankingsN[0].username, "natas");
	sprintf(rankingsN[1].username, "maria");
	sprintf(rankingsN[2].username, "joão");
	sprintf(rankingsN[3].username, "marcos");
	sprintf(rankingsN[4].username, "gabriel");
	sprintf(rankingsN[5].username, "pedro");
	sprintf(rankingsN[6].username, "ana");
	sprintf(rankingsN[7].username, "joaquim");
	sprintf(rankingsN[8].username, "hector");
	sprintf(rankingsN[9].username, "vitória"); 
	fwrite(&rankingsN, sizeof(rankingsN), 1, rankFile);
	fclose(rankFile);
}


void  loadingScreen ( ALLEGRO_BITMAP *image, short velocity ) {
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_SAMPLE *sample = NULL;

	char name[50];

	timer = al_create_timer(.85 / (FPS/2 * velocity) );

	sample = al_load_sample("./source/yuh.ogg");
	if (!sample) { puts("Loading sound effect not loaded."); return; } 


	al_play_sample(sample, .5, 0, velocity, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_start_timer(timer);
	
	puts("Displaying animation...");
	while ( al_get_timer_count(timer) < 300 ) {

		sprintf(name, "./source/loading/a_000%03d.JPEG", (int) al_get_timer_count(timer));
		printf("\rFrame: %d", (int) al_get_timer_count(timer)); fflush(stdout);
		image = al_load_bitmap(name);
		al_draw_bitmap(image, 0, 0, 0);
		al_destroy_bitmap(image);
		al_flip_display();
	} puts("\nAnimation ended.");

	
	al_destroy_sample(sample); al_destroy_timer(timer);
	al_clear_to_color(al_map_rgb(255,255,255)); al_flip_display();
	return;
}


void  redrawEscapeBitmap ( ALLEGRO_FONT *bebasBig, ALLEGRO_FONT *bebasMedium, ALLEGRO_FONT *bebasSmall, ALLEGRO_BITMAP *escape, int *soundBitmap[], bool muteSound, float volume, ALLEGRO_DISPLAY *display, bool isInGame, char playingNow[], bool returnHover, bool reiniciarHover ) {

	al_set_target_bitmap(escape);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	al_draw_filled_rectangle(0, 0, 800, 600, al_map_rgba_f(0, 0, 0, 0.65));

	al_draw_filled_rectangle(width/2.55-124, height/2-180, width/2.05+221, height/2+10, al_map_rgba_f(0, 0, 0, 0.45));
	al_draw_text(bebasMedium, al_map_rgb(255,255,255), width/2-59, height/2-150, 0, "Música: ");
	al_draw_line(width/2.55+16, height/2-50, width/2+166, height/2-50, al_map_rgb(80,80,80),  4);
	al_draw_scaled_bitmap(soundBitmap[muteSound], 0, 0, 1024, 1024, width/2.55-soundPixelSize-14, height/2-soundPixelSize-14, soundPixelSize, soundPixelSize, 0);
	al_draw_line(width/2.55+15, height/2-50, width/2.55+(230*volume)+35, height/2-50, al_map_rgb(255,200,200),  4);
	al_draw_filled_circle(width/2.44+(230*volume)+15, height/2-50, 10, al_map_rgb(255,255,255));

	char name[100]; sprintf(name, "Tocando agora: %s", playingNow);
	al_draw_text(bebasSmall, branco, (800-al_get_text_width(bebasSmall, name))/2, 590-al_get_font_line_height(bebasSmall), 0, name);

	if ( isInGame ) {
		al_draw_filled_rectangle(width/2.55-174, height/2+20, width/2.05+273, height/2+210, al_map_rgba_f(0, 0, 0, 0.45));
		al_draw_text(bebasBig, returnHover ? al_map_rgb(144,255,32) : branco, width/4.8, height/2+80, 0, "return");
		al_draw_text(bebasBig, reiniciarHover ? al_map_rgb(255,25,180) : branco, width/1.9, height/2+80, 0, "REINICIAR");
	} returnToDisplay; return;
}


void  redrawFrontBitmap ( ALLEGRO_FONT *bebasBig, ALLEGRO_FONT *bebasMedium, ALLEGRO_BITMAP *screen, short *initAnimation, bool hoverInit, bool hoverRanking, bool hoverSair, bool hoverAjuda, ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *tmp ) {

	al_set_target_bitmap(screen);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));

	al_draw_bitmap(tmp, 0, 0, 0);

	if ( hoverInit ) {
		
		al_draw_line( (float)((*initAnimation)*-10+400), 474, (float)((*initAnimation)*10+400), 474, branco, 5 );
		if ( *initAnimation < 8 && hoverInit ) { *initAnimation += 1; }
	} else if ( *initAnimation <= 8 ) {
		al_draw_line( (float)((*initAnimation)*-10+400), 474, (float)((*initAnimation)*10+400), 474, branco, 5 );
		if ( *initAnimation > 0 && !hoverInit ) { *initAnimation -= 1; }
	}

	al_draw_text( bebasBig, al_map_rgb(255,255,255), 321,  402, 0, "INICIAR" );
	al_draw_text( bebasMedium, hoverRanking ? al_map_rgb(200, 20, 255) : al_map_rgb(255,255,255), 38,  516, 0, "RANKING" );
	al_draw_text( bebasMedium, hoverAjuda   ? al_map_rgb(50, 255, 190) : al_map_rgb(255,255,255), 344, 517, 0, "AJUDA"   );
	al_draw_text( bebasMedium, hoverSair    ? al_map_rgb(255, 0, 0)    : al_map_rgb(255,255,255), 684, 517, 0, "SAIR"    );

	al_set_target_bitmap(al_get_backbuffer(display));
}


void  flashScreen ( float *beginWith40, float howMuchToDecrease ) {
	al_draw_filled_rectangle(0, 0, width, height, al_map_rgba_f(*beginWith40/40, *beginWith40/40, *beginWith40/40, *beginWith40/40));
	*beginWith40 -= howMuchToDecrease;
}


void  printFps ( ALLEGRO_FONT *bebasSmall,  int frame, int frameCount ) { 

	al_draw_textf(bebasSmall, al_map_rgb(255,255,0), 0, -3, 0, "FPS: %.1f", frame/(float) frameCount*60);
	al_draw_textf(bebasSmall, al_map_rgb(255,255,0), 0, 12, 0, "LOS: %d", frameCount-frame);
	al_draw_textf(bebasSmall, al_map_rgb(255,255,0), 0, 27, 0, "FRAME: %d", frame);
}


float tt( float *loop, float *in ) {

	if ( *loop == 0 ) {
		*in+=11.4;
	} else { *in-=11.4; }

	if ( *in > 555) {
		*loop = 1;
	} else if ( *in <  -555 ){ *loop = 0; }

	return *in;
}


void  showAjuda ( ALLEGRO_FONT *bebasBig, ALLEGRO_FONT *bebasMedium, ALLEGRO_FONT *bebasRanking, ALLEGRO_FONT *bebasSmall, ALLEGRO_DISPLAY *display, unsigned short r[], unsigned short g[], unsigned short b[] ) {

	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT event;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_BITMAP *BG = NULL;

	bool redraw = true, sair = false;

	float xx = 0, yy = 0, flash = 40, rst = 0;
	unsigned int key, frame = 0;
	timer = al_create_timer(1.0 / FPS);
	queue = al_create_event_queue();
	

	al_register_event_source( queue, al_get_timer_event_source(timer)   	 );
	al_register_event_source( queue, al_get_display_event_source(display) 	 );
	al_register_event_source( queue, al_get_keyboard_event_source()        	 );


	al_start_timer(timer);
	while ( !sair ) {
		al_wait_for_event(queue, &event);

		switch ( event.type ) {
			case ALLEGRO_EVENT_TIMER: redraw = true;
			break;

			case ALLEGRO_EVENT_KEY_CHAR: key = event.keyboard.keycode;
				if (key == 59) sair = true;
			break;
			
			default: printf("Unsupported event received: %d\n", event.type); break;
		}

		if ( redraw && al_is_event_queue_empty(queue) ) {

			drawGameBackground(BG, &rst, xx, yy, frame, r, g, b);
			printFps(bebasSmall, frame, (int)al_get_timer_count(timer));

			al_draw_text(bebasBig, branco, (800-al_get_text_width(bebasBig, "Ajuda:"))/2, 25, 0, "Ajuda");
			al_draw_filled_rectangle(148,110,651,538,al_map_rgba_f(0,0,0,.5));

			al_draw_multiline_text(bebasRanking, branco, 170,120,480, al_get_font_line_height(bebasRanking)-5, 0, "Bem-vindo ao jogo 2048 The game! O jogo é baseado na soma de blocos de mesmo valor, quanto mais blocos você soma de uma só vez, mais pontos você ganha! O jogo termina quando um bloco diferente do bloco abaixo for colocado no topo. Você pode usar o mouse ou as setas do teclado para jogar. Divirta-se!");

			al_draw_text(bebasMedium, branco, 140, 531, 0, "Pressione esc para voltar" );
			if ( flash > 0 ) flashScreen(&flash, 1);
			al_flip_display(); frame++;
			redraw = false;
		}
	}
	al_destroy_bitmap(BG);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
}


short displayMenu ( ALLEGRO_FONT *bebasBig, ALLEGRO_FONT *bebasMedium, ALLEGRO_FONT *bebasRanking, ALLEGRO_FONT *bebasSmall, char playerName[20], ALLEGRO_DISPLAY *display, ALLEGRO_AUDIO_STREAM *mus, unsigned short r[], unsigned short b[], unsigned short g[] ) {
	
	FILE                 *rankFile    =  fopen("ranking.txt", "r");
	Rank                 rankings[10];
	ALLEGRO_TIMER 		 *timer       =  NULL;
	ALLEGRO_EVENT 		 event; 
	ALLEGRO_EVENT_QUEUE  *queue       =  NULL;
	ALLEGRO_BITMAP 		 *bg          =  NULL;
	ALLEGRO_BITMAP 		 *front       =  NULL;
	ALLEGRO_BITMAP 		 *escape      =  al_create_bitmap(width,height);
	ALLEGRO_BITMAP       *frontBitmap =  al_create_bitmap(width,height);
	ALLEGRO_BITMAP 		 *tmp         =  NULL;
	ALLEGRO_BITMAP 		 *bmp         =  al_load_bitmap("./source/menu/logo.png");
	tmp = al_load_bitmap("./source/menu/m_000140.png");
	
	if ( !rankFile ) {
		createRanking();
		rankFile  =  fopen("ranking.txt", "r");
	}

	puts( !rankFile ? "!!!ERROR LOADING RANKING FILE!!!" : "Rankings fetched...");
	fread(&rankings, sizeof(rankings), 1, rankFile);


	bool 			nameError = false, continueHover = false, askForUsername = true, hoverInit = false, hoverRanking = false, redraw = true, hoverAjuda = false, hoverSair = false, muteSound = false, escapeBool = false, redrawEsc = false, mouseDragButton = false, redrawFront = true;
	unsigned int  	counter = 0, frame = 0, *soundBitmap[2];
	float 			volume = 0.5, yo=0, xo=0, flash = 0;
	char 			name[50];
	short 			index = 0, select = 0, x, y, initAnimation = 0, updateScreen = 0;
	unsigned short  key;
	
	al_attach_audio_stream_to_mixer(mus, al_get_default_mixer()); al_set_audio_stream_playmode(mus, ALLEGRO_PLAYMODE_LOOP);
	al_set_audio_stream_gain(mus, volume);

	timer = al_create_timer(1.0 / FPS);
	queue = al_create_event_queue();
	al_register_event_source( queue, al_get_timer_event_source(timer)   	 );
	al_register_event_source( queue, al_get_mouse_event_source()   		  	 );
	al_register_event_source( queue, al_get_display_event_source(display) 	 );
	al_register_event_source( queue, al_get_keyboard_event_source()        	 );

	soundBitmap[0] 		= 	al_load_bitmap( "./source/menu/soundon.png"		 );
	soundBitmap[1] 		= 	al_load_bitmap( "./source/menu/soundoff.png"	 );

	redrawEscapeBitmap( bebasBig, bebasMedium, bebasSmall, escape, soundBitmap, muteSound, volume, display, 0, "Aha - Take me On Instrumental", 0, 0);

	puts("Displaying menu..."); al_start_timer(timer);

	while ( !select ) {
		
		al_wait_for_event( queue, &event );

		switch ( event.type ) {
			case ALLEGRO_EVENT_TIMER: redraw = true;
				if ( escapeBool ) break;
				if ( mouseOverInit    && !hoverRanking && !hoverAjuda && !hoverSair    && !askForUsername ) { if ( updateScreen < 10  ) { updateScreen++; } hoverInit = true; redrawFront = updateScreen != 10;  }  else  if ( updateScreen >= 0 && !hoverRanking && !hoverAjuda && !hoverSair   ) { redrawFront = true; updateScreen--; /* initAnimation -= initAnimation == 0 ? 0: 1 ;*/ hoverInit    = false; break; } 
				if ( mouseOverRanking && !hoverInit    && !hoverSair  && !hoverAjuda   && !askForUsername ) { redrawFront = updateScreen != 1; if ( updateScreen < 1  ) { updateScreen++; } hoverRanking = true; }  else  if ( updateScreen >= 0 && !hoverInit && !hoverAjuda   && !hoverSair    ) { redrawFront = true; updateScreen--; hoverRanking = false; break; } 
				if ( mouseOverAjuda   && !hoverInit    && !hoverSair  && !hoverRanking && !askForUsername ) { redrawFront = updateScreen != 1; if ( updateScreen < 1  ) { updateScreen++; } hoverAjuda   = true; }  else  if ( updateScreen >= 0 && !hoverInit && !hoverRanking && !hoverSair    ) { redrawFront = true; updateScreen--; hoverAjuda   = false; break; }
				if ( mouseOverSair    && !hoverInit    && !hoverAjuda && !hoverRanking && !askForUsername ) { redrawFront = updateScreen != 1; if ( updateScreen < 1  ) { updateScreen++; } hoverSair    = true; }  else  if ( updateScreen >= 0 && !hoverInit && !hoverAjuda   && !hoverRanking ) { redrawFront = true; updateScreen--; hoverSair    = false; break; }
			break;

			case 10: break;
			case 12: break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE: select = 3; break;

			case ALLEGRO_EVENT_MOUSE_AXES: x = event.mouse.x; y = event.mouse.y;
				if ( mouseOverVolumeBar && mouseDragButton ) { volume = (((float)x)-325)/240; volume = volume < 0 ? 0 : volume; al_set_audio_stream_gain(mus, volume); redrawEsc = true; }
				if ( mouseOverContinue  && askForUsername && !escapeBool ) { continueHover = true; } else { continueHover = false; } 
			break;
				

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: x = event.mouse.x; y = event.mouse.y;
				printf("Click (%d, %d)\n", x,y);
				if ( mouseOverVolumeBar && !mouseDragButton ) mouseDragButton = true;

				if ( mouseOverContinue && askForUsername && !escapeBool ) { if ( index == 0 ) break; for ( int x = 0; x < 10; x++ ) { if ( !strcmp(rankings[x].username, playerName) ) { nameError = true; break; } } if ( !nameError ) askForUsername = false; }
				if ( mouseOverSound    && escapeBool      )  { muteSound = !muteSound; al_set_mixer_playing(al_get_default_mixer(), !muteSound); redrawEsc = true; }
				if ( escapeBool        && !askForUsername )  { break;	   }
				if ( mouseOverInit     && !askForUsername )  { select = 4; }
				if ( mouseOverRanking  && !askForUsername )  { al_pause_event_queue(queue, true); showRanking(bebasBig, bebasMedium, bebasRanking, bebasSmall, playerName, display, r, g, b); al_pause_event_queue(queue, false); }
				if ( mouseOverAjuda    && !askForUsername )  { al_pause_event_queue(queue, true); showAjuda(bebasBig, bebasMedium, bebasRanking, bebasSmall, display, r, g, b); al_pause_event_queue(queue, false); }
				if ( mouseOverSair     && !askForUsername )  { select = 3; }
			break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
				if (mouseDragButton) printf("New volume: %.2f\n", volume);
				mouseDragButton = false;
			break;
				
			case ALLEGRO_EVENT_KEY_CHAR: key = event.keyboard.keycode;
				
				if ( askForUsername ) {
					if ( nameError ) nameError = false;
					if ( key >= ALLEGRO_KEY_A && key <= ALLEGRO_KEY_Z && index < 19 ) { playerName[index++] = 96+key; playerName[index] = '\0'; }
					if ( key == ALLEGRO_KEY_BACKSPACE && index > 0  ) { playerName[index] = '\0'; playerName[--index] = '\0'; }
					if ( key == ALLEGRO_KEY_ENTER && askForUsername ) { if ( !index ) break; for ( int x = 0; x < 10; x++ ) {  if ( !strcmp(rankings[x].username, playerName) ) { nameError = true; break; } } if ( !nameError ) askForUsername = false; }
				} else {
					select     = key == ALLEGRO_KEY_ENTER  ? 4 : select;
					escapeBool = key == ALLEGRO_KEY_ESCAPE ? !escapeBool : escapeBool;
				}
					
			break;
								
			default: printf("Unsupported event received: %d\n", event.type); break;
		}

		if (redraw && al_is_event_queue_empty(queue)) {

			if ( !bg ) { sprintf(name, "./source/menubg/1/1_000%03d.jpeg", frame%76); bg = al_load_bitmap(name); }

			al_draw_bitmap(bg, 0, 0, 0);
			al_destroy_bitmap(bg); bg = NULL;
			printFps(bebasSmall, frame, al_get_timer_count(timer));

			if ( frame > 120 ) {
				flash = flash < 0 ? 40 : flash;

				if ( redrawFront ) {
					redrawFront = false;
					redrawFrontBitmap(bebasBig, bebasMedium, frontBitmap, &initAnimation, hoverInit, hoverRanking, hoverSair, hoverAjuda, display, tmp);
					counter++;
					printf("\rredrawed %02d, %02d, %02d", counter, updateScreen, initAnimation); fflush(stdout);
				} al_draw_bitmap(frontBitmap, 0, 0, 0);

				if ( askForUsername ) {
					al_draw_filled_rectangle(0,0,width, height, al_map_rgba_f(0,0,0,.9));

					al_draw_text(bebasMedium, branco, (800-al_get_text_width(bebasMedium, "Digite seu nome:"))/2, (250-70)/2, 0, "Digite seu nome:");
					al_draw_filled_rectangle(150, 390, 650, 460, branco);

					if ( nameError ) { al_draw_text(bebasRanking, al_map_rgb_f(1,.2,0), 266, 340, 0, "este nome já existe!" ); }

					al_draw_text(bebasMedium, nameError ? al_map_rgb_f(1,0,0) : al_map_rgb(0,0,0), 160, 390, 0, playerName);
					al_draw_text(bebasRanking, continueHover ? al_map_rgb(20,135,90) : branco, (800-al_get_text_width(bebasRanking, "continuar"))/2, 500, 0, "continuar");			

				}

				if ( redrawEsc ) { 
					redrawEscapeBitmap( bebasBig, bebasMedium, bebasSmall, escape, soundBitmap, muteSound, volume, display, 0, "Aha - Take me On Instrumental", 0, 0); al_draw_bitmap(escape, 0, 0, 0);
					redrawEsc = false;
				} else if ( escapeBool ) {
					al_draw_bitmap(escape, 0, 0, 0);
				}
				if ( frame < 160) {
					flashScreen(&flash, 1);
				}

			} else {
				flash-=2.8;
				al_draw_scaled_bitmap(bmp, 0,0,600,600,400-tt(&yo, &xo)/2,-400-flash, tt(&yo, &xo), 550,0 );
			} 
		


			al_flip_display();
			redraw = false; frame++;
		}
		
	}
	al_destroy_timer(timer); al_destroy_event_queue(queue);
	al_destroy_bitmap(tmp); al_destroy_bitmap(escape);
	al_destroy_bitmap(soundBitmap[0]); al_destroy_bitmap(soundBitmap[1]); al_destroy_bitmap(frontBitmap); al_destroy_bitmap(front); al_destroy_bitmap(bmp);
	puts("Done cleanup...");

	if ( select == 4 ) al_destroy_audio_stream(mus);
	return select;
}


void  showRanking ( ALLEGRO_FONT *bebasBig, ALLEGRO_FONT *bebasMedium, ALLEGRO_FONT *bebasRanking, ALLEGRO_FONT *bebasSmall, char playerName[], ALLEGRO_DISPLAY *display, unsigned short r[], unsigned short g[], unsigned short b[] ) {

	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT event;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_BITMAP *trophee = al_load_bitmap("./source/menu/trophee.png");
	ALLEGRO_BITMAP *BG = NULL;

	bool redraw = true, sair = false;
	int key;

	FILE *rankFile = NULL;
	Rank rankings[10];
	rankFile = fopen("ranking.txt", "r");
	if ( !rankFile ) {
		createRanking();
		rankFile = fopen("ranking.txt", "r");
	}

	float xx = 0, yy = 0, flash = 40, rst = 0;
	unsigned int frame = 0;
	timer = al_create_timer(1.0 / FPS);
	queue = al_create_event_queue();
	

	al_register_event_source( queue, al_get_timer_event_source(timer)   	 );
	al_register_event_source( queue, al_get_display_event_source(display) 	 );
	al_register_event_source( queue, al_get_keyboard_event_source()        	 );

	fread(&rankings, sizeof(rankings), 1, rankFile);
	al_start_timer(timer);

	while ( !sair ) {
		al_wait_for_event(queue, &event);

		switch ( event.type ) {
			case ALLEGRO_EVENT_TIMER: redraw = true;
			break;

			case ALLEGRO_EVENT_KEY_CHAR: key = event.keyboard.keycode;
			if (key == 59) { sair = true; }
				

			default: printf("Unsupported event received: %d\n", event.type); break;
		}

		if ( redraw && al_is_event_queue_empty(queue) ) {

			drawGameBackground(BG, &rst, xx, yy, frame, r, g, b);
			printFps(bebasSmall, frame, (int)al_get_timer_count(timer));

			al_draw_text(bebasBig, branco, (800-al_get_text_width(bebasBig, "ranking:"))/2, 25, 0, "ranking:");
			al_draw_filled_rectangle(148,110,651,538,al_map_rgba_f(0,0,0,.5));
			for ( int i = 0; i < 10; i++ ) {
				al_draw_textf(bebasRanking, strcmp(rankings[i].username, playerName) ? branco : al_map_rgb_f(1,0,0), 175, 110+41*i, 0, "%s", rankings[i].username);
				al_draw_textf(bebasRanking, strcmp(rankings[i].username, playerName) ? branco : al_map_rgb_f(1,0,0), 450, 110+41*i, 0, "%d", rankings[i].points);
				if ( i < 3 ) al_draw_tinted_scaled_bitmap(trophee, i == 0 ? al_map_rgb(255,255,255) : i == 1 ? al_map_rgb(0,255,0) : al_map_rgb(255,127,50),0,0, 17, 27, 610, 114+40*i, 26, 37, 0);
			}
			al_draw_text(bebasMedium, branco, 140, 531, 0, "Pressione esc para voltar" );
			if ( flash > 0 ) flashScreen(&flash, 1);
			al_flip_display(); frame++;
			redraw = false;
		}
	}
	al_destroy_bitmap(trophee);
	al_destroy_bitmap(BG);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	fclose(rankFile);
}


void  createBlock ( ALLEGRO_FONT *bebasMedium, unsigned int number, ALLEGRO_BITMAP *block, float brightness ) { 

	al_set_target_bitmap(block);

	if ( number == 0 ) { al_clear_to_color(al_map_rgb(59,60,61)); return; }

	char numberName[10] = "";
	sprintf(numberName, "%d", number);
	ALLEGRO_COLOR color = number == 2 ? al_map_rgb(211, 84, 194) : number == 4 ? al_map_rgb(103,209,73) : number == 8 ? al_map_rgb(64,202,200) : number == 16 ? al_map_rgb(66,129,208) : number == 32 ? al_map_rgb(222,96,74) : number == 64 ? al_map_rgb(137,123,246) : number == 128 ? al_map_rgb(200,200,200) : al_map_rgb(number, number, number);
	al_clear_to_color(color);
	if ( number >= 1024 ) { al_draw_rectangle(0, 0, 69,69, al_map_rgb(251,201,51), 8); }

	int numberSize = floor(log10(number)+1);

	ALLEGRO_FONT *font = al_load_ttf_font("./source/BebasNeue.ttf", 54, 4);

	if ( numberSize > 3 ) {
		al_destroy_font(font);
		font = al_load_ttf_font("./source/BebasNeue.ttf", 54-(numberSize-3)*8, 4);
	}
	al_draw_text(font, al_map_rgb(250,250,250), (69-al_get_text_width(font, numberName))/2, (69-al_get_font_line_height(font))/2,0, numberName );
	

	al_destroy_font(font);
	if ( brightness > 0 ) { al_draw_filled_rectangle(0,0,69,69,al_map_rgba_f(brightness,brightness,brightness,brightness)); }
}


void  createAnimation ( Animation Anim[99], int b1x, int b1y, int b1value, int b2x, int b2y, int b2value, int b3x, int b3y, int b3value, int destX, int destY, int destvalue, int *index  ) {

	//printf(" x: %d y: %d ", b3x, b3y);
	//printf(" destx: %d desty: %d \n", destX, destY);
	printf("time: %d\n", *index);
	Anim[*index].destBlockX = destX;
	Anim[*index].destBlockY = destY;
	Anim[*index].destValue = destvalue;
	Anim[*index].b1X = b1x;
	Anim[*index].b1Y = b1y;
	Anim[*index].b1Value = b1value;
	Anim[*index].b2X = b2x;
	Anim[*index].b2Y = b2y;
	Anim[*index].b2Value = b2value;
	Anim[*index].b3X = b3x;
	Anim[*index].b3Y = b3y;
	Anim[*index].b3Value = b3value;
}


void  copyMatrix ( unsigned int matrixEntrada[8][8], unsigned int matrixsaida[8][8] ) {

	for ( int i = 0; i <= 7; i++ ) {
	    for ( int j = 0; j <= 7; j++ ) {
			matrixsaida[i][j] = matrixEntrada[i][j];
        }
	} printf("matrix copied ");

}


void  verifyNearbyBlocks ( Animation Anim[99], unsigned int matrix[99][8][8], short x, short y, ALLEGRO_BITMAP *block, ALLEGRO_BITMAP *bg, unsigned long *points, unsigned int *biggest, unsigned int *animation ) {
	
	//printf(" x: %d y: %d \n", x, y);
	if ( matrix[*animation][y][x] == 0 || x < 0 || y < 0 || x > 6 || y > 6 ) return;
	if ( matrix[*animation][y][x] == matrix[*animation][y][x-1] ) {
		//3 sides
		if ( matrix[*animation][y][x] == matrix[*animation][y][x+1] && matrix[*animation][y][x] == matrix[*animation][y+1][x] ) { matrix[*animation][y][x] = matrix[*animation][y][x]*8; *points += matrix[*animation][y][x]; *biggest = *biggest < matrix[*animation][y][x] ? matrix[*animation][y][x] : *biggest; createAnimation(Anim, x-1,y,matrix[*animation][y][x-1],x+1,y,matrix[*animation][y][x+1],x,y+1,matrix[*animation][y+1][x],x,y,matrix[*animation][y][x]/8, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][y][x-1] = 0; matrix[*animation][y][x+1] = 0; matrix[*animation][y+1][x] = 0; printf("3 blocks cleared %d %d\n", x, y); /* drawBlock(x-1, y, x, y, block, bg); drawBlock(x+1, y, x, y, block, bg); drawBlock(x, y+1, x, y, block, bg); */ verifyNearbyBlocks( Anim, matrix, x, y , block, bg, &*points, &*biggest, &*animation); }
		//dos 2 lados
		else if ( matrix[*animation][y][x] == matrix[*animation][y][x+1] ) { matrix[*animation][y][x] = matrix[*animation][y][x]*4; *points += matrix[*animation][y][x]; *biggest = *biggest < matrix[*animation][y][x] ? matrix[*animation][y][x] : *biggest; createAnimation(Anim, x-1,y,matrix[*animation][y][x-1],x+1,y,matrix[*animation][y][x+1],-1,-1,-1,x,y,matrix[*animation][y][x]/4, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][y][x-1] = 0; matrix[*animation][y][x+1] = 0; printf("both sides cleared %d %d\n", x, y);/*  drawBlock(x+1, y, -1, -1, block, bg);  drawBlock(x-1, y, x, y, block, bg); */ verifyNearbyBlocks( Anim, matrix, x, y , block, bg, &*points, &*biggest, &*animation); }
		//embaixo and left
		else if ( matrix[*animation][y][x] == matrix[*animation][y+1][x] ) { matrix[*animation][y][x] = matrix[*animation][y][x]*4; *points += matrix[*animation][y][x]; *biggest = *biggest < matrix[*animation][y][x] ? matrix[*animation][y][x] : *biggest; createAnimation(Anim, x-1,y,matrix[*animation][y][x-1],-1,-1,-1,x,y+1,matrix[*animation][y+1][x],x,y,matrix[*animation][y][x]/4, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][y+1][x] = 0; matrix[*animation][y][x-1] = 0; printf("below and left  %d %d\n", x, y); /* drawBlock(x-1, y, -1, -1, block, bg); drawBlock(x, y+1, x, y, block, bg); */ verifyNearbyBlocks( Anim, matrix, x, y , block, bg, &*points, &*biggest, &*animation);  }
		//lado esquerdo
		else { matrix[*animation][y][x] = matrix[*animation][y][x]*2; *points += matrix[*animation][y][x]; *biggest = *biggest < matrix[*animation][y][x] ? matrix[*animation][y][x] : *biggest; createAnimation(Anim, x-1,y,matrix[*animation][y][x-1],-1,-1,-1,-1,-1,-1,x,y,matrix[*animation][y][x]/2, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][y][x-1] = 0; printf("left %d %d\n", x, y); /* drawBlock(x-1, y, x, y, block, bg); */ verifyNearbyBlocks( Anim, matrix, x, y , block, bg, &*points, &*biggest, &*animation); }
		
	} else if ( matrix[*animation][y][x] == matrix[*animation][y][x+1] ) {
		//embaixo and right
		if ( matrix[*animation][y][x] == matrix[*animation][y+1][x] ) { matrix[*animation][y][x] = matrix[*animation][y][x]*4; *points += matrix[*animation][y][x]; *biggest = *biggest < matrix[*animation][y][x] ? matrix[*animation][y][x] : *biggest; createAnimation(Anim,-1,-1,-1,x+1,y,matrix[*animation][y][x+1],x,y+1,matrix[*animation][y+1][x],x,y,matrix[*animation][y][x]/4, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][y+1][x] = 0; matrix[*animation][y][x+1] = 0; printf("below and right  %d %d\n", x, y); /* drawBlock(x+1, y, -1, -1, block, bg); drawBlock(x, y+1, x, y, block, bg); */ verifyNearbyBlocks( Anim, matrix, x, y , block, bg, &*points, &*biggest, &*animation);  }
		//lado direito
		else { matrix[*animation][y][x] = matrix[*animation][y][x]*2; *points += matrix[*animation][y][x]; *biggest = *biggest < matrix[*animation][y][x] ? matrix[*animation][y][x] : *biggest; createAnimation(Anim, -1,-1,-1,x+1,y,matrix[*animation][y][x+1],-1,-1,-1,x,y,matrix[*animation][y][x]/2, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][y][x+1] = 0; printf("right %d %d\n", x, y); /* drawBlock(x+1, y, x, y, block, bg); */ verifyNearbyBlocks( Anim, matrix, x, y , block, bg, &*points, &*biggest, &*animation); }
	}
	//embaixo
	if ( matrix[*animation][y][x] == matrix[*animation][y+1][x] ) { matrix[*animation][y+1][x] = matrix[*animation][y][x]*2; *points += matrix[*animation][y+1][x]; *biggest = *biggest < matrix[*animation][y+1][x] ? matrix[*animation][y+1][x] : *biggest; createAnimation(Anim, -1,-1,-1,-1,-1,-1,x,y,matrix[*animation][y][x],x,y+1, matrix[*animation][y+1][x]/2, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][y][x] = 0; printf("below %d %d\n", x, y); /* drawBlock(x, y, x, y+1, block, bg); */ verifyNearbyBlocks( Anim, matrix, x, y+1, block, bg, &*points, &*biggest, &*animation);  }
	//gravity
	for ( int o = 6; o > 0; o-- ) {
		for ( int t = -1; t < 2; t++ ) {
			if ( o < 0 || x+t < 0 || o-1 < 0 || x+t > 6 || o > 7 ) continue;
			if ( matrix[*animation][o][x+t] == 0 && matrix[*animation][o-1][x+t] != 0 ) { matrix[*animation][o][x+t] = matrix[*animation][o-1][x+t]; createAnimation(Anim, -1,-1,-1,-1,-1,-1,x+t,o-1,matrix[*animation][o-1][x+t],x+t,o, 0, &*animation); *animation+=1; copyMatrix(matrix[*animation-1], matrix[*animation]); matrix[*animation][o-1][x+t] = 0; printf("gravity %d %d\n", x+t, o); /* drawBlock(x+t, o-1, x+t, o, block, bg); */ verifyNearbyBlocks( Anim, matrix, x+t, o, block, bg, &*points, &*biggest, &*animation ); } //gravidade
		}
	}
}


void  drawBlock ( ALLEGRO_FONT *bebasMedium, unsigned int matrix[99][8][8], short oldY, short oldX, short newY, short newX, short time, ALLEGRO_BITMAP *block, ALLEGRO_BITMAP *bg, short override ) {
	
	if ( override != -1 ) { 
		printf("1: %d 2: %d time: %d override: %d\n", oldY, oldX, time, override);
		createBlock(bebasMedium, override, block, 0); al_set_target_bitmap(bg);
		al_draw_bitmap(block, oldY*72+1, oldX*72+2, 0);
		return;
	}

	printf("1: %d 2: %d time: %d == %d\n", oldY, oldX, time, matrix[time][oldX][oldY]);
	createBlock(bebasMedium, matrix[time][oldX][oldY], block,0); al_set_target_bitmap(bg);
	al_draw_bitmap(block, oldY*72+1, oldX*72+2, 0);

	if ( newX == -1 ) return;
	createBlock(bebasMedium, matrix[time][newX][newY], block,0); al_set_target_bitmap(bg);
	al_draw_bitmap(block, newY*72+1, newX*72+2, 0);
}


void  playBlockSound ( ALLEGRO_AUDIO_STREAM *blockSound, int number ) {
	char name[50];
	if ( number != -1 )	sprintf(name, "./source/mus/stone%d.ogg", number);
	blockSound = al_load_audio_stream(name, 4, 1024);
	al_attach_audio_stream_to_mixer(blockSound, al_get_default_mixer());/*  al_set_audio_stream_playmode(blockSound, ALLEGRO_PLAYMODE_ONCE); */
	al_set_audio_stream_gain(blockSound, 2);
}


short placeBlock ( ALLEGRO_FONT *bebasMedium, Animation Anim[99], unsigned int matrix[99][8][8], short x, short y, int number, ALLEGRO_BITMAP *block, ALLEGRO_BITMAP *bg, ALLEGRO_DISPLAY *display, unsigned long *points, unsigned short *tallest, unsigned short *tallestC, unsigned int *biggestPowerOfTwo, unsigned int *animation ) {

	al_set_target_bitmap(bg);
	if ( matrix[*animation][y][x] != 0 ) {
		for ( int k = 6; k >= 0; k-- ) {
			if ( k == 0 && matrix[*animation][k][x] != 0 ) return 1;
			if ( matrix[*animation][k][x] != 0 ) continue;
			matrix[*animation][k][x] = number; break;
		}
	} else { matrix[*animation][y][x] = number;	}
	
	drawBlock(bebasMedium, matrix, x, y, -1, -1, *animation, block, bg, -1);
	
	verifyNearbyBlocks(Anim, matrix, x, y, block, bg, &*points, &*biggestPowerOfTwo, &*animation);

	*tallest = 0;
	for ( int i = 0; i < 7; i++ ) {
		if ( *tallest != 0 ) break;
		for ( int j = 0; j < 7; j++ ) {
			if ( matrix[*animation][i][j] != 0 ) {
				*tallest = i; *tallestC = j;
				break;
			}
		}
	} al_set_target_bitmap(al_get_backbuffer(display));
	return 0;
}


void  drawGameBackground ( ALLEGRO_BITMAP *BG, float *rst, float xx, float yy, unsigned int frame, unsigned short r[], unsigned short g[], unsigned short b[] ) {

	char name[50];
	short t = ceil(xx*100);

	al_clear_to_color(bgcolor);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);

	sprintf(name, "./source/menubg/2/2_000%03d.jpeg", frame%43); BG = al_load_bitmap(name);
	al_draw_bitmap(BG, 0, 0, 0); al_destroy_bitmap(BG);
	
	for (int p = 0, s = 0; s < height/1.5-15; p++) {
		*rst+=.08;
		s = 20*p+((int)*rst%20);
		t+=4;
		al_draw_triangle(tR('x', 0, s, yy, xx, width, height),tR('y', 0, s, yy, xx, width, height),tR('x', 120, s, yy, xx, width, height),tR('y', 120, s, yy,xx, width, height),tR('x', 240, s, yy, xx, width, height),tR('y', 240, s, yy, xx, width, height), al_map_rgb(r[t%305], g[t%305], b[t%305]), 1.4+p/8) ;
	}

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
}


void  drawFirework ( ALLEGRO_BITMAP *bmp, int frame, int seed, float speed, ALLEGRO_DISPLAY *display ) {

	al_set_target_bitmap(bmp);
	al_clear_to_color(al_map_rgba_f(0,0,0,0));
	for ( int i = 0; i < seed; i++ ) {
		al_draw_line(tR('x', 360/seed*i, frame*frame/10, 0, 0, 400, 400), tR('y', 360/seed*i, frame*frame/10, 0, 0, 400, 400), tR('x', 360/seed*i, 1+speed*frame, 0, 0, 400, 400), tR('y', 360/seed*i, 1+speed*frame, 0, 0, 400, 400), al_map_rgb(255-frame/1.2,255-1.5*frame,255-3*frame), 60/(frame+1) );
	} al_set_target_bitmap(al_get_backbuffer(display));	
}


bool  saveRanking ( char playerName[20], unsigned long points ) {

	FILE *rankFile = fopen("ranking.txt", "r");
	if ( !rankFile ) {
		createRanking();
		rankFile = fopen("ranking.txt", "r");
	}
	Rank rankings[10];

	Rank new;
	new.points = points;
	sprintf(new.username, playerName);

	fread(&rankings, sizeof(rankings), 1, rankFile);

	bool entrarRanking = false;

	for ( int i = 0; i < 10 && !entrarRanking; i++ ) {
			if ( rankings[i].points <= points ) {
				for ( int x = 9; x > i; x-- ) {
					rankings[x] = rankings[x-1];
				}
				rankings[i] = new;
				entrarRanking = true;
			}
	}
	fclose(rankFile);
	rankFile = fopen("ranking.txt", "w");
	fwrite(&rankings, sizeof(rankings), 1, rankFile);
	fclose(rankFile);
	return entrarRanking;
} 


short endGame ( ALLEGRO_FONT *bebasBig, ALLEGRO_FONT *bebasMedium, ALLEGRO_FONT *bebasRanking, ALLEGRO_FONT *bebasSmall, char playerName[20], ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *bg, ALLEGRO_BITMAP *BG, ALLEGRO_BITMAP *grid, ALLEGRO_BITMAP *escape, ALLEGRO_AUDIO_STREAM *mus, unsigned long pontos, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, int soundBitmap[2], unsigned short r[],  unsigned short g[],  unsigned short b[], ALLEGRO_TIMER *timer1Sec ) {
	
	char name[50];
	mus = al_load_audio_stream("./source/mus/Rick Astley - Never Gonna Give You Up.opus", 4, 1024);
	sprintf(name, "Pontos: %d", pontos);

	bool 		 	sairHover = false, menuHover = false, continuarHover = false, enterName = false, inRanking = saveRanking(playerName,pontos), notEnd = true, redraw = true, muteSound = false, escapeBool = false, redrawEsc = false, mouseDragButton = false;
	unsigned long   points = 0, frame = 0;
	float 			fire = -1, continueAnimation = 0, fireworkFlash = 30, volume = 0.4, xx=-pi/2, yy=-pi/2, flash = 40, rst = 0, op = 1, an2 = 0, an = 0, off = 0, l = 0;
	unsigned short  selection, fireworkRnd = rand()%10+5, key = 0, x, y, playingNow = 2, textWidth = (short) al_get_text_width(bebasBig, name), bebasBigHeight = (short) al_get_font_line_height(bebasBig);
	
	char *phrases[] = { "você é fraco", "ruim", "quase mais ou menos", "quase bom", "menos que bom", "bom", "agora piorou", "menos pior", "melhorou", "gostei", "interessante", "poderia melhorar tb né", "nada menos que a obrigação", "poderia ter sido pior", "talvez na proxima", "tá bom agora gostei", "top", "top dos top", "mais que top", "topzera", "além de top", "TOP P KRL" };


	Rank                  rankings[10];
	ALLEGRO_SAMPLE 		  *bit 			    =   al_load_sample("./source/mus/bit.ogg");
	ALLEGRO_AUDIO_STREAM  *fireworkAudio    =   al_load_audio_stream("./source/mus/firework.ogg", 4, 1024 );
	ALLEGRO_BITMAP 		  *phraseBitmap     =   al_create_bitmap(1000, al_get_font_line_height(bebasBig)+20);
	ALLEGRO_BITMAP 		  *firework		    =   al_create_bitmap(400,400);
	ALLEGRO_BITMAP 		  *continueBitmap   =   al_create_bitmap(width, height);
	ALLEGRO_BITMAP 		  *entrouRanking    =   al_create_bitmap(800, 150);
	ALLEGRO_BITMAP 		  *pointsBitmap     =   al_create_bitmap(textWidth, bebasBigHeight);
	ALLEGRO_BITMAP        *trophee          =   al_load_bitmap("./source/menu/trophee.png");
	FILE 				  *rankFile         =   fopen("ranking.txt", "r");


	if ( !rankFile ) {
		createRanking();
		rankFile = fopen("ranking.txt", "r");
	}

	fread(&rankings, sizeof(rankings), 1, rankFile);

	puts("playing rick roll");
	al_attach_audio_stream_to_mixer(mus, al_get_default_mixer()); al_set_mixer_gain(al_get_default_mixer(), volume); al_set_audio_stream_playmode(mus, ALLEGRO_PLAYMODE_LOOP);

	al_set_target_bitmap(pointsBitmap);
	al_draw_textf(bebasBig, branco, 0,0,0, "pontos: %d", pontos);

	al_set_target_bitmap(entrouRanking);
	al_draw_filled_rectangle(0,0,800,100, al_map_rgba_f(0,0,0,.4));
	al_draw_text(bebasBig, branco, 106, 7, 0, "Você entrou no ranking!");

	al_set_target_bitmap(phraseBitmap);
	al_draw_text(bebasBig, branco, (1000-al_get_text_width(bebasBig, phrases[0]))/2, 0, 0, phrases[0]);
	al_set_target_bitmap(al_get_backbuffer(display));

	redrawEscapeBitmap(bebasBig, bebasMedium, bebasSmall, escape, soundBitmap, muteSound, volume, display, 0, "Rick Astley - Never Gonna Give You Up", 0, 0);

	al_set_timer_count(timer, 0);
	while ( notEnd ) {
		al_wait_for_event( queue, &event );

		switch ( event.type ) {
			case ALLEGRO_EVENT_TIMER: redraw = true;
			break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE: notEnd = false;

			case 10: break;
			case 12: break;

			case ALLEGRO_EVENT_MOUSE_AXES: x = event.mouse.x; y = event.mouse.y;
				if ( mouseOverVolumeBar   && mouseDragButton ) { volume = (((float)x)-325)/240; volume = volume < 0 ? 0 : volume; al_set_audio_stream_gain(mus, volume); redrawEsc = true; }
				if ( mouseOverContinuar   && !escapeBool &&  enterName ) { continuarHover = true; }  else  { continuarHover = false; }
				if ( mouseOverMenu        && !escapeBool && !enterName ) { menuHover = true; }  else  { menuHover = false; }
				if ( mouseOverSairEndGame && !escapeBool && !enterName ) { sairHover = true; }  else  { sairHover = false; }
			break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: x = event.mouse.x; y = event.mouse.y;
				if ( event.mouse.button != 1 ) break;
				if ( mouseOverVolumeBar   && !mouseDragButton ) { mouseDragButton = true; }
				if ( mouseOverSound       && escapeBool )  { muteSound = !muteSound; al_set_mixer_playing(al_get_default_mixer(), !muteSound); redrawEsc = true; redraw = true; al_resume_timer(timer); }
				if ( mouseOverContinuar   && !escapeBool &&  enterName ) { continueAnimation = 90; }
				if ( mouseOverMenu        && !escapeBool && !enterName ) { notEnd = false; selection = 0; }
				if ( mouseOverSairEndGame && !escapeBool && !enterName ) { notEnd = false; selection = 1; }
			break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_UP: x = event.mouse.x; y = event.mouse.y;
				if (mouseDragButton) printf("New volume: %.2f\n", volume);
				mouseDragButton = false;
			break;
				
			case ALLEGRO_EVENT_KEY_CHAR: key = event.keyboard.keycode;
				escapeBool = key == 59 ? !escapeBool : escapeBool; redraw = true;
			break;

			default: printf("Unsupported event received: %d\n", event.type); break;
		}

		if ( redraw && al_is_event_queue_empty(queue) ) {

			drawGameBackground(BG, &rst, xx, yy, frame, r, g, b); xx+= .01; yy += .011;
			
			al_draw_tinted_bitmap(grid, al_map_rgba_f(op,op,op,op), 148, 96, 0);  al_draw_tinted_bitmap(bg, al_map_rgba_f(op,op,op,op), 148, 96, 0);

			al_draw_filled_rectangle(0, 0, width, height, al_map_rgba_f(0,0,0,.35));

			if ( inRanking && (al_get_timer_count(timer)-30) > 78 ) {

				if ( fire == -1 ) { al_attach_audio_stream_to_mixer(fireworkAudio, al_get_default_mixer()); fire = 0; }
				

				if ( (al_get_timer_count(timer)-107) < 40 ) {
					al_draw_line(200, height-9*(al_get_timer_count(timer)-107), 200, height-10*(al_get_timer_count(timer)-107), al_map_rgb(250,190,20), 60/(al_get_timer_count(timer)-107) ); 
					al_draw_line(600, height-9*(al_get_timer_count(timer)-107), 600, height-10*(al_get_timer_count(timer)-107), al_map_rgb(250,190,20), 60/(al_get_timer_count(timer)-107) ); 
				} else if ( (al_get_timer_count(timer)-107) > 77 ) {
				
					if ( fire < 40 ) { if ( fireworkFlash > 0 ) flashScreen(&fireworkFlash, 5);  drawFirework(firework, fire, fireworkRnd, 5, display); al_draw_bitmap(firework, 0,10,0); drawFirework(firework, fire++, fireworkRnd, 5, display); al_draw_bitmap(firework, 400,10,0); enterName = true; }
				}
			}

			if ( enterName && continueAnimation == 0 ) {

				al_set_target_bitmap(continueBitmap); al_clear_to_color(al_map_rgba_f(0,0,0,0));
				al_draw_filled_rectangle(148,110,651,538,al_map_rgba_f(0,0,0,.5));
				for ( int i = 0; i < 10; i++ ) {
					al_draw_textf(bebasRanking, strcmp(rankings[i].username, playerName) ? branco : al_map_rgb_f(1,0,0), 175, 110+41*i, 0, "%s", rankings[i].username);
					al_draw_textf(bebasRanking, strcmp(rankings[i].username, playerName) ? branco : al_map_rgb_f(1,0,0), 450, 110+41*i, 0, "%d", rankings[i].points);
					if ( i < 3 ) al_draw_tinted_scaled_bitmap(trophee, i == 0 ? al_map_rgb(255,255,255) : i == 1 ? al_map_rgb(0,255,0) : al_map_rgb(255,127,50),0,0, 17, 27, 610, 114+40*i, 26, 37, 0);
				}
				al_draw_bitmap(entrouRanking, 0, 10,0);
				al_draw_filled_rectangle(150, 543, 651, 595, al_map_rgba_f(0, 0, 0, .6));
				al_draw_text(bebasMedium, continuarHover ? al_map_rgb(250,175,20) : branco, 300, 535, 0, "continuar");
				returnToDisplay;
				al_draw_bitmap(continueBitmap,0,0,0);


			} else if ( continueAnimation > 1 ) {
				enterName = false;
				al_draw_tinted_bitmap(continueBitmap, al_map_rgba_f(continueAnimation/90,continueAnimation/90,continueAnimation/90,continueAnimation/90),0,0,0);
				continueAnimation -= 1;
			} else if ( !enterName && continueAnimation == 1 ) {
				inRanking = false;
			} 

			if ( flash < 10 && !inRanking ) { al_draw_filled_rectangle((width-textWidth)/2-50, 185*an-(150*an2), 850-(width-textWidth)/2, bebasBigHeight+215*an-(150*an2), al_map_rgba_f(0,0,0,.6));

				
				if ( an == 1 && pontos == points ) { al_draw_scaled_bitmap(pointsBitmap, 0,0, textWidth, bebasBigHeight, (width-textWidth-(off/555)*20)/2*an, 200-(off/555)*4-(150*an2), textWidth+20*(off/555), bebasBigHeight+8*(off/555), 0  ); tt(&l, &off); }
				else {
					al_draw_textf(bebasBig, al_map_rgba(r[frame%305], g[frame%305], b[frame%305], 255*an), (width-textWidth)/2*an, 200, 0, "Pontos: %d", points );
					if ( an == 1 ) al_play_sample(bit, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				if ( (int)points/15000 != (int)(points-pontos/300)/15000 ) {
					al_set_target_bitmap(phraseBitmap); al_clear_to_color(al_map_rgba_f(0,0,0,0));
					al_draw_text(bebasBig, branco, (1000-al_get_text_width(bebasBig, phrases[((int)points/15000)%22]))/2, 0, 0, phrases[((int)points/15000)%22]);
					returnToDisplay;
				}
				if ( an == 1 ) {
					al_draw_scaled_rotated_bitmap(phraseBitmap, 500,al_get_font_line_height(bebasBig), 805-(width-textWidth)/2, 220-(150*an2), .4,.4, .4, 0 );
					if ( points == pontos  ) {

						an2 += an2 <= 1 ? .03 : 0; if ( an2 > 1 ) an2 = 1;

						al_draw_filled_rectangle(200/(an2+.0001),250,600/(an2+.0001),320,al_map_rgba_f(0,0,0,.6));
						al_draw_textf( bebasRanking, branco, 225/(an2+.0001), 262, 0, "tempo decorrido: %.2d:%.2d:%.2d", al_get_timer_count(timer1Sec)/3600%60, al_get_timer_count(timer1Sec)/60/60%60, al_get_timer_count(timer1Sec)%60 );

						al_draw_filled_rectangle(200,800-400*an2,600,980-490*an2, al_map_rgba_f(0,0,0,.6));
						al_draw_text(bebasMedium, menuHover ? al_map_rgb(40,205,138) : branco, 215*an2, 410, 0, "Menu");
						al_draw_text(bebasMedium, sairHover ? al_map_rgb(255,14,80) : branco, 500/(an2+.0001), 410, 0, "Sair");
					}
				}

				an += an >= 1 ? 0 : .0385; if ( an > 1 ) an = 1;
				points += an >= 1 && points < pontos ? (pontos < 300 ? 5 : ceil(pontos/300)) : 0; if ( points > pontos ) points = pontos;
			}
			op -= op <= .4 ? 0 : .005;

			printFps(bebasSmall, frame, al_get_timer_count(timer));
 
			if ( redrawEsc ) {
				redrawEscapeBitmap( bebasBig, bebasMedium, bebasSmall, escape, soundBitmap, muteSound, volume, display, 0, "Rick Astley - Never Gonna Give You Up.opus",0,0 ); al_draw_bitmap(escape, 0, 0, 0);
				redrawEsc = false;
			} else if ( escapeBool ) {
				al_draw_bitmap( escape, 0, 0, 0 );
			}

			if ( flash > 0 ) { flashScreen(&flash, .5); }
			al_flip_display();
			redraw = false; frame++;
		}
	}

	fclose(rankFile);

	al_destroy_audio_stream(fireworkAudio);
	al_destroy_audio_stream(mus);
	al_destroy_bitmap(phraseBitmap);
	al_destroy_bitmap(firework);
	al_destroy_bitmap(continueBitmap);
	al_destroy_bitmap(entrouRanking);
	al_destroy_bitmap(pointsBitmap);
	al_destroy_bitmap(trophee);

	return selection;
}


short startGame ( ALLEGRO_FONT *bebasBig, ALLEGRO_FONT *bebasMedium, ALLEGRO_FONT *bebasRanking, ALLEGRO_FONT *bebasSmall, char playerName[20], ALLEGRO_DISPLAY *display, unsigned short r[], unsigned short g[], unsigned short b[] ) {

	ALLEGRO_TIMER *timer        	  =  NULL;
	ALLEGRO_TIMER *timer1Sec       	  =  NULL;
	ALLEGRO_EVENT event;
	ALLEGRO_AUDIO_STREAM *mus 		  =  NULL;
	ALLEGRO_AUDIO_STREAM *blockSound  =  NULL;
	ALLEGRO_SAMPLE *death 			  =  al_load_sample("./source/mus/lost.ogg");
	ALLEGRO_EVENT_QUEUE *queue  	  =  NULL;
	ALLEGRO_BITMAP *BG          	  =  NULL;
	ALLEGRO_BITMAP *bg          	  =  al_create_bitmap(504, 504);
	ALLEGRO_BITMAP *grid        	  =  al_create_bitmap(504, 504);
	ALLEGRO_BITMAP *escape      	  =  al_create_bitmap(width, height);
	ALLEGRO_BITMAP *showNextBlock     =  al_create_bitmap(69,69);
	ALLEGRO_BITMAP *nextBlock   	  =  al_create_bitmap(69,69);
	ALLEGRO_BITMAP *blockAux    	  =  al_create_bitmap(69,69);
	ALLEGRO_BITMAP *blankBlock  	  =  al_create_bitmap(69,69);
	ALLEGRO_BITMAP *destBlock  	      =  al_create_bitmap(69,69);
	Animation anim;
	Animation Anim[99];

	bool 		 	returnHover = false, reiniciarHover = false, loaded = false, glowArea = false, notEnd = true, blockDown = false, redraw = true, muteSound = false, escapeBool = false, redrawEsc = false, mouseDragButton = false;
	unsigned int  	frame  = 0, blockNumber = 2, *soundBitmap[2], biggestPowerOfTwo = 512, animIndex = 0, animBefore = 0, showNext = 2;
	unsigned long   points = 0;
	float 			volume = 0.6, blockYposition = 0, blockXPosition = 3, speedOfDescendingBlock = biggestPowerOfTwo/1000+1, animationProgress = 0, xx=-pi/2, yy=-pi/2, flash = 40, rst = 0;
	unsigned short 	render = 0, key = 0, x, y, tallest = 6, tallestColumn = 6, glowAreaValue = 0;
	
	int xMouseClick = blockXPosition;

	unsigned int matrix[99][8][8];

	resetMatrix(matrix);

	mus = al_load_audio_stream("./source/mus/Mike Noise - Low Earth Orbit.opus", 4, 1024);
	al_attach_audio_stream_to_mixer(mus, al_get_default_mixer()); al_set_audio_stream_playmode(mus, ALLEGRO_PLAYMODE_LOOP);
	al_set_audio_stream_gain(mus, volume);
	
	soundBitmap[0]  =  al_load_bitmap( "./source/menu/soundon.png"	 );
	soundBitmap[1]  =  al_load_bitmap( "./source/menu/soundoff.png"  );
	puts( !soundBitmap[0] || !soundBitmap[1] ? "!!!!Failed loading soundBitmap!!!!" : "Sound bitmap loaded");

	puts("Grid drawn...");
	createBlock( bebasMedium, blockNumber, nextBlock, 0 );
	createBlock( bebasMedium, 0, blankBlock, 0 );
	al_set_target_bitmap(grid);
	for ( int x = 0; x < 7; x++ ) {
		for ( int i = 0; i < 7; i++ ) {
			al_draw_bitmap( blankBlock, i*72+2, x*72+2, 0 );
		}
	}
	returnToDisplay;

	timer     = al_create_timer(1.0 / FPS);
	timer1Sec = al_create_timer(1.0);
	queue     = al_create_event_queue();
	
	placeBlock(bebasMedium, Anim, matrix, 0, 0, 0, nextBlock, bg, display, &points, &tallest, &tallestColumn, &biggestPowerOfTwo, &animIndex );

	al_register_event_source( queue, al_get_timer_event_source(timer)     );
	al_register_event_source( queue, al_get_mouse_event_source()   		  );
	al_register_event_source( queue, al_get_display_event_source(display) );
	al_register_event_source( queue, al_get_keyboard_event_source()       );

	redrawEscapeBitmap(bebasBig, bebasMedium, bebasSmall, escape, soundBitmap, muteSound, volume, display, 1, "Mike Noise - Low Earth Orbit.opus", 0, 0);

	puts("All set! Commencing game loop..."); al_start_timer(timer); al_start_timer(timer1Sec);
	while ( notEnd || animIndex != animBefore ) {
		al_wait_for_event( queue, &event );

		switch ( event.type ) {
			case ALLEGRO_EVENT_TIMER: redraw = true;
				if ( escapeBool ) break;
				if ( !blockDown && al_get_timer_count(timer)%60 == 59 && blockYposition == 0 ) { blockDown = true; blockNumber = showNext; showNext = 2 << (int) fmod(rand(), log( biggestPowerOfTwo/32 < 16 ? 16 : biggestPowerOfTwo/32 )/log(2)); createBlock(bebasMedium, blockNumber, nextBlock,0); al_set_target_bitmap(al_get_backbuffer(display)); }
			break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE: notEnd = false; break;

			case 10: break;
			case 12: break;

			case ALLEGRO_EVENT_MOUSE_AXES: x = event.mouse.x; y = event.mouse.y;
				if ( mouseOverVolumeBar && mouseDragButton ) { volume = (((float)x)-325)/240; volume = volume < 0 ? 0 : volume; al_set_audio_stream_gain(mus, volume); redrawEsc = true; }
				if ( mouseOverReturn    && escapeBool ) { returnHover = true;    redrawEsc = true; render = 1; } else { returnHover = false;    if ( render == 1 && escapeBool && !returnHover && !reiniciarHover && !redrawEsc ) { redrawEsc = true; render = 0; } }
				if ( mouseOverReiniciar && escapeBool ) { reiniciarHover = true; redrawEsc = true; render = 1; } else { reiniciarHover = false; if ( render == 1 && escapeBool && !returnHover && !reiniciarHover && !redrawEsc ) { redrawEsc = true; render = 0; } }
			break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: x = event.mouse.x; y = event.mouse.y;
				if ( event.mouse.button != 1 ) break;
				if ( mouseOverVolumeBar && !mouseDragButton ) { mouseDragButton = true; redraw = true; al_resume_timer(timer); }
				if ( mouseOverSound && escapeBool )  { muteSound = !muteSound; al_set_mixer_playing(al_get_default_mixer(), !muteSound); redrawEsc = true; redraw = true; al_resume_timer(timer); }
				
				if ( mouseOverReturn    && escapeBool ) { escapeBool = false; break; }
				if ( mouseOverReiniciar && escapeBool ) { notEnd = false; escapeBool = false; break; }
				
				if ( escapeBool ) break;

				if ( x > 148 && x < 652 && blockDown ) { xMouseClick = ceil((x-148)/72); speedOfDescendingBlock = 40; }
				if ( glowArea ) { puts("cant go there"); glowAreaValue = 60; }
			break;

			case ALLEGRO_EVENT_MOUSE_BUTTON_UP: x = event.mouse.x; y = event.mouse.y;
				if (mouseDragButton) printf("New volume: %.2f\n", volume);
				mouseDragButton = false;
			break;
				
			case ALLEGRO_EVENT_KEY_CHAR: key = event.keyboard.keycode;
				if ( key == 59 ) {
					escapeBool = !escapeBool;
					if ( escapeBool ) {
						al_stop_timer(timer1Sec);
					} else {
						al_resume_timer(timer1Sec);
					}
				}

				speedOfDescendingBlock = key == ALLEGRO_KEY_DOWN ? 40 : speedOfDescendingBlock;

				xMouseClick = key == 82 ? xMouseClick-1 : key == 83 ? xMouseClick+1 : xMouseClick;
				
				xMouseClick = xMouseClick < 0 ? 0 : xMouseClick > 6 ? 6 : xMouseClick;
			break;

			default: printf("Unsupported event received: %d\n", event.type); break;					
		}

		if ( redraw && al_is_event_queue_empty(queue) ) {

			drawGameBackground(BG, &rst, xx, yy, frame, r, g, b);xx+= .01; yy += .011;
			
			if ( glowArea && glowAreaValue > 0 ) { al_draw_rectangle(148,96,652,600,al_map_rgb((glowAreaValue > 255 ? 255 : glowAreaValue),0,0), 4); --glowAreaValue; }
			
			al_draw_tinted_bitmap(grid, al_map_rgba_f(.9,.9,.9,.9), 148, 96, 0);  al_draw_tinted_bitmap(bg, al_map_rgba_f(.9,.9,.9,.9), 148, 96, 0);

			if ( animIndex != animBefore && !escapeBool ) {
				
				anim = Anim[animBefore];
				if ( !loaded ) { createBlock(bebasMedium, ( anim.b1Value == -1 ? anim.b2Value == -1 ? anim.b3Value : anim.b2Value : anim.b1Value ), blockAux,0); al_set_target_bitmap(al_get_backbuffer(display)); loaded = true; }

				if ( anim.b1X != -1 ) { 
					if ( anim.b1X != anim.destBlockX && anim.b1X+anim.b1Y != 0 ) { anim.b1X += anim.destBlockX > anim.b1X ? .06 : -.06;  }
					if ( (int)(anim.b1X +.05) == anim.destBlockX ) { puts("finished 1"); animBefore++; drawBlock(bebasMedium, matrix, anim.destBlockX-1, anim.destBlockY, anim.destBlockX, anim.destBlockY, animBefore, blockAux, bg, -1); if ( anim.b3X != -1 ) { drawBlock(bebasMedium, matrix, anim.b3X, anim.destBlockY+1, -1, -1, animBefore, blockAux, bg, 0); } if ( anim.b2X != -1 ) { drawBlock(bebasMedium, matrix, anim.b2X+1, anim.destBlockY, anim.destBlockX, anim.destBlockY, animBefore, blockAux, bg, -1); } al_set_target_bitmap(al_get_backbuffer(display)); loaded = false; continue; }
					else {
						al_draw_bitmap( blankBlock, (anim.destBlockX-1)*72+149, anim.b1Y*72+98, 0 );
						al_draw_bitmap( blockAux, anim.b1X*72+149, anim.b1Y*72+98, 0 );
					}
				}

				if ( anim.b2X != -1 ) {
					if ( anim.b2X != anim.destBlockX && anim.b2X+anim.b2Y != 0 ) { anim.b2X += anim.destBlockX > anim.b2X ? .06 : -.06;  }
					if ( (int)(anim.b2X+.95) == anim.destBlockX ) { puts("finished 2"); animBefore++; drawBlock(bebasMedium, matrix, anim.b2X+1, anim.destBlockY, anim.destBlockX, anim.destBlockY, animBefore, blockAux, bg, -1); if ( anim.b1X != -1 ) { drawBlock(bebasMedium, matrix, anim.destBlockX-1, anim.destBlockY, -1, -1, animBefore, blockAux, bg, -1); } if ( anim.b3X != -1 ) { drawBlock(bebasMedium, matrix, anim.b3X, anim.destBlockY+1, -1, -1, animBefore, blockAux, bg, 0); } al_set_target_bitmap(al_get_backbuffer(display)); loaded = false; continue; }
					else {
						al_draw_bitmap( blankBlock, (anim.destBlockX+1)*72+149, anim.b2Y*72+98, 0 );
						al_draw_bitmap( blockAux, anim.b2X*72+149, anim.b2Y*72+98, 0 );
					}
				}

				if ( anim.b3X != -1 ) {
					if ( anim.b3Y != anim.destBlockY && anim.b3X+anim.b3Y != 0 ) { anim.b3Y += anim.destBlockY > anim.b3Y ? .06 : -.06;  }
					if ( (int)(anim.b3Y) == anim.destBlockY && anim.b2X+anim.b1X == -2 ) { if ( matrix[animBefore][(int)anim.destBlockY+1][(int)anim.destBlockX] != 0 || anim.destBlockY == 7 ) playBlockSound(blockSound, rand()%6+1 ); puts("finished 3"); animBefore++; drawBlock(bebasMedium, matrix, anim.b3X, anim.destBlockY-1, anim.destBlockX, anim.destBlockY, animBefore, blockAux, bg, -1);  al_set_target_bitmap(al_get_backbuffer(display)); loaded = false; continue; }
					else {
						if ( anim.destValue == 0 ) { al_draw_bitmap( blankBlock, anim.b3X*72+149, (anim.destBlockY+1)*72+26, 0 ); }
						al_draw_bitmap( blankBlock, anim.b3X*72+149, (anim.destBlockY > anim.b3Y ? anim.destBlockY : anim.destBlockY+2)*72+26, 0 );
						al_draw_bitmap( blockAux, anim.b3X*72+149, anim.b3Y*72+98, 0 );
					}
				}
				
				if ( anim.destValue != 0 ) {
					animationProgress +=.015;
					createBlock(bebasMedium, anim.destValue, destBlock, animationProgress); al_set_target_bitmap(al_get_backbuffer(display));
					al_draw_bitmap( destBlock, anim.destBlockX*72+149, anim.destBlockY*72+98, 0 ); yy += .02; xx+=.04; rst+=.01;
				}

				Anim[animBefore] = anim;
			} else { animationProgress = 0; }

			if ( blockDown && !escapeBool && !(animIndex != animBefore ) ) { x = (int) blockXPosition; y = (int)blockYposition/72;
				if ( x > tallestColumn ) {
					if ( xMouseClick >= tallestColumn || y < tallest ) {
						if ( blockXPosition != xMouseClick && matrix[animBefore][y][x] == 0 ) { blockXPosition += blockXPosition < xMouseClick ? 8/24.0 : -8/24.0; glowArea = false; }
					} else { glowArea = true; }
				
				} else {		
					if ( xMouseClick <= tallestColumn || y < tallest ) {
						if ( blockXPosition != xMouseClick && matrix[animBefore][y][x] == 0 ) { blockXPosition += blockXPosition < xMouseClick ? 8/24.0 : -8/24.0; glowArea = false; }
					} else { glowArea = true; }
				}
				
				if ( (int) blockXPosition == xMouseClick && blockXPosition != xMouseClick ) { blockXPosition = (int) blockXPosition; }

				if ( matrix[animBefore][(int)y][x] == blockNumber && matrix[animBefore][(int)y-1][x] == 0 || (int)y == 7 || matrix[animBefore][(int)y][x] != blockNumber && matrix[animBefore][(int)y-1][x] == 0 && matrix[animBefore][(int)y][x] != 0 ) {
					playBlockSound(blockSound, rand()%6+1 );
					printf("-> placing at %d\n", y-1);
					if ( y-1 <= 0 && matrix[animBefore][(int)y][x] != blockNumber ) { notEnd = false; continue; }
					notEnd = placeBlock(bebasMedium, Anim, matrix, x, y-1, blockNumber, nextBlock, bg, display, &points, &tallest, &tallestColumn, &biggestPowerOfTwo, &animIndex) ? 0 : 1;
					blockDown = false;
					continue;
				}				
				al_draw_bitmap(nextBlock, blockXPosition*72+149 , blockYposition+26, 0);
				if ( animIndex == animBefore ) { blockYposition += speedOfDescendingBlock/5.0; }
				
			} else if ( !escapeBool && !(animIndex != animBefore) ) {
				blockXPosition = xMouseClick; blockYposition = 0;
				speedOfDescendingBlock = biggestPowerOfTwo/4096+1 > 40 ? 40 : biggestPowerOfTwo/4096+1;
			}
			al_draw_filled_rectangle(0,0,800,100, al_map_rgba_f(0,0,0,.80));
			
			al_draw_textf( bebasMedium, branco, 50, 10, 0,  "pontos: %lu", points );
			al_draw_textf( bebasRanking, branco, 295, 58, 0, "tempo: %.2d:%.2d:%.2d", al_get_timer_count(timer1Sec)/3600%60, al_get_timer_count(timer1Sec)/60%60, al_get_timer_count(timer1Sec)%60 );
			
			createBlock(bebasMedium, showNext, showNextBlock, 0); returnToDisplay;
			al_draw_scaled_rotated_bitmap(showNextBlock, 34,34,715,45,.6,.6,0,0);
			al_draw_text( bebasRanking, branco, 570, 22.5, 0,  "próximo:");
			printFps(bebasSmall, frame, al_get_timer_count(timer));
			if ( redrawEsc ) {
				al_draw_bitmap( nextBlock, blockXPosition*72+149 , blockYposition+26, 0    );
				redrawEscapeBitmap( bebasBig, bebasMedium, bebasSmall, escape, soundBitmap, muteSound, volume, display, 1, "Mike Noise - Low Earth Orbit", returnHover, reiniciarHover ); al_draw_bitmap(escape, 0, 0, 0);
				redrawEsc = false;
			} else if ( escapeBool ) {
				al_draw_bitmap( nextBlock, blockXPosition*72+149 , blockYposition+26, 0    );
				al_draw_bitmap( escape, 0, 0, 0 );
			}

			if ( flash > 0 ) { flashScreen(&flash, .07); }
			al_flip_display();
			redraw = false; frame++;
			if ( animIndex > 5 && !(animIndex != animBefore && !escapeBool) ) { puts("cansado");
				copyMatrix(matrix[animIndex], matrix[0]);
				animIndex  = 0;
				animBefore = 0;
			}
		}
	} puts("Fim de jogo!");
	al_detach_audio_stream(mus); al_destroy_bitmap(nextBlock); al_destroy_bitmap(blankBlock); al_destroy_bitmap(blockAux); al_destroy_bitmap(destBlock); al_destroy_bitmap(showNextBlock);
	al_stop_timer(timer1Sec);
	al_play_sample(death, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

	usleep(2.95*1000000);
	short sel = endGame(bebasBig, bebasMedium, bebasRanking, bebasSmall, playerName, display, bg, BG, grid, escape, mus, points, timer, queue, event, soundBitmap, r, g, b, timer1Sec);

	al_destroy_sample(death);
	
	al_destroy_audio_stream(mus); al_destroy_bitmap(grid); al_destroy_bitmap(bg); al_destroy_bitmap(escape); al_destroy_event_queue(queue); al_destroy_timer(timer);
	puts("Done cleanup....");
	al_destroy_audio_stream(blockSound);
	al_destroy_timer(timer1Sec);
	return sel;
}


short menuRoutine ( ALLEGRO_DISPLAY *display ) {

	ALLEGRO_BITMAP *image = NULL;
	ALLEGRO_AUDIO_STREAM *muss;
	ALLEGRO_FONT *bebasBig;
	ALLEGRO_FONT *bebasMedium;
	ALLEGRO_FONT *bebasRanking;
	ALLEGRO_FONT *bebasSmall;

	char playerName[20] = "";
	unsigned short r[] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 250, 245, 240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0 };
	unsigned short g[] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 250, 245, 240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned short b[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 250, 245, 240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0 };

	puts("Loading fonts...");
	bebasBig 	 =  al_load_ttf_font_stretch("./source/BebasNeue.ttf", 73, 72, 4);
	bebasMedium  =  al_load_ttf_font_stretch("./source/BebasNeue.ttf", 59, 58, 4);
    bebasRanking =  al_load_ttf_font_stretch("./source/BebasNeue.ttf", 40, 39, 4);
	bebasSmall 	 =  al_load_ttf_font_stretch("./source/BebasNeue.ttf", 21, 20, 4);

	printf("Loading menu music..."); fflush(stdout);

	muss = al_load_audio_stream("./source/mus/Aha - Take me On Instrumental.opus", 4, 1024);
	if ( !muss ) { puts("Menu music not loaded. Aborting"); return 1; } puts(" Done.");
	
	puts("Loading animation...");
	loadingScreen(image, 1.4);

	puts("Playing menu music...");
	
	switch ( displayMenu( bebasBig, bebasMedium, bebasRanking, bebasSmall, playerName, display, muss, r, g, b ) ) {
		case 4:
			puts("iniciar");
			if ( startGame( bebasBig, bebasMedium, bebasRanking, bebasSmall, playerName, display, r, g, b) == 0 ) return 3;
			puts("hmm");
			break;
		case 3:
			puts("sair");
			break;
		case 2:
			puts("ajuda");
			break;
		case 1:
			puts("Ranking");
			break;
	}

	al_destroy_font(bebasBig);
	al_destroy_font(bebasMedium);
	al_destroy_font(bebasRanking);
	al_destroy_font(bebasSmall);
	al_destroy_bitmap(image);
	al_destroy_audio_stream(muss);
	return 0;
}


short startAllegro ( ) {

	if ( !al_init_primitives_addon() 
		|| !al_init_image_addon() 
		|| !al_init() 
		|| !al_install_audio()
		|| !al_reserve_samples(2)
		|| !al_init_acodec_addon()
		|| !al_install_mouse() 
		|| !al_install_keyboard() 
		|| !al_init_font_addon() 
		|| !al_init_ttf_addon() ) {
		fprintf(stderr, "Failed to initialize.");
		return 1;
	} return 0;
}


void  main ( int argc, char *argv[] ) {
	
	ALLEGRO_DISPLAY *display = NULL;

	srand(time(NULL));

	al_set_app_name("2048 The Game");

	puts(startAllegro() ? "!!!!!!!Allegro library NOT STARTED!!!!!!!" : "Allegro library started...");

	display = al_create_display(width, height);

	puts( !display ? "Failed to create display.\n" : "Display created..." );

	puts("Starting menu...");

	if ( menuRoutine( display ) == 3 ) menuRoutine( display );

	al_destroy_mixer(al_get_default_mixer());
	al_destroy_voice(al_get_default_voice());
	al_destroy_display(display);

	return;
}