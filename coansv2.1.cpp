#include<iostream>
#include<vector>
#include <algorithm>

#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<SDL/SDL.h>
#include<SDL/SDL_video.h>
#include <SDL/SDL_gfxPrimitives.h>
#include"./header/MT.h"
using namespace std;

#define INIT		      100	/*解集団の初期化範囲*/
#define INIT_OPPOMEMT	100	/*敵集団の初期化範囲*/
#define INIT_OPTIMAL	100	/*敵集団の初期化範囲*/
#define Ns		        5	/*初期集団数*/
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

struct Indiv{
  vector<double>  n; //解
	vector<int>     result; //対戦結果
	vector<int>     List2;
	int             nitch; /*0...所属ニッチなし 1~...所属しているニッチ番号*/
public:
	vector<int> tmp;
	void Init(); //解集団初期化
	void SetNitch();
};

/*対戦結果と近傍リストとニッチ番号を初期化*/
void Indiv::Init()
{
  result.erase(result.begin(),result.end()-1);
  List2.erase(List2.begin(),List2.end()-1);
  nitch = 0;
}

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
	Indiv pop[Ns];
	
	clock_t start,end;
	double time_count;
	int i,j,k;

	/*SDL初期化*/
	//SDL_Surface *window; // ウィンドウ（画像）データ、及び、文字列（画像）へのポインタ
	
	/*SDLの全ての機能を初期化*/
	/*
	if ( SDL_Init(SDL_INIT_VIDEO)  < 0 ){
	  printf("failed to initialize SDL.\n");
	  fprintf(stderr,"%s\n",SDL_GetError());
	  SDL_GetError();
	  exit(-1); 
	}
	*/
	/* ウィンドウ生成（800*600、1677万色）*/
	/*
	if((window = SDL_SetVideoMode(WINDOW_X, WINDOW_Y, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}
	*/
	//SDL_Thread *thr_keyboad,*thr_window;
	//thr_keyboad = SDL_CreateThread(thread_keyboad,NULL);
	/*初期解生成*/
	for(i=0;i<Ns;i++){
		for(j=0;j<DEM;j++){
			pop[i].n.push_back( GetRand_Real(INIT) ); //実数値乱数
		}
	}
	AnsList3(pop);
	//SDL_Quit();
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
	int tmp_anslist3[Kp];
  vector<double>::iterator min;
  size_t index;
	
  vector<vector<double> > DisSave(Ns);
  vector<vector<int> >    IndexSave(Ns);
  
	/*自身以外の個体との距離を総当たりで計測.自身も含む.*/
	/*ついでに近い順に個体の番号も取得*/
	for(i=0;i<Ns;i++){
		for(j=0;j<Ns;j++){
		  if(i != j)
	  		DisSave[i].push_back(cal_kotai_distance(pop[i],pop[j]));
	  	else
	  	  DisSave[i].push_back(10000);
		}
	  for(j=0;j<DEM;j++){
      min = min_element(DisSave[i].begin(),DisSave[i].end());
      index = distance(DisSave[i].begin(),min);
      DisSave[i][index] = 10000;
    }
	}
	//ベクター確認用
	/*
  for(i=0;i<DisSave.size();i++){
    for(j=0;j<DisSave[i].size();j++){
      cout << j << ":" << DisSave[i][j] << endl;
    }
		cout << endl;
  }
  */
	/*
	for(i=0;i<Ns;i++){
		for(j=0;j<Kp;j++){
			pop[i].Neigh_List2[j] = pop[i].obj[j];
			//printf("pop[%d].AnsList[%d] = %d\n",i,j,pop[i].Neigh_List2[j]);
		}
		//putchar('\n')
	}
	*/
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
/*
void Pare_Numbers(Indiv pare[])
{
	int i,j;
	for(i=0;i<Np*2;i++){
		for(j=i+1;j<Np*2;j++){
			Numbers(&pare[i],&pare[j]);
		}
	}
}
*/

/*************
対戦させる関数 0で1番目の引数の個体が勝ち、1で2番目の引数の個体が勝ち
*************/
/*
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
	
	distance_one = cal_coord_distance(one,&Optimal[min_one]);
	distance_another = cal_coord_distance(another,&Optimal[min_another]);

	one->eval = distance_one;
	another->eval = distance_another;
	if(one->eval < another->eval){
		one->win++;
	}else if(one->eval > another->eval){
		another->win++;
	}
	battle_n++;
}
*/
/*****************
再帰的にニッチを割り当てていく
*****************/
/*
void Set_Nitch(int i){
	int tmp_NeighList2; 
	int j;
  count_nitch=0;
	if(Opponent[i].flag != 1){
		Opponent[i].nitch = count_nitch;
		Opponent[i].flag = 1;
		for(j=0;j<Kp;j++){
			tmp_NeighList2 = Opponent[i].Neigh_List2[j];
			Set_Nitch(tmp_NeighList2);
			count_nitch++;
		}
	}
}
*/
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
	/*
	for(i=0;i<No;i++){
		sprintf(tmp_n,"%d",Opponent[i].nitch);
		stringColor(window, (Opponent[i].n[0]*2)+center[0]-2,
			    (Opponent[i].n[1]*2)+center[1]-2, tmp_n, 0x000000ff);
		trigonColor(window, (Opponent[i].n[0]*2)+center[0],
				    (Opponent[i].n[1]*2)+center[1]-5,
				    (Opponent[i].n[0]*2)+center[0]+6,
				    (Opponent[i].n[1]*2)+center[1]+6,
				    (Opponent[i].n[0]*2)+center[0]-5,
				    (Opponent[i].n[1]*2)+center[1]+6,
				    0x0000ffff);	// 三角形を描画
	}
	*/
}  
/********************
pop集団をプロット
********************/
void Pop_Prot(Indiv pop[],SDL_Surface *window)
{
	int i;
	/*
	for(i=0;i<Ns;i++){
		if(pop[i].flag == 0){
			circleColor(window,
				(pop[i].n[0]*2)+center[0],
				(pop[i].n[1]*2)+center[1],
				4,
				0xff0000ff);
		}
	}
	*/
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

