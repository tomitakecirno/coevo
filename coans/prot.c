/**********************************
data.datをプロットするプログラム
入力ファイル	:data.dat
敵集団のサイズ	:No
敵個体のx座標	:x[No]
敵個体のx座標	:y[No]
敵集団のニッチ番号	:nitch[No]
世代数		:gene
SDL使用
**********************************/
#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>

#define WINDOW_X	800	/*定義域*/
#define WINDOW_Y	800	/*地域*/
#define PROT_X		600	/*定義域*/
#define PROT_Y		600	/*地域*/
#define END		50


/*ループの時に使うフラグ*/
int end_flag;
int end_count=0;

static int thread_keyboad(void *data);
void Prot_Frame(SDL_Surface *window);

int main(void){
	/*SDL関係の初期化*/
	end_flag = 1; /*ここでフラグを1にしておく*/
	SDL_Window *window;
	char name[10];
	/*SDL初期化*/
	if ( SDL_Init(SDL_INIT_VIDEO)  < 0 ){
	        printf("failed to initialize SDL.\n");
	        fprintf(stderr,"%s\n",SDL_GetError());
	        SDL_GetError();
	        exit(-1); 
	}
	/* ウィンドウ生成（800*600、1677万色）*/
	window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, WINDOW_X,
                                  WINDOW_Y,0);
	if(window == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}
	
	/*スレッドを立てる*/
	SDL_Thread *thr_keyboad;
	thr_keyboad = SDL_CreateThread(thread_keyboad,"keyboard",NULL);
	/*ファイルオープン*/
	while(end_flag){
		/*背景を白に*/
		/*
		SDL_FillRect(window, NULL, 0x00ffffff);
		*/
		/*ファイル操作に関する処理*/
		/*フレームをプロット*/
		/*
		Prot_Frame(window);
		*/
		/*画面の変化をウィンドウに反映*/
		/*
		SDL_Flip(window);
		*/
		/*
		sprintf(name,"./picture/test%d.bmp",end_count);
		if(end_count%10 == 0){
			SDL_SaveBMP(window,name);
		}
		end_count++;
		if(end_count>END){
			end_flag = 0;
		}
		*/
	}
	SDL_Delay(500);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

/****************
キー入力のスレッド
****************/
static int thread_keyboad(void *data){
	SDL_Event event;
	while(end_flag){
		if(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					end_flag = 0;
					printf("end\n");
					break;
				}
				break;
			}
		}
	}
}
/**********
枠を描画
***********/
/*
void Prot_Frame(SDL_Surface *window)
{
	lineColor(window, 
		0, 
		WINDOW_Y/2,
		WINDOW_X,
		WINDOW_Y/2,
		0x000000ff);
	lineColor(window,
		WINDOW_X/2,
		0,
		WINDOW_X/2,
		WINDOW_Y,
		0x000000ff);
	lineColor(window,
		WINDOW_X-PROT_X,
		WINDOW_Y-PROT_Y,
		PROT_X,
		WINDOW_Y-PROT_Y,
		0x000000ff);
	lineColor(window, 
		WINDOW_X-PROT_X,
		PROT_Y,
		PROT_X,
		PROT_Y,
		0x000000ff);
	lineColor(window,
		WINDOW_X-PROT_X,
		WINDOW_Y-PROT_Y,
		WINDOW_X-PROT_X,
		PROT_Y,
		0x000000ff);
	lineColor(window, 
		PROT_X,
		WINDOW_Y-PROT_Y,
		PROT_X,
		PROT_Y,
		0x000000ff);
}
*/



	
