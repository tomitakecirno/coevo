#pragma once
/* #define NDEBUG */

//集団
#define KO		300	//集団
#define KOT		100		//敵
#define PARENT	40		//親
#define CHILD	200		//子
#define KU		5000	//世代数
#define PER		100		//
#define BATTLE_PER		10050000//
#define TRIAL		10


//クラスタリング
#define K_List1	5	/*ニッチの集団数*/
#define K_List2	2	/*ニッチの集団数*/
#define K_List3	2	/*ニッチの集団数*/

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
#define MUTATION	1/((POLL1+1)*(POLL2+1)*(POLL3+1))

#define WIN_FIRST	0.8	//先手で勝った場合
#define WIN_LAST	1.2	//後手で勝った場合
#define LOSE		0	//負けた場合

//network
#define INPUT		3	//山1
#define MIDDLE		2	//山2
#define OUTPUT		1	//山3
#define VEC_SIZE	(INPUT*MIDDLE)+(MIDDLE*OUTPUT)

#define DELETE		50

#define ENEMY		10

#define CSV_DIR		"./csv"
