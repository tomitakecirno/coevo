#pragma once
/* #define NDEBUG */
#define GAME_NUM	1 /* Num:1 Numbers:2 */

//Nim
#if GAME_NUM == 0
	#define DEM			2				//dimention
	#define KO			DEM*5			//�W�c
	#define KO_L		KO*2				//�W�c
	#define PARENT		DEM+1			//�e
	#define CHILD		DEM*2				//�q
	#define BATTLE_PER	KO*CHILD*50		//
	#define END_GA		BATTLE_PER*20	//�����ヂ�f���̏I������
	#define WIN_RATE	60
	#define STEP_SIZE	12.0
	#define W_SIZE		DEM

	#define K_List1		DEM+2	/*�j�b�`�̏W�c��*/
	#define K_List2		2	/*�j�b�`�̏W�c��*/
	#define K_UPGMA		DEM
#elif GAME_NUM == 1
	#define NIM			3	//�j���̎R�̐�
	#define POLL1		5	//�R1
	#define POLL2		5	//�R2
	#define POLL3		5	//�R3
	#define STRA_LEN	(POLL1+1)*(POLL2+1)*(POLL3+1)

	#define WIN_FIRST	0.8	//���ŏ������ꍇ
	#define WIN_LAST	1.2	//���ŏ������ꍇ
	#define LOSE		0	//�������ꍇ

	//network
	#define INPUT		9	//�R1
	#define MIDDLE		9	//�R2
	#define OUTPUT		1	//�R3
	#define W_SIZE		(INPUT*MIDDLE)+(MIDDLE*OUTPUT)
	
	//population
	#define KO_L		900				//�W�c
	#define KO			450					//�W�c
	#define PARENT		91					//�e
	#define CHILD		100					//�q
	#define BATTLE_PER	KO*CHILD*50			//
	#define END_GA		BATTLE_PER*20		//�����ヂ�f���̏I������

	#define K_List1		100	/*�j�b�`�̏W�c��*/
	#define K_List2		2	/*�j�b�`�̏W�c��*/
	#define K_UPGMA		W_SIZE
#endif

//Floreano
#if GAME_NUM == 1
	#define F_TRIAL	10	/*�j�b�`�̏W�c��*/
	#define FLORET	5	/*�j�b�`�̏W�c��*/
	#define F_KU	500	/*�j�b�`�̏W�c��*/
#endif

#define CSV_DIR		"./csv"
#define TRIAL		1

#define ALFA		0
#define DELETE		50
#define ENEMY		10
#define WIN_RATE	60
#define STEP_SIZE	12.0

