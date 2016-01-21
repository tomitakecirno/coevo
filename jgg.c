#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include"./header/MT.h"

#define INIT		10	/*解集団の初期化範囲*/
#define INIT_OPPOMEMT	50	/*敵集団の初期化範囲*/
#define LIMIT		100	/*定義域*/
#define Y		100	/*地域*/
#define Ns		100	/*初期集団数*/
#define No		100	/*敵集団数*/
#define Np		5	/*親個体数*/
#define Nc		10	/*子個体数*/
#define DEM		2	/*次元数*/
#define T		1	/*ステップサイズ*/
#define END_STEP	200	/*終わるタイミング*/
#define WINDOW_X	800	/*定義域*/
#define WINDOW_Y	800	/*地域*/
#define PROT_X		600	/*定義域*/
#define PROT_Y		600	/*地域*/
#define K		10	/*ニッチの集団数*/

double center[2] = {WINDOW_X/2,WINDOW_Y/2};

typedef struct{
	double n[DEM];
	int flag;
	int win;
	int lose;
	int draw;
	int eval;
	int nitch; /*0...所属ニッチなし 1~...所属しているニッチ番号*/
	int obj[No+1];
	double dis[No+1];
	int Neigh_List2[K];
}Indiv;
typedef struct{
	double n[DEM];
	int flag;
	int win;
	int lose;
	int draw;
	int eval;
	int nitch; /*0...所属ニッチなし 1~...所属しているニッチ番号*/
	int obj[No+1];
	double dis[No+1];
	int Neigh_List2[K];
}Sub_Indiv;

int end_flag = 1;
int true_flag = 1;
int count_nitch=1;
int opponent_number = No;


static int thread_keyboad(void *data);
void creat_child(int pare[],int child[]);
void Numbers(Indiv pare[],Indiv pare2[]);
void Pare_Numbers(Indiv pare[]);
void OneOnOne_Numbers(Indiv pare[],int N);
void select_child(int optimal,int child[],int surv[]);
void Init_Indiv(Indiv pare[],int N);
void sort_eval(Indiv pare[],int N);
void sort_distance(Indiv pare[],int N);
double GetRand32(double pGetMax);
void RexStar(Indiv pare[],Indiv child[],SDL_Surface *window);
void Init_Opponent(Indiv Opponent[]);
double cal_distance(double a_x,double a_y,double b_x,double b_y);
void Prot_Frame(SDL_Surface *window);
void Opponent_Prot(Indiv Opponent[],SDL_Surface *window);
void Unit_Prot(Indiv pop[],SDL_Surface *window,int N);

