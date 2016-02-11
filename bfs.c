#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<SDL/SDL.h>
#include<SDL/SDL_video.h>
#include <SDL/SDL_gfxPrimitives.h>
#include"./header/MT.h"

#define INIT		100	/*解集団の初期化範囲*/
#define INIT_OPPOMEMT	100	/*敵集団の初期化範囲*/
#define INIT_OPTIMAL	100	/*敵集団の初期化範囲*/
#define Ns		25	/*初期集団数*/
#define No		25	/*敵集団数*/
#define Np		3	/*親個体数*/
#define Nc		10	/*子個体数*/
#define DEM		2	/*次元数*/
#define ALFA		1
#define BETA		2
#define RAD_SHARE	90
#define MAX_POINT	25
#define T		1	/*ステップサイズ*/
#define END_STEP	300	/*終わるタイミング*/
#define WINDOW_X	800	/*定義域*/
#define WINDOW_Y	800	/*地域*/
#define PROT_X		600	/*定義域*/
#define PROT_Y		600	/*地域*/
#define Optimal_N	1

double center[2] = {WINDOW_X/2,WINDOW_Y/2};
typedef struct{
	double n[DEM];
	int flag;
	int win;
	double eval;
}Indiv;
Indiv Opponent[No];

typedef struct{
	double n[DEM];
}Xy_str;
Xy_str Optimal[Optimal_N];

int end_flag;
int true_flag;
int while_flag;
int end_count;

static int thread_keyboad(void *data);

void Init_Indiv(Indiv pare[],int N);
void Init_Indiv(Indiv pare[],int N);
void Init_Opponent();
void Init_Optimal(void);

void sort_win(Indiv pare[],int N);
double GetRand_Real(double pGetMax);

double cal_distance(double a_x,double a_y,double b_x,double b_y);
double Sharing(double dis);
void FitnessShare(Indiv unit[],int N);

void RexStar(Indiv pare[],Indiv child[],SDL_Surface *window);

void Pare_Numbers(Indiv pare[]);
void Child_Opponent_Numbers(Indiv child[],Indiv Opponent[]);
int Numbers(Indiv *one,Indiv *another);
double ScalingEval(int point,double eval);

void Opponent_Prot(SDL_Surface *window);
void Pop_Prot(Indiv pop[],SDL_Surface *window);
void Unit_Prot(Indiv pop[],SDL_Surface *window,int N);
void Unit_Optimal(SDL_Surface *window);
void Prot_Frame(SDL_Surface *window);

main(){

	int count;
	int tmp;
	int i,j,k;
	Indiv pop[Ns];		/*解集団配列*/
	Indiv pare[Np];		/*親集団配列*/
	Indiv child[Nc];	/*子集団配列*/

	init_genrand((unsigned)time(NULL)); /*乱数初期化*/
	Init_Indiv(pop,Ns); /*解集団構造体初期化*/
	Init_Indiv(pare,Np); /*親集団構造体初期化*/
	Init_Indiv(child,Nc); /*子集団構造体初期化*/
	Init_Opponent(); /*相手集団構造体初期化*/
	Init_Optimal();

	/*初期解生成*/
	for(i=0;i<Ns;i++){
		for(j=0;j<DEM;j++){
			pop[i].n[j] = GetRand_Real(INIT); /*実数値乱数*/
		}
	}

	/*SDL初期化*/
	SDL_Surface *window;
	
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

	end_flag = 1;
	end_count = 0;
	SDL_Thread *thr_keyboad,*thr_window;
	thr_keyboad = SDL_CreateThread(thread_keyboad,NULL);

	while(end_flag){
		/*親を選ぶ*/
		count = 0;
		while_flag = 1;
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
		/*子個体でナンバーズ*/
		for(i=0;i<Nc;i++){
			for(j=i;j<Nc;j++){
				Numbers(&child[i],&child[j]);
			}
		}
		/*評価の良い順にソート*/
		sort_win(child,Nc);
		/*Np個体残す*/
		for(i=0;i<Np;i++){
			for(j=0;pop[j].flag != 1;j++){}
			pop[j] = child[i];
			pop[j].flag = 0;
		}

		/*プロット処理*/
		SDL_FillRect(window, NULL, 0x00ffffff);
		Unit_Optimal(window);
		Prot_Frame(window);
		Pop_Prot(pop,window);
		SDL_Flip(window);

		Init_Indiv(pare,Np);
		Init_Indiv(child,Nc);
		end_count++;
		if(end_count > END_STEP){
			end_flag = 0;
		}
	}
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
					printf("end\n");
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
	}
}
/*********
敵集団初期化
*********/
void Init_Opponent(){
	int i,j,k;
	
	/*敵集団初期化*/
	for(i=0;i<No;i++){
		for(j=0;j<DEM;j++){
			Opponent[i].n[j] = GetRand_Real(INIT_OPPOMEMT); /*実数値乱数*/
		}
		Opponent[i].flag = 0;
		Opponent[i].win = 0;
	}
}

