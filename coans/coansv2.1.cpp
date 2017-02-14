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
#define Ns		        10	/*初期集団数*/
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
	vector<int> Result; //対戦結果
	vector<int> List1;	
	vector<int> List2;
	int eval;
	int nitch; /*0...所属ニッチなし 1~...所属しているニッチ番号*/
public:
	void Init();
};

/*対戦結果と近傍リストとニッチ番号を初期化*/
void Indiv::Init()
{
  Result.clear();
  List2.clear();
  nitch = 0;
  eval = 0;
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
vector<Indiv> MakeList(vector<Indiv> &pop);
vector<Indiv> AnsList1(vector<vector<int> > &IndexSave,vector<Indiv> &pop);
vector<Indiv> AnsList2(vector<vector<int> > &IndexSave,vector<Indiv> &pop);
int SetNitch(int nitch_number,int kotai,vector<Indiv> &pop);
void ExtensionXLM(int MainPare,vector<int> &SubPare,vector<Indiv> &child,vector<Indiv> &pop);
vector<Indiv> FitnessChild(vector<Indiv> &child, vector<Indiv> &pop);

static int thread_keyboad(void *data);
static int thread_window(void *data);
double GetRand_Real(double pGetMax);
int GetRand_Int(int pGetMax);
double rand_normal( double mu, double sigma );
double Uniform( void );
void Prot_Frame(SDL_Surface *window);
void Pop_Prot(vector<Indiv> &pop,SDL_Surface *window);
void Unit_Prot(vector<Indiv> &pop,SDL_Surface *window,int N);
void Unit_Optimal(SDL_Surface *window);
/***********
メインルーチン
***********/
main(){
	/*個体を格納する配列定義*/
	vector<Indiv> pop(Ns);
    
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/
	clock_t start,end;
	double time_count;

  /*
	//SDL初期化
	SDL_Surface *window; // ウィンドウ（画像）データ、及び、文字列（画像）へのポインタ
	//SDLの全ての機能を初期化
	if ( SDL_Init(SDL_INIT_VIDEO)  < 0 ){
	  printf("failed to initialize SDL.\n");
	  fprintf(stderr,"%s\n",SDL_GetError());
	  SDL_GetError();
	  exit(-1); 
	}
	//ウィンドウ生成（800*600、1677万色）
	if((window = SDL_SetVideoMode(WINDOW_X, WINDOW_Y, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}
	//SDL_Thread *thr_keyboad,*thr_window;
	//thr_keyboad = SDL_CreateThread(thread_keyboad,NULL);
	*/
	/*初期解生成*/
	for(int i=0;i<pop.size();i++){
    pop[i].Init(); //初期化
		for(int j=0;j<DEM;j++){
			pop[i].n.push_back( GetRand_Real(INIT) ); //実数値乱数
		}
	}
	//近傍リスト生成
	pop = MakeList(pop);
	//ニッチ番号振り分け
	int count_nitch = 1;
	int tmp;
	for(int i=0;i<Ns;i++){
    if(SetNitch(count_nitch,i,pop) == 1){
      count_nitch++;
    }
  }
  int MainPare;
  int tmpIndex;
  int tmpSub;
  vector<int> SubPare;
	//主親、副親を選ぶ
	MainPare = GetRand_Int(Ns);
	for(int i=0;i<DEM;i++){
	  if(pop[MainPare].List1.empty()){
      break;
    }
  	tmpIndex = GetRand_Int(pop[MainPare].List1.size());
  	tmpSub = pop[MainPare].List1[tmpIndex];
	  SubPare.push_back(tmpSub); //要素を追加
	  pop[MainPare].List1.erase(pop[MainPare].List1.begin()+tmpIndex); //追加した要素を削除
	}
  if(!SubPare.empty()){
    //子個体生成
  	vector<Indiv> child(Nc);
  	//拡張XLM
  	ExtensionXLM(MainPare,SubPare,child,pop);
  	child.push_back(pop[MainPare]);
  	//対戦
  	for(int i=0; i<pop.size(); i++){
  	  for(int j=0; j<child.size(); j++){
      	if(Numbers(&pop[i],&child[j])==0){
      	  pop[i].Result.push_back(1);
      	  child[j].Result.push_back(0);
      	}else{
      	  pop[i].Result.push_back(0);
      	  child[j].Result.push_back(1);
      	}
      }
    }
    //子個体＋主親の評価値取得
    child = FitnessChild(child,pop);
    for(int i=0;i<child.size();i++){
      cout << i << ":" << child[i].eval << endl;
    }
    //一番いい個体を集団へ戻す
    size_t index;
    vector<int> tmpEval;
    vector<int>::iterator max;
    for(int i=0;i<child.size();i++){
      tmpEval.push_back(child[i].eval);
    }
    max = max_element(tmpEval.begin(),tmpEval.end());
    index = distance(tmpEval.begin(),max);
    cout << index << endl;
    pop[MainPare] = child[index];
  }
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
vector<Indiv> MakeList(vector<Indiv> &pop)
{
  vector<vector<double> > DisSave(Ns); 
  vector<vector<int> >    IndexSave(Ns); //K番目以内の個体を記録しておく
  vector<double>::iterator min;
  size_t index;
  
	/*自身以外の個体との距離を総当たりで計測.自身も含む.*/
	/*ついでに近い順に個体の番号も取得*/
	for(int i=0;i<Ns;i++){
		for(int j=0;j<Ns;j++){
		  if(i != j)
	  		DisSave[i].push_back(cal_kotai_distance(pop[i],pop[j]));
	  	else
	  	  DisSave[i].push_back(10000);
		}
	  for(int j=0;j<DEM;j++){
      min = min_element(DisSave[i].begin(),DisSave[i].end());
      index = distance(DisSave[i].begin(),min);
      IndexSave[i].push_back(index);
      DisSave[i][index] = 10000;
    }
	}
	//AnsList1
	pop = AnsList1(IndexSave,pop);
	//AnsList2
	pop = AnsList2(IndexSave,pop);
  /*
	//ベクター確認用
  for(i=0;i<DisSave.size();i++){
    cout << i << ":";
    for(j=0;j<DisSave[i].size();j++){
      cout << DisSave[i][j] << " ";
    }
		cout << endl;
  }
	cout << endl;
	*/
	//ベクター確認用
	/*
  for(int i=0;i<IndexSave.size();i++){
    cout << i << ":";
    for(int j=0;j<IndexSave[i].size();j++){
      cout << IndexSave[i][j] << " ";
    }
		cout << endl;
  }
  cout << endl;
  */
  return pop;
}
/******************
List1
******************/
vector<Indiv> AnsList1(vector<vector<int> > &IndexSave,vector<Indiv> &pop)
{
	//近傍リスト2
	int Tmp;
	for(int i=0;i<Ns;i++){
	  if(IndexSave[i].empty()){} //空だったら何もしない
	  else {
	    for(int j=0;j<IndexSave[i].size();j++){
        pop[i].List1.push_back(IndexSave[i][j]);
      }
    }
	}
	return pop;
}

/******************
List2
******************/
vector<Indiv> AnsList2(vector<vector<int> > &IndexSave,vector<Indiv> &pop)
{
	//近傍リスト2
	int Tmp;
	for(int i=0;i<Ns;i++){
	  if(IndexSave[i].empty()){} //空だったら何もしない
	  else {
	    for(int j=0;j<IndexSave[i].size();j++){
        Tmp = IndexSave[i][j]; //一旦インデックスを保存
        if(count(IndexSave[Tmp].begin(),IndexSave[Tmp].end(),i) != 0)
          pop[i].List2.push_back(Tmp); //相手にも存在すれば近傍リストへ加える
      }
    }
	}
	return pop;
}
/******************
List3
******************/
/******************
  拡張XLM
******************/
void ExtensionXLM(int MainPare,vector<int> &SubPare,vector<Indiv> &child,vector<Indiv> &pop)
{
	double SubPare_Gra[DEM] = {0}; /*基準となる重心*/
	double sum_n[DEM] = {0}; /*x,yそれぞれの和を一時的に保存しておく配列*/
	
	//親の解のそれぞれのベクトルを足す
	for(int i=0;i<SubPare.size();i++){
		for(int j=0;j<DEM;j++){
			sum_n[j] += pop[ SubPare[i] ].n[j];
		}
	}
	//ベクトルの重心を求める
	for(int i=0;i<DEM;i++){
		SubPare_Gra[i] = sum_n[i]/SubPare.size();
	}
	//重心から親に対するベクトルを求める
	double vector[SubPare.size()][DEM];
	for(int i=0;i<SubPare.size();i++){
		for(int j=0;j<DEM;j++){
			vector[i][j] = pop[ SubPare[i] ].n[j] - SubPare_Gra[j];
		}
	}
	//子個体生成
	double sum_coe[DEM]={0},coe; //親のベクトル*乱数の総和
	double child_save[DEM];
	for(int k=0;k<Nc;k++){
		true_flag = 1;
		while(true_flag){
			for(int i=0;i<DEM;i++){
				sum_coe[i] = 0;
			}
			for(int i=0;i<SubPare.size();i++){
				coe = rand_normal(0,1/sqrt(SubPare.size()));
				for(int j=0;j<DEM;j++){
					sum_coe[j] += coe * vector[i][j];
				}
			}
			for(int i=0;i<DEM;i++){
				child_save[i] = pop[MainPare].n[i]+sum_coe[i];
				if(fabs(child_save[i]) > 100.00){
				  break;
				}else if(i==DEM-1){
      		for(int j=0;j<DEM;j++){
      			child[k].n.push_back(child_save[j]);
      		}
      		true_flag = 0;
        }
		  }
	  }
	}
}

vector<Indiv> FitnessChild(vector<Indiv> &child, vector<Indiv> &pop)
{
  int sum_win;
  for(int i=0;i<child.size();i++){
    sum_win = 0;
    for(int j=0;j<child[i].Result.size();j++){
      sum_win += child[i].Result[j];
    }
    child[i].eval = sum_win;
  }
  return child;
}

/*****************
実数乱数生成
*****************/
double GetRand_Real(double pGetMax)
{
	double get_save;
	const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
	get_save = getrand_int32Max/2;
 
  return ((double)genrand_int32()-get_save)/get_save*pGetMax;
}

/*****************
自然数乱数生成
*****************/
int GetRand_Int(int pGetMax)
{
  const double getrand_int32Max = 4294967295; // genrand_int32() の最大値。除算のためdouble
  return int(genrand_int32() / getrand_int32Max * pGetMax);
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

	battle_n++;
	one->eval = distance_one;
	another->eval = distance_another;
	if(one->eval < another->eval){
		return 0;
	}else if(one->eval > another->eval){
		return 1;
	}
}
/*****************
再帰的にニッチを割り当てていく
*****************/
int SetNitch(int nitch_number,int kotai,vector<Indiv> &pop)
{
	int tmp;
	if(pop[kotai].nitch == 0){
		pop[kotai].nitch = nitch_number;
		for(int i=0;i<pop[kotai].List2.size();i++){
			tmp = pop[kotai].List2[i];
			SetNitch(nitch_number,tmp,pop);
		}
		return 1;
	}
	return 0;
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