/***********
メインルーチンmain
***********/
main(){

	/*個体を格納する配列定義*/
	Indiv pop[Ns]; /*解集団配列*/
	Indiv pare[Np]; /*親集団配列*/
	Indiv child[Nc]; /*子集団配列*/
	Indiv surv[Np]; /*残される子を入れておく*/
	Indiv Opponent[No+1]; /*敵集団の配列*/
	
	int i,j,k; /*for文用*/

	init_genrand((unsigned)time(NULL)); /*乱数初期化*/
	Init_Indiv(pop,Ns); /*解集団構造体初期化*/
	Init_Indiv(pare,Np); /*親集団構造体初期化*/
	Init_Indiv(child,Nc); /*子集団構造体初期化*/
	Init_Opponent(Opponent); /*相手集団構造体初期化*/
	/*
	printf("Init後\n");
	for(i=0;i<No;i++){
		for(j=0;j<DEM;j++){
			printf("Opponent[%d].n[%d] = %f\n",i,j,Opponent[i].n[j]);
		}
		printf("Opponent[%d].nitch = %d\n",i,Opponent[i].nitch);
		for(j=0;j<No;j++){
			printf("Opponent[%d].obj[%d] = %d\n",i,j,Opponent[i].obj[j]);
			printf("Opponent[%d].dis[%d] = %f\n",i,j,Opponent[i].dis[j]);
		}
		putchar('\n');
	}
	*/
	/*SDL初期化*/
	SDL_Surface *window; // ウィンドウ（画像）データ、及び、文字列（画像）へのポインタ
	SDL_Event event;
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

	/*初期解生成*/
	for(i=0;i<Ns;i++){
		for(j=0;j<DEM;j++){
			pop[i].n[j] = GetRand32(INIT); /*実数値乱数*/
		}
	}
	/*初期集団からランダムに親を選ぶ*/
	int while_flag = 1;
	int count= 0; /*親の数をカウント*/
	int tmp;
	int end_count = 0;
	while(end_flag){
		/*ウィンドウの（背景）色を変更（白色の矩形で塗りつぶし）*/
		SDL_FillRect(window, NULL, 0x00ffffff);
		
		count = 0;
		while_flag = 1;
		/*親を選ぶ*/
		while(while_flag){

			tmp = genrand_int32() % Ns;
			if(pop[tmp].flag == 0){

				for(j=0;j<DEM;j++){
					pare[count].n[j] = pop[tmp].n[j];
				}
				pare[count].flag = 0;
				pop[tmp].flag = 1;
				count++;
			}
			if(count == Np){
				while_flag = 0;
			}
		}

		/*RexStarにより子個体を生成*/
		RexStar(pare,child,window);
		/*子個体を相手集団と戦わせる*/
		Numbers(child,Opponent);
		/*評価の良い順にソート*/
		sort_eval(child,Nc);
		/*子個体の最良Np個を初期集団へ（世代交代）*/
		count = 0;
		for(i=0;i<Np;i++){
			/*親として選ばれた場所に子個体を入れる*/
			for(j=0;pop[j].flag != 1;j++){}
			pop[j] = child[i];
		}
		/*update入る前*/
		/*子集団の全勝個体を相手集団へ*/
		/*
		printf("Update入る前\n");
		for(i=0;i<No;i++){
			for(j=0;j<DEM;j++){
				printf("Opponent[%d].n[%d] = %f\n",i,j,Opponent[i].n[j]);
			}
			printf("Opponent[%d].nitch = %d\n",i,Opponent[i].nitch);
		}
		*/
		int win_count=0;
		for(i=0;i<Nc;i++){
			if(child[i].win == No){
				Update_Opponent(child[i],Opponent);
			}
		}
		/*
		for(i=0;i<Nc;i++){
			if(child[i].win == No)
			Update_Opponent(child[i],Opponent);
		}
		*/
		/*pop集団をプロット*/
		Unit_Prot(pop,window,Ns);
		for(i=0;i<No;i++){
			for(j=0;j<DEM;j++){
				printf("Opponent[%d].n[%d] = %f\n",i,j,Opponent[i].n[j]);
			}
			putchar('\n');
		}
		/*敵集団をプロット*/
		Opponent_Prot(Opponent,window);
		//Unit_Prot(child,window,Nc);
		/*親集団構造体初期化*/
		Init_Indiv(pare,Np);
		/*子集団構造体初期化*/
		Init_Indiv(child,Nc); 
		/*試行回数を数える*/
		end_count++;
		if(end_count == END_STEP){
			end_flag = 0;
		}
		printf("end_count = %d\n",end_count);
		printf("end_flag = %d\n",end_flag);
		/*枠をプロット*/
		Prot_Frame(window);
		SDL_Flip(window); /*ウィンドウに反映*/
		SDL_Delay(100);
	}
	//printf("end_count = %d\n",end_count);
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
					true_flag = 0;
					break;
				}
				break;
			}
		}
	}
}

/*********
初期化
*********/
void Init_Indiv(Indiv pare[],int N){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<DEM;j++){
			pare[i].n[j] = 0; /*実数値乱数*/
		}
		pare[i].flag = 0;
		pare[i].win = 0;
		pare[i].lose = 0;
		pare[i].draw = 0;
		pare[i].eval = 0;
		pare[i].nitch = 0;
	}
}

