#pragma once
/* #define NDEBUG */

//�W�c
#define KO_L		1500				//�W�c
#define KO			150					//�W�c
#define KOT			100					//�G
#define PARENT		10					//�e
#define CHILD		100					//�q
#define KU			3000				//���㐔
#define PER			100					//
#define BATTLE_PER	KO*(CHILD+1)*2*20	//
#define END_GA		KO*(CHILD+1)*2*300		//�����ヂ�f���̏I������
#define TRIAL		1
#define WIN_RATE	60
#define CYCLE		1

//�N���X�^�����O
#define K_List1	10	/*�j�b�`�̏W�c��*/
#define K_List2	2	/*�j�b�`�̏W�c��*/
#define K_List3	2	/*�j�b�`�̏W�c��*/
#define K_UPGMA		90

//Floreano
#define F_TRIAL	10	/*�j�b�`�̏W�c��*/
#define FLORET	5	/*�j�b�`�̏W�c��*/
#define F_KU	500	/*�j�b�`�̏W�c��*/

//Nim
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
#define ALFA		0

#define DELETE		50

#define ENEMY		10

#define CSV_DIR		"./csv"
