#pragma once
/* #define NDEBUG */

#define GAME_NUM	1 /* Num:1 Numbers:0 */

//Numbers
constexpr int DEM = 2;	//������

//Nim
constexpr int NIM = 3;			//�j���̎R�̐�
constexpr int POLL1 = 5;			//�R1
constexpr int POLL2 = 5;			//�R2
constexpr int POLL3 = 5;			//�R3
constexpr int EVALUATION_VEC = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);			//�R3
constexpr double WIN_FIRST = 0.8;	//���ŏ������ꍇ
constexpr double WIN_LAST = 1.2;	//���ŏ������ꍇ

constexpr int INPUT	 = 9;
constexpr int MIDDLE = 9;
constexpr int OUTPUT = 1;


//�W�c�ݒ�
#if GAME_NUM == 0
	constexpr auto STRA_DIR		= "./numbers";
	constexpr int W_SIZE		= DEM;				//������
	constexpr int KO			= W_SIZE * 5;		//�W�c
	constexpr int KO_L			= KO * 3;			//�W�c
	constexpr int PARENT		= W_SIZE + 1;		//�e
	constexpr int CHILD			= W_SIZE * 2;		//�q
	constexpr int BATTLE_PER	= KO*CHILD * 50;	//
	constexpr int END_GA		= BATTLE_PER * 20;	//�����ヂ�f���̏I������
	constexpr double INIT_VALUE = 10.0;

	constexpr int K_List1 = W_SIZE + 2;	/*�j�b�`�̏W�c��*/
	constexpr int K_List2 = 2;			/*�j�b�`�̏W�c��*/
#elif GAME_NUM == 1
	constexpr auto STRA_DIR		= "./nim";
	constexpr int W_SIZE		= (INPUT*MIDDLE) + (MIDDLE*OUTPUT);
	constexpr int KO			= W_SIZE * 7;		//�W�c
	constexpr int KO_L			= KO * 3;			//�W�c
	constexpr int PARENT		= W_SIZE + 1;		//�e
	constexpr int CHILD			= W_SIZE * 2;		//�q
	constexpr int BATTLE_PER	= KO*CHILD * 25;	//
	constexpr int END_GA		= BATTLE_PER * 20;	//�����ヂ�f���̏I������
	constexpr double INIT_VALUE = 1.0;

	constexpr int K_List1 = W_SIZE + 10;	/*�j�b�`�̏W�c��*/
	constexpr int K_List2 = 2;				/*�j�b�`�̏W�c��*/
#endif

constexpr auto CSV_DIR		= "./csv";
constexpr int K_UPGMA		= W_SIZE;
constexpr int TRIAL			= 5;
constexpr int DELETE		= 50;
constexpr int ENEMY			= 10;
constexpr double WIN_RATE	= 60;
constexpr double STEP_SIZE	= 12.0;
constexpr double ALFA		= 0.1;