/*********
敵集団初期化
*********/
void Init_Opponent(Indiv Opponent[]){
	int i,j,k;
	int obj_count;
	int tmp_obj;
	double tmp_dis;
	
	/*敵集団初期化*/
	
	for(i=0;i<No;i++){
		for(j=0;j<DEM;j++){
			Opponent[i].n[j] = GetRand32(INIT_OPPOMEMT); /*実数値乱数*/
		}
		Opponent[i].flag = 0;
		if(i == No)
		Opponent[No].flag = -1;
		Opponent[i].win = 0;
		Opponent[i].lose = 0;
		Opponent[i].draw = 0;
		Opponent[i].eval = 0;
		Opponent[i].nitch = 0;
		for(j=0;j<K;j++){
			Opponent[i].Neigh_List2[j] = -1; /*実数値乱数*/
		}
	}
	
	/*自身以外の個体との距離を総当たりで計測*/
	for(i=0;i<No;i++){
		obj_count=i; /*カウント初期化*/
		for(j=i+1;j<No;j++){
			Opponent[i].obj[obj_count] = j; /*配列の一番目に相手の番号を入れる*/
			Opponent[j].obj[i] = i;
			Opponent[i].dis[obj_count] = cal_distance(Opponent[i].n[0],
							Opponent[i].n[1],
							Opponent[j].n[0],
							Opponent[j].n[1]);
			Opponent[j].dis[i] = Opponent[i].dis[obj_count];
			obj_count++;
		}
	}
	/*近い順に並べ替え*/
	for(i=0;i<No;i++){
		for(j=0;j<No-2;j++){
			for(k=No-2;k>j;k--){
				/* 前の要素の方が大きかったら*/
				if(Opponent[i].dis[k-1]>Opponent[i].dis[k]){
					/*相手番号を並べ替え*/
					tmp_obj = Opponent[i].obj[k];
					Opponent[i].obj[k] = Opponent[i].obj[k-1];
					Opponent[i].obj[k-1] = tmp_obj;
					/*距離を並べ替えを並べ替え*/
					tmp_dis = Opponent[i].dis[k];
					Opponent[i].dis[k] = Opponent[i].dis[k-1];
					Opponent[i].dis[k-1] = tmp_dis;
				}
			}
		}
	}
	/*近傍リスト2生成*/
	int save_obj;
	int count_nitch_flag = 0;
	count_nitch = 1;
	for(i=0;i<No;i++){
		obj_count=0; /*近傍リストをカウント*/
		/*近傍リストを作る個体の近傍をＫ番目まで見る*/
		for(j=0;j<K;j++){
			save_obj = Opponent[i].obj[j];
			/*近傍Ｋ番目までの個体の近傍にiが存在すればリストに加える*/
			for(k=0;k<K;k++){
				if(i == Opponent[save_obj].obj[k]){ /*ミス*/
					Opponent[i].Neigh_List2[obj_count] = Opponent[i].obj[j];
					obj_count++;
					/*まだニッチに所属していなければニッチの番号を加える*/
					if(Opponent[i].nitch == 0){
						Opponent[i].nitch = count_nitch;
						count_nitch_flag = 1;
					}
					if(Opponent[save_obj].nitch == 0){
						Opponent[save_obj].nitch = Opponent[i].nitch;
						count_nitch_flag = 1;
					}					
				}
			}
		}
		if(Opponent[i].nitch == 0){
			Opponent[i].nitch = count_nitch;
			count_nitch_flag = 1;
		}
		for(j=obj_count;j<=K;j++){
			Opponent[i].Neigh_List2[j] = -1;
		}
		/*カウントを更新*/
		if(count_nitch_flag == 1){
			count_nitch++;
			count_nitch_flag = 0;
		}	
	}
}

/**********
評価値の降順に並べ替え
*********/
void sort_eval(Indiv pare[],int N){
	int i,j;
	Indiv tmp_pare;
	int Eval[N];
	/*ソートする*/
	for(i=0;i<N-1;i++){
		for(j=N-1;j>i;j--){
			/* 前の要素の方が大きかったら */
			if(pare[j-1].eval<pare[j].eval){
				tmp_pare = pare[j]; /* 交換する */
				pare[j] = pare[j-1];
				pare[j-1] = tmp_pare;
            		}
            	}
        }
}

/**********
距離の昇順に並べ替え
*********/
void sort_distance(Indiv pare[],int N)
{
	int i,j,k;
	int save_obj;
	double save_dis;
	int Eval[N];
	/*ソートする*/
	for(k=0;k<No;k++){
		for(i=0;i<N-1;i++){
			for(j=N-1;j>i;j--){
				/* 前の要素の方が大きかったら */
				if(pare[i].dis[j]>pare[j].dis[j-1]){
					save_obj = pare[i].obj[j]; /* 交換する */
					pare[i].obj[j] = pare[i].obj[j-1];
					pare[i].obj[j-1] = save_dis;
        	    		}
        	    	}
        	}
        }
}

