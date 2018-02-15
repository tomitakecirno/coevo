#pragma once
/* #define NDEBUG */

#define _USE_MATH_DEFINES
#define GAME_NUM 1 /* Num:1 Numbers:0 NumbersKai:2*/

//Numbers
constexpr int DEM = 10;	//次元数
constexpr double OPT_X = 100;	//次元数
constexpr double OPT_Y = 100;	//次元数

//Numbers_kai
constexpr int kaiDEM = 3;	//次元数
constexpr double kaiOPT_X = 70.0;
constexpr double kaiOPT_Y = 70.0;
constexpr double PI = 3.141592653589793;


//Nim
constexpr int NIM = 3;			//ニムの山の数
constexpr int POLL1 = 5;			//山1
constexpr int POLL2 = 5;			//山2
constexpr int POLL3 = 5;			//山3
constexpr int EVALUATION_VEC = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);			//山3
constexpr double WIN_FIRST = 0.8;	//先手で勝った場合
constexpr double WIN_LAST = 1.2;	//後手で勝った場合

constexpr int INPUT	 = 9;
constexpr int MIDDLE = 12;
constexpr int OUTPUT = 1;


//集団設定
#if GAME_NUM == 0
	constexpr auto STRA_DIR		= "./numbers";
	constexpr int W_SIZE		= DEM;				//次元数
	constexpr int KO			= W_SIZE * 7;		//集団
	constexpr int KO_L			= KO * 3;			//集団
	constexpr int PARENT		= W_SIZE + 1;		//親
	constexpr int CHILD			= W_SIZE * 2;		//子
	constexpr int BATTLE_PER	= KO*CHILD*20;	//
	constexpr int END_GA		= BATTLE_PER * 20;	//世代交代モデルの終了条件
	constexpr double INIT_VALUE = 10.0;
	constexpr int battle_n		= 1;

	constexpr int K_List1 = W_SIZE + 2;	/*ニッチの集団数*/
	constexpr int K_List2 = 2;			/*ニッチの集団数*/
#elif GAME_NUM == 1
	constexpr auto STRA_DIR		= "./nim";
	constexpr int W_SIZE		= (INPUT*MIDDLE) + (MIDDLE*OUTPUT);
	constexpr int KO			= W_SIZE * 7;		//集団
	constexpr int KO_L			= KO * 3;			//集団
	constexpr int PARENT		= W_SIZE + 1;		//親
	constexpr int CHILD			= W_SIZE * 2;		//子
	constexpr int BATTLE_PER	= KO*CHILD * 20;	//
	//constexpr int BATTLE_PER	= KO*CHILD;	//
	constexpr int END_GA		= BATTLE_PER * 50;	//世代交代モデルの終了条件
	constexpr double INIT_VALUE = 1.0;
	constexpr int battle_n		= 2;

	constexpr int K_List1 = W_SIZE + 10;	/*ニッチの集団数*/
	constexpr int K_List2 = 2;				/*ニッチの集団数*/
#elif GAME_NUM == 2
	constexpr auto STRA_DIR = "./numbers_kai";
	constexpr int W_SIZE = kaiDEM;
	constexpr int KO = W_SIZE * 7;				//集団
	constexpr int KO_L = KO * 3;				//集団
	constexpr int PARENT = W_SIZE + 1;			//親
	constexpr int CHILD = W_SIZE * 2;			//子
	constexpr int BATTLE_PER = KO*CHILD * 20;	//
	constexpr int END_GA = BATTLE_PER * 20;		//世代交代モデルの終了条件
	constexpr double INIT_VALUE = 10.0;
	constexpr int battle_n = 1;

constexpr int K_List1 = W_SIZE + 5;	/*ニッチの集団数*/
constexpr int K_List2 = 2;				/*ニッチの集団数*/
#endif

constexpr auto CSV_DIR		= "./csv";
constexpr int K_UPGMA		= W_SIZE;
constexpr int TRIAL			= 10;
constexpr int DELETE		= 50;
constexpr int ENEMY			= 10;
constexpr double WIN_RATE	= 60;
constexpr double STEP_SIZE	= 2.0;
constexpr double ALFA		= 0.1;