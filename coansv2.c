#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#include<SDL/SDL.h>
#include<SDL/SDL_video.h>
#include <SDL/SDL_gfxPrimitives.h>
#include"./header/MT.h"



#define INIT		      100	/*解集団の初期化範囲*/
#define INIT_OPPOMEMT	100	/*敵集団の初期化範囲*/
#define INIT_OPTIMAL	100	/*敵集団の初期化範囲*/
#define Ns		        30	/*初期集団数*/
#define No		        30	/*敵集団数*/
#define Np		        4	  /*親個体数*/
#define Nc		        5	  /*子個体数*/
#define DEM		        2	  /*次元数*/
#define T		          1	  /*ステップサイズ*/
#define END_STEP	    500	/*終わるタイミング*/
#define WINDOW_X	    400	/*定義域*/
#define WINDOW_Y	    400	/*地域*/
#define PROT_X		    200	/*定義域*/
#define PROT_Y		    200	/*地域*/
#define Kp		        5	  /*ニッチの集団数*/
#define Ko		        3	  /*ニッチの集団数*/
#define DELETE		    100
#define Optimal_N	    4

double center[2] = {WINDOW_X/2,WINDOW_Y/2};
typedef struct{
	double n[DEM];
	int flag; /*1...主親　2...副親*/
	double eval;
	int win;
	int nitch; /*0...所属ニッチなし 1~...所属しているニッチ番号*/
	int obj[No];
	double dis[No];
	int Neigh_List2[Kp];
}Indiv;
Indiv Opponent[No];

typedef struct{
	double n[DEM];
	double hosei_n;
}Xy_str;
Xy_str Gra_Nitch[No];
Xy_str Optimal[Optimal_N];

int end_flag = 1;
int true_flag = 1;
int while_flag = 1;
int count_nitch=1;
int end_count = 0;
int battle_n=0;


void Init_Indiv(Indiv pare[],int N);
void Init_Opponent(void);
void Init_Opponent_BattleData(void);
void Init_Optimal(void);

void creat_child(int pare[],int child[]);

void Pare_Numbers(Indiv pare[]); /*REXstar内で扱う*/
int Numbers(Indiv *one,Indiv *another);

void sort_win(Indiv pare[],int N);
void sort_distance(Indiv *unit,int N);
double cal_coord_distance(Indiv *one,Xy_str *coord);
double cal_kotai_distance(Indiv one, Indiv another);

void NeighList_Opponent(void);
void AnsList3(Indiv pop[]);
void Set_Nitch(int i);
void ExtensionXLM(Indiv MainPare, Indiv SubPare[], int pare_count, Indiv child[]);

