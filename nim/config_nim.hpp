#pragma once
/* #define NDEBUG */

#define KO		300	//集団
#define KOT		100		//敵
#define PARENT	40		//親
#define CHILD	200		//子
#define KU		5000	//世代数
#define PER		100		//
#define BATTLE_PER		10050000//

#define K_List1	5	/*ニッチの集団数*/
#define K_List2	2	/*ニッチの集団数*/
#define K_List3	2	/*ニッチの集団数*/
#define KL1		50	/*ニッチの集団数*/

#define F_TRIAL	10	/*ニッチの集団数*/
#define FLORET	5	/*ニッチの集団数*/
#define F_KU	500	/*ニッチの集団数*/

#define NIM			3	//ニムの山の数
#define POLL1		5	//山1
#define POLL2		5	//山2
#define POLL3		5	//山3

#define WIN_FIRST	0.8	//先手で勝った場合
#define WIN_LAST	1.2	//後手で勝った場合
#define LOSE		0	//負けた場合

#define DELETE		50
#define CSV_DIR		"./csv"

#define MUTATION	1/((POLL1+1)*(POLL2+1)*(POLL3+1))

#define STRA_LEN	(POLL1+1)*(POLL2+1)*(POLL3+1)

#define TRIAL		10

#define ENEMY		10
