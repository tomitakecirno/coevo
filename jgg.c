/***********************************************************************************
***********************************************************************************/

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
#define Ns		20	/*初期集団数*/
#define No		20	/*敵集団数*/
#define Np		3	/*親個体数*/
#define Nc		10	/*子個体数*/
#define DEM		2	/*次元数*/
#define T		1	/*ステップサイズ*/
#define END_STEP	300	/*終わるタイミング*/
#define WINDOW_X	800	/*定義域*/
#define WINDOW_Y	800	/*地域*/
#define PROT_X		600	/*定義域*/
#define PROT_Y		600	/*地域*/
#define K		3	/*ニッチの集団数*/
#define DELETE		50
#define Optimal_N	1

double center[2] = {WINDOW_X/2,WINDOW_Y/2};


typedef struct{
	double n[DEM];
	int flag;
	int win;
	int nitch; /*0...所属ニッチなし 1~...所属しているニッチ番号*/
	int comp_flag;
	int gene_count;
	int delete_flag;
	int obj[No];
	double dis[No];
	int Neigh_List2[K];
}Indiv;
Indiv Opponent[No];

typedef struct{
	double n[DEM];
}Xy_str;
Xy_str Gra_Nitch[No];
Xy_str Optimal[Optimal_N];

int end_flag = 1;
int true_flag = 1;
int while_flag = 1;
int count_nitch=1;


void Init_Indiv(Indiv pare[],int N);
void Init_Opponent(void);
void Init_Opponent_BattleData(void);
void Init_Optimal(void);

void creat_child(int pare[],int child[]);

void Child_Opponent_Numbers(Indiv child[],Indiv Opponent[]); /*子個体と相手集団を戦わせる*/
void Pare_Numbers(Indiv pare[]); /*REXstar内で扱う*/
int Numbers(Indiv *one,Indiv *another);

void sort_win(Indiv pare[],int N);
void sort_distance(Indiv pare[],int N);
double cal_distance(double a_x,double a_y,double b_x,double b_y);

void RexStar(Indiv pare[],Indiv child[],SDL_Surface *window);
void NeighList_Opponent(void);
void Set_Nitch(int i);
void Update_Opponent(Indiv child);

