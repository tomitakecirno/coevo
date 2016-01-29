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
#include<math.h>
#include<stdio.h>
#include<SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#define WINDOW_X	800	/*定義域*/
#define WINDOW_Y	800	/*地域*/
#define PROT_X		600	/*定義域*/
#define PROT_Y		600	/*地域*/


/*ループの時に使うフラグ*/
int end_flag;
void SDL_InitSet(SDL_Surface *window);
static int thread_keyboad(void *data);
void Prot_Frame(SDL_Surface *window);

int main(void){
	/*SDL関係の初期化*/
	end_flag = 1; /*ここでフラグを1にしておく*/
	SDL_Surface *window;
	SDL_InitSet(window);

	/*スレッドを立てる*/

	int Opponent_Size;
	int Opponent_X[Opponent_Size];
	int Opponent_Y[Opponent_Size];
	int Opponent_Nitch[Opponent_Size];
	int gene;

	while(end_flag){
		/*背景を白に*/
		/*エラー*/
		SDL_FillRect(window, NULL, 0x00ffffff);
		/*フレームをプロット*/
		Prot_Frame(window);
		/*画面の変化をウィンドウに反映*/
		SDL_Flip(window);
	}

	return 0;
}
/****************
SDL初期化
*****************/
void SDL_InitSet(SDL_Surface *window){
	if ( SDL_Init(SDL_INIT_VIDEO)  < 0 ){
	        printf("failed to initialize SDL.\n");
	        fprintf(stderr,"%s\n",SDL_GetError());
	        SDL_GetError();
	        exit(-1); 
	}
	/* ウィンドウ生成（800*600、1677万色）*/
	if((window = SDL_SetVideoMode(WINDOW_X, WINDOW_Y, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}
	SDL_Thread *thr_keyboad;
	thr_keyboad = SDL_CreateThread(thread_keyboad,NULL);

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
void Prot_Frame(SDL_Surface *window)
{
	lineColor(window, 
		0, 
		WINDOW_Y/2,
		WINDOW_X,
		WINDOW_Y/2,
		0x000000ff);
	/*Y軸*/
	lineColor(window,
		WINDOW_X/2,
		0,
		WINDOW_X/2,
		WINDOW_Y,
		0x000000ff);
	/*内枠X軸上側*/
	lineColor(window,
		WINDOW_X-PROT_X,
		WINDOW_Y-PROT_Y,
		PROT_X,
		WINDOW_Y-PROT_Y,
		0x000000ff);
	/*内枠X軸下側*/
	lineColor(window, 
		WINDOW_X-PROT_X,
		PROT_Y,
		PROT_X,
		PROT_Y,
		0x000000ff);
	/*内枠Y軸左側*/
	lineColor(window,
		WINDOW_X-PROT_X,
		WINDOW_Y-PROT_Y,
		WINDOW_X-PROT_X,
		PROT_Y,
		0x000000ff);
	/*内枠Y軸右側*/
	lineColor(window, 
		PROT_X,
		WINDOW_Y-PROT_Y,
		PROT_X,
		PROT_Y,
		0x000000ff);
}



	
