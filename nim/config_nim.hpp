#pragma once
/* #define NDEBUG */

//集団
#define KO_L		1500				//集団
#define KO			150					//集団
#define KOT			100					//敵
#define PARENT		10					//親
#define CHILD		100					//子
#define KU			3000				//世代数
#define PER			100					//
#define BATTLE_PER	KO*(CHILD+1)*2*20	//
#define END_GA		KO*(CHILD+1)*2*300		//世代交代モデルの終了条件
#define TRIAL		1
#define WIN_RATE	60
#define CYCLE		1

//クラスタリング
#define K_List1	10	/*ニッチの集団数*/
#define K_List2	2	/*ニッチの集団数*/
#define K_List3	2	/*ニッチの集団数*/
#define K_UPGMA		90

//Floreano
#define F_TRIAL	10	/*ニッチの集団数*/
#define FLORET	5	/*ニッチの集団数*/
#define F_KU	500	/*ニッチの集団数*/

//Nim
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
#define ALFA		0

#define DELETE		50

#define ENEMY		10

#define CSV_DIR		"./csv"