static int thread_keyboad(void *data);
static int thread_window(void *data);
double GetRand_Real(double pGetMax);
void Prot_Frame(SDL_Surface *window);
void Opponent_Prot(SDL_Surface *window);
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

	int i,j,k; /*for文用*/
	char name[10];
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/
	Init_Indiv(pop,Ns); /*解集団構造体初期化*/
	Init_Indiv(pare,Np); /*親集団構造体初期化*/
	Init_Indiv(child,Nc); /*子集団構造体初期化*/
	Init_Opponent(); /*相手集団構造体初期化*/
	Init_Optimal();
	
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
	int count= 0; /*親の数をカウント*/
	int tmp;
	int end_count = 0;
	while(end_flag){
		/*ウィンドウの（背景）色を変更（白色の矩形で塗りつぶし）*/
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
		/*子集団と親集団をpare_childへ統合する*/
		for(i=0;i<Np;i++){
			pare_child[i] = pare[i];
		}
		for(i=0;i<Nc;i++){
			pare_child[Np+i] = child[i];
		}
		/*子個体を相手集団と戦わせる*/
		Child_Opponent_Numbers(pare_child,Opponent);
		/*評価の良い順にソート*/
		sort_win(pare_child,Np+Nc);
		/*
		Np個体残そう
		*/
		for(i=0;i<Np;i++){
			for(j=0;pop[j].flag != 1;j++){}
			pop[j] = pare_child[i];
			pop[j].flag = 0;
			for(k=0;Opponent[k].delete_flag == 0 && k<No;k++){}
			if(k != No){
				Opponent[k] = pare_child[i];
				Opponent[k].comp_flag = 1;
				Opponent[k].delete_flag = 0;
			}if(k == No){
				Update_Opponent(pare_child[i]);
			}
		}
		/*生存競争を1世代に1回でも行っていればカウント初期化。行っていなければカウント。*/
		for(i=0;i<No;i++){
			if(Opponent[i].delete_flag != 1){
				if(Opponent[i].comp_flag == 0){
					Opponent[i].gene_count++;
				}else if(Opponent[i].comp_flag == 1){
					Opponent[i].gene_count = 0;
				}
				Opponent[i].comp_flag = 0;
				if(DELETE <= Opponent[i].gene_count){
					Opponent[i].delete_flag = 1;
					Opponent[i].nitch = 0;
				}
			}
		}

		/*子個体の最良Np個を初期集団へ（世代交代）*/
		/*pop集団をプロット*/
		SDL_FillRect(window, NULL, 0x00ffffff);
		Pop_Prot(pop,window);
		/*
		Unit_Optimal(window);
		Prot_Frame(window);
		SDL_Flip(window);
		if(end_count%20 == 0){
			sprintf(name,"./picture/pop/pop0%d.bmp",end_count);
			SDL_SaveBMP(window,name);
		}
		*/
		/*敵集団をプロット*/
		//SDL_FillRect(window, NULL, 0x00ffffff);
		Opponent_Prot(window);
		Unit_Optimal(window);
		Prot_Frame(window);
		SDL_Flip(window);
		if(end_count%20 == 0){
			sprintf(name,"./picture/list2/opponent0%d.bmp",end_count);
			SDL_SaveBMP(window,name);
		}
				//SDL_Delay(500);
		//Unit_Prot(child,window,Nc);
		/*親集団構造体初期化*/
		Init_Indiv(pare,Np);
		/*子集団構造体初期化*/
		Init_Indiv(child,Nc); 
		/*試行回数を数える*/
		end_count++;
		if(end_count > END_STEP){
			end_flag = 0;
		}
	}

	double sum_unit[DEM] = {0};
	
	for(i=0;i<Ns;i++){
		for(j=0;j<DEM;j++){
			sum_unit[j] += pop[i].n[j];
		}
	}
	sum_unit[0] = sum_unit[0]/Ns;
	sum_unit[1] = sum_unit[1]/Ns;

	printf("pop = (%.2f,%.2f)\n",sum_unit[0],sum_unit[1]);
	for(i=0;i<Optimal_N;i++){
		printf("Optimal[%d] = (%.2f,%.2f)\n",i,Optimal[i].n[0],Optimal[i].n[1]);
	}
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
		pare[i].gene_count = 0;
		pare[i].comp_flag = 0;
		pare[i].delete_flag = 0;
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
		Opponent[i].gene_count = 0;
		Opponent[i].comp_flag = 0;
		Opponent[i].delete_flag = 0;
	}
	Init_Opponent_BattleData();
	NeighList_Opponent();
}
/*******************
敵集団の戦闘データ初期化
*******************/
void Init_Opponent_BattleData(void)
{
	int i,j;
	for(i=0;i<No;i++){
		Opponent[i].flag = 0;
		Opponent[i].win = 0;
		Opponent[i].nitch = 0;
		for(j=0;j<K;j++){
			Opponent[i].Neigh_List2[j] = -1;
		}
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
	int tmp_obj;
	int tmp_dis;
	for(i=0;i<No;i++){
		Opponent[i].flag = 0;
		if(i == No)
		Opponent[No].flag = -1;
		Opponent[i].win = 0;
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
	/*重心を求める*/
	double	sum_x,sum_y;
	int	count_sum;
	for(i=1;i<count_nitch;i++){
		count_sum=0;
		sum_x = 0;
		sum_y = 0;
		for(j=0;j<No;j++){
			if(Opponent[j].nitch == i){
				sum_x += Opponent[j].n[0];
				sum_y += Opponent[j].n[1];
				count_sum++;
			}
		}
		Gra_Nitch[i].n[0] = sum_x/(double)count_sum;
		Gra_Nitch[i].n[0] = sum_y/(double)count_sum;
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
void sort_distance(Indiv pare[],int N)
{
	int i,j,k;
	int save_obj;
	double save_dis;
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
double cal_distance(double a_x,double a_y,double b_x,double b_y){
	double cal_save;
	cal_save = pow(b_x-a_x,2)+pow(b_y-a_y,2);
	return( sqrt(cal_save) );
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
	double dis_one = cal_distance(one->n[0],one->n[1],Optimal[0].n[0],Optimal[0].n[1]);
	double dis_another = cal_distance(another->n[0],another->n[1],Optimal[0].n[0],Optimal[0].n[1]);

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
	}/*else if(distance_one == distance_another){
		one->win++;
		another->win++;
	}*/
}


/*************
相手集団を更新
*************/
void Update_Opponent(Indiv child)
{
	int i,j,k;
	int obj_count;
	int min_indiv;
	double min_dis;
	double tmp_dis;
	
	/*いらない相手を残すと有害。勝ち数で残す個体判断しているので、余計な勝ち数がカウントされる。*/
	/*距離が最小のニッチの重心を見つけて、ニッチ番号を取得*/
	child.nitch = 1;
	min_dis = cal_distance(Gra_Nitch[1].n[0],Gra_Nitch[1].n[1],
			       child.n[0],child.n[1]);
	for(i=1;i<count_nitch;i++){
		if(cal_distance(Gra_Nitch[i].n[0],Gra_Nitch[i].n[1],child.n[0],child.n[1]) < min_dis){
		   	min_dis = cal_distance(Gra_Nitch[i].n[0],Gra_Nitch[i].n[1],child.n[0],child.n[1]);
		   	child.nitch = i;
		}
	}
	/*戦闘データ初期化*/
	for(i=0;i<No;i++){
		if(Opponent[i].nitch == child.nitch){
			Opponent[i].win = 0;
			Opponent[i].flag = 0;
		}
	}
	child.win = 0;
	child.flag = 0;
	/*該当するニッチの個体と子個体でリーグ戦*/
	for(i=0;i<No;i++){
		if(Opponent[i].nitch == child.nitch && Opponent[i].flag == 0){
			for(j=i+1;j<No;j++){
				if(Opponent[i].nitch == child.nitch){
					Numbers(&Opponent[i],&Opponent[j]);
				}
			}
			Numbers(&Opponent[i],&child);	/*ナンバーズ*/
			Opponent[i].flag = 1; 		/*フラグ立て*/
			Opponent[i].comp_flag = 1; 	/*生存競争したフラグ*/
		}
	}
	/*評価値の最小個体を求める*/
	int min_win = No;
	for(i=0;i<No;i++){
		if(Opponent[i].nitch == child.nitch && Opponent[i].win < min_win){
			min_win = Opponent[i].win;
			min_indiv = i;
		}
	}
	/*削除予定の個体の枠へ子個体を入れる、または相手集団の中で子個体より弱い相手のところに入れる*/
	if(Opponent[min_indiv].win < child.win){
			Opponent[min_indiv] = child;
			Opponent[min_indiv].comp_flag = 1;
			Opponent[min_indiv].delete_flag = 0;
	}
	/*対戦データおよびニッチ番号初期化*/
	Init_Opponent_BattleData();
	NeighList_Opponent();
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