static int thread_keyboad(void *data);
static int thread_window(void *data);
double GetRand_Real(double pGetMax);
double rand_normal( double mu, double sigma );
double Uniform( void );
void Prot_Frame(SDL_Surface *window);
void Pop_Prot(Indiv pop[],SDL_Surface *window);
void Unit_Prot(Indiv pop[],SDL_Surface *window,int N);
void Unit_Optimal(SDL_Surface *window);
/***********
メインルーチンmain
***********/
main(){
	/*個体を格納する配列定義*/
	Indiv pop[Ns];		/*解集団配列*/
	Indiv pare[Np];		/*親集団配列*/
	Indiv child[Nc];	/*子集団配列*/
	Indiv pare_child[Np+Nc];/*子個体と親個体を合わせる*/
	Indiv MainPare;

	int i,j,k; /*for文用*/
	char name[10];
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/
	Init_Indiv(pop,Ns); /*解集団構造体初期化*/
	Init_Indiv(pare,Np); /*親集団構造体初期化*/
	Init_Indiv(child,Nc); /*子集団構造体初期化*/
	Init_Optimal();

	clock_t start,end;
	double time_count;
	double battle_count;
	
	/*SDL初期化*/
	SDL_Surface *window; // ウィンドウ（画像）データ、及び、文字列（画像）へのポインタ
	
	/*SDLの全ての機能を初期化*/
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
	SDL_Thread *thr_keyboad,*thr_window;
	thr_keyboad = SDL_CreateThread(thread_keyboad,NULL);
	/*初期解生成*/
	for(i=0;i<Ns;i++){
		for(j=0;j<DEM;j++){
			pop[i].n[j] = GetRand_Real(INIT); /*実数値乱数*/
		}
	}
	/*初期集団からランダムに親を選ぶ*/
	int while_flag = 1;
	int pare_count; /*親の数をカウント*/
	int tmp_rand;
	int tmp_n;
	int MainPare_n;
	while(end_flag){
		SDL_FillRect(window, NULL, 0x00ffffff);
		/*近傍リスト3生成*/
		AnsList3(pop);
		/*主親を選ぶ*/
		while_flag = 1;
		pare_count = 0;
		while(while_flag){
			MainPare_n = genrand_int32() % Ns; /*主親の番号取得*/
			for(i=0;pop[MainPare_n].Neigh_List2[i] != -1 && i<Kp;i++){}
			pare_count = i;
			/*近傍リストに個体があれば主親にして、ループを抜ける*/
			if(pare_count  >= Np){
				MainPare = pop[MainPare_n]; /*主親へ*/
				MainPare.win = 0;
				while_flag = 0;
			}
		}
		/*副親を選ぶ*/
		if(pare_count < Np){
			/*規定の親個体数より近傍リストの個体が少なければリストの個体全てを副親に*/
			int SubPare_n[pare_count];
			Indiv SubPare[pare_count];
			for(i=0;i<pare_count;i++){
				SubPare_n[i] = MainPare.Neigh_List2[i];
				SubPare[i] = pop[SubPare_n[i]];
				SubPare[i].win = 0;
			}
			/*主親と副親で拡張XLM*/
			ExtensionXLM(MainPare,SubPare,pare_count,child);
		}else {
			/*規定以上の個体数あったらランダムに選ぶ*/
			pare_count = Np;
			Indiv SubPare[Np];
			int SubPare_n[Np] = {0};
			for(i=0;i<Np;i++){
				while_flag = 1;
				while(while_flag){
					tmp_rand = genrand_int32() % pare_count;
					/*リストが空でなければ追加*/
					if(MainPare.Neigh_List2[tmp_rand] != -1){
						SubPare_n[i] = MainPare.Neigh_List2[tmp_rand];
						SubPare[i] = pop[SubPare_n[i]];
						SubPare[i].win = 0;
						/*個体と取り出して空にする*/
						MainPare.Neigh_List2[tmp_rand] = -1;
						while_flag = 0;
					}
				}
			}
			/*主親と副親で拡張XLM*/
			ExtensionXLM(MainPare,SubPare,pare_count,child);
		}

    /*子個体作れたから自分の集団と対戦させる*/
		/*対戦？*/
		start = clock();
		for(i=0;i<Ns;i++){
			for(j=0;j<Nc;j++){
				Numbers(&pop[i],&child[j]);
			}
			Numbers(&pop[i],&MainPare);
		}
		end = clock();
		time_count += (double)(end - start);
		/*デバッグ用*/
		/*評価の良い順にソート*/
		sort_win(child,Nc);
    /*
		for(i=0;i<Nc;i++){
		  printf("child[%d].win = %d\n",i,child[i].win);
		}
		printf("MainPare.win = %d\n",MainPare.win);
		*/
		//printf("child[0].win = %d\n",child[0].win);
		//printf("MainPare.win = %d\n",MainPare.win);
		
		/*子個体の最良個体が主親より良ければ入れ替える*/
		if(MainPare.win < child[0].win){
			MainPare = child[0];
		}
		/*主親を集団に戻す*/
		pop[MainPare_n] = MainPare;
		Unit_Optimal(window);
		Pop_Prot(pop,window);
		Prot_Frame(window);
		SDL_Flip(window);
		if(end_count % 20 == 0){
			sprintf(name,"./picture/pop_oppo0%d.bmp",end_count);
			SDL_SaveBMP(window,name);
		}
		//SDL_Delay(500);

		/*構造体初期化*/
		Init_Indiv(child,Nc);
		/*試行回数を数える*/
		end_count++;
		if(end_count > END_STEP){
			end_flag = 0;
		}
	}
	/*最適解の座標表示*/
	/*
	for(i=0;i<Optimal_N;i++){
		printf("Optimal[%d] = (",i);
		for(j=0;j<DEM;j++){
			printf("%.2f, ",Optimal[i].n[j]);
		}
		printf("\n");
	}
	*/
	//printf("end_count = %d\n",end_count);
	/*集団の最適な個体を表示*/
	/*
	for(i=0;i<Ns;i++){
	  for(j=0;j<Ns;j++){
		  Numbers(&pop[i],&pop[j]);
		}
	}
	sort_win(pop,Ns);
	for(i=0;i<Ns;i++){
	  printf("pop[%d].n = (",i);
	  for(j=0;j<DEM;j++){
			printf("%.2f, ",pop[i].n[j]);
		}
	  printf("\n");
	}
	for(i=0;i<Ns;i++){
		printf("pop[%d].eval = %.2f\n",i,pop[i].eval);
	}
  */
  /*対戦回数と秒数*/
  int eval_n=0;
  for(i=0;i<Ns;i++){
    if(pop[i].eval <= 0.5){
      eval_n++;
    }
  }
  printf("%d\n",eval_n);
  double eval_ave = eval_n/Ns;
  printf("%f\n",eval_ave);
  printf("ave:%f battle:%d  time:%f\n",eval_ave,battle_n/END_STEP,time_count/END_STEP);
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
					while_flag = 0;
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
		pare[i].nitch = 0;
	}
}