/*****************
乱数生成
*****************/
double GetRand32(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	get_save = getrand_int32Max/2;
 
  return ((double)genrand_int32()-get_save)/get_save*pGetMax;
}

/*************
距離計測
*************/
double cal_distance(double a_x,double a_y,double b_x,double b_y){
	double cal_save;
	cal_save = pow(b_x-a_x,2)+pow(b_y-a_y,2);
	return( sqrt(cal_save) );
}

/************
JGG＋REXstar
************/
void RexStar(Indiv pare[],Indiv child[],SDL_Surface *window)
{
	double base_gra[DEM] = {0}; /*基準となる重心*/
	double sum_n[DEM] = {0}; /*x,yそれぞれの和を一時的に保存しておく配列*/
	int i,j;
	Indiv sub_child[Nc];
	Init_Indiv(sub_child,Nc);
	
	/*ベースとなる重心を求める*/
	for(i=0;i<Np;i++){
		for(j=0;j<DEM;j++){
			sum_n[j] += pare[i].n[j]; /*親のx,yをそれぞれ足す*/
		}
	}
	for(i=0;i<DEM;i++){
		base_gra[i] = sum_n[i]/Np; /*親のそれぞれx,yの和を割る*/
	}

	/*重心から親に対するベクトルを求める*/
	Indiv vector[Np];
	for(i=0;i<Np;i++){
		for(j=0;j<DEM;j++){
			vector[i].n[j] = pare[i].n[j] - base_gra[j];
		}
	}

	/*重心をもとに親を複製する*/
	Indiv sub_pare[Np];
	Init_Indiv(sub_pare,Np);
	for(i=0;i<Np;i++){
		for(j=0;j<DEM;j++){		
			sub_pare[i].n[j] = pare[i].n[j] - 2*vector[j].n[j];
		}
	}

	/*親をまとめる*/
	Indiv sum_pare[Np*2];
	Init_Indiv(sum_pare,Np*2);
	
	for(i=0;i<Np;i++){
		/*前半分にpare構造体を、後半分に複製体を入れる*/
		sum_pare[i] = pare[i];
		sum_pare[Np+i] = sub_pare[i];
	}
	/*ナンバーズを適用して評価のいい順にソート*/
	Pare_Numbers(sum_pare);
	sort_eval(sum_pare,Np*2);
	/*上位半分の両個体の重心、下位半分の両個体の重心を求める*/
	double high_gra[DEM] = {0};
	for(j=0;j<DEM;j++){
		sum_n[j] = 0; /*親のx,yをそれぞれ足す*/
	}
	for(i=0;i<Np;i++){
		for(j=0;j<DEM;j++){
			sum_n[j] += sum_pare[i].n[j]; /*親のx,yをそれぞれ足す*/
		}
	}

	for(i=0;i<DEM;i++){
		high_gra[i] = sum_n[i]/Np; /*xbを求める*/
	}
	/*
	filledCircleColor(window,
		(high_gra[0]*2)+center[0],
		(high_gra[1]*2)+center[1],
		4,
		0xff0000ff);
	*/
	/*子個体生成*/
	double diag[DEM];
	double sum_coe[DEM]={0},coe=0; /*親のベクトル*乱数の総和*/
	double child_save[DEM];
	int count = 0;
	true_flag = 1;
	while(true_flag){
		/*式11の第２項の値生成*/
		for(i=0;i<DEM;i++){
			diag[i] = genrand_real1()*T;
			sum_coe[i] = 0;
		}
		/*式(11)の第３項の動作*/
		for(i=0;i<Np;i++){
			coe = GetRand32( sqrt(3/(DEM+1)) );/*一様乱数生成*/
			for(j=0;j<DEM;j++){
				sum_coe[j] += coe * (sum_pare[i].n[j]-base_gra[j]);
			}
		}
		for(i=0;i<DEM;i++){
			child_save[i] = base_gra[i] + diag[i]*(high_gra[i]-base_gra[i])+sum_coe[i];
		}
		if(fabs(child_save[0]) <= 100 && fabs(child_save[1]) <= 100){
			for(i=0;i<DEM;i++){
				child[count].n[i] = child_save[i]; /*式(11)の動作*/
			}
			count++;
		}
		if(count == Nc){
			true_flag = 0;
		}
	}
}