/*******************
最適解の戦闘データ初期化
*******************/
void Init_Optimal(void)
{
	int i,j;
	for(i=0;i<Optimal_N;i++){
		for(j=0;j<DEM;j++){
			Optimal[i].n[0] = GetRand_Real(INIT_OPTIMAL);
			Optimal[i].n[1] = GetRand_Real(INIT_OPTIMAL);
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

/*************
距離計測
*************/
double cal_distance(double a_x,double a_y,double b_x,double b_y)
{
	double cal_save;
	cal_save = pow(b_x-a_x,2)+pow(b_y-a_y,2);
	return( sqrt(cal_save) );
}

/*********
ナンバーズ
*********/
void Child_Opponent_Numbers(Indiv child[],Indiv Opponent[])
{
	int i,j;
	/*絶対値が一番小さいパラメータを求める*/
	for(i=0;i<Nc+Np;i++){
		for(j=0;j<No;j++){
			Numbers(&child[i],&Opponent[j]);
		}
	}
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
	double dis_one = cal_distance(one->n[0],
				      one->n[1],
				      Optimal[0].n[0],
				      Optimal[0].n[1]);
	double dis_another = cal_distance(another->n[0],
					  another->n[1],
					  Optimal[0].n[0],
					  Optimal[0].n[1]);

	for(i=1;i<Optimal_N;i++){
		if(cal_distance(one->n[0],one->n[1],Optimal[i].n[0],Optimal[i].n[1]) < dis_one){
			dis_one = cal_distance(one->n[0],one->n[1],Optimal[i].n[0],Optimal[i].n[1]);
			min_one = i;
		}
		if(cal_distance(another->n[0],another->n[1],Optimal[i].n[0],Optimal[i].n[1]) < dis_another){
			dis_one = cal_distance(one->n[0],one->n[1],Optimal[i].n[0],Optimal[i].n[1]);
			min_another = i;
		}
	}
		
	double distance_one;
	double distance_another;
	
	/*最適値に対して絶対値が一番小さい方が勝ち*/
	distance_one = cal_distance(one->n[0],one->n[1],
					Optimal[min_one].n[0],Optimal[min_one].n[1]);
	distance_another = cal_distance(another->n[0],another->n[1],
					Optimal[min_another].n[0],Optimal[min_another].n[1]);
	if(distance_one < distance_another){
		one->win++;
	}else if(distance_one > distance_another){
		another->win++;
	}
}

/************
JGG＋REX
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
	sort_win(sum_pare,Np*2);
	/*上位半分の両個体の重心、下位半分の両個体の重心を求める*/
	double high_gra[DEM] = {0};
	for(j=0;j<DEM;j++){
		sum_n[j] = 0;
	}
	for(i=0;i<Np;i++){
		for(j=0;j<DEM;j++){
			sum_n[j] += sum_pare[i].n[j];
		}
	}

	for(i=0;i<DEM;i++){
		high_gra[i] = sum_n[i]/Np;
	}
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
			coe = GetRand_Real( sqrt(3/(DEM+1)) );
			for(j=0;j<DEM;j++){
				sum_coe[j] += coe * (sum_pare[i].n[j]-base_gra[j]);
			}
		}
		for(i=0;i<DEM;i++){
			child_save[i] = base_gra[i] + diag[i]*(high_gra[i]-base_gra[i])+sum_coe[i];
		}
		if(fabs(child_save[0]) <= 100.00 && fabs(child_save[1]) <= 100.00){
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
/****************
適応度共有法
****************/
void FitnessShare(Indiv unit[],int N)
{
	int i,j,k;
	double sh_sum;
	double tmp_dis;
	double fs_n;
	for(i=0;i<N;i++){
		/*分母を求める*/
		sh_sum = 0;
		for(j=0;j<N;j++){
			tmp_dis = cal_distance(unit[i].n[0],
					       unit[i].n[1],
					       unit[j].n[0],
					       unit[j].n[1]);
			sh_sum += Sharing(tmp_dis);
		}
		/*補正後の評価値を代入*/
		unit[i].eval = (double)unit[i].win/sh_sum;
		/*さらに補正*/
		unit[i].eval = ScalingEval(unit[i].win,unit[i].eval);
	}
}
/****************
シェアリング関数
****************/
double Sharing(double dis)
{
	double sharing_n;
	if(dis < RAD_SHARE){
		sharing_n = 1-pow(dis/RAD_SHARE,ALFA);
	}else {
		sharing_n = 0;
	}
	return sharing_n;
}
/****************
スケーリング関数
****************/
double ScalingEval(int point,double eval)
{
	double scaling_n;
	double weight_unit;

	weight_unit = 1-pow((double)point/MAX_POINT,BETA);
	scaling_n = weight_unit*eval;

	return scaling_n;
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
				    0x000000ff);	// 三角形を描画
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
	sprintf(num,"0");
	sprintf(num2,"-100");
	sprintf(num3,"100");
	stringColor(window, center[0],center[1], num, 0x000000ff);
	stringColor(window, WINDOW_X-PROT_X-30,WINDOW_Y/2, num2, 0x000000ff);
	stringColor(window, PROT_X,WINDOW_Y/2, num3, 0x000000ff);
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
				0x000000ff);
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
			0x00000000);
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






