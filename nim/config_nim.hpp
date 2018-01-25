#pragma once
/* #define NDEBUG */
#define GAME_NUM	1 /* Num:1 Numbers:2 */

//Nim
#if GAME_NUM == 0
	#define DEM			2				//dimention
	#define KO			DEM*5			//集団
	#define KO_L		KO*2				//集団
	#define PARENT		DEM+1			//親
	#define CHILD		DEM*2				//子
	#define BATTLE_PER	KO*CHILD*50		//
	#define END_GA		BATTLE_PER*20	//世代交代モデルの終了条件
	#define WIN_RATE	60
	#define STEP_SIZE	12.0
	#define W_SIZE		DEM

	#define K_List1		DEM+2	/*ニッチの集団数*/
	#define K_List2		2	/*ニッチの集団数*/
	#define K_UPGMA		DEM
#elif GAME_NUM == 1
	#define NIM			3	//ニムの山の数
	#define POLL1		5	//山1
	#define POLL2		5	//山2
	#define POLL3		5	//山3
	#define STRA_LEN	(POLL1+1)*(POLL2+1)*(POLL3+1)

	#define WIN_FIRST	0.8	//先手で勝った場合
	#define WIN_LAST	1.2	//後手で勝った場合
	#define LOSE		0	//負けた場合

	//network
	#define INPUT		9	//山1
	#define MIDDLE		9	//山2
	#define OUTPUT		1	//山3
	#define W_SIZE		(INPUT*MIDDLE)+(MIDDLE*OUTPUT)
	
	//population
	#define KO_L		900				//集団
	#define KO			450					//集団
	#define PARENT		91					//親
	#define CHILD		100					//子
	#define BATTLE_PER	KO*CHILD*50			//
	#define END_GA		BATTLE_PER*20		//世代交代モデルの終了条件

	#define K_List1		100	/*ニッチの集団数*/
	#define K_List2		2	/*ニッチの集団数*/
	#define K_UPGMA		W_SIZE
#endif

//Floreano
#if GAME_NUM == 1
	#define F_TRIAL	10	/*ニッチの集団数*/
	#define FLORET	5	/*ニッチの集団数*/
	#define F_KU	500	/*ニッチの集団数*/
#endif

#define CSV_DIR		"./csv"
#define TRIAL		1

#define ALFA		0
#define DELETE		50
#define ENEMY		10
#define WIN_RATE	60
#define STEP_SIZE	12.0

