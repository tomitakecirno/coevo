#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>

#include <SDL2/SDL_gfxPrimitives.h>
#include"./header/MT.h"

#define INIT		100	/*解集団の初期化範囲*/
#define INIT_OPPOMEMT	100	/*敵集団の初期化範囲*/
#define LIMIT		100	/*定義域*/
#define Y		100	/*地域*/
#define Ns		50	/*初期集団数*/
#define No		30	/*敵集団数*/
#define Np		5	/*親個体数*/
#define Nc		50	/*子個体数*/
#define DEM		2	/*次元数*/
#define T		2	/*ステップサイズ*/
#define END_STEP	500	/*終わるタイミング*/
#define WINDOW_X	800	/*定義域*/
#define WINDOW_Y	800	/*地域*/
#define PROT_X		600	/*定義域*/
#define PROT_Y		600	/*地域*/
#define K		3	/*ニッチの集団数*/

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
Indiv Opponent[No];

typedef struct{
	double x;
	double y;
}Gravity;
Gravity Gra_Nitch[No];
int end_flag = 1;
int true_flag = 1;
int while_flag = 1;
int count_nitch=1;


void Init_Indiv(Indiv pare[],int N);
void Init_Opponent(void);
void Init_Opponent_BattleData(void);

void creat_child(int pare[],int child[]);
void select_child(int optimal,int child[],int surv[]);

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
double GetRand32(double pGetMax);
void Prot_Frame(SDL_Surface *window);
void Opponent_Prot(SDL_Surface *window);
void Pop_Prot(Indiv pop[],SDL_Surface *window);
void Unit_Prot(Indiv pop[],SDL_Surface *window,int N);
/*
void test(Indiv *child){
	child->win+=1;
	printf("child.win = %d\n",child->win);
}
*/
/***********
メインルーチンmain
***********/
main(){
	/*個体を格納する配列定義*/
	Indiv pop[Ns];		/*解集団配列*/
	Indiv pare[Np];		/*親集団配列*/
	Indiv child[Nc];	/*子集団配列*/
	Indiv surv[Np]; 	/*残される子を入れておく*/
	Indiv pare_child[Np+Nc];/*子個体と親個体を合わせる*/

	int i,j,k; /*for文用*/
	char name[10];
	/*
	for(i=0;i<10;i++){
		printf("前child.win = %d\n",child[0].win);
		test(&child[0]);
		printf("後ろchild.win = %d\n\n",child[0].win);
	}
	return 0;
	*/
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
	SDL_Thread *thr_keyboad,*thr_window;
	thr_keyboad = SDL_CreateThread(thread_keyboad,NULL);
	thr_window = SDL_CreateThread(thread_window,NULL);

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
		for(i=0;i<(Np+Nc);i++){
			printf("pare_child[%d].win = %d\n",i,pare_child[i].win);
		}
		*/
		/*
		全勝個体を残す　→　相手に変化がほとんど見られない
		解集団へ残す個体を全て残す　→　逆に変化しすぎ
		勝率8割以上の個体を残す　→　最適解付近の個体以外変化無し。もうちょい進化圧がほしい
		*/
		for(i=0;i<Np;i++){
			for(j=0;pop[j].flag != 1;j++){}
			pop[j] = pare_child[i];
			pop[j].flag = 0;
			Update_Opponent(pare_child[i]);
		}
		/*子個体の最良Np個を初期集団へ（世代交代）*/
		/*pop集団をプロット*/
		Pop_Prot(pop,window);
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

		SDL_Delay(500);
		/*画像出力*/
		/*
		if(end_count%100 == 0){
			sprintf(name,"./picture/pop/pop1%d.bmp",end_count);
			SDL_SaveBMP(window,name);
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
/****************

画面出力のスレッド
****************/
static int thread_window(void *data)
{
	SDL_Event event;
	SDL_Surface *window_sub; // ウィンドウ（画像）データ、及び、文字列（画像）へのポインタ
	
	if((window_sub = SDL_SetVideoMode(WINDOW_X, WINDOW_Y, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}
	
	while(end_flag){
		SDL_FillRect(window_sub, NULL, 0x00ffffff);
		SDL_Flip(window_sub); /*ウィンドウに反映*/
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
	
	/*敵集団初期化*/
	
	for(i=0;i<No;i++){
		for(j=0;j<DEM;j++){
			Opponent[i].n[j] = GetRand32(INIT_OPPOMEMT); /*実数値乱数*/
		}
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
		Opponent[i].lose = 0;
		Opponent[i].draw = 0;
		Opponent[i].eval = 0;
		Opponent[i].nitch = 0;
		for(j=0;j<K;j++){
			Opponent[i].Neigh_List2[j] = -1;
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
		Gra_Nitch[i].x = sum_x/(double)count_sum;
		Gra_Nitch[i].y = sum_y/(double)count_sum;
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
	sort_win(sum_pare,Np*2);

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
	int k;
	int base = 0; /*ナンバーズを行うパラメータを決定*/
	double base_fabs;
	/*絶対値が一番小さいパラメータを求める*/
	base_fabs = fabs(one->n[0] - another->n[0]);
	base = 0;
	for(k=1;k<DEM;k++){
		if(fabs(one->n[k] - another->n[k]) < base_fabs){
			base_fabs = fabs(one->n[k] - another->n[k]);
			base = k;
		}
	}
	if(one->n[base] > another->n[base]){
		one->win++;
	}else if(one->n[base] < another->n[base]){
		another->win++;
	}
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
	min_dis = cal_distance(Gra_Nitch[1].x,Gra_Nitch[1].y,
			       child.n[0],child.n[1]);
	for(i=1;i<count_nitch;i++){
		if(cal_distance(Gra_Nitch[i].x,Gra_Nitch[i].y,child.n[0],child.n[1]) < min_dis){
		   	min_dis = cal_distance(Gra_Nitch[i].x,Gra_Nitch[i].y,child.n[0],child.n[1]);
		   	child.nitch = i;
		}
	}
	/*戦闘データ初期化*/
	for(i=0;i<No;i++){
		if(Opponent[i].nitch == child.nitch){
			Opponent[i].win = 0;
			Opponent[i].lose = 0;
			Opponent[i].draw = 0;
			Opponent[i].eval = 0;
			Opponent[i].flag = 0;
		}
	}
	child.win = 0;
	child.lose = 0;
	child.draw = 0;
	child.eval = 0;
	child.flag = 0;
	/*該当するニッチの個体と子個体でリーグ戦*/
	for(i=0;i<No;i++){
		if(Opponent[i].nitch == child.nitch && Opponent[i].flag == 0){
			for(j=i+1;j<No;j++){
				if(Opponent[i].nitch == child.nitch){
					Numbers(&Opponent[i],&Opponent[j]);
				}
			}
			Numbers(&Opponent[i],&child);
			Opponent[i].flag = 1; /*フラグ立て*/
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
	/*相手集団の中で子個体より弱い相手のところに入れる*/
	if(Opponent[min_indiv].win < child.win){
		Opponent[min_indiv] = child;
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
	/*ニッチの重心を描画*/
	for(i=1;i<count_nitch;i++){
		filledCircleColor(window,
		(Gra_Nitch[i].x*2)+center[0],
		(Gra_Nitch[i].y*2)+center[1],
		5,
		0x000000ff);
	}
	/*個体を描画*/
	for(i=0;i<No;i++){
		sprintf(tmp_n,"%d",Opponent[i].nitch);
		stringColor(window, (Opponent[i].n[0]*2)+center[0], (Opponent[i].n[1]*2)+center[1], tmp_n, 0x000000ff);
		tmp_n[0] = '\0';
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
			filledCircleColor(window,
				(pop[i].n[0]*2)+center[0],
				(pop[i].n[1]*2)+center[1],
				3,
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
		filledCircleColor(window,
			(pop[i].n[0]*2)+center[0],
			(pop[i].n[1]*2)+center[1],
			3,
			0xff0000ff);
	}
}