/*******************
最適解の戦闘データ初期化
*******************/
void Init_Optimal(void)
{
	double tmp_n = -100;
	int i,j;
	Optimal[0].n[0] = -50;
	Optimal[0].n[1] = -50;
	//Optimal[0].hosei_n = pow(10,-20);
	
	Optimal[1].n[0] = -50;
	Optimal[1].n[1] =  50;
	
	//Optimal[1].hosei_n = pow(10,-10);
	
	Optimal[2].n[0] =  50;
	Optimal[2].n[1] = -50;
	
	//Optimal[2].hosei_n = pow(10,-30);
	
	Optimal[3].n[0] =  50;
	Optimal[3].n[1] =  50;
	
	//Optimal[3].hosei_n = 0;
	/*
	for(i=0;i<Optimal_N;i++){
		for(j=0;j<DEM;j++){
			Optimal[i].n[j] = GetRand_Real(INIT_OPTIMAL);
		}
	}
	*/
}
/********************
近傍リスト3を作る
********************/
void AnsList3(Indiv pop[])
{
	int i,j,k;
	int kotai_count;
	int tmp_obj;
	int tmp_dis;
	int tmp_anslist3[Kp];
	for(i=0;i<Ns;i++){
		for(j=0;j<Kp;j++){
			pop[i].Neigh_List2[j] = -1; /*実数値乱数*/
		}
	}
	/*ここ関数化できるかも.距離を計測して昇順に並べ替え*/
	for(i=0;i<Ns;i++){
		/*自身以外の個体との距離を総当たりで計測.自身も含む.*/
		for(j=0;j<Ns;j++){
			pop[i].obj[j] = j; /*配列の一番目に相手の番号を入れる*/
			pop[i].dis[j] = cal_kotai_distance(pop[i],pop[j]);
		}
		/*近い順に並べ替え*/
		sort_distance(&pop[i],Ns);
	}
	/*
	for(i=0;i<Ns;i++){
		for(j=0;j<Ns;j++){
			printf("pop[%d].obj[%d] = %d\n",i,j,pop[i].obj[j]);
			printf("pop[%d].dis[%d] = %.2f\n",i,j,pop[i].dis[j]);
		}
	}
	*/
	for(i=0;i<Ns;i++){
		for(j=0;j<Kp;j++){
			pop[i].Neigh_List2[j] = pop[i].obj[j];
			//printf("pop[%d].AnsList[%d] = %d\n",i,j,pop[i].Neigh_List2[j]);
		}
		//putchar('\n')
	}
	/*近傍リスト3生成*/
	/*
	for(i=0;i<Ns;i++){
		kotai_count=0;
		for(j=0;j<Ns;j++){
			for(k=1;k<K;k++){
				if(pop[j].obj[k] == i){
					pop[i].Neigh_List2[kotai_count] = j;
					kotai_count++;
					
				}
			}
		}
		for(j=0;j<K;j++){
			printf("pop[%d].AnsList[%d] = %d\n",i,j,pop[i].Neigh_List2[j]);
		}
	}
	*/
}
/**********************
拡張XLM
**********************/
void ExtensionXLM(Indiv MainPare, Indiv SubPare[], int pare_count, Indiv child[])
{
	double SubPare_Gra[DEM] = {0}; /*基準となる重心*/
	double sum_n[DEM] = {0}; /*x,yそれぞれの和を一時的に保存しておく配列*/
	int i,j,k;
	Indiv sub_child[Nc];
	Init_Indiv(sub_child,Nc);
	
	/*ベースとなる重心を求める*/
	for(i=0;i<pare_count;i++){
		for(j=0;j<DEM;j++){
			sum_n[j] += SubPare[i].n[j]; /*親のx,yをそれぞれ足す*/
		}
	}
	for(i=0;i<DEM;i++){
		SubPare_Gra[i] = sum_n[i]/pare_count; /*親のそれぞれx,yの和を割る*/
	}
	/*重心から親に対するベクトルを求める*/
	double vector[pare_count][DEM];
	for(i=0;i<pare_count;i++){
		for(j=0;j<DEM;j++){
			vector[i][j] = SubPare[i].n[j] - SubPare_Gra[j];
		}
	}
	/*子個体生成*/
	double sum_coe[DEM]={0},coe; /*親のベクトル*乱数の総和*/
	double child_save[DEM];
	for(k=0;k<Nc;k++){
		true_flag = 1;
		while(true_flag){
			for(i=0;i<DEM;i++){
				sum_coe[i] = 0;
			}
			for(i=0;i<pare_count;i++){
				coe = rand_normal(0,1/sqrt(pare_count));
				for(j=0;j<DEM;j++){
					sum_coe[j] += coe * vector[i][j];
				}
			}
			for(i=0;i<DEM;i++){
				child_save[i] = MainPare.n[i]+sum_coe[i];
			}
			if(fabs(child_save[0]) <= 100.00 && fabs(child_save[1]) <= 100.00){
				for(i=0;i<DEM;i++){
					child[k].n[i] = child_save[i];
				}
				true_flag = 0;
			}
		}
	}
}
/**********
評価値の降順に並べ替え
*********/
void sort_win(Indiv pare[],int N){
	int i,j;
	Indiv tmp_pare;
	/*ソートする*/
	for(i=0;i<N-1;i++){
		for(j=N-1;j>i;j--){
			/* 前の要素の方が大きかったら */
			if(pare[j-1].win<pare[j].win){
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
void sort_distance(Indiv *unit,int N)
{
	int i,j,k;
	int save_obj;
	double save_dis;
	/*ソートする*/
	for(i=0;i<N-1;i++){
		for(j=N-1;j>i;j--){
			/* 前の要素の方が大きかったら */
			if(unit->dis[j]<unit->dis[j-1]){
				/* 個体番号を交換する */
				save_obj = unit->obj[j]; 
				unit->obj[j] = unit->obj[j-1];
				unit->obj[j-1] = save_obj;
				/* 個体番号を交換する */

				save_dis = unit->dis[j]; /* 交換する */
				unit->dis[j] = unit->dis[j-1];
				unit->dis[j-1] = save_dis;
            		}
            	}
        }
}

/*****************
乱数生成
*****************/
double GetRand_Real(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	get_save = getrand_int32Max/2;
 
  return ((double)genrand_int32()-get_save)/get_save*pGetMax;
}
/*****************
正規分布
*****************/
double rand_normal( double mu, double sigma )
{
    double z=sqrt( -2.0*log(Uniform()) ) * sin( 2.0*M_PI*Uniform() );
    return mu + sigma*z;
}
/*****************
乱数生成
*****************/
double Uniform( void )
{
    return genrand_real3();
}
/*************
距離計測
*************/
double cal_coord_distance(Indiv *one,Xy_str *coord)
{
	double cal_sum = 0;
	int i;
	for(i=0;i<DEM;i++){
		cal_sum += (one->n[i] - coord->n[i])*(one->n[i] - coord->n[i]);
	}
	return( sqrt(cal_sum) );
}

/*************
距離計測

*************/
double cal_kotai_distance(Indiv one, Indiv another)
{
	int i;
	double cal_sum = 0;
	for(i=0;i<DEM;i++){
		cal_sum += (one.n[i] - another.n[i])*(one.n[i] - another.n[i]);
	}
	return sqrt(cal_sum);
}

/************
親のナンバーズ.rexstarで利用
************/
void Pare_Numbers(Indiv pare[])
{
	int i,j;
	/*絶対値が一番小さいパラメータを求める*/

	for(i=0;i<Np*2;i++){
		for(j=i+1;j<Np*2;j++){
			Numbers(&pare[i],&pare[j]);
		}
	}
}

/*************
対戦させる関数 0で1番目の引数の個体が勝ち、1で2番目の引数の個体が勝ち
*************/
int Numbers(Indiv *one,Indiv *another)
{
	int i;
	int min_one = 0,min_another = 0;
	double dis_one = cal_coord_distance(one,&Optimal[0]);
	double dis_another = cal_coord_distance(another,&Optimal[0]);
	double save_one;
	double save_another;

	for(i=1;i<Optimal_N;i++){
		if(i == 3){
			save_one = cal_coord_distance(one,&Optimal[i])+Optimal[i].hosei_n;
			save_another = cal_coord_distance(another,&Optimal[i])+Optimal[i].hosei_n;
		}else if(i != 3){
			save_one = cal_coord_distance(one,&Optimal[i])+Optimal[i].hosei_n;
			save_another = cal_coord_distance(another,&Optimal[i])+Optimal[i].hosei_n;
		}
		if(save_one < dis_one){
			dis_one = save_one;
			min_one = i;
		}
		if(save_another < dis_another){
			dis_another = save_another;
			min_another = i;
		}
	}
		
	double distance_one;
	double distance_another;
	
	/*最適値に対して絶対値が一番小さい方が勝ち*/
	distance_one = cal_coord_distance(one,&Optimal[min_one]);
	distance_another = cal_coord_distance(another,&Optimal[min_another]);

	one->eval = distance_one;
	another->eval = distance_another;
	/*元：距離が近いほうが勝ち*/
	if(one->eval < another->eval){
		one->win++;
	}else if(one->eval > another->eval){
		another->win++;
	}
	battle_n++;
}
/*****************
再帰的にニッチを割り当てていく
*****************/
void Set_Nitch(int i){
	int tmp_NeighList2; /*近傍リスト内の個体の番号を一時的に保存しておく*/
	int j;
  count_nitch=0;
	if(Opponent[i].flag != 1){
		/*ニッチに属していなければニッチ番号を割り振る*/
		Opponent[i].nitch = count_nitch;
		Opponent[i].flag = 1;
			/*近傍リストを見ていく*/
		for(j=0;j<Kp;j++){
			tmp_NeighList2 = Opponent[i].Neigh_List2[j];
			Set_Nitch(tmp_NeighList2);
			count_nitch++;
		}
	}
}
/**********
枠を描画
***********/
void Prot_Frame(SDL_Surface *window)
{
	char num[5],num2[5],num3[5];
	lineColor(window, 
		0, 
		WINDOW_Y/2,
		WINDOW_X,
		WINDOW_Y/2,
		0x000000ff);
	/*Y軸*/
	lineColor(window,
		1,
		1,
		1,
		WINDOW_Y,
		0x000000ff);
	/*内枠X軸上側*/
	lineColor(window,
		1,
		1,
		WINDOW_X,
		1,
		0x000000ff);
	/*Y軸*/
	lineColor(window,
		WINDOW_X-1,
		1,
		WINDOW_X-1,
		WINDOW_Y-1,
		0x000000ff);
	/*内枠X軸上側*/
	lineColor(window,
		1,
		WINDOW_Y-1,
		WINDOW_X-1,
		WINDOW_Y-1,
		0x000000ff);
	lineColor(window,
		WINDOW_X/2,
		0,
		WINDOW_X/2,
		WINDOW_Y,
		0x000000ff);
	sprintf(num,"0");
	sprintf(num2,"-100");
	sprintf(num3,"100");
	stringColor(window, center[0],center[1], num, 0x000000ff);
	stringColor(window, 0,WINDOW_Y/2, num2, 0x000000ff);
	stringColor(window, WINDOW_X-25,WINDOW_Y/2, num3, 0x000000ff);
	stringColor(window, WINDOW_X/2,0, num3, 0x000000ff);
	stringColor(window, WINDOW_X/2,WINDOW_Y-10, num2, 0x000000ff);

	char num4[10];
	sprintf(num4,"%d",end_count);
	stringColor(window, 50,50, num4, 0x000000ff);
}

/***************
敵集団をプロット
***************/
void Opponent_Prot(SDL_Surface *window)
{
	int i;
	char tmp_n[5];
	/*個体を描画*/
	for(i=0;i<No;i++){
	/*
		sprintf(tmp_n,"%d",Opponent[i].nitch);
		stringColor(window, (Opponent[i].n[0]*2)+center[0]-2,
			    (Opponent[i].n[1]*2)+center[1]-2, tmp_n, 0x000000ff);
	*/
		trigonColor(window, (Opponent[i].n[0]*2)+center[0],
				    (Opponent[i].n[1]*2)+center[1]-5,
				    (Opponent[i].n[0]*2)+center[0]+6,
				    (Opponent[i].n[1]*2)+center[1]+6,
				    (Opponent[i].n[0]*2)+center[0]-5,
				    (Opponent[i].n[1]*2)+center[1]+6,
				    0x0000ffff);	// 三角形を描画
	}
}  
/********************
pop集団をプロット
********************/
void Pop_Prot(Indiv pop[],SDL_Surface *window)
{
	int i;
	for(i=0;i<Ns;i++){
		if(pop[i].flag == 0){
			circleColor(window,
				(pop[i].n[0]*2)+center[0],
				(pop[i].n[1]*2)+center[1],
				4,
				0xff0000ff);
		}
	}
}
/********************
集団をプロット
********************/
void Unit_Prot(Indiv pop[],SDL_Surface *window,int N)
{
	int i;
	for(i=0;i<N;i++){
		circleColor(window,
			(pop[i].n[0]*2)+center[0],
			(pop[i].n[1]*2)+center[1],
			4,
			0x000000ff);
	}
}
/********************
最適解をプロット
********************/
void Unit_Optimal(SDL_Surface *window)
{
	int i;
	for(i=0;i<Optimal_N;i++){
		boxColor(window,
			(Optimal[i].n[0]*2)+center[0]-4,
			(Optimal[i].n[1]*2)+center[1]-4,
			(Optimal[i].n[0]*2)+center[0]+4,
			(Optimal[i].n[1]*2)+center[1]+4,
			0x000000ff);
	}
}