/*********
ナンバーズ
*********/
void Numbers(Indiv pare[],Indiv pare2[])
{
	int i,j,k;
	int base = 0; /*ナンバーズを行うパラメータを決定*/
	double base_fabs;
	double save_fabs;
	/*絶対値が一番小さいパラメータを求める*/
	for(i=0;i<Nc;i++){
		for(j=0;j<No;j++){
			base_fabs = fabs(pare[i].n[0] - pare2[j].n[0]);
			base = 0;
			for(k=1;k<DEM;k++){
				if((save_fabs = fabs(pare[i].n[0] - pare2[j].n[0])) < base_fabs){
					base_fabs = save_fabs;
					base = k;
				}
			}
			if(pare[i].n[base]<pare2[j].n[base]){
				pare2[j].win += 1;
				pare[i].lose += 1;
			}else if(pare[i].n[base] > pare2[j].n[base]){
				pare[i].win += 1;
				pare2[j].lose += 1;
			}else if(pare[i].n[base] = pare2[j].n[base]){
				pare[i].draw += 1;
				pare2[j].draw += 1;
			}
		}
	}
	for(i=0;i<Nc;i++){
		pare[i].eval = pare[i].win+(-1)*pare[i].lose;
	}
	for(i=0;i<No;i++){
		pare2[i].eval = pare2[i].win+(-1)*pare2[i].lose;
	}
	/*
	printf("子個体の勝率\n");
	for(i=0;i<Np;i++){
		printf("child[%d].win = %d\n",i,pare[i].win);
		printf("child[%d].lose = %d\n",i,pare[i].lose);
		printf("child[%d].draw = %d\n",i,pare[i].draw);
	}
	putchar('\n');
	*/

}
/************************
1on1でナンバーズ
************************/
void OneOnOne_Numbers(Indiv pare[],int N)
{
	int i,j,k;
	int base = 0; /*ナンバーズを行うパラメータを決定*/
	double base_fabs;
	double save_fabs;
	/*絶対値が一番小さいパラメータを求める*/
	/*
	printf("１個体目\n");
	for(j=0;j<DEM;j++){
		printf("pare.n[%d] = %f\n",j,pare.n[j]);
	}
	printf("2個体目\n");
	for(j=0;j<DEM;j++){
		printf("pare2.n[%d] = %f\n",j,pare2.n[j]);
	}
	*/
	for(i=0;i<N;i++){
		for(j=i+1;j<N;j++){
			base_fabs = fabs(pare[i].n[0] - pare[j].n[0]);
			base = 0;
			for(k=1;k<DEM;k++){
				if((save_fabs = fabs(pare[i].n[0] - pare[j].n[0])) < base_fabs){
					base_fabs = save_fabs;
					base = k;
				}
			}
			if(pare[i].n[base]<pare[j].n[base]){	
				pare[j].win += 1;
				pare[i].lose += 1;
			}else if(pare[i].n[base] > pare[j].n[base]){
				pare[i].win += 1;
				pare[j].lose += 1;
			}else if(pare[i].n[base] = pare[j].n[base]){
				pare[i].draw += 1;
				pare[j].draw += 1;
			}
			pare[i].eval = pare[i].win+(-1)*pare[i].lose;
			pare[j].eval = pare[j].win+(-1)*pare[j].lose;
		}
	}
}

/************
親のナンバーズ
************/
void Pare_Numbers(Indiv pare[])
{
	int i,j,k;
	int base = 0; /*ナンバーズを行うパラメータを決定*/
	double base_fabs;
	double save_fabs;
	/*絶対値が一番小さいパラメータを求める*/
	for(i=0;i<Np;i++){
		for(j=i+1;j<Np;j++){
			base_fabs = fabs(pare[i].n[0] - pare[j].n[0]);
			base = 0;
			for(k=1;k<DEM;k++){
				if((save_fabs = fabs(pare[i].n[0] - pare[j].n[0])) < base_fabs){
					base_fabs = save_fabs;
					base = k;
				}
			}
			if(pare[i].n[base]<pare[j].n[base]){	
				pare[j].win += 1;
				pare[i].lose += 1;
			}else if(pare[i].n[base] > pare[j].n[base]){
				pare[i].win += 1;
				pare[j].lose += 1;
			}else if(pare[i].n[base] = pare[j].n[base]){
				pare[i].draw += 1;
				pare[j].draw += 1;
			}
		}
	}
	for(i=0;i<Np;i++){
		pare[i].eval = pare[i].win+(-1)*pare[i].lose;
	}
}

