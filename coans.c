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
#define Ns		30	/*初期集団数*/
#define No		30	/*敵集団数*/
#define Np		4	/*親個体数*/
#define Nc		10	/*子個体数*/
#define DEM		2	/*次元数*/
#define T		1	/*ステップサイズ*/
#define END_STEP	1000	/*終わるタイミング*/
#define WINDOW_X	800	/*定義域*/
#define WINDOW_Y	800	/*地域*/
#define PROT_X		600	/*定義域*/
#define PROT_Y		600	/*地域*/
#define K		6	/*ニッチの集団数*/
#define DELETE		100
#define Optimal_N	4

double center[2] = {WINDOW_X/2,WINDOW_Y/2};
typedef struct{
	double n[DEM];
	int flag; /*1...主親　2...副親*/
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
int end_count = 0;


void Init_Indiv(Indiv pare[],int N);
void Init_Opponent(void);
void Init_Opponent_BattleData(void);
void Init_Optimal(void);

void creat_child(int pare[],int child[]);

void Child_Opponent_Numbers(Indiv child[],Indiv Opponent[]); /*子個体と相手集団を戦わせる*/
void Pare_Numbers(Indiv pare[]); /*REXstar内で扱う*/
int Numbers(Indiv *one,Indiv *another);

void sort_win(Indiv pare[],int N);
void sort_distance(Indiv *unit,int N);
double cal_distance(double a_x,double a_y,double b_x,double b_y);
double cal_kotai_distance(Indiv one, Indiv another);

void NeighList_Opponent(void);
void AnsList3(Indiv pop[]);
void Set_Nitch(int i);
void ExtensionXLM(Indiv MainPare, Indiv SubPare[], int pare_count, Indiv child[]);
void Update_Opponent(Indiv child);

static int thread_keyboad(void *data);
static int thread_window(void *data);
double GetRand_Real(double pGetMax);
double rand_normal( double mu, double sigma );
double Uniform( void );
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
	Indiv MainPare;

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
			for(i=0;pop[MainPare_n].Neigh_List2[i] != -1 && i<K;i++){}
			pare_count = i;
			/*近傍リストに個体があれば主親にして、ループを抜ける*/
			if(pare_count  > 0){
				MainPare = pop[MainPare_n]; /*主親へ*/
				while_flag = 0;
			}
		}
		/*副親を選ぶ*/
		if(pare_count < Np){
			/*規定の親個体数より近傍リストの個体が少なければリストの個体全てを副親に*/
			int SubPare_n[pare_count];
			Indiv SubPare[pare_count];
			for(i=0;i<pare_count;i++){
				SubPare_n[i] = MainPare.Neigh_List2[i]; /*副親の番号取得*/
				SubPare[i] = pop[SubPare_n[i]];
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
						/*個体と取り出して空にする*/
						MainPare.Neigh_List2[tmp_rand] = -1;
						while_flag = 0;
					}
				}
			}
			/*主親と副親で拡張XLM*/
			ExtensionXLM(MainPare,SubPare,pare_count,child);
		}
		
		for(i=0;i<Nc;i++){
			for(j=i+1;j<Nc;j++){
				Numbers(&child[i],&child[j]);
			}
			Numbers(&child[i],&MainPare);
		}
		/*評価の良い順にソート*/
		sort_win(child,Nc);
		/*主親より評価値のいい子個体がいたら入れ替える*/
		if(MainPare.win < child[0].win){
			MainPare = child[0];
		}
		pop[MainPare_n] = MainPare;
		Pop_Prot(pop,window);
		Unit_Optimal(window);
		Prot_Frame(window);
		SDL_Flip(window);
		/*
		if(end_count%20 == 0){
			sprintf(name,"./picture/nitch/10/opponent0%d.bmp",end_count);
			SDL_SaveBMP(window,name);
		}
		*/
		//SDL_Delay(500);

		/*構造体初期化*/
		Init_Indiv(child,Nc);
		/*試行回数を数える*/
		end_count++;
		/*
		if(end_count > END_STEP){
			end_flag = 0;
		}
		*/
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
	double tmp_n = -100;
	int i,j;
	Optimal[0].n[0] = -50;
	Optimal[0].n[1] = -50;
	
	Optimal[1].n[0] = -50;
	Optimal[1].n[1] =  50;
	
	Optimal[2].n[0] =  50;
	Optimal[2].n[1] = -50;
	
	Optimal[3].n[0] =  50;
	Optimal[3].n[1] =  50;

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
/********************
近傍リスト3を作る
********************/
void AnsList3(Indiv pop[])
{
	int i,j,k;
	int kotai_count;
	int tmp_obj;
	int tmp_dis;
	int tmp_anslist3[K];
	for(i=0;i<Ns;i++){
		for(j=0;j<K;j++){
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
	/*近傍リスト3生成*/
	int save_obj;
	int count_nitch_flag = 0;
	for(i=0;i<Ns;i++){
		kotai_count=0; /*近傍リストをカウント*/
		/*近傍リストを作る個体の近傍をＫ番目まで見る*/
		for(j=0;j<Ns;j++){
			/*近傍Ｋ番目までの個体の近傍にiが存在すればリストに加える*/
			for(k=1;k<K;k++){
				if(pop[j].obj[k] == i){
					pop[i].Neigh_List2[kotai_count] = j;
					kotai_count++;
					printf("pop[%d].AnsList[%d] = %d\n",i,kotai_count,pop[i].Neigh_List2[kotai_count]);
				}
			}
		}
	}
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
double cal_distance(double a_x,double a_y,double b_x,double b_y){
	double cal_save;
	cal_save = pow(b_x-a_x,2)+pow(b_y-a_y,2);
	return( sqrt(cal_save) );
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


/*********
ナンバーズ
*********/
void Child_Opponent_Numbers(Indiv child[],Indiv Opponent[])
{
	int i,j;
	/*絶対値が一番小さいパラメータを求める*/
	for(i=0;i<Nc;i++){
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