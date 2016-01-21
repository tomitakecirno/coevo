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
	int obj[No];
	double dis[No];
	int Neigh_List2[K];
}Indiv;
Opponent[No];

int end_flag = 1;
int true_flag = 1;
int while_flag = 1;
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
void Set_Nitch(int i);
void Init_Opponent(void);
double cal_distance(double a_x,double a_y,double b_x,double b_y);
void Prot_Frame(SDL_Surface *window);
void Opponent_Prot(SDL_Surface *window);
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
	
	int i,j,k; /*for文用*/

	init_genrand((unsigned)time(NULL)); /*乱数初期化*/
	Init_Indiv(pop,Ns); /*解集団構造体初期化*/
	Init_Indiv(pare,Np); /*親集団構造体初期化*/
	Init_Indiv(child,Nc); /*子集団構造体初期化*/
	Init_Opponent(); /*相手集団構造体初期化*/
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
		Numbers(child);
		/*評価の良い順にソート*/
		sort_eval(child,Nc);
		/*子個体の最良Np個を初期集団へ（世代交代）*/
		count = 0;
		for(i=0;i<Np;i++){
			/*親として選ばれた場所に子個体を入れる*/
			for(j=0;pop[j].flag != 1;j++){}
			pop[j] = child[i];
		}
		int win_count=0;
		for(i=0;i<Nc;i++){
			if(child[i].win == No){
				Update_Opponent(child[i]);
			}
		}
		/*pop集団をプロット*/
		Unit_Prot(pop,window,Ns);
		/*敵集団をプロット*/
		Opponent_Prot(window);
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
void Init_Opponent(){
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

/*****************
再帰的にニッチを割り当てていく
*****************/
void Set_Nitch(int i){
	int tmp_NeighList2; /*近傍リスト内の個体の番号を一時的に保存しておく*/
	int j;
	if(Opponent[i].flag != 1){
		/*ニッチに属していなければニッチ番号を割り振る*/
		Opponent[i].nitch = count_nitch;
		Opponent[i].flag = 1;
			/*近傍リストを見ていく*/
		for(j=0;j<K;j++){
			tmp_NeighList2 = Opponent[i].Neigh_List2[j];
			Set_Nitch(tmp_NeighList2);
		}
	}
}
/********************
近傍リストを作る
********************/
void NeighList_Opponent(void)
{
	int i,j,k;
	int obj_count;
	for(i=0;i<No;i++){
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
	for(i=0;i<No;i++){
		obj_count=0; /*近傍リストをカウント*/
		/*近傍リストを作る個体の近傍をＫ番目まで見る*/
		for(j=0;j<K;j++){
			save_obj = Opponent[i].obj[j];
			/*近傍Ｋ番目までの個体の近傍にiが存在すればリストに加える*/
			for(k=0;k<K;k++){
				if(i == Opponent[save_obj].obj[k]){
					Opponent[i].Neigh_List2[obj_count] = Opponent[i].obj[j];
					obj_count++;
				}
			}
		}
	}
	/*ニッチ番号を割り振る*/
	count_nitch=1; /*ニッチ番号を初期化*/
	for(i=0;i<No;i++){
		if(Opponent[i].flag != 1){
			Set_Nitch(i);
			count_nitch++;
		}
	}
	for(i=0;i<No;i++){
		Opponent[i].flag = 0;
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
Update_Opponent(Indiv child)
{
	int i,j,k;
	int obj_count;
	double min_dis;
	double tmp_dis;

	/*距離が最小の個体を見つけて、ニッチ番号を取得*/
	min_indiv = 0;
	min_dis = cal_distance(Opponent[min_indiv].n[0],Opponent[min_indiv].n[1],
			       child.n[0],child.n[1]);
	for(i=0;i<No;i++){
		if(cal_distance(Opponent[i].n[0],Opponent[i].n[1],
		   child.n[0],child.n[1]) < min_dis){
		   	min_dis = cal_distance(Opponent[i].n[0],Opponent[i].n[1],
		   				child.n[0],child.n[1]);
		   	child.nitch = Opponent[i].nitch;
		}
	}

	/*戦闘データ初期化して、対戦させる*/
	for(i=0;i<No;i++){
		if(Opponent[i].nitch == child.nitch){
			Opponent[i].win = 0;
			Opponent[i].lose = 0;
			Opponent[i].draw = 0;
			Opponent[i].eval = 0;
		}
	}
	/*対戦させる*/

	/*評価値の最小個体を求める*/
	/*最小個体の位置に子個体を入れる*/

	/*ニッチ番号初期化*/
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
void Opponent_Prot(SDL_Surface *window)
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