/*************
相手集団を更新
*************/
Update_Opponent(Indiv child, Indiv Opponent[])
{
	int tmp_obj;
	double tmp_dis;
	int i,j,k;
	int obj_count;
	Indiv sub_Opponent[No+1];
	int save_tag[10];
	/*sub_opponentに相手集団と子個体を追加*/
	for(i=0;i<No+1;i++){
		for(j=0;j<DEM;j++){
			if(i == No){
				sub_Opponent[i].n[j] = child.n[j];
			}else {
				sub_Opponent[i].n[j] = Opponent[i].n[j];
			}
		}
		sub_Opponent[i].flag = 0;
		sub_Opponent[i].win = 0;
		sub_Opponent[i].lose = 0;
		sub_Opponent[i].draw = 0;
		sub_Opponent[i].eval = 0;
		sub_Opponent[i].nitch = Opponent[i].nitch;
	}
	/*自身以外の個体との距離を総当たりで計測*/
	for(i=0;i<No+1;i++){
		obj_count=i; /*カウント初期化*/
		for(j=i+1;j<No+1;j++){
			sub_Opponent[i].obj[obj_count] = j; /*配列の一番目に相手の番号を入れる*/
			sub_Opponent[j].obj[i] = i;
			sub_Opponent[i].dis[obj_count] = cal_distance(Opponent[i].n[0],
							Opponent[i].n[1],
							Opponent[j].n[0],
							Opponent[j].n[1]);
			sub_Opponent[j].dis[i] = sub_Opponent[i].dis[obj_count];
			obj_count++;
		}
	}
	/*sub_opponentの各要素を並べ替え*/
	for(i=0;i<No+1;i++){
		for(j=0;j<No+1-2;j++){
			for(k=No+1-2;k>j;k--){
				/* 前の要素の方が大きかったら*/
				if(sub_Opponent[i].dis[k-1]>sub_Opponent[i].dis[k]){
					/*相手番号を並べ替え*/
					tmp_obj = sub_Opponent[i].obj[k];
					sub_Opponent[i].obj[k] = sub_Opponent[i].obj[k-1];
					sub_Opponent[i].obj[k-1] = tmp_obj;
					/*距離を並べ替えを並べ替え*/
					tmp_dis = sub_Opponent[i].dis[k];
					sub_Opponent[i].dis[k] = sub_Opponent[i].dis[k-1];
					sub_Opponent[i].dis[k-1] = tmp_dis;
				}
			}
		}
	}
	int save_obj;
	int count_nitch_flag = 0;
	obj_count=0;
	for(i=0;i<No+1;i++){
		obj_count=0; /*近傍リストをカウント*/
		/*近傍リストを作る個体の近傍をＫ番目まで見る*/
		for(j=0;j<K;j++){
			save_obj = sub_Opponent[i].obj[j];
			/*近傍Ｋ番目までの個体の近傍にiが存在すればリストに加える*/
			for(k=0;k<K;k++){
				if(i == sub_Opponent[save_obj].obj[k]){
					sub_Opponent[i].Neigh_List2[obj_count] = sub_Opponent[i].obj[j];
					obj_count++;
				}
			}
		}
		for(j=obj_count;j<=K;j++){
			sub_Opponent[i].Neigh_List2[j] = -1;
		}
	}
	/*入れる予定のニッチを取得*/
	int forecast_nitch=0;
	int add_nitch_count=0;
	int add_flag = 0;
	save_obj=0;
	/*childの近傍リストに個体がないかどうかを確かめる*/
	for(i=0;i<K;i++){
		if(sub_Opponent[No].Neigh_List2[i]!=-1){
			add_flag = 1;
			save_obj = sub_Opponent[No].Neigh_List2[i];
			forecast_nitch = sub_Opponent[save_obj].nitch;
		}
		break;
	}
	sub_Opponent[No].nitch = forecast_nitch;
	printf("追加予定のニッチ:%d\n",forecast_nitch);
	for(i=0;i<No;i++){
		if(sub_Opponent[i].nitch == forecast_nitch){
		printf("atai\n");
			add_nitch_count++;
		}
	}
	/*
	for(j=0;j<DEM;j++){
		printf("sub_Opponent[%d].n[%d] = %f\n",No,j,sub_Opponent[No].n[j]);
	}
	*/
	//printf("sub_Opponent[%d].win = %f\n",No,sub_Opponent[No].win);
	//printf("sub_Opponent[%d].eval = %f\n",No,sub_Opponent[No].eval);
	//putchar('\n');
	//printf("アドフラグ = %d\n",add_flag);
	
	if(add_flag == 1 && add_nitch_count){
		/*追加予定のニッチの個体数を取得*/
		printf("ニッチ内の個体数:%d\n",add_nitch_count);
		/*子個体枠を追加*/
		add_nitch_count++;
		/*ニッチ内の個体＋子個体を入れて対戦するための構造体を定義*/
		Indiv battle_Opponent[add_nitch_count];
		for(i=0;i<add_nitch_count;i++){
			for(j=0;sub_Opponent[j].nitch!=forecast_nitch;j++){}
				battle_Opponent[i] = sub_Opponent[j];
				sub_Opponent[j].nitch = -1;
				sub_Opponent[j].flag = 1;
		}
		/*ナンバーズで競争*/
		OneOnOne_Numbers(battle_Opponent,add_nitch_count);
		/*評価値の良い順に並び替え*/
		sort_eval(battle_Opponent,add_nitch_count);
		/*
		printf("ソート後\n");
		for(i=0;i<add_nitch_count;i++){
			for(j=0;j<DEM;j++){
				printf("battle_Opponent[%d].n[%d] = %f\n",i,j,battle_Opponent[i].n[j]);
			}
			printf("battle_Opponent[%d].win = %d\n",i,battle_Opponent[i].win);
			printf("battle_Opponent[%d].eval = %d\n",i,battle_Opponent[i].eval);
			putchar('\n');

		}
		*/
		/*battle内の個体をサブに戻す*/
		for(i=0;i<add_nitch_count;i++){
			for(j=0;sub_Opponent[j].flag!=1;j++){}
			sub_Opponent[j] = battle_Opponent[i];
			sub_Opponent[j].flag = 0;
		}
		/*
		printf("戻す後\n");
		for(i=0;i<No+1;i++){
			for(j=0;j<DEM;j++){
				printf("sub_Opponent[%d].n[%d] = %f\n",i,j,sub_Opponent[i].n[j]);
			}
			printf("sub_Opponent[%d].flag = %d\n",i,sub_Opponent[i].flag);
			putchar('\n');

		}
		*/

	}
	/*sub内の個体を元の相手集団に戻す*/
	for(i=0;i<No;i++){
		Opponent[i] = sub_Opponent[i];
	}
	/*
		printf("戻す後\n");
		for(i=0;i<No+1;i++){
			for(j=0;j<DEM;j++){
				printf("Opponent[%d].n[%d] = %f\n",i,j,Opponent[i].n[j]);
			}
			printf("Opponent[%d].flag = %d\n",i,Opponent[i].flag);
			printf("Opponent[%d].eval = %d\n",i,Opponent[i].eval);
			printf("Opponent[%d].nitch = %d\n",i,Opponent[i].nitch);
			putchar('\n');

		}
		*/

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

/***************
敵集団をプロット
***************/
void Opponent_Prot(Indiv Opponent[],SDL_Surface *window)
{
	int i;
	char tmp_n[5];
	for(i=0;i<No;i++){
		sprintf(tmp_n,"%d",Opponent[i].nitch);
		stringColor(window, (Opponent[i].n[0]*2)+center[0], (Opponent[i].n[1]*2)+center[1], tmp_n, 0x000000ff);
		tmp_n[0] = '\0';
	}
}

/********************
集団をプロット
********************/
void Unit_Prot(Indiv pop[],SDL_Surface *window,int N)
{
	int i;
	for(i=0;i<N;i++){
		filledCircleColor(window,
			(pop[i].n[0]*2)+center[0],
			(pop[i].n[1]*2)+center[1],
			3,
			0xff0000ff);
	}
}

