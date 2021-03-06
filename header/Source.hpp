#include "DxLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>    
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

//#include "stdafx.h"
#pragma warning( disable:4996 )
//#pragma warning( disable:8000 )
#define _AFX_SECURE_NO_WARNINGS
#define PA 20
#define KA 50
#define NA 40  
#define DPA 40

#define PAHIT 40
#define KAHIT 50
#define NAHIT 40  
#define DPAHIT 20

#define RIGHT 2
#define LEFT 0


#define I1		15	//入力層
#define J1		30	//中間層
#define I2		30	//        
#define J2		13	//出力層
#define KO		250	//個体数
#define KOT		100	//対戦相手数
#define KU		2000	//世代数
#define PER		200	//世代数
#define K		2	//クラスタ数
#define PARENT	5	//親の数
#define CHILD	100	//子供の数
#define LLL		1

#define RIVAL 1		//対戦相手の対戦回数
#define COUNT 0		//
#define COUNT_T 0	//

#define AICOUNT 14
//1:プレイ 2:学習 3:対戦結果
#define DE 2

///////
int counter = 0, FpsTime[2] = { 0, }, FpsTime_i = 0;
double Fps = 0.0;
char Key[256];
struct Player {        
	int x = 0;
	int y = 0;
	int attackstate = 0;
	int movestate = 0;
	int hitstate = 0;
	int direction = 0;
	int movecount = 0;
	int attackcount = 0;
	int hitcount = 0;
	int imagecount = 0;
	int delaycount = 0;
	int hitlimit = 0;
	int hp = 300;
	int win = 0;
	int combo = 1;
	int fallspeed = 0;
};

int Round = 1;
int Start = 0;
int Start2 = 0;
int countS = 100;
int wincount1 =0;
int losecount1 = 0;
int wincount2 = 0;
int losecount2 = 0;
int drawcount = 0;
int wincount[K] = { 0 };
int losecount[K] = { 0 };
int drawcount2[K] = { 0 };
int time2 = 3600;
int guard1 = 0;
int guard2 = 0;
int n_mo;
int flag_sl[K] = { 0 };
int flag_ss = 0;
int flag_s = 0;


double w1[I1][J1];//1P
double w2[I2][J2];
double w3[I2][J1];

double w1_T[I1][J1];//2P
double w2_T[I2][J2];
double w3_T[I2][J1];

double w1_GA[KO][I1][J1];//集団
double w2_GA[KO][I2][J2];
double w3_GA[KO][I2][J1];

double w1_GA_c[KO][I1][J1];
double w2_GA_c[KO][I2][J2];
double w3_GA_c[KO][I2][J1];

double w1_GA_C[KO][I1][J1];//ふろれ
double w2_GA_C[KO][I2][J2];

double w1_GA_T[KOT][I1][J1];//対戦相手集団
double w2_GA_T[KOT][I2][J2];
double w3_GA_T[KOT][I2][J1];

double w1_k[K][I1][J1];//クラスタの中心
double w2_k[K][I2][J2];
double w3_k[K][I2][J1];

double w1_g[I1][J1] = { 0 };//重心
double w2_g[I2][J2] = { 0 };
double w3_g[I2][J1] = { 0 };

double w1_g_T[K][I1][J1] = { 0 };//重心(対戦相手)
double w2_g_T[K][I2][J2] = { 0 };
double w3_g_T[K][I2][J1] = { 0 };

double w1_c[CHILD + PARENT][I1][J1] = { 0 };//子個体
double w2_c[CHILD + PARENT][I2][J2] = { 0 };
double w3_c[CHILD + PARENT][I2][J1] = { 0 };

double w1_c_T[K][CHILD + PARENT][I1][J1] = { 0 };//子個体(対戦相手)
double w2_c_T[K][CHILD + PARENT][I2][J2] = { 0 };
double w3_c_T[K][CHILD + PARENT][I2][J1] = { 0 };

double w1_G[300][I1][J1];//集団の最良個体
double w2_G[300][I2][J2];
double w3_G[300][I2][J1];

double u2_c[J1] = { 0 };
double u2_c_T[J1] = { 0 };

int w_cluster[KOT];
double w_distance[KOT] = {10000};
double rd[500] = { 0 };
int rdc[500];
int sv[10000] = { 0 };

FILE *file;
FILE *fp;
char filename[256];
int f_name = 0;
struct Player player1;
struct Player player2;
struct Player player1s;
struct Player player2s;
struct Player playerR;

int img[65] = { 0 };

//
double value[KO] = { 0 };
double value_c[KO] = { 0 };
double value_T[KOT] = { 0 };
double value_c_T[K][KOT] = { 0 };
double win_count[KO] = { 0 };
double win_count_c[KO] = { 0 };
double win_count_T[KOT] = { 0 };
double win_count_c_T[K][KOT] = { 0 };
int parent[KO] = { 0 };//親番号
int parent_T[K][KO] = { 0 };//親番号(対戦相手)
int judge = 0;
int v_count = 0;
int cluster_in[K][KOT + 1] = { 0 };
int cluster_in_number[K] = { 0 };
int good_rival[K] = { 0 };
int good_rival_main[RIVAL];
int c_number[CHILD+PARENT];
int c_number_T[K][CHILD + PARENT];
int c_good_number[KO];
int value_w[KO] = { 0 };
int value_w_T[K][KOT] = { 0 };
int deaa = 0;
//

void direction(void);
void move(struct Player *playerO, struct Player *playerD);
void hit(struct Player *player);
void attack(struct Player *playerO, struct Player *playerD);
void image(struct Player *player);
void statefix(struct Player *player);
int hitrange(struct Player *playerO, struct Player *playerD, int x_range);
int bound(struct Player *playerO, struct Player *playerD);
void win(int p1hp,int p2hp);
void HPandGAUGEimage(void);
void Time(void);
void TimeGA(void);
void start(void);
void startGA(void);
void FpsTimeFanction(void);
void round(void);
void roundGA(void);
void NN1P(void);
void NN2P(void);
void NNR1P(void);
void NNR2P(void);
void Floreano(void);
void Floreano2(void);
void gamemode(void);
void gmode(void);
void n_learn(void);
void Competition(void);
double rand_normal(double mu, double sigma);
double Uniform(void);
void QSort(double x[], int y[], int left, int right);
void Swap(double x[], int y[], int i, int j);
void ALL(void);
void playeraa(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DE == 1) {
		gamemode();
	}
	else if(DE == 0) {
		gmode();
	}
	else if(DE == 2){
		n_learn();
	}
	else {
		ALL();
	}
	WaitKey();				// キー入力待ち
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}
void gamemode(void) {
	if ((file = fopen("AI/test.dat", "rb")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}
	if ((fp = fopen("AI/test_T.dat", "rb")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}
	srand((unsigned int)time(NULL));
	char Buf[256];

	player1.x = 200;
	player1.y = 400;
	player2.x = 800;
	player2.y = 400;
	int c = 0;
	int i = 0;
	int j = 0;


	fread(w1, sizeof(double), I1 * J1, file);
	fread(w2, sizeof(double), I2 * J2, file);
	fread(w3, sizeof(double), I2 * J1, file);
	fread(w1_T, sizeof(double), I1 * J1, fp);
	fread(w2_T, sizeof(double), I2 * J2, fp);
	fread(w3_T, sizeof(double), I2 * J1, fp);

	ChangeWindowMode(TRUE);
	SetGraphMode(1200, 600, 32);
	SetBackgroundColor(255, 255, 255);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		exit(0);	// エラーが起きたら直ちに終了
	}
	//画像読み込み
	img[0] = LoadGraph("gp/1.png");
	img[1] = LoadGraph("gp/2.png");
	img[2] = LoadGraph("gp/3.png");
	img[3] = LoadGraph("gp/4.png");
	img[4] = LoadGraph("gp/5.png");
	img[5] = LoadGraph("gp/6.png");
	img[6] = LoadGraph("gp/7.png");
	img[7] = LoadGraph("gp/8.png");
	img[8] = LoadGraph("gp/9.png");
	img[9] = LoadGraph("gp/10.png");
	img[10] = LoadGraph("gp/11.png");
	img[11] = LoadGraph("gp/12.png");
	img[12] = LoadGraph("gp/13.png");
	img[13] = LoadGraph("gp/14.png");
	img[14] = LoadGraph("gp/15.png");
	img[15] = LoadGraph("gp/16.png");
	img[16] = LoadGraph("gp/17.png");
	img[17] = LoadGraph("gp/18.png");
	img[18] = LoadGraph("gp/19.png");
	img[19] = LoadGraph("gp/20.png");
	img[20] = LoadGraph("gp/21.png");
	img[21] = LoadGraph("gp/22.png");
	img[22] = LoadGraph("gp/23.png");
	img[23] = LoadGraph("gp/24.png");
	img[24] = LoadGraph("gp/25.png");
	img[25] = LoadGraph("gp/26.png");
	img[26] = LoadGraph("gp/27.png");
	img[27] = LoadGraph("gp/28.png");
	img[28] = LoadGraph("gp/29.png");
	img[29] = LoadGraph("gp/30.png");
	img[30] = LoadGraph("gp/31.png");
	img[31] = LoadGraph("gp/32.png");
	img[32] = LoadGraph("gp/33.png");
	img[33] = LoadGraph("gp/34.png");
	img[34] = LoadGraph("gp/35.png");
	img[35] = LoadGraph("gp/36.png");
	img[36] = LoadGraph("gp/37.png");
	img[37] = LoadGraph("gp/38.png");
	img[38] = LoadGraph("gp/39.png");
	img[39] = LoadGraph("gp/40.png");
	img[40] = LoadGraph("gp/41.png");
	img[41] = LoadGraph("gp/42.png");
	img[42] = LoadGraph("gp/43.png");
	img[43] = LoadGraph("gp/44.png");
	img[44] = LoadGraph("gp/45.png");
	img[45] = LoadGraph("gp/46.png");
	img[46] = LoadGraph("gp/47.png");
	img[47] = LoadGraph("gp/48.png");

	img[48] = LoadGraph("gp/49.png");
	img[49] = LoadGraph("gp/50.png");
	img[50] = LoadGraph("gp/51.png");
	img[51] = LoadGraph("gp/52.png");
	img[52] = LoadGraph("gp/Round1.png");
	img[53] = LoadGraph("gp/Round2.png");
	img[54] = LoadGraph("gp/Round3.png");
	img[55] = LoadGraph("gp/count1.png");
	img[56] = LoadGraph("gp/count2.png");
	img[57] = LoadGraph("gp/count3.png");
	img[58] = LoadGraph("gp/Start.png");
	img[59] = LoadGraph("gp/1pwin.png");
	img[60] = LoadGraph("gp/2pwin.png");
	img[61] = LoadGraph("gp/53.png");
	img[62] = LoadGraph("gp/54.png");

	int sf = 0,sff = 0;

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		if (sf == 0) {
			ClearDrawScreen();//裏画面消す
			SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に
			unsigned int Cr;
			Cr = GetColor(0, 0, 255);        // 青色の値を取得
			DrawFormatString(500, 200, Cr, "AI VS AI : Y_key");
			DrawFormatString(500, 300, Cr, "PLAYER VS AI : H_key");
			DrawFormatString(500, 400, Cr, "PLAYER VS PLYAER : N_key");
			ScreenFlip();//裏画面を表画面にコピー
			GetHitKeyStateAll(Buf);
			WaitKey();
			if (Buf[KEY_INPUT_Y] == 1) {
				sff = 1;
				sf = 1;
			}
			if (Buf[KEY_INPUT_H] == 1) {
				sff = 2;
				sf = 1;
			}
			if (Buf[KEY_INPUT_N] == 1) {
				sff = 3;
				sf = 1;
			}
		}
		else {
			ClearDrawScreen();//裏画面消す
			SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に
			GetHitKeyStateAll(Buf);
			if (Start2 == 0) {
				HPandGAUGEimage();
				image(&player1);
				image(&player2);
				start();
				Time();
				move(&player1, &player2);
				move(&player2, &player1);
				deaa = 0;
				attack(&player1, &player2);
				attack(&player2, &player1);
				hit(&player1);
				hit(&player2);
				direction();
				statefix(&player1);
				statefix(&player2);
				win(player1.hp, player2.hp);
				//HPandGAUGEimage();
			}
			else {
				round();
			}
			//FpsTimeFanction();
			ScreenFlip();//裏画面を表画面にコピー
			if (Start == 1 && Start2 == 0) {
				player1s = player1;
				player2s = player2;
				if (sff == 1) {
					NNR1P();
					NNR2P();
				}
				else if (sff == 2) {
					NNR2P();
				}

				//player1----------------------------------------------------------
				if (Start == 1 && Start2 == 0) {
					if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3) {
						if (player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
							if (player1.movestate != 3) {
								if (player1.attackstate != 3) {
									if (Buf[KEY_INPUT_D] == 1) {
										if (player1.direction == RIGHT) {
											player1.movestate = 1;
										}
										else if (player1.direction == LEFT) {
											player1.movestate = 2;
										}
									}
									if (Buf[KEY_INPUT_A] == 1) {
										if (player1.direction == RIGHT) {
											player1.movestate = 2;
										}
										else if (player1.direction == LEFT) {
											player1.movestate = 1;
										}
									}
								}
								if (Buf[KEY_INPUT_S] == 1) {
									player1.movestate = 3;
								}
								if (Buf[KEY_INPUT_R] == 1) {
									player1.attackstate = 1;
								}
								if (Buf[KEY_INPUT_F] == 1) {
									player1.attackstate = 2;
								}
								if (Buf[KEY_INPUT_V] == 1) {
									player1.attackstate = 5;
								}
								if (Buf[KEY_INPUT_T] == 1) {
									player1.attackstate = 4;
								}
								if (Buf[KEY_INPUT_G] == 1) {
									player1.attackstate = 3;
								}
							}
						}
					}
				}
				//--------------------------------------------------------------------

				//player2-------------------------------------------------------------

				if (Start == 1 && Start2 == 0) {
					if (player2.movestate == 0 &&
						player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 7 && player2.hitstate != 8 &&
						player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 3) {
						if (Buf[KEY_INPUT_J] == 1)
						{
							player2.movestate = 1;
						}
						if (Buf[KEY_INPUT_L] == 1)
						{
							player2.movestate = 2;
						}
						if (Buf[KEY_INPUT_I] == 1)
						{
							if (Buf[KEY_INPUT_L] == 1)
							{
								player2.movestate = 5;
							}
							else if (Buf[KEY_INPUT_J] == 1)
							{
								player2.movestate = 6;
							}
							else {
								player2.movestate = 3;
							}
						}
						if (Buf[KEY_INPUT_K] == 1)
						{
							player2.movestate = 4;
						}
					}
					if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 7 && player2.hitstate != 8 &&
						player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 3) {
						if (Buf[KEY_INPUT_O] == 1)
						{
							player2.attackstate = 1;
						}
						if (Buf[KEY_INPUT_H] == 1)
						{
							player2.attackstate = 2;
						}
						if (player2.movestate != 3 && player2.movestate != 5 && player2.movestate != 6) {
							if (Buf[KEY_INPUT_P] == 1)
							{
								player2.attackstate = 3;
							}
							if (player2.movestate != 4) {
								if (Buf[KEY_INPUT_U] == 1)
								{
									player2.attackstate = 4;
								}
							}
						}
					}
				}

				//------------------------------------------------------------------

			}
		}
	}
}
void gmode(void) {

	if ((file = fopen("gp/test.txt", "w")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}
	fp = fopen("gp/test2.txt", "w");
	srand((unsigned int)time(NULL));
	char Buf[256];
	player1.x = 200;
	player1.y = 400;
	player2.x = 800;
	player2.y = 400;
	int c = 0;
	int i = 0;
	int j = 0;


	for (c = 0; c < KO; c++) {
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_GA[c][i][j] = rand() % 10-10;
			}
		}
	}
	for (c = 0; c < KO; c++) {
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_GA[c][i][j] = rand() % 10-10;
			}
		}
	}
	for (c = 0; c < KOT; c++) {
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_GA_T[c][i][j] = w1_GA[c][i][j];
			}
		}
	}
	for (c = 0; c < KOT; c++) {
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_GA_T[c][i][j] = w2_GA[c][i][j];
			}
		}
	}

	ChangeWindowMode(TRUE);
	SetGraphMode(1200, 600, 32);
	SetBackgroundColor(255, 255, 255);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		exit(0);			// エラーが起きたら直ちに終了
	}
	while (1) {
		if (Start2 == 0) {
			startGA();
			TimeGA();
			direction();
			move(&player1,&player2);
			move(&player2,&player1);
			hit(&player1);
			hit(&player2);
			attack(&player1, &player2);
			attack(&player2, &player1);
			statefix(&player1);
			statefix(&player2);
			win(player1.hp, player2.hp);
			Floreano();
		}
		else {
			Floreano();
		}
		if (Start == 1 && Start2 == 0) {
			player1s = player1;
			player2s = player2;
			NN1P();
			NN2P();
		}
	}
}
void direction(void) {
	if (player1.x > player2.x) {
		if (player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4) {
			player1.direction = LEFT;
		}
		if (player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4) {
			player2.direction = RIGHT;
		}
	}
	else if (player1.x < player2.x) {
		if (player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4) {
			player1.direction = RIGHT;
		}
		if (player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4) {
			player2.direction = LEFT;
		}
	}

}
void move(struct Player *playerO, struct Player *playerD) {
	switch (playerO->movestate)
	{
	case 0:
		break;
	case 1://左移動//前方移動
		if (playerO->direction == RIGHT) {
			if (bound(playerO, playerD) == 0 || playerO->x > playerD->x) {

				playerO->x += 5;
				if (bound(playerO, playerD) == 1) {
					if (playerD->movestate == 1 || playerD->movestate == 2) {

						deaa = 1;
					}
				}
				else {
					deaa = 0;
				}
			}
			else {
				if (playerD->x < 1000 && playerO->x < playerD->x) {
					if (deaa == 1) {
						playerO->x += 5;
						deaa = 0;
					}
					else {
						playerO->x += 1;
						playerD->x += 1;
					}
				}
			}
		}
		else {
			if (bound(playerO, playerD) == 0 || playerO->x < playerD->x) {
				playerO->x -= 5;
				if (bound(playerO, playerD) == 1) {
					if (playerD->movestate == 1 || playerD->movestate == 2) {
						deaa = 1;
					}
				}
				else {
					deaa = 0;
				}
			}
			else {
				if (playerD->x > 0 && playerO->x > playerD->x) {
					if (deaa == 1) {
						playerO->x -= 5;
						deaa = 0;
					}
					else {
						playerO->x -= 1;
						playerD->x -= 1;
					}
				}
			}
		}
		break;
	case 2://右移動//後方移動
		if (playerO->direction == RIGHT) {
			if (bound(playerO, playerD) == 0 || playerO->x < playerD->x) {
				playerO->x -= 5;
				if (bound(playerO, playerD) == 1) {
					if (playerD->movestate == 1 || playerD->movestate == 2) {
						deaa = 1;
					}
				}
				else {
					deaa = 0;
				}
			}
			else {
				if (playerD->x > 0 && playerO->x > playerD->x) {
					if (deaa == 1) {
						playerO->x -= 5;
						deaa = 0;
					}
					else {
						playerO->x -= 1;
						playerD->x -= 1;
					}
				}
			}
		}
		else {
			if (bound(playerO, playerD) == 0 || playerO->x > playerD->x) {
				playerO->x += 5;
				if (bound(playerO, playerD) == 1) {
					if (playerD->movestate == 1 || playerD->movestate == 2) {
						deaa = 1;
					}
				}
				else {
					deaa = 0;
				}
			}
			else {
				if (playerD->x < 1000 && playerO->x < playerD->x) {
					if (deaa == 1) {
						playerO->x += 5;
						deaa = 0;
					}
					else {
						playerO->x += 1;
						playerD->x += 1;
					}
				}
			}
		}
		break;
	case 3://しゃがみ
		break;
	default:
		break;
	}
}
void hit(struct Player *player) {
	switch (player->hitstate)
	{
	case 0:
		break;
	case 1:
		if (player->hitcount <= 0) {
			player->hitcount = PAHIT / (player->combo);
			if (player->direction == LEFT) {
				player->x += 25;
			}
			else {
				player->x -= 25;
			}
		}
		player->hitcount--;
		if (player->hitcount <= 0) {
			player->combo = 1;
			player->hitstate = 0;
		}
		break;
	case 2:
		if (player->hitcount <= 0) {
			player->hitcount = KAHIT / (player->combo);
			if (player->direction == LEFT) {
				player->x += 100;
			}
			else {
				player->x -= 100;
			}
		}
		player->hitcount--;
		if (player->hitcount <= 0) {
			player->combo = 1;
			player->hitstate = 0;
		}
		break;
	case 3:
		if (player->hitcount <= 0) {
			player->fallspeed = 0;
			n_mo = player->direction;
			player->hitcount = 90;
		}
		else if (player->hitcount > 85) {
			if (n_mo == LEFT) {
				player->x -= 30;
			}
			else {
				player->x += 30;
			}
			player->y -= 30;
		}
		else {
			if (n_mo == LEFT) {
				player->x -= 5;
			}
			else {
				player->x += 5;
			}
			player->y += 1 + player->fallspeed;
			player->fallspeed++;
		}
		player->hitcount--;
		if (player->y >= 450) {
			player->y = 400;
			player->combo = 1;
			player->hitstate = 0;
			player->hitcount = 0;
			player->fallspeed = 0;
		}
		break;
	default:
		break;
	}
}
void attack(struct Player *playerO, struct Player *playerD)       {
	switch (playerO->attackstate)
	{
	case 0:
		break;
	case 1:
		if (playerO->delaycount <= 0) {
			if (playerO->movestate == 3) {
				playerO->delaycount = PA;
			}
			else {
				playerO->delaycount = PA;
			}
		}
		if (playerO->delaycount >= PA - 4 && playerO->hitlimit == 0) {
			if (playerD->hitstate != 3) {
				if (hitrange(playerO, playerD, 50) == 1) {
					if (playerD->attackstate == 1 || playerD->attackstate == 2 || playerD->attackstate == 4 || playerD->attackstate == 5) {//相打ち
						if (playerD->hitlimit == 0) {
							playerO->hitlimit = 1;
							playerD->hitlimit = 1;
							if (playerO->direction == RIGHT) {
								playerO->x -= 200;
								playerD->x += 200;
							}
							else {
								playerO->x += 200;
								playerD->x -= 200;
							}
						}
					}
					if (playerO->hitlimit == 0) {
						if (playerD->attackstate == 3) {//ガード状態
							if (playerO->movestate == 0 && playerD->movestate == 3) {//立ち攻撃/しゃがみガード
								playerD->hitstate = 1;
								if (playerD->hitcount > 0) {
									playerD->combo++;
								}
								playerD->hitcount = 0;
								playerD->hp -= 10;
								playerD->attackstate = 0;
								playerD->delaycount = 0;
								playerD->hitlimit = 0;
							}
							else if (playerO->movestate == 3 && playerD->movestate == 0) {//しゃがみ攻撃/立ちガード
								playerD->hitstate = 1;
								if (playerD->hitcount > 0) {
									playerD->combo++;
								}
								playerD->hitcount = 0;
								playerD->hp -= 10;
								playerD->attackstate = 0;
								playerD->delaycount = 0;
								playerD->hitlimit = 0;
							}
							else {
								playerD->hp -= 1;
								if (playerO->delaycount == RIGHT) { playerD->x += 2; }
								else { playerD->x -= 2; }
								guard1 = 1;
							}
						}
						else {//ノーガード
							playerD->hitstate = 1;
							if (playerD->hitcount > 0) {
								playerD->combo++;
							}
							playerD->hitcount = 0;
							playerD->hp -= 10;
							playerD->attackstate = 0;
							playerD->delaycount = 0;
							playerD->hitlimit = 0;
						}
						playerO->hitlimit = 1;
					}
				}
			}
		}
		if (playerO->delaycount <  PA-4  ) {
			playerO->hitlimit = 1;
		}
		playerO->delaycount--;
		if (playerO->delaycount <= 0) {
			playerO->attackstate = 0;
			playerO->delaycount = 0;
			playerO->hitlimit = 0;
		}
		break;
	case 2:
		if (playerO->delaycount <= 0) {
			if (playerO->movestate == 3) {
				playerO->delaycount = KA;
			}
			else {
				playerO->delaycount = KA;
			}
		}
		if (playerO->delaycount >= KA - 4 && playerO->hitlimit == 0) {
			if (playerD->hitstate != 3) {
				if (hitrange(playerO, playerD, 70) == 1) {
					if (playerD->attackstate == 1 || playerD->attackstate == 2 || playerD->attackstate == 4 || playerD->attackstate == 4) {//相打ち
						if (playerD->hitlimit == 0) {
							playerO->hitlimit = 1;
							playerD->hitlimit = 1;
							if (playerO->direction == RIGHT) {
								playerO->x -= 200;
								playerD->x += 200;
							}
							else {
								playerO->x += 200;
								playerD->x -= 200;
							}
						}
					}
					if (playerO->hitlimit == 0) {
						if (playerD->attackstate == 3) {//ガード状態
							if (playerO->movestate == 0 && playerD->movestate == 3) {//立ち攻撃/しゃがみガード
								playerD->hitstate = 2;
								if (playerD->hitcount > 0) {
									playerD->combo++;
								}
								playerD->hitcount = 0;
								playerD->hp -= 20;
								playerD->attackstate = 0;
								playerD->delaycount = 0;
								playerD->hitlimit = 0;
							}
							else if (playerO->movestate == 3 && playerD->movestate == 0) {//しゃがみ攻撃/立ちガード
								playerD->hitstate = 2;
								if (playerD->hitcount > 0) {
									playerD->combo++;
								}
								playerD->hitcount = 0;
								playerD->hp -= 20;
								playerD->attackstate = 0;
								playerD->delaycount = 0;
								playerD->hitlimit = 0;
							}
							else {
								playerD->hp -= 1;
								if (playerO->delaycount == LEFT) { playerD->x += 4; }
								else { playerD->x -= 4; }
							}
						}
						else {//ノーガード
							playerD->hitstate = 2;
							if (playerD->hitcount > 0) {
								playerD->combo++;
							}
							playerD->hitcount = 0;
							playerD->hp -= 20;
							playerD->attackstate = 0;
							playerD->delaycount = 0;
							playerD->hitlimit = 0;
						}
						playerO->hitlimit = 1;
					}
				}
			}
		}
		if (playerO->delaycount <  KA-4) {
			playerO->hitlimit = 1;
		}
		playerO->delaycount--;
		if (playerO->delaycount <= 0) {
			playerO->attackstate = 0;
			playerO->delaycount = 0;
			playerO->hitlimit = 0;
		}
		break;
	case 3://ガード
		break;
	case 4:
		if (playerO->delaycount <= 0) {
			playerO->delaycount = NA;
		}
		if (playerO->delaycount >= NA - 4 && playerO->hitlimit == 0) {
			if (playerD->hitstate != 3) {
				if (hitrange(playerO, playerD, 30) == 1) {
					if (playerD->attackstate == 1 || playerD->attackstate == 2 || playerD->attackstate == 4 || playerD->attackstate == 5) {//相打ち
						if (playerD->hitlimit == 0 && playerD->direction == 0) {
							playerO->hitlimit = 1;
							playerD->hitlimit = 1;
							if (playerO->direction == RIGHT) {
								playerO->x -= 200;
								playerD->x += 200;
							}
							else {
								playerO->x += 200;
								playerD->x -= 200;
							}
						}
					}
					if (playerO->hitlimit == 0) {
						if (playerD->hitcount > 0) {
							playerD->combo++;
						}
						playerD->hitstate = 3;
						playerD->hitcount = 0;
						playerO->hitlimit = 1;
						playerD->attackstate = 0;
						playerD->delaycount = 0;
						playerD->hitlimit = 0;
						playerD->hp -= 20;
					}
				}
			}
		}
		if (playerO->delaycount < NA - 4) {
			playerO->hitlimit = 1;
		}
		playerO->delaycount--;
		if (playerO->delaycount <= 0) {
			playerO->attackstate = 0;
			playerO->delaycount = 0;
			playerO->hitlimit = 0;
		}
		break;
	case 5:
		if (playerO->delaycount <= 0) {
			playerO->attackcount = 40;
			if (playerO->movestate == 3) {
				playerO->delaycount = DPA;
			}
			else {
				playerO->delaycount = DPA;
			}
		}
		if (playerO->delaycount >= DPA - 4 && playerO->hitlimit == 0 && playerO->attackcount < 10) {
			if (playerD->hitstate !=3) {
				if (hitrange(playerO, playerD, 50) == 1) {
					if (playerD->attackstate == 1 || playerD->attackstate == 2 || playerD->attackstate == 4 || playerD->attackstate == 5) {//相打ち
						if (playerD->hitlimit == 0) {
							playerO->hitlimit = 1;
							playerD->hitlimit = 1;
							if (playerO->direction == RIGHT) {
								playerO->x -= 200;
								playerD->x += 200;
							}
							else if (playerO->direction == LEFT) {
								playerO->x += 200;
								playerD->x -= 200;
							}

						}
					}
					if (playerO->hitlimit == 0) {
						if (playerD->attackstate == 3) {//ガード状態
							if (playerO->movestate == 0 && playerD->movestate == 3) {//立ち攻撃/しゃがみガード
								playerD->hitstate = 1;
								if (playerD->hitcount > 0) {
									playerD->combo++;
								}
								playerD->hitcount = 0;
								playerD->hp -= 10;
								playerD->attackstate = 0;
								playerD->delaycount = 0;
								playerD->hitlimit = 0;
							}
							else if (playerO->movestate == 3 && playerD->movestate == 0) {//しゃがみ攻撃/立ちガード
								playerD->hitstate = 1;
								if (playerD->hitcount > 0) {
									playerD->combo++;
								}
								playerD->hitcount = 0;
								playerD->hp -= 10;
								playerD->attackstate = 0;
								playerD->delaycount = 0;
								playerD->hitlimit = 0;
							}
							else {
								playerD->hp -= 1;
								if (playerO->delaycount == RIGHT) { playerD->x += 2; }
								else { playerD->x -= 2; }
								guard1 = 1;
							}
						}
						else {//ノーガード
							playerD->hitstate = 1;
							if (playerD->hitcount > 0) {
								playerD->combo++;
							}
							playerD->hitcount = 0;
							playerD->hp -= 10;
							playerD->attackstate = 0;
							playerD->delaycount = 0;
							playerD->hitlimit = 0;
						}
						playerO->hitlimit = 1;
					}
				}
			}
		}
		if (playerO->attackcount > 0&& playerO->attackcount < 10  ) {
			if (playerO->direction == LEFT) {
				if (bound(playerO, playerD) == 0 || playerO->x < playerD->x) {
					playerO->x -= 10;
				}
				else {
					if (playerD->x > 0 && playerO->x > playerD->x) {
						//playerO->x -= 1;
						//playerD->x -= 1;
					}
				}
			}
			else {
				if (bound(playerO, playerD) == 0 || playerO->x > playerD->x) {
					playerO->x += 10;
				}
				else {
					if (playerD->x < 1000 && playerO->x < playerD->x) {
						//playerO->x += 1;
						//playerD->x += 1;
					}
				}
			}
			playerO->attackcount--;
		}
		else if (playerO->attackcount >= 10) {
			playerO->attackcount--;
		}
		if (playerO->attackcount == 0) {
			if (playerO->delaycount < DPA - 4) {
				playerO->hitlimit = 1;
			}
			playerO->delaycount--;
			if (playerO->delaycount <= 0) {
				playerO->attackstate = 0;
				playerO->delaycount = 0;
				playerO->hitlimit = 0;
			}
		}
		break;
		break;
	default:
		break;
	}
}
void image(struct Player *player) {
	if (player->attackstate == 0 && player->hitstate == 0) {
		switch (player->movestate)
		{
		case 0:
			if (player->direction == 0) {
				DrawGraph(player->x, player->y, img[26], TRUE);
			}
			else if (player->direction == 2) {
				DrawGraph(player->x, player->y, img[0], TRUE);
			}
			break;
		case 1:
			if (player->direction == 0) {
				if (player->imagecount < 1 || player->imagecount>8) {
					player->imagecount = 8;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				player->imagecount--;
			}
			else if (player->direction == 2) {
				if (player->imagecount < 27 || player->imagecount>34) {
					player->imagecount = 27;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				player->imagecount++;
			}
			break;
		case 2:
			if (player->direction == 0) {
				if (player->imagecount < 1 || player->imagecount>8) {
					player->imagecount = 1;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				player->imagecount++;
			}
			else if (player->direction == 2) {
				if (player->imagecount < 27 || player->imagecount>34) {
					player->imagecount = 34;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				player->imagecount--;
			}
			break;
		case 3:
			if (player->direction == 0) {
				player->imagecount = 46;
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
			else if (player->direction == 2) {
				player->imagecount = 20;
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
			break;
		default:
			break;
		}
	}
	switch (player->attackstate)
	{
	case 0:
		break;
	case 1:
		if (player->movestate == 3) {//しゃがみ
			if (player->direction == 0) {
				if (player->imagecount != 46) {
					player->imagecount = 46;
				}
				if (player->delaycount <= PA - 2) {
					player->imagecount = 48;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
			else if (player->direction == 2) {
				if (player->imagecount != 20) {
					player->imagecount = 20;
				}
				if (player->delaycount <= PA - 2) {
					player->imagecount = 22;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
		}
		else {//立ち
			if (player->direction == 0) {
				if (player->movestate == 0) {
					if (player->imagecount != 35) {
						player->imagecount = 35;
					}
					if (player->delaycount <= PA - 2) {
						player->imagecount = 36;
					}
					DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				}
			}
			else if (player->direction == 2) {
				if (player->movestate == 0) {
					if (player->imagecount != 9) {
						player->imagecount = 9;
					}
					if (player->delaycount <= PA - 2) {
						player->imagecount = 10;
					}
					DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				}
			}
		}
		break;
	case 2:
		if (player->delaycount <= 0) {
			if (player->movestate == 5) {
				player->delaycount = KA;
			}
			else if (player->movestate == 3) {
				player->delaycount = KA;
			}
			else {
				player->delaycount = KA;
			}
		}
		if (player->movestate == 3) {
			if (player->direction == 0) {
				if (player->imagecount != 46) {
					player->imagecount = 46;
				}
				if (player->delaycount <= KA - 2) {
					player->imagecount = 49;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
			else if (player->direction == 2) {
				if (player->imagecount != 20) {
					player->imagecount = 20;
				}
				if (player->delaycount <= KA - 2) {
					player->imagecount = 23;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
		}
		else {
			if (player->direction == 0) {
				if (player->movestate == 0) {
					if (player->imagecount != 37) {
						player->imagecount = 37;
					}
					if (player->delaycount <= KA - 2) {
						player->imagecount = 38;
					}
					DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				}
			}
			else if (player->direction == 2) {
				if (player->movestate == 0) {
					if (player->imagecount != 11) {
						player->imagecount = 11;
					}
					if (player->delaycount <= KA - 2) {
						player->imagecount = 12;
					}
					DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				}
			}
		}
		break;
	case 3:
		if (player->movestate == 3) {
			if (player->direction == 0) {
				player->imagecount = 47;
			}
			else if (player->direction == 2) {
				player->imagecount = 21;
			}
		}
		else {
			if (player->direction == 0) {
				player->imagecount = 41;
			}
			else if (player->direction == 2) {
				player->imagecount = 15;
			}
		}
		DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
		break;
	case 4:
		if (player->delaycount <= 0) {
			player->delaycount = NA;
		}
		if (player->direction == 0) {
			if (player->imagecount !=39) {
				player->imagecount = 39;
			}
			if (player->delaycount <= NA - 2) {
				player->imagecount = 40;
			}
		}
		else if (player->direction == 2) {
			if (player->imagecount !=13) {
				player->imagecount = 13;
			}
			if (player->delaycount <= NA - 2) {
				player->imagecount = 14;
			}
		}
		DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
		break;
	case 5:
		if (player->movestate == 3) {//しゃがみ
			if (player->direction == 0) {
				if (player->imagecount != 46) {
					player->imagecount = 46;
				}
				if (player->attackcount < 10) {
					player->imagecount = 48;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
			else if (player->direction == 2) {
				if (player->imagecount != 20) {
					player->imagecount = 20;
				}
				if (player->attackcount < 10) {
					player->imagecount = 22;
				}
				DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
			}
		}
		else {//立ち
			if (player->direction == 0) {
				if (player->movestate == 0) {
					if (player->imagecount != 35) {
						player->imagecount = 35;
					}
					if (player->attackcount < 10) {
						player->imagecount = 36;
					}
					DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				}
			}
			else if (player->direction == 2) {
				if (player->movestate == 0) {
					if (player->imagecount != 9) {
						player->imagecount = 9;
					}
					if (player->attackcount < 10) {
						player->imagecount = 10;
					}
					DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
				}
			}
		}
		break;
	default:
		break;
	}
	switch (player->hitstate) {
	case 0:
		break;
	case 1:
		if (player->direction == LEFT) {
			DrawGraph(player->x, player->y, img[62], TRUE);
		}
		else if (player->direction == RIGHT) {
			DrawGraph(player->x, player->y, img[61], TRUE);
		}
		break;
	case 2:
		if (player->direction == LEFT) {
			DrawGraph(player->x, player->y, img[62], TRUE);
		}
		else if (player->direction == RIGHT) {
			DrawGraph(player->x, player->y, img[61], TRUE);
		}
		break;
	case 3:
		if (player->direction == LEFT) {
			if (player->hitcount <= 0) {
				player->hitcount = 100;
			}
			if (player->hitcount > 90) {
				player->imagecount = 24;
			}
			else {
				player->imagecount = 25;
			}
		}
		else if (player->direction == RIGHT) {
			if (player->hitcount <= 0) {
				player->hitcount = 100;
			}
			if (player->hitcount > 90) {
				player->imagecount = 50;
			}
			else {
				player->imagecount = 51;
			}
		}
		DrawGraph(player->x, player->y, img[player->imagecount], TRUE);
		break;
	default:
		break;
	}
}
void statefix(struct Player *player) {
	if (player->movestate == 1 || player->movestate == 2 || player->movestate == 3) {
		if (player->attackstate == 0) {
			player->movestate = 0;
		}
	}
	if (player->attackstate == 3) {
		player->attackstate = 0;
		if (player->movestate != 3) {
			player->movestate = 0;
		}
	}
	if (player->x < 0) {
		player->x = 0;
	}
	if (player->x > 1000) {
		player->x = 1000;
	}
	if (player->y > 400 && player->movestate == 0 && player->hitstate == 0) {
		player->y = 400;
	}



}
int hitrange(struct Player *playerO, struct Player *playerD, int x_range) {
	if (playerD->attackstate == 2) {
		if (playerO->direction == RIGHT) {
			if (playerO->x + 100 < playerD->x + 200 && playerO->x + 100 + x_range > playerD->x - 0) {
				if (playerO->y + 100 - 100 < playerD->y + 200 && playerO->y + 200 > playerD->y + 100 - 100) {
					return 1;
				}
			}
		}
		else if (playerO->direction == LEFT) {
			if (playerO->x + 100 > playerD->x  && playerO->x + 100 - x_range < playerD->x + 200) {
				if (playerO->y + 100 - 100 < playerD->y + 200 && playerO->y + 200 > playerD->y + 100 - 100) {
					return 1;
				}
			}
		}
	}
	else {
		if (playerO->direction == RIGHT) {
			if (playerO->x + 100 + x_range > playerD->x+50) {
				if (playerO->y + 100 - 100 < playerD->y + 200 && playerO->y + 200 > playerD->y + 100 - 100) {
					return 1;
				}
			}
		}
		else if (playerO->direction == LEFT) {
			if (playerO->x + 100 - x_range < playerD->x + 150) {
				if (playerO->y + 100 - 100 < playerD->y + 200 && playerO->y + 200 > playerD->y + 100 - 100) {
					return 1;
				}
			}
		}
	}
	return 0;
}
int bound(struct Player *playerO, struct Player *playerD) {
	if (playerO->direction == RIGHT) {
		if (playerO->x + 100 > playerD->x + 30) {
				return 1;
		}
	}
	else {
		if (playerO->x + 100 < playerD->x + 170) {
			return 1;
		}
	}
	return 0;
}
void win(int p1hp, int p2hp) {
	if (p1hp <= 0) {
		if (Start2 == 0) {
			Start2 = 1;
			player2.win = 1;
			countS = 100;
		}
	}
	if (p2hp <= 0) {
		if (Start2 == 0) {
			Start2 = 1;
			player1.win = 1;
			countS = 100;
		}
	}
}
void HPandGAUGEimage(void) {
	unsigned int Cr;
	Cr = GetColor(0, 0, 255);        // 青色の値を取得
	DrawBox(50, 50, (player1.hp*5/3)+50, 80, Cr, TRUE);
	DrawBox(1150-(player2.hp*5/3), 50, 1150, 80, Cr, TRUE);
	Cr = GetColor(0, 0, 0);
	if (guard1== 1) {
		DrawString(650, 250, "Guard", Cr);
	}
	if (player2.combo > 1) {
		DrawString(0, 450, "COMBO", Cr);
	}
	DrawFormatString(0, 150, Cr, "hp         :%d。", player1.hp);
	DrawFormatString(0, 170, Cr, "player.x   :%d。", abs((player1s.x + 100) - (player2s.x + 100)));
	if (player1.direction == RIGHT) {//右向き
		DrawFormatString(0, 190, Cr, "player.x   :%d。", abs(player1.x+100 - 1200));
	}
	else {//左向き
		DrawFormatString(0, 190, Cr, "player.x   :%d。", abs(player1.x+100));
	}
	DrawFormatString(0, 210, Cr, "movestate  :%d。", player1.movestate);
	DrawFormatString(0, 230, Cr, "attackstate:%d。", player1.attackstate);
	DrawFormatString(0, 250, Cr, "hitstate   :%d。", player1.hitstate);
	DrawFormatString(0, 270, Cr, "delaycount :%d。", player1.delaycount);
	DrawFormatString(0, 290, Cr, "hitcount   :%d。", player1.hitcount);
	DrawFormatString(0, 310, Cr, "hitlimit   :%d。", player1.hitlimit);
	DrawFormatString(0, 330, Cr, "direction  :%d。", player1.direction);


	if (player1.combo > 1) {
		DrawString(1000, 450, "COMBO", Cr);
	}
	DrawFormatString(1000, 150, Cr, "hp         :%d。", player2.hp);
	DrawFormatString(1000, 170, Cr, "player.x   :%d。", abs((player2s.x + 100) - (player1s.x + 100)));
	if (player2.direction == RIGHT) {//右向き
		DrawFormatString(1000, 190, Cr, "player.x   :%d。", abs(player2.x+100 - 1200));
	}
	else {//左向き
		DrawFormatString(1000, 190, Cr, "player.x   :%d。", abs(player2.x+100));
	}
	DrawFormatString(1000, 210, Cr, "movestate  :%d。", player2.movestate);
	DrawFormatString(1000, 230, Cr, "attackstate:%d。", player2.attackstate);
	DrawFormatString(1000, 250, Cr, "hitstate   :%d。", player2.hitstate);
	DrawFormatString(1000, 270, Cr, "delaycount :%d。", player2.delaycount);
	DrawFormatString(1000, 290, Cr, "hitcount   :%d。", player2.hitcount);
	DrawFormatString(1000, 310, Cr, "hitlimit   :%d。", player2.hitlimit);
	DrawFormatString(1000, 330, Cr, "direction  :%d。", player2.direction);

	//DrawBox(player1.x, player1.y, player1.x+200, player1.y+200, GetColor(0, 0, 0), FALSE);
	//DrawBox(player2.x, player2.y, player2.x + 200, player2.y + 200, GetColor(0, 0, 0), FALSE);
}
void start(void) {
	if (Start == 0) {
		if (countS > 240) {
		DrawGraph(0, 0, img[57], TRUE);
		}
		else if (countS > 160) {
			DrawGraph(0, 0, img[56], TRUE);
		}
		else if (countS > 80) {
			DrawGraph(0, 0, img[55], TRUE);
		}
		else {
			DrawGraph(0, 0, img[58], TRUE);
		}
		if (countS == 0) {
			Start = 1;
			Round++;
		}
		countS--;
	}
}
void startGA(void) {
	if (Start == 0) {
		countS--;
	}
}
void Time(void) {
	if (Start == 1 && time2 > 0) {
		time2--;
	}
	DrawFormatString(600, 50, GetColor(0, 255, 0), "%d", time2 / 60);
	if (time2 == 0) {
		if (player1.hp > player2.hp) {
			if (Start2 == 0) {
				Start2 = 1;
				player1.win = 1;
				countS = 100;
			}
		}
		else if (player1.hp < player2.hp) {
			if (Start2 == 0) {
				Start2 = 1;
				player2.win = 1;
				countS = 100;
			}
		}
		else {
			if (Start2 == 0) {
				Start2 = 1;
				countS = 100;
			}
		}
	}
}
void TimeGA(void) {
	if (Start == 1 && time2 > 0) {
		time2--;
	}
	if (time2 <= 0) {
		if (player1.hp > player2.hp) {
			if (Start2 == 0) {
				Start2 = 1;
				player1.win = 1;
				countS = 100;
			}
		}
		else if (player1.hp < player2.hp) {
			if (Start2 == 0) {
				Start2 = 1;
				player2.win = 1;
				countS = 100;
			}
		}
		else {
			if (Start2 == 0) {
				Start2 = 1;
				countS = 100;
			}
		}
	}
}
void FpsTimeFanction() {
	if (FpsTime_i == 0)
		FpsTime[0] = GetNowCount();               //1周目の時間取得
	if (FpsTime_i == 49) {
		FpsTime[1] = GetNowCount();               //50周目の時間取得
		Fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 50.0f);//測定した値からfpsを計算
		FpsTime_i = 0;//カウントを初期化
	}
	else
		FpsTime_i++;//現在何周目かカウント
	if (Fps != 0) {
		//DrawFormatString(565, 460, GetColor(0, 255, 0), "FPS %.1f", Fps); //fpsを表示
		/*
		char str1[128] = { 0 };
		sprintf_s(str1, "%f", Fps);
		OutputDebugString(str1);
		OutputDebugString("\n");
		*/
	}
	return;
}
void round() {
	if (countS >= 0) {
		if (player1.win == 1) {
			DrawGraph(0, 0, img[59], TRUE);
			ScreenFlip();//裏画面を表画面にコピー
			WaitKey();				// キー入力待ち
			DxLib_End();				// ＤＸライブラリ使用の終了処理
			exit(0);				// ソフトの終了 
		}
		else if (player2.win == 1) {
			DrawGraph(0, 0, img[60], TRUE);
			ScreenFlip();//裏画面を表画面にコピー
			WaitKey();				// キー入力待ち
			DxLib_End();				// ＤＸライブラリ使用の終了処理
			exit(0);		// ソフトの終了 
		}
		else if (player1.win == 0 && player2.win == 0) {
			DrawGraph(0, 0, img[51], TRUE);
			ScreenFlip();//裏画面を表画面にコピー
			WaitKey();				// キー入力待ち
			DxLib_End();				// ＤＸライブラリ使用の終了処理
			exit(0);		// ソフトの終了 

		}
	}
	countS--;
	if (countS == 0) {
		Start = 0;
		Start2 = 0;
		countS = 400;
		player1.win =0;
		player2.win =0;
		player1.hp = 100;
		player2.hp = 100;
		player1.x = 200;
		player1.y = 400;
		player2.x = 800;
		player2.y = 400;
	}
}
void roundGA(void) {
	int A = 0;
	static int L;
	A = 1;
	L++;
	fprintf(file, "%d:%d:%d:%d:\n", player1.win, player2.win, player1.hp, player2.hp);
	if (A == 1) {
		if (player1.win == 1) {
			wincount1++;
			losecount2++;
		}
		else if (player2.win == 1) {
			wincount2++;
			losecount1++;
		}
		else if (player1.win == 0 && player2.win == 0) {
			drawcount++;
		}
		Start = 0;
		Start2 = 0;
		countS = 400;
		player1.win = 0;
		player2.win = 0;
		player1.hp = 300;
		player2.hp = 300;
		player1.x = 200;
		player1.y = 400;
		player2.x = 800;
		player2.y = 400;
		time2 = 3600;
	}
	if (L == 10) {
		fprintf(file, "%d:%d:%d:%d:\n", player1.win, player2.win, player1.hp, player2.hp);
		fprintf(file, "終わり%d:%d:%d:%d:%d:\n", wincount1, losecount1,wincount2,losecount2, drawcount);
		fclose(file);
		exit(0);
	}
}
void Floreano(void) {
	static int countSV = 0;
	static int countT = 0;
	static int countF = 0;
	int c = 0, i = 0, j = 0,h = 0,maxc = 0;
	maxc = 0;
	double dx = 0, min_ca = 0, max_ca = 0;
	int rand_s = 0,rand_s2 = 0;
	if (Start2 == 0 && Start == 0) {
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_T[i][j] = w1_GA_T[countT][i][j];
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_T[i][j] = w2_GA_T[countT][i][j];
			}
		}
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1[i][j] = w1_GA[countSV][i][j];
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2[i][j] = w2_GA[countSV][i][j];
			}
		}
		Start = 1;
	}
	if (Start2 == 1 && Start == 1) {
		if (player1.win == 1) {
			/*+ player1.hp * 10 / 300 - player2.hp * 10 / 300*/
			rd[countSV] += (((double)player1.hp) / 300.0 - ((double)player2.hp)  / 300.0)/KOT*KO;
			wincount1++;
			losecount2++;
		}
		else if (player2.win == 1) {
			//rd[countSV] += player1.hp * 2 / 300 - player2.hp * 2 / 300;
			wincount2++;
			losecount1++;
		}
		else if (player1.win == 0 && player2.win == 0) {
			//rd[countSV] = 1;
			drawcount++;
		}
		
		/////
		Start = 0;
		Start2 = 0;
		countS = 400;
		player1.win = 0;
		player2.win = 0;
		player1.hp = 300;
		player2.hp = 300;
		player1.x = 200;
		player1.y = 400;
		player2.x = 800;
		player2.y = 400;
		time2 = 3600;
		/////
		if (countSV == KO - 1 && countT == KOT - 1) {
			//ルーレット1
			for (i = 0; i < KO; i++) {
				for (j = 0; j < rd[i]+1; j++) {
					sv[h] = i;
					h++;
				}
			}
			//ルーレット2
			for (i = 0; i < KO / 4; i++) {
				rdc[i] = sv[rand() % h];
			}
			//ルーレット３
			for (h = 0; h < KO / 4; h++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_GA_C[h][i][j] = w1_GA[rdc[h]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_GA_C[h][i][j] = w2_GA[rdc[h]][i][j];
					}
				}
			}
			//最大評価値探索
			for (i = 0; i < KO - 1; i++) {
				if (rd[maxc] <= rd[i + 1]) {
					maxc = i + 1;
					char str10[128] = { 0 };
					sprintf_s(str10, "%f::", rd[i + 1]);
					//OutputDebugString(str10);
				}
			}
			OutputDebugString("\n");
			for (i = 0; i < KO; i++) {
				rd[i] = 0;
			}
			//対戦相手の更新
			for (h = 0; h < KOT-1; h++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_GA_T[KOT-1-h][i][j] = w1_GA_T[KOT-2-h][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_GA_T[KOT - 1 - h][i][j] = w2_GA_T[KOT - 2 - h][i][j];
					}
				}
			}
	
			//対戦相手の更新
			for (i = 0; i < I1; i++) {
				for (j = 0; j < J1; j++) {
					w1_GA_T[0][i][j] = w1_GA[maxc][i][j];
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J2; j++) {
					w2_GA_T[0][i][j] = w2_GA[maxc][i][j];
				}
				
			}
		
			//個体の更新
			for (h = 0; h < KO / 4; h++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_GA[h][i][j] = w1_GA_C[h][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_GA[h][i][j] = w2_GA_C[h][i][j];
					}
				}
			}
			//BLX-α
			for (h = KO / 4; h < KO; h++) {
				rand_s = rand() % (KO / 4);
				while ((rand_s2 = rand() %( KO / 4)) != rand_s) {
				}
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						dx = fabs(w1_GA[rand_s][i][j] - w1_GA[rand_s2][i][j]);
						min_ca = fmin(w1_GA[rand_s][i][j], w1_GA[rand_s2][i][j]) - 0.3*dx;
						max_ca = fmax(w1_GA[rand_s][i][j], w1_GA[rand_s2][i][j]) + 0.3*dx;
						w1_GA[h][i][j] = min_ca + rand()*(max_ca - min_ca + 1.0) / (1.0 + RAND_MAX);
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						dx = fabs(w2_GA[rand_s][i][j] - w2_GA[rand_s2][i][j]);
						min_ca = fmin(w2_GA[rand_s][i][j], w2_GA[rand_s2][i][j]) - 0.3*dx;
						max_ca = fmax(w2_GA[rand_s][i][j], w2_GA[rand_s2][i][j]) + 0.3*dx;
						w2_GA[h][i][j] = min_ca + rand()*(max_ca - min_ca + 1.0) / (1.0 + RAND_MAX);
					}
				}
			}
			countSV = 0;
			countT = 0;
			countF++;
			if (countF >= KU) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						fprintf(file, "%f", w1_GA_T[0][i][j]);
						fprintf(file, "\n");
						//fprintf(file, " ");
					}
					//fprintf(file, "\n");
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						fprintf(file, "%f", w2_GA_T[0][i][j]);
						fprintf(file, "\n");
						//fprintf(file, " ");
					}
					//fprintf(file, "\n");
				}
				fprintf(fp, "終わり%d:%d:%d:%d:%d:\n", wincount1, losecount1, wincount2, losecount2, drawcount);
				fclose(file);
				fclose(fp);
				exit(0);
			}
			else if (countF % 20 == 0 && countF != 0) {
				if (wincount1 == 0 && wincount2 == 0) {
					exit(0);
				}
			}
		}
		else {
			countSV++;
			
			char str1[128] = { 0 };
			sprintf_s(str1, "個体%3d", countSV);
			OutputDebugString(str1);
			char str2[128] = { 0 };
			sprintf_s(str2, "対戦相手%d", countT);
			char str3[128] = { 0 };
			sprintf_s(str3, "世代%d", countF);
			char str4[128] = { 0 };
			sprintf_s(str4, "勝利数%d", wincount1);
			char str5[128] = { 0 };
			sprintf_s(str5, "負け数%d", losecount1);
			char str6[128] = { 0 };
			sprintf_s(str6, "勝率%f",wincount1/((double)wincount1+ (double)losecount1+ (double)drawcount+1.0)*100);
			OutputDebugString(":");
			OutputDebugString(str2);
			OutputDebugString(":");
			OutputDebugString(str3);
			OutputDebugString(":");
			OutputDebugString(str4);
			OutputDebugString(":");
			OutputDebugString(str5);
			OutputDebugString(":");
			OutputDebugString(str6);
			OutputDebugString("\n");
			
			if (countSV >= KO) {
				countT++;
				countSV = 0;
			}
		}
	}
}
void NN1P(void) {
	int i = 0;
	int j = 0;
	int max = 0;
	double u1[I1] = { 0 };
	double u2[J1] = { 0 };
	double u3[J2] = { 0 };

	u1[0] = (double)time2 / 3600;//残り時間
	u1[1] = (double)player1s.hp / 300;//自分のHP
	u1[2] = (double)(abs((player1s.x + 100) - (player2s.x + 100))) / 1000;//相手の相対位置

	if (player1s.direction == 0) {//右向き
		u1[3] = (double)abs(player1s.x - 1000) / 1000;//壁との相対位置
	}
	else {//左向き
		u1[3] = (double)abs(player1s.x) / 1000;//壁との相対位置
	}
	u1[4] = (double)player1s.movestate / 3;//自分の移動状態
	u1[5] = (double)player1s.attackstate / 5;//自分の攻撃状態
	u1[6] = (double)player1s.hitstate / 3;//自分のヒット状態
	u1[7] = (double)player1s.hitcount / 50;//自分のひるみ時間
	u1[8] = (double)player1s.delaycount / 50;//自分の攻撃時間
	u1[9] = (double)player2s.hp / 300;//相手のHP
	u1[10] = (double)player2s.movestate / 3;//相手の移動状態
	u1[11] = (double)player2s.attackstate / 5;//相手の攻撃状態
	u1[12] = (double)player2s.hitstate / 3;//相手の攻撃状態
	u1[13] = (double)player2s.hitcount / 90;//相手のひるみ時間
	u1[14] = (double)player2s.delaycount / 50;//相手の攻撃時間

	for (i = 0; i < I1; i++) {
		for (j = 0; j < J1; j++) {
			u2[j] += u1[i] * w1[i][j];
		}
	}
	for (i = 0; i < I2; i++) {
		for (j = 0; j < J2; j++) {
			u3[j] += 1.0 / (1.0 + exp(-u2[i]))*w2[i][j];
		}
	}

	for (i = 0; i < J2 - 1; i++) {
		if (u3[max] < u3[i + 1]) {
			max = i + 1;
		}
	}


	switch (max) {
	case 0://立ち
		if (player1.movestate == 0 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 0;
		}
		break;
	case 1://前方移動
		if (player1.movestate == 0 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 3 && player1.attackstate != 4 && player1.attackstate != 5) {
			if (player1.direction == 0) {//右向き
				player1.movestate = 2;//右
			}
			else {//左向き
				player1.movestate = 1;//左
			}
		}
		break;
	case 2://後方移動
		if (player1.movestate == 0 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 3 && player1.attackstate != 4 && player1.attackstate != 5) {
			if (player1.direction == 0) {//右向き
				player1.movestate = 1;//左
			}
			else {//左向き
				player1.movestate = 2;//右
			}
		}
		break;
	case 3://しゃがみ
		if (player1.movestate == 0 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 3 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
		}
		break;
	case 4://弱攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 1;
		}
		break;
	case 5://強攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 2;
		}
		break;
	case 6://ガード
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 3;
		}
		break;
	case 7://投げ攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8 
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 4;
		}
		break;
	case 8://移動攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 5;
		}
		break;
	case 9://しゃがみ弱攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 1;
		}
		break;
	case 10://しゃがみ強攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 2;
		}
		break;
	case 11://しゃがみガード
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 3;
		}
		break;
	case 12://しゃがみ弱攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 5 && player1.hitstate != 6 && player1.hitstate != 7 && player1.hitstate != 8
			&& player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 5;
		}
		break;
	default:
		break;
	}

	//char str4[128] = { 0 };
	//sprintf_s(str4, "%f", u1[2]);
	//OutputDebugString(str4);
	//OutputDebugString("\n");

}
void NN2P(void) {
	int i = 0;
	int j = 0;
	int max = 0;
	double u1[I1] = { 0 };
	double u2[J1] = { 0 };
	double u3[J2] = { 0 };

	u1[0] = (double)time2 / 3600;//残り時間
	u1[1] = (double)player2s.hp / 300;//自分のHP
	u1[2] = (double)(abs((player2s.x + 100) - (player1s.x + 100))) / 1000;//相手の相対位置

	if (player2s.direction == 0) {//右向き
		u1[3] = (double)abs(player2s.x - 1000) / 1000;//壁との相対位置
	}
	else {//左向き
		u1[3] = (double)abs(player2s.x) / 1000;//壁との相対位置
	}
	u1[4] = (double)player2s.movestate / 3;//自分の移動状態
	u1[5] = (double)player2s.attackstate / 5;//自分の攻撃状態
	u1[6] = (double)player2s.hitstate / 3;//自分のヒット状態
	u1[7] = (double)player2s.hitcount / 50;//自分のひるみ時間
	u1[8] = (double)player2s.delaycount / 50;//自分の攻撃時間
	u1[9] = (double)player1s.hp / 300;//相手のHP
	u1[10] = (double)player1s.movestate / 3;//相手の移動状態
	u1[11] = (double)player1s.attackstate / 5;//相手の攻撃状態
	u1[12] = (double)player1s.hitstate / 3;//相手の攻撃状態
	u1[13] = (double)player1s.hitcount / 50;//相手のひるみ時間
	u1[14] = (double)player1s.delaycount / 50;//相手の攻撃時間

	for (i = 0; i < I1; i++) {
		for (j = 0; j < J1; j++) {
			u2[j] += u1[i] * w1_T[i][j];
		}
	}
	for (i = 0; i < I2; i++) {
		for (j = 0; j < J2; j++) {
			u3[j] += 1.0 / (1.0 + exp(-u2[i]))*w2_T[i][j];
		}
	}

	for (i = 0; i < J2 - 1; i++) {
		if (u3[max] < u3[i + 1]) {
			max = i + 1;
		}
	}

	switch (max) {
	case 0://立ち
		if (player2.movestate == 0 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 &&
			player2.attackstate != 1 && player2.attackstate != 2  && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 0;
		}
		break;
	case 1://前方移動
		if (player2.movestate == 0 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 3 && player2.attackstate != 4 && player2.attackstate != 5) {
			if (player2.direction == 0) {//右向き
				player2.movestate = 2;//右
			}
			else {//左向き
				player2.movestate = 1;//左
			}
		}
		break;
	case 2://後方移動
		if (player2.movestate == 0 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 3 && player2.attackstate != 4 && player2.attackstate != 5) {
			if (player2.direction == 0) {//右向き
				player2.movestate = 1;//左
			}
			else {//左向き
				player2.movestate = 2;//右
			}
		}
		break;
	case 3://しゃがみ
		if (player2.movestate == 0 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 3 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
		}
		break;
	case 4://弱攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 1;
		}
		break;
	case 5://強攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 2;
		}
		break;
	case 6://ガード
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 3;
		}
		break;
	case 7://投げ
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8 
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 4;
		}
		break;
	case 8://弱攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 5;
		}
		break;
	case 9://しゃがみ弱攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 1;
		}
		break;
	case 10://しゃがみ強攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 2;
		}
		break;
	case 11://しゃがみガード
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 3;
		}
		break;
	case 12://しゃがみ弱攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 5 && player2.hitstate != 6 && player2.hitstate != 7 && player2.hitstate != 8
			&& player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 5;
		}
		break;
	default:
		break;
	}
}
void NNR1P(void) {
	int i = 0;
	int j = 0;
	int max = 0;
	double u1[I1] = { 0 };
	double u2[J1] = { 0 };
	double u3[J2] = { 0 };

	u1[0] = (double)time2 / 3600;//残り時間
	u1[1] = (double)player1s.hp / 300;//自分のHP
	u1[2] = (double)(abs((player1s.x + 100) - (player2s.x + 100))) / 1000;//相手の相対位置

	if (player1s.direction == RIGHT) {//右向き
		u1[3] = (double)abs(player1s.x + 100 - 1200) / 1000;//壁との相対位置
	}
	else {//左向き
		u1[3] = (double)abs(player1s.x + 100) / 1000;//壁との相対位置
	}
	u1[4] = (double)player1s.movestate / 3;//自分の移動状態
	u1[5] = (double)player1s.attackstate / 5;//自分の攻撃状態
	u1[6] = (double)player1s.hitstate / 3;//自分のヒット状態
	if (player1s.attackstate == 1) {
		u1[7] = (double)player1s.delaycount/ PA; //攻撃時間
	}
	else if (player1s.attackstate == 2) {
		u1[7] = (double)player1s.delaycount / KA; //攻撃時間
	}
	else if (player1s.attackstate == 4) {
		u1[7] = (double)player1s.delaycount / NA; //攻撃時間
	}
	else if (player1s.attackstate == 5) {
		u1[7] = (double)player1s.delaycount / DPA;//攻撃時間
	}
	if (player1s.hitstate == 1) {
		u1[8] = (double)player1s.hitcount / PAHIT;//自分のひるみ時間
	}
	else if (player1s.hitstate == 2) {
		u1[8] = (double)player1s.hitcount / KAHIT;//自分のひるみ時間
	}
	else if (player1s.hitstate == 3) {
		u1[8] = (double)player1s.hitcount / NAHIT;//自分のひるみ時間
	}
	u1[9] = (double)player2s.hp / 300;//相手のHP
	u1[10] = (double)player2s.movestate / 3;//相手の移動状態
	u1[11] = (double)player2s.attackstate / 5;//相手の攻撃状態
	u1[12] = (double)player2s.hitstate / 3;//相手の攻撃状態
	if (player2s.attackstate == 1) {
		u1[13] = (double)player2s.delaycount / PA;//相手の攻撃時間
	}
	else if (player2s.attackstate == 2) {
		u1[13] = (double)player2s.delaycount / KA;//相手の攻撃時間
	}
	else if (player2s.attackstate == 4) {
		u1[13] = (double)player2s.delaycount / NA;//相手の攻撃時間
	}
	else if (player2s.attackstate == 5) {
		u1[13] = (double)player2s.delaycount / DPA;//相手の攻撃時間
	}
	if (player2s.hitstate == 1) {
		u1[14] = (double)player2s.hitcount / PAHIT;//相手のひるみ時間
	}
	else if (player2s.hitstate == 2) {
		u1[14] = (double)player2s.hitcount / KAHIT;//相手のひるみ時間
	}
	else if (player2s.hitstate == 3) {
		u1[14] = (double)player2s.hitcount / NAHIT;//相手のひるみ時間
	}
	for (i = 0; i < I1; i++) {
		for (j = 0; j < J1; j++) {
			u2[j] += u1[i] * w1[i][j];
		}
	}

	for (i = 0; i < I2; i++) {
		for (j = 0; j < J1; j++) {
			u2[j] += 1.0 / (1.0 + exp(-u2_c[i])) * w3[i][j];
		}
	}

	for (i = 0; i < I2; i++) {
		for (j = 0; j < J2; j++) {
			u3[j] += 1.0 / (1.0 + exp(-u2[i]))*w2[i][j];
		}
	}

	for (i = 0; i < J2 - 1; i++) {
		if (u3[max] < u3[i + 1]) {
			max = i + 1;
		}
	}
	for (j = 0; j < J1; j++) {
		u2_c[j] = u2[j];
	}

	switch (max) {
	case 0://立ち
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 0;
		}
		break;
	case 1://前方移動
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 3 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 1;
		}
		break;
	case 2://後方移動
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 3 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 2;
		}
		break;
	case 3://しゃがみ
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
		}
		break;
	case 4://弱攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 1;
		}
		break;
	case 5://強攻撃
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 2;
		}
		break;
	case 6://ガード
		if (player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 3;
		}
		break;
	case 7://投げ攻撃
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 4;
		}
		break;
	case 8://移動攻撃
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.attackstate = 5;
		}
		break;
	case 9://しゃがみ弱攻撃
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 1;
		}
		break;
	case 10://しゃがみ強攻撃
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 2;
		}
		break;
	case 11://しゃがみガード
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 3;
		}
		break;
	case 12://しゃがみ弱攻撃
		if (player1.movestate != 3 && player1.hitstate != 1 && player1.hitstate != 2 && player1.hitstate != 3 &&
			player1.attackstate != 1 && player1.attackstate != 2 && player1.attackstate != 4 && player1.attackstate != 5) {
			player1.movestate = 3;
			player1.attackstate = 5;
		}
		break;
	default:
		break;
	}

	//char str4[128] = { 0 };
	//sprintf_s(str4, "%f", u1[2]);
	//OutputDebugString(str4);
	//OutputDebugString("\n");

}
void NNR2P(void) {
	int i = 0;
	int j = 0;
	int max = 0;
	double u1_T[I1] = { 0 };
	double u2_T[J1] = { 0 };
	double u3_T[J2] = { 0 };

	u1_T[0] = (double)time2 / 3600;//残り時間
	u1_T[1] = (double)player2s.hp / 300;//自分のHP
	u1_T[2] = (double)(abs((player2s.x + 100) - (player1s.x + 100))) / 1000;//相手の相対位置

	if (player2s.direction == RIGHT) {//右向き
		u1_T[3] = (double)abs(player2s.x+100- 1200) / 1000;//壁との相対位置
	}
	else {//左向き
		u1_T[3]= (double)abs(player2s.x+100) / 1000;//壁との相対位置
	}
	u1_T[4] = (double)player2s.movestate / 3;//自分の移動状態
	u1_T[5] = (double)player2s.attackstate / 5;//自分の攻撃状態
	u1_T[6] = (double)player2s.hitstate / 3;//自分のヒット状態
	if (player2s.attackstate == 1) {
		u1_T[7] = (double)player2s.delaycount / PA;//相手の攻撃時間
	}
	else if (player2s.attackstate == 2) {
		u1_T[7] = (double)player2s.delaycount / KA;//相手の攻撃時間
	}
	else if (player2s.attackstate == 4) {
		u1_T[7] = (double)player2s.delaycount / NA;//相手の攻撃時間
	}
	else if (player2s.attackstate == 5) {
		u1_T[7] = (double)player2s.delaycount / DPA;//相手の攻撃時間
	}
	if (player2s.hitstate == 1) {
		u1_T[8] = (double)player2s.hitcount / PAHIT;//相手のひるみ時間
	}
	else if (player2s.hitstate == 2) {
		u1_T[8] = (double)player2s.hitcount / KAHIT;//相手のひるみ時間
	}
	else if (player2s.hitstate == 3) {
		u1_T[8] = (double)player2s.hitcount / NAHIT;//相手のひるみ時間
	}
	u1_T[9] = (double)player1s.hp / 300;//相手のHP
	u1_T[10] = (double)player1s.movestate / 3;//相手の移動状態
	u1_T[11] = (double)player1s.attackstate / 5;//相手の攻撃状態
	u1_T[12] = (double)player1s.hitstate / 3;//相手の攻撃状態
	if (player1s.attackstate == 1) {
		u1_T[13] = (double)player1s.delaycount / PA; //攻撃時間
	}
	else if (player1s.attackstate == 2) {
		u1_T[13] = (double)player1s.delaycount / KA; //攻撃時間
	}
	else if (player1s.attackstate == 4) {
		u1_T[13] = (double)player1s.delaycount / NA; //攻撃時間
	}
	else if (player1s.attackstate == 5) {
		u1_T[13] = (double)player1s.delaycount / DPA;//攻撃時間
	}
	if (player1s.hitstate == 1) {
		u1_T[14] = (double)player1s.hitcount / PAHIT;//自分のひるみ時間
	}
	else if (player1s.hitstate == 2) {
		u1_T[14] = (double)player1s.hitcount / KAHIT;//自分のひるみ時間
	}
	else if (player1s.hitstate == 3) {
		u1_T[14] = (double)player1s.hitcount / NAHIT;//自分のひるみ時間
	}

	for (i = 0; i < I1; i++) {
		for (j = 0; j < J1; j++) {
			u2_T[j] += u1_T[i] * w1_T[i][j];
		}
	}

	for (i = 0; i < I2; i++) {
		for (j = 0; j < J1; j++) {
			u2_T[j] += 1.0 / (1.0 + exp(-u2_c_T[i])) * w3_T[i][j];
		}
	}

	for (i = 0; i < I2; i++) {
		for (j = 0; j < J2; j++) {
			u3_T[j] += 1.0 / (1.0 + exp(-u2_T[i]))*w2_T[i][j];
		}
	}

	for (i = 0; i < J2 - 1; i++) {
		if (u3_T[max] < u3_T[i + 1]) {
			max = i + 1;
		}
	}

	for (j = 0; j < J1; j++) {
		u2_c_T[j] = u2_T[j];
	}

	switch (max) {
	case 0://立ち
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 0;
		}
		break;
	case 1://前方移動
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 3 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 1;
		}
		break;
	case 2://後方移動
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 3 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 2;
		}
		break;
	case 3://しゃがみ
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
		}
		break;
	case 4://弱攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 1;
		}
		break;
	case 5://強攻撃
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 2;
		}
		break;
	case 6://ガード
		if (player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 3;
		}
		break;
	case 7://投げ攻撃
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 4;
		}
		break;
	case 8://移動攻撃
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.attackstate = 5;
		}
		break;
	case 9://しゃがみ弱攻撃
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 1;
		}
		break;
	case 10://しゃがみ強攻撃
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 2;
		}
		break;
	case 11://しゃがみガード
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 3;
		}
		break;
	case 12://しゃがみ弱攻撃
		if (player2.movestate != 3 && player2.hitstate != 1 && player2.hitstate != 2 && player2.hitstate != 3 &&
			player2.attackstate != 1 && player2.attackstate != 2 && player2.attackstate != 4 && player2.attackstate != 5) {
			player2.movestate = 3;
			player2.attackstate = 5;
		}
		break;
	default:
		break;
	}
}
void Floreano2(void) {
	static int countSV = 0;
	static int countT = 0;
	static int countF = 0;
	int c = 0, i = 0, j = 0, h = 0, maxc = 0;
	maxc = 0;
	double dx = 0, min_ca = 0, max_ca = 0;
	int rand_s = 0, rand_s2 = 0;
	if (Start2 == 0 && Start == 0) {
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_T[i][j] = w1_GA_T[countT][i][j];
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_T[i][j] = w2_GA_T[countT][i][j];
			}
		}
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1[i][j] = w1_GA[countSV][i][j];
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2[i][j] = w2_GA[countSV][i][j];
			}
		}
		Start = 1;
	}
	if (Start2 == 1 && Start == 1) {
		if (player1.win == 1) {
			/*+ player1.hp * 10 / 300 - player2.hp * 10 / 300*/
			rd[countSV] += (((double)player1.hp) / 300.0 - ((double)player2.hp) / 300.0) / 2.0 / KOT*KO;
			wincount1++;
			losecount2++;
		}
		else if (player2.win == 1) {
			//rd[countSV] += player1.hp * 2 / 300 - player2.hp * 2 / 300;
			wincount2++;
			losecount1++;
		}
		else if (player1.win == 0 && player2.win == 0) {
			//rd[countSV] = 1;
			drawcount++;
		}

		/////
		Start = 0;
		Start2 = 0;
		countS = 400;
		player1.win = 0;
		player2.win = 0;
		player1.hp = 300;
		player2.hp = 300;
		player1.x = 200;
		player1.y = 400;
		player2.x = 800;
		player2.y = 400;
		time2 = 3600;
		/////
		if (countSV == KO - 1 && countT == KOT - 1) {
			//ルーレット1
			for (i = 0; i < KO; i++) {
				for (j = 0; j < rd[i] + 1; j++) {
					sv[h] = i;
					h++;
				}
			}
			//ルーレット2
			for (i = 0; i < KO / 4; i++) {
				rdc[i] = sv[rand() % h];
			}
			//ルーレット３
			for (h = 0; h < KO / 4; h++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_GA_C[h][i][j] = w1_GA[rdc[h]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_GA_C[h][i][j] = w2_GA[rdc[h]][i][j];
					}
				}
			}
			//最大評価値探索
			for (i = 0; i < KO - 1; i++) {
				if (rd[maxc] <= rd[i + 1]) {
					maxc = i + 1;
					char str10[128] = { 0 };
					sprintf_s(str10, "%f::", rd[i + 1]);
					//OutputDebugString(str10);
				}
			}
			OutputDebugString("\n");
			for (i = 0; i < KO; i++) {
				rd[i] = 0;
			}
			//対戦相手の更新
			for (h = 0; h < KOT - 1; h++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_GA_T[KOT - 1 - h][i][j] = w1_GA_T[KOT - 2 - h][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_GA_T[KOT - 1 - h][i][j] = w2_GA_T[KOT - 2 - h][i][j];
					}
				}
			}

			//対戦相手の更新
			for (i = 0; i < I1; i++) {
				for (j = 0; j < J1; j++) {
					w1_GA_T[0][i][j] = w1_GA[maxc][i][j];
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J2; j++) {
					w2_GA_T[0][i][j] = w2_GA[maxc][i][j];
				}

			}

			//個体の更新
			for (h = 0; h < KO / 4; h++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_GA[h][i][j] = w1_GA_C[h][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_GA[h][i][j] = w2_GA_C[h][i][j];
					}
				}
			}
			//BLX-α
			for (h = KO / 4; h < KO; h++) {
				rand_s = rand() % (KO / 4);
				while ((rand_s2 = rand() % (KO / 4)) != rand_s) {
				}
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						dx = fabs(w1_GA[rand_s][i][j] - w1_GA[rand_s2][i][j]);
						min_ca = fmin(w1_GA[rand_s][i][j], w1_GA[rand_s2][i][j]) - 0.3*dx;
						max_ca = fmax(w1_GA[rand_s][i][j], w1_GA[rand_s2][i][j]) + 0.3*dx;
						w1_GA[h][i][j] = min_ca + rand()*(max_ca - min_ca + 1.0) / (1.0 + RAND_MAX);
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						dx = fabs(w2_GA[rand_s][i][j] - w2_GA[rand_s2][i][j]);
						min_ca = fmin(w2_GA[rand_s][i][j], w2_GA[rand_s2][i][j]) - 0.3*dx;
						max_ca = fmax(w2_GA[rand_s][i][j], w2_GA[rand_s2][i][j]) + 0.3*dx;  
						w2_GA[h][i][j] = min_ca + rand()*(max_ca - min_ca + 1.0) / (1.0 + RAND_MAX);
					}
				}
			}
			countSV = 0;
			countT = 0;
			countF++;
			if (countF >= KU) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						fprintf(file, "%f", w1_GA_T[0][i][j]);
						fprintf(file, "\n");
						//fprintf(file, " ");
					}
					//fprintf(file, "\n");
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						fprintf(file, "%f", w2_GA_T[0][i][j]);
						fprintf(file, "\n");
						//fprintf(file, " ");
					}
					//fprintf(file, "\n");
				}
				fprintf(fp, "終わり%d:%d:%d:%d:%d:\n", wincount1, losecount1, wincount2, losecount2, drawcount);
				fclose(file);
				fclose(fp);
				exit(0);
			}
			else if (countF % 20 == 0 && countF != 0) {
				if (wincount1 == 0 && wincount2 == 0) {
					exit(0);
				}
			}
		}
		else {
			countSV++;

			char str1[128] = { 0 };
			sprintf_s(str1, "個体%3d", countSV);
			OutputDebugString(str1);
			char str2[128] = { 0 };
			sprintf_s(str2, "対戦相手%d", countT);
			char str3[128] = { 0 };
			sprintf_s(str3, "世代%d", countF);
			char str4[128] = { 0 };
			sprintf_s(str4, "勝利数%d", wincount1);
			char str5[128] = { 0 };
			sprintf_s(str5, "負け数%d", losecount1);
			char str6[128] = { 0 };
			sprintf_s(str6, "勝率%f", wincount1 / ((double)wincount1 + (double)losecount1 + (double)drawcount + 1.0) * 100);
			OutputDebugString(":");
			OutputDebugString(str2);
			OutputDebugString(":");
			OutputDebugString(str3);
			OutputDebugString(":");
			OutputDebugString(str4);
			OutputDebugString(":");
			OutputDebugString(str5);
			OutputDebugString(":");
			OutputDebugString(str6);
			OutputDebugString("\n");

			if (countSV >= KO) {
				countT++;
				countSV = 0;
			}
		}
	}
}
void n_learn(void) {
	ChangeWindowMode(TRUE);
	SetGraphMode(1200, 600, 32);
	SetBackgroundColor(255, 255, 255);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		exit(0);			// エラーが起きたら直ちに終了
	}

	if ((file = fopen("gp/test3.txt", "w")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}
	char Buf[256];
	player1.x = 200; player1.y = 400; player2.x = 800; player2.y = 400;
	int c = 0, i = 0, j = 0, k = 0;
	int number = 0, rival_number = 0;
	int learn = 0;
	int flag = 0;
	int endflag = 0;
	int generation = 0;
	srand((unsigned)time(NULL));

	//集団初期個体
	for (c = 0; c < KO; c++) {
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_GA[c][i][j] = rand() % 20 - 10;
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_GA[c][i][j] = rand() % 20 - 10;
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J1; j++) {
				w3_GA[c][i][j] = rand() % 20 - 10;
			}
		}

	}
	//対戦集団初期個体
	for (c = 0; c < KOT; c++) {
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_GA_T[c][i][j] = rand() % 20 - 10;
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_GA_T[c][i][j] = rand() % 20 - 10;
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J1; j++) {
				w3_GA_T[c][i][j] = rand() % 20 - 10;
			}
		}

	}
	//対戦集団クラスタ分割
		//クラスタの中心を設定
	int lk = 0;
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_k[lk][i][j] = rand() % 20 - 10;
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_k[lk][i][j] = rand() % 20 - 10;
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J1; j++) {
				w3_k[lk][i][j] = rand() % 20 - 10;
			}
		}

		while (lk < K) {
			double distance = 0;
			double distance_a[K][KOT] = { 0 };
			double distance_b[KOT] = { 0 };

			for (c = 0; c < KOT; c++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						distance += pow(w1_k[lk][i][j] - w1_GA_T[c][i][j], 2);
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						distance += pow(w2_k[lk][i][j] - w2_GA_T[c][i][j], 2);
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						distance += pow(w3_k[lk][i][j] - w3_GA_T[c][i][j], 2);
					}
				}
				distance_a[lk][c] = distance;
				distance_b[lk] += distance;
				distance = 0;
			}

			for (k = 0; k < lk; k++) {
				for (c = 0; c < KOT; c++) {
					distance_a[lk][c] += distance_a[k][c];
				}
				distance_b[lk] += distance_b[k];
			}

			double L = ((double)rand() / RAND_MAX)*distance_b[lk];
			int RL = 0;

			while (L > RL) {
				L -= distance_a[lk][RL];
				RL++;
			}

			for (i = 0; i < I1; i++) {
				for (j = 0; j < J1; j++) {
					w1_k[lk][i][j] = w1_GA_T[RL - 1][i][j];
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J2; j++) {
					w2_k[lk][i][j] = w2_GA_T[RL - 1][i][j];
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J1; j++) {
					w3_k[lk][i][j] = w3_GA_T[RL - 1][i][j];
				}
			}
			lk++;
		}





	//クラスタ分割
	double distance = 0;
	for (k = 0; k < K; k++) {
		for (c = 0; c < KOT; c++) {
			for (i = 0; i < I1; i++) {
				for (j = 0; j < J1; j++) {
					distance += pow(w1_k[k][i][j] - w1_GA_T[c][i][j], 2);
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J2; j++) {
					distance += pow(w2_k[k][i][j] - w2_GA_T[c][i][j], 2);
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J1; j++) {
					distance += pow(w3_k[k][i][j] - w3_GA_T[c][i][j], 2);
				}
			}
			if (distance <= w_distance[c] || k == 0) {
				w_distance[c] = distance;
				w_cluster[c] = k;
			}
			distance = 0;
		}
	}
	//クラスタ内包
	for (c = 0; c < KOT; c++) {
		cluster_in[w_cluster[c]][cluster_in_number[w_cluster[c]]] = c;
		cluster_in_number[w_cluster[c]]++;
	}
	for (k = 0; k < K; k++) {
		if (cluster_in_number[k] < PARENT/2) {
			char str12[128] = { 0 };
			sprintf_s(str12, "%d,", cluster_in_number[k]);
			OutputDebugString(str12);
			OutputDebugString("error");
			OutputDebugString("\n");
			exit(0);
		}
	}
	//全数対戦
	/*
	for (rival_number = 0; rival_number < KOT; rival_number++) {
		for (i = 0; i < I1; i++) {
			for (j = 0; j < J1; j++) {
				w1_T[i][j] = w1_GA_T[rival_number][i][j];
			}
		}
		for (i = 0; i < I2; i++) {
			for (j = 0; j < J2; j++) {
				w2_T[i][j] = w2_GA_T[rival_number][i][j];
			}
		}
		for (number = 0; number < KO; number++) {
			for (i = 0; i < I1; i++) {
				for (j = 0; j < J1; j++) {
					w1[i][j] = w1_GA[number][i][j];
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J2; j++) {
					w2[i][j] = w2_GA[number][i][j];
				}
			}
			Start = 1;
			Competition();//対戦
			value[number] += (player1.hp - player2.hp) / 300.0;//集団評価
			value_T[rival_number] += (player2.hp - player1.hp) / 300.0;//相手集団評価
			if (player1.win == 1) {
				wincount1++;
			}
			else if (player2.win == 1) {
				losecount1++;
			}
			else {
				drawcount++;
			}
			//
			player1 = playerR;
			player2 = playerR;
			Start = 0;
			Start2 = 0;
			countS = 400;
			player1.win = 0;
			player2.win = 0;
			player1.hp = 300;
			player2.hp = 300;
			player1.x = 200;
			player1.y = 400;
			player2.x = 800;
			player2.y = 400;
			time2 = 3600;
			//
		}
		char str12[128] = { 0 };
		sprintf_s(str12, "%d,%f", rival_number, value_T[rival_number]);
		OutputDebugString(str12);
		OutputDebugString("\n");
	}
	*/
	if ((double)wincount1 / (double)(losecount1 + wincount1) < 0.80) {//初期の学習の集団決定
		learn = 1;
	}
	else {
		learn = 1;
	}
	wincount1 = 0;
	losecount1 = 0;
	drawcount = 0;
	while (1) {
		while (learn == 1) {//集団の学習
			//プログラム上の初期化

			for (i = 0; i < CHILD + PARENT; i++) {
				parent[i] = 0;
				c_number[i] = i;
			}
			for (i = 0; i < KO; i++) {
				value_c[i] = 0;
				value_w[i] = 0;
				win_count_c[i] = 0;
			}
			for (i = 0; i < I1; i++) {
				for (j = 0; j < J1; j++) {
					w1_g[i][j] = 0;
					for (c = 0; c < CHILD + PARENT; c++) {
						w1_c[c][i][j] = 0;
					}
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J2; j++) {
					w2_g[i][j] = 0;
					for (c = 0; c < CHILD + PARENT; c++) {
						w2_c[c][i][j] = 0;
					}
				}
			}
			for (i = 0; i < I2; i++) {
				for (j = 0; j < J1; j++) {
					w3_g[i][j] = 0;
					for (c = 0; c < CHILD + PARENT; c++) {
						w3_c[c][i][j] = 0;
					}
				}
			}

			if (flag == 0) {
				//相手集団の最良個体を選択
				
				for (i = 0; i < K; i++) {
					good_rival[i] = cluster_in[i][0];
					for (j = 0; j < cluster_in_number[i]; j++) {
						if (value_T[good_rival[i]] < value_T[cluster_in[i][j]]) {
							good_rival[i] = cluster_in[i][j];

						}
					}
				}
				
				for (k = 0; k < K; k++) {
					good_rival[i] = k;
				}
				/*
				for (i = 0; i < KO; i++) {
					c_good_number[i] = i;
				}
				QSort(value, c_good_number, 0, KO - 1);
				for (c = 0; c < KO / 2; c++) {
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_GA_c[c][i][j] = w1_GA[c_good_number[c]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_GA_c[c][i][j] = w2_GA[c_good_number[c]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_GA_c[c][i][j] = w3_GA[c_good_number[c]][i][j];
						}
					}
				}
				for (c = 0; c < KO / 2; c++) {
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_GA[c][i][j] = w1_GA_c[c][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_GA[c][i][j] = w2_GA_c[c][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_GA[c][i][j] = w3_GA_c[c][i][j];
						}
					}
				}
				for (c = KO / 2; c < KO; c++) {
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_GA[c][i][j] = rand() % 20 - 10;;
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_GA[c][i][j] = rand() % 20 - 10;;
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_GA[c][i][j] = rand() % 20 - 10;;
						}
					}
				}
				*/
				//評価値の初期化
				for (i = 0; i < KO; i++) {
					value[i] = 0;
					win_count[i] = 0;
				}
				flag = 1;
			}
			//親個体選択
			for (i = 0; i < PARENT; i++) {
				do {
					judge = 0;
					parent[i] = rand() % KO;

					for (j = 0; j < i; j++) {
						if (parent[i - (j + 1)] == parent[i]) {
							judge = 1;
						}
					}
				} while (judge == 1);
			}
			//重心計算
			for (number = 0; number < PARENT; number++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_g[i][j] += w1_GA[parent[number]][i][j] / PARENT;
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_g[i][j] += w2_GA[parent[number]][i][j] / PARENT;
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						w3_g[i][j] += w3_GA[parent[number]][i][j] / PARENT;
					}
				}
			}
			//親個体挿入
			for (number = CHILD; number < PARENT + CHILD; number++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_c[number][i][j] = w1_GA[parent[number - CHILD]][i][j];

					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_c[number][i][j] = w2_GA[parent[number - CHILD]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						w3_c[number][i][j] = w3_GA[parent[number - CHILD]][i][j];
					}
				}
			}
			//交叉REX
			for (number = 0; number < CHILD; number++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						for (c = 0; c < PARENT; c++) {
							//w1_c[number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w1_GA[parent[c]][i][j] - w1_g[i][j]);
							w1_c[number][i][j] += (( ( (double)rand() / RAND_MAX) * 2 - 1) / PARENT)
								*(w1_GA[parent[c]][i][j] - w1_g[i][j]);
						}
						//w1_c[number][i][j] += w1_g[i][j];
						w1_c[number][i][j] += w1_GA[parent[0]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						for (c = 0; c < PARENT; c++) {
							//w2_c[number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w2_GA[parent[c]][i][j] - w2_g[i][j]);
							w2_c[number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / PARENT) 
								* (w2_GA[parent[c]][i][j] - w2_g[i][j]);
						}
						//w2_c[number][i][j] += w2_g[i][j];
						w2_c[number][i][j] += w2_GA[parent[0]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						for (c = 0; c < PARENT; c++) {
							//w3_c[number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w3_GA[parent[c]][i][j] - w3_g[i][j]);
							w3_c[number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / PARENT) 
								* (w3_GA[parent[c]][i][j] - w3_g[i][j]);
						}
						//w3_c[number][i][j] += w3_g[i][j];
						w3_c[number][i][j] += w3_GA[parent[0]][i][j];
					}
				}
			}
			//対戦
			int aa = 0;
			if (f_name >= LLL) {
				aa = LLL;
			}
			else {
				aa = f_name;
			}
			for (rival_number = 0; rival_number < K+aa; rival_number++) {//相手ループ
				if (rival_number >= K&&f_name != 0) {
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_T[i][j] = w1_G[0][i][j];
						}
					}

					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_T[i][j] = w2_G[0][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_T[i][j] = w3_G[0][i][j];
						}
					}

				}
				else if(rival_number < K) {
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_T[i][j] = w1_GA_T[good_rival[rival_number]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_T[i][j] = w2_GA_T[good_rival[rival_number]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_T[i][j] = w3_GA_T[good_rival[rival_number]][i][j];
						}
					}
				}
				for (number = 0; number < CHILD + PARENT; number++) {//集団ループ
					if (number >= CHILD&&win_count[parent[number - CHILD]] == 1) {
						value_c[number] = value[parent[number - CHILD]];
						win_count_c[number] = 1;

					}
					else {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								w1[i][j] = w1_c[number][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								w2[i][j] = w2_c[number][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								w3[i][j] = w3_c[number][i][j];
							}
						}

						Competition();//対戦
						value_c[number] += (player1.hp - player2.hp) / 300.0;//集団評価
						if (player1.win == 1) {
							wincount1++;
							//value_c[number] += (player1.hp - player2.hp) / 300.0;//集団評価

							value_w[number]++;
							//if (value_w[number] >(K + aa)/2) {
							//	win_count_c[number] = 1;
							//}
							if (value_w[number] == K + aa) {
								value_c[number] += 100;
								flag_s = 1;
								win_count_c[number] = 1;
							}
							value_c[number] += 10;
						}
						else if (player2.win == 1) {
							losecount1++;
							/*
							char str12[128] = { 0 };
							sprintf_s(str12, "%d", rival_number + aa);
							OutputDebugString(str12);
							*/
						}
						else {
							drawcount++;
						}
					}
				}
			}
			QSort(value_c, c_number, 0, CHILD + PARENT - 1);

			//個体を加える
			for (number = 0; number < PARENT; number++) {//集団ループ
				value[parent[number]] = value_c[number];
				win_count[parent[number]] = win_count_c[c_number[number]];
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_GA[parent[number]][i][j] = w1_c[c_number[number]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_GA[parent[number]][i][j] = w2_c[c_number[number]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						w3_GA[parent[number]][i][j] = w3_c[c_number[number]][i][j];
					}
				}
			}
			//勝率判定
			if (flag_s == 1) {
				flag_ss++;
			}
			int ccc = 0;
			for (i = 0; i < KO; i++) {
				ccc += win_count[i];
			}
			//if ((double)wincount1 / (double)(wincount1 + losecount1+drawcount) > 0.70||flag_s == 1) {
			if (ccc >= KO / 2) {
				if (v_count == COUNT) {
					/*
					char str12[128] = { 0 };
					sprintf_s(str12, "%d:%3f:%d:%d", learn, (double)wincount1 / (double)(wincount1 + losecount1), wincount1, losecount1);
					OutputDebugString(str12);
					OutputDebugString("\n");
					*/
					flag = 0;
					learn = 2;
					wincount1 = 0;
					losecount1 = 0;
					drawcount = 0;
					v_count = 0;
					flag_s = 0;
					flag_ss = 0;
				}
				else {
					v_count++;
				}
			}
			else {
				v_count = 0;
			}
			if (endflag == 1) {
				generation++;
				if (generation == 1000) {
					exit(0);
				}
			}
			char str12[128] = { 0 };
			//sprintf_s(str12, "%d:%3f:%d:%d:%3.2f:%d", learn, (double)wincount1 / (double)(wincount1 + losecount1), wincount1, losecount1,value_c[0],value_w[c_number[0]]);
			sprintf_s(str12, "%2.0f:%.1f:%.1f", (double)ccc/(double)KO*100,value_c[0], value_c[c_number[CHILD]]);
			OutputDebugString(str12);
			OutputDebugString("\n");
			wincount1 = 0;
			losecount1 = 0;
			drawcount = 0;
		}

		//相手集団の学習
		while (learn == 2) {
			int flag_s = 0;
			for (k = 0; k < K; k++) {
				for (i = 0; i < CHILD + PARENT; i++) {
					c_number_T[k][i] = i;
					parent_T[k][i] = 0;
				}
				for (i = 0; i < KOT; i++) {
					value_c_T[k][i] = 0;
					value_w_T[k][i] = 0;
					win_count_c_T[k][i] = 0;
					w_distance[i] = 0;
				}
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1_g_T[k][i][j] = 0;
						for (c = 0; c < CHILD + PARENT; c++) {
							w1_c_T[k][c][i][j] = 0;
						}
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2_g_T[k][i][j] = 0;
						for (c = 0; c < CHILD + PARENT; c++) {
							w2_c_T[k][c][i][j] = 0;
						}
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						w3_g_T[k][i][j] = 0;
						for (c = 0; c < CHILD + PARENT; c++) {
							w3_c_T[k][c][i][j] = 0;
						}
					}
				}
			}
			if (flag == 0) {
				//評価値の初期化
				for (i = 0; i < KOT; i++) {
					value_T[i] = 0;
					win_count_T[i] = 0;
				}
				//集団の最良個体を選択
				for (i = 0; i < KO; i++) {
					c_good_number[i] = i;
				}

				QSort(value, c_good_number, 0, KO - 1);

				for (i = 0; i < RIVAL; i++) {
					good_rival_main[i] = c_good_number[i];
				}
				char str12[128] = { 0 };
				sprintf_s(str12, "afefae%d", good_rival_main[0]);
				OutputDebugString(str12);
				OutputDebugString("\n");

				sprintf(filename, "AIC/%d.dat", f_name);
				if ((fp = fopen(filename, "wb+")) == NULL) {
					fprintf(stderr, "%s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				
				fwrite(w1_GA[good_rival_main[0]], sizeof(double), I1*J1, fp);
				fwrite(w2_GA[good_rival_main[0]], sizeof(double), I2*J2, fp);
				fwrite(w3_GA[good_rival_main[0]], sizeof(double), I2*J1, fp);
				
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						//fprintf(fp, "%lf", w1_GA[good_rival_main[0]][i][j]);
						//fprintf(fp, "\n");
						w1_G[f_name%LLL][i][j] = w1_GA[good_rival_main[0]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						//fprintf(fp, "%lf", w2_GA[good_rival_main[0]][i][j]);
						//fprintf(fp, "\n");
						w2_G[f_name%LLL][i][j] = w2_GA[good_rival_main[0]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						//fprintf(fp, "%lf", w3_GA[good_rival_main[0]][i][j]);
					//	fprintf(fp, "\n");
						w3_G[f_name%LLL][i][j] = w3_GA[good_rival_main[0]][i][j];
					}
				}
				
				fclose(fp);
				f_name++;
				flag = 1;
			}
			//親個体選択
			for (k = 0; k < K; k++) {
				for (i = 0; i < PARENT&& i < cluster_in_number[k]; i++) {
					do {
						judge = 0;
						parent_T[k][i] = rand() % (cluster_in_number[k]);
						for (j = 0; j < i; j++) {
							if (parent_T[k][i - (j + 1)] == parent_T[k][i]) {
								judge = 1;
							}
						}
					} while (judge == 1);
				}
			}
			//重心計算
			for (k = 0; k < K; k++) {
				for (number = 0; number < PARENT&&number < cluster_in_number[k]; number++) {
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_g_T[k][i][j] += w1_GA_T[cluster_in[k][parent_T[k][number]]][i][j] / PARENT;
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_g_T[k][i][j] += w2_GA_T[cluster_in[k][parent_T[k][number]]][i][j] / PARENT;
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_g_T[k][i][j] += w3_GA_T[cluster_in[k][parent_T[k][number]]][i][j] / PARENT;
						}
					}
				}
			}
			//親個体を挿入
			for (k = 0; k < K; k++) {
				for (number = CHILD; number < PARENT + CHILD&& number < CHILD + cluster_in_number[k]; number++) {
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_c_T[k][number][i][j] = w1_GA_T[cluster_in[k][parent_T[k][number - CHILD]]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_c_T[k][number][i][j] = w2_GA_T[cluster_in[k][parent_T[k][number - CHILD]]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_c_T[k][number][i][j] = w3_GA_T[cluster_in[k][parent_T[k][number - CHILD]]][i][j];
						}
					}
				}

			}
			//交叉REX
			for (k = 0; k < K; k++) {
				for (number = 0; number < CHILD; number++) {
					if (cluster_in_number[k] + 1 == PARENT) {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								for (c = 0; c < PARENT; c++) {
									//w1_c_T[k][number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w1_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w1_g_T[k][i][j]);
									w1_c_T[k][number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / PARENT)*2*(w1_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w1_g_T[k][i][j]);
								}
								w1_c_T[k][number][i][j] += w1_g_T[k][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								for (c = 0; c < PARENT; c++) {
									//w2_c_T[k][number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w2_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w2_g_T[k][i][j]);
									w2_c_T[k][number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / PARENT) *2*(w2_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w2_g_T[k][i][j]);
								}
								w2_c_T[k][number][i][j] += w2_g_T[k][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								for (c = 0; c < PARENT; c++) {
									//w2_c_T[k][number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w2_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w2_g_T[k][i][j]);
									w3_c_T[k][number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / PARENT) *2*(w3_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w3_g_T[k][i][j]);
								}
								w3_c_T[k][number][i][j] += w3_g_T[k][i][j];
							}
						}
					}
					else {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								for (c = 0; c < cluster_in_number[k] ; c++) {
									//w1_c_T[k][number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w1_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w1_g_T[k][i][j]);
									w1_c_T[k][number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / cluster_in_number[k] )*2*(w1_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w1_g_T[k][i][j]);
								}
								w1_c_T[k][number][i][j] += w1_g_T[k][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								for (c = 0; c < cluster_in_number[k] ; c++) {
									//w2_c_T[k][number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w2_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w2_g_T[k][i][j]);
									w2_c_T[k][number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / cluster_in_number[k] ) *2*(w2_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w2_g_T[k][i][j]);
								}
								w2_c_T[k][number][i][j] += w2_g_T[k][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								for (c = 0; c < cluster_in_number[k] ; c++) {
									//w2_c_T[k][number][i][j] += rand_normal(0.0, 1.0 / (PARENT))*(w2_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w2_g_T[k][i][j]);
									w3_c_T[k][number][i][j] += ((((double)rand() / RAND_MAX) * 2 - 1) / cluster_in_number[k] ) *2*(w3_GA_T[cluster_in[k][parent_T[k][c]]][i][j] - w3_g_T[k][i][j]);
								}
								w3_c_T[k][number][i][j] += w3_g_T[k][i][j];
							}
						}
					}
				}
			}
			//対戦
			for (c = 0; c < RIVAL; c++) {
				for (i = 0; i < I1; i++) {
					for (j = 0; j < J1; j++) {
						w1[i][j] = w1_GA[good_rival_main[c]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J2; j++) {
						w2[i][j] = w2_GA[good_rival_main[c]][i][j];
					}
				}
				for (i = 0; i < I2; i++) {
					for (j = 0; j < J1; j++) {
						w3[i][j] = w3_GA[good_rival_main[c]][i][j];
					}
				}
				for (k = 0; k < K; k++) {
					for (number = 0; number < CHILD + PARENT&& number < CHILD + cluster_in_number[k] + 1; number++) {//対戦相手集団ループ
						if (number >= CHILD&&win_count_T[cluster_in[k][parent_T[k][number - CHILD]]] == 1) {
							value_c_T[k][number] = value_T[cluster_in[k][parent_T[k][number - CHILD]]];
							win_count_c_T[k][number] = 1;
						}
						else {
							for (i = 0; i < I1; i++) {
								for (j = 0; j < J1; j++) {
									w1_T[i][j] = w1_c_T[k][number][i][j];
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J2; j++) {
									w2_T[i][j] = w2_c_T[k][number][i][j];
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J1; j++) {
									w3_T[i][j] = w3_c_T[k][number][i][j];
								}
							}
							Competition();//対戦
							value_c_T[k][number] += (player2.hp - player1.hp) / 300.0;//集団評価
							if (player2.win == 1) {
								//   value_c_T[k][number] += (player2.hp - player1.hp) / 300.0;//集団評価
								value_w_T[k][number]++;
								if (value_w_T[k][number] == RIVAL) {
									//value_c_T[k][number] += RIVAL;
								}
								wincount1++;
								wincount[k]++;
								flag_sl[k] = 1;
								win_count_c_T[k][number] = 1;
							}
							else if (player1.win == 1) {
								losecount1++;
								losecount[k]++;
							}
							else {
								value_w_T[k][number]--;
								if (value_w_T[k][number] == -RIVAL) {
									//value_c_T[k][number] -= RIVAL;
								}
								drawcount++;
								drawcount2[k]++;
								win_count_c_T[k][number] = 1;
								flag_sl[k] = 1;
							}
						}
					}
				}
			}
			for (k = 0; k < K; k++) {
				if (cluster_in_number[k] < PARENT) {
					QSort(value_c_T[k], c_number_T[k], 0, CHILD + cluster_in_number[k] - 1);
				}
				else {
					QSort(value_c_T[k], c_number_T[k], 0, CHILD + PARENT - 1);
				}
				char str12[128] = { 0 };
				sprintf_s(str12, "%d:%3.3f:%3.3f:%3.3f,%3.3f,%3.3f:%d",
					k, value_c_T[k][0], value_c_T[k][1], value_c_T[k][2], value_c_T[k][3], value_c_T[k][4], cluster_in_number[k]);
				OutputDebugString(str12);
				OutputDebugString("\n");
			}
			//個体を加える
			for (k = 0; k < K; k++) {
				for (number = 0; number < PARENT && number < cluster_in_number[k]; number++) {//集団ループ
					value_T[cluster_in[k][parent_T[k][number]]] = value_c_T[k][number];
					win_count_T[cluster_in[k][parent_T[k][number]]] = win_count_c_T[k][c_number_T[k][number]];
					for (i = 0; i < I1; i++) {
						for (j = 0; j < J1; j++) {
							w1_GA_T[cluster_in[k][parent_T[k][number]]][i][j] = w1_c_T[k][c_number_T[k][number]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J2; j++) {
							w2_GA_T[cluster_in[k][parent_T[k][number]]][i][j] = w2_c_T[k][c_number_T[k][number]][i][j];
						}
					}
					for (i = 0; i < I2; i++) {
						for (j = 0; j < J1; j++) {
							w3_GA_T[cluster_in[k][parent_T[k][number]]][i][j] = w3_c_T[k][c_number_T[k][number]][i][j];
						}
					}
				}
			}
			double par = 0;
			for (k = 0; k < K; k++) {
				par += (double)wincount[k] / (double)(wincount[k] + losecount[k]);
			}
			for (k = 0; k < K; k++) {
				if (flag_sl[k] == 1) {
					flag_s++;
				}
			}
			int aaa = 0;
			if (generation <= 5) {
				aaa = 4;
			}
			else if (generation > 5&& generation <= 7) {
				aaa = 8;
			}
			else if (generation > 7) {
				aaa = 16;
			}
			//if ((par/K > 0.50)||flag_s >= K*4/aaa) {
			int ccca = 0;
			for (i = 0; i < KOT; i++) {
				ccca += win_count_T[i];
			}
			if (ccca>=KOT/2) {
				for (k = 0; k < K; k++) {
					flag_sl[k] = 0;
				}
				if (v_count == COUNT_T) {
					flag = 0;
					for (k = 0; k < K; k++) {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								w1_k[k][i][j] = 0;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								w2_k[k][i][j] = 0;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								w3_k[k][i][j] = 0;
							}
						}
						
						for (c = 0; c < cluster_in_number[k]; c++) {
							for (i = 0; i < I1; i++) {
								for (j = 0; j < J1; j++) {
									w1_k[k][i][j] += w1_GA_T[cluster_in[k][c]][i][j] / (double)(cluster_in_number[k]);
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J2; j++) {
									w2_k[k][i][j] += w2_GA_T[cluster_in[k][c]][i][j] / (double)(cluster_in_number[k]);
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J1; j++) {
									w3_k[k][i][j] += w3_GA_T[cluster_in[k][c]][i][j] / (double)(cluster_in_number[k]);
								}
							}
						}
						
					}
					/*
					for (i = 0; i < K; i++) {
						good_rival[i] = cluster_in[i][0];
						for (j = 0; j < cluster_in_number[i]; j++) {
							if (value_T[good_rival[i]] < value_T[cluster_in[i][j]]) {
								good_rival[i] = cluster_in[i][j];

							}
						}
					}
					
					for (k = 0; k < K; k++) {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								w1_k[k][i][j] = w1_GA_T[good_rival[k]][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								w2_k[k][i][j] = w2_GA_T[good_rival[k]][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								w3_k[k][i][j] = w3_GA_T[good_rival[k]][i][j];
							}
						}
					}
					
					for (c = 0; c < KOT; c++) {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								w1_GA_T[c][i][j] = rand() % 20 - 10;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								w2_GA_T[c][i][j] = rand() % 20 - 10;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								w3_GA_T[c][i][j] = rand() % 20 - 10;
							}
						}
					}
					
					for (k = 0; k < K; k++) {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								w1_GA_T[k][i][j] = w1_k[k][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								w2_GA_T[k][i][j] = w2_k[k][i][j];
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								w3_GA_T[k][i][j] = w3_k[k][i][j];
							}
						}
					}
					for (k = 0; k < K; k++) {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								w1_k[k][i][j] = rand() % 20 - 10;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								w2_k[k][i][j] = rand() % 20 - 10;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								w3_k[k][i][j] = rand() % 20 - 10;
							}
						}
					}
					*/
					for (k = 0; k < K; k++) {
						for (c = 0; c < KOT; c++) {
							for (i = 0; i < I1; i++) {
								for (j = 0; j < J1; j++) {
									distance += pow(w1_k[k][i][j] - w1_GA_T[c][i][j], 2);
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J2; j++) {
									distance += pow(w2_k[k][i][j] - w2_GA_T[c][i][j], 2);
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J1; j++) {
									distance += pow(w3_k[k][i][j] - w3_GA_T[c][i][j], 2);
								}
							}
							if (distance <= w_distance[c] || k == 0) {
								w_distance[c] = distance;
								w_cluster[c] = k;
							}
							distance = 0;
						}
					}
					for (k = 0; k < K; k++) {
						cluster_in_number[k] = 0;
					}
					for (c = 0; c < KOT; c++) {
						cluster_in[w_cluster[c]][cluster_in_number[w_cluster[c]]] = c;
						cluster_in_number[w_cluster[c]]++;
					}
					for (k = 0; k < K; k++) {
						if (cluster_in_number[k] < PARENT) {
							OutputDebugString("error");
						}
					}
					/*
					char str12[128] = { 0 };
					for (k = 0; k < K; k++) {
						sprintf_s(str12, "%.0f:", (double)wincount[k] / (double)(wincount[k] + losecount[k])*100);
						OutputDebugString(str12);
					}
					OutputDebugString("\n");
					sprintf_s(str12, "%.0f", par * 100 / K);
					OutputDebugString(str12);
					OutputDebugString("\n");
					*/
					flag = 0;
					learn = 1;
					wincount1 = 0;
					losecount1 = 0;
					drawcount = 0;
					for (k = 0; k < K; k++) {
						wincount[k] = 0;
						losecount[k] = 0;
						drawcount2[k] = 0;
					}
					generation = 0;
					v_count = 0;
				}
				else {
					v_count++;
				}
			}
			else {
				generation++;
				v_count = 0;
				if (generation == 1000) {
					learn = 1;
					generation = 0;
				}
				else if (generation + 1 % 5000 == 0) {
					for (k = 0; k < K; k++) {
						for (i = 0; i < I1; i++) {
							for (j = 0; j < J1; j++) {
								w1_k[k][i][j] = 0;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J2; j++) {
								w2_k[k][i][j] = 0;
							}
						}
						for (i = 0; i < I2; i++) {
							for (j = 0; j < J1; j++) {
								w3_k[k][i][j] = 0;
							}
						}
						for (c = 0; c < cluster_in_number[k]; c++) {
							for (i = 0; i < I1; i++) {
								for (j = 0; j < J1; j++) {
									w1_k[k][i][j] += w1_GA_T[cluster_in[k][c]][i][j] / (double)(cluster_in_number[k]);
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J2; j++) {
									w2_k[k][i][j] += w2_GA_T[cluster_in[k][c]][i][j] / (double)(cluster_in_number[k]);
								}
							}
							for (i = 0; i < I2; i++) {
								for (j = 0; j < J1; j++) {
									w3_k[k][i][j] += w3_GA_T[cluster_in[k][c]][i][j] / (double)(cluster_in_number[k]);
								}
							}
						}
					}
				}
			}
			char str12[128] = { 0 };
			/*
			for (k = 0; k < K; k++) {
				sprintf_s(str12, "%.0f", (double)wincount[k] / (double)(wincount[k] + losecount[k]) * 100);
				OutputDebugString(str12);
				OutputDebugString(":");
			}
			OutputDebugString(":::");
			sprintf_s(str12, "%.0f", par*100/K);
			*/
			sprintf_s(str12, "%2.0f", (double)ccca / (double)KOT * 100);
			OutputDebugString(str12);
			OutputDebugString("\n");
			

			wincount1 = 0;
			losecount1 = 0;
			drawcount = 0;
			for (k = 0; k < K; k++) {
				wincount[k] = 0;
				losecount[k] = 0;
				drawcount2[k] = 0;
			}
		}
	}
}
void Competition(void) {
	for (int i = 0; i < J1; i++) {
		u2_c[i] = 0;
		u2_c_T[i] = 0;
	}
	player1 = playerR;
	player2 = playerR;
	Start = 0;
	Start2 = 0;
	countS = 400;
	player1.win = 0;
	player2.win = 0;
	player1.hp = 300;
	player2.hp = 300;
	player1.x = 200;
	player1.y = 400;
	player2.x = 800;
	player2.y = 400;
	time2 = 3600;
	Start = 1;
	while (Start2 == 0) {
		TimeGA();
		move(&player1, &player2);
		move(&player2, &player1);
		deaa = 0;
		attack(&player1, &player2);
		attack(&player2, &player1);
		hit(&player1);
		hit(&player2);
		direction();
		statefix(&player1);
		statefix(&player2);
		win(player1.hp, player2.hp);
		if (Start2 == 0) {
			player1s = player1;
			player2s = player2;
			NNR1P();
			NNR2P();
		}
	}
}
double rand_normal(double mu, double sigma) {
	double z = sqrt(-2.0*log(Uniform())) * sin(2.0*M_PI*Uniform());
	return mu + sigma*z;
}
double Uniform(void) {
	static int x = 10;
	int a = 1103515245, b = 12345, c = 2147483647;
	x = (a*x + b)&c;

	return ((double)x + 1.0) / ((double)c + 2.0);
}
/* クイックソートを行う */
void QSort(double x[],int y[],int left, int right)
{
	int i, j;
	double pivot;

	i = left;                      /* ソートする配列の一番小さい要素の添字 */
	j = right;                     /* ソートする配列の一番大きい要素の添字 */

	pivot = x[(left + right) / 2]; /* 基準値を配列の中央付近にとる */

	while (1) {                    /* 無限ループ */

		while (x[i] > pivot)       /* pivot より大きい値が */
			i++;                   /* 出るまで i を増加させる */

		while (pivot > x[j])       /* pivot より小さい値が */
			j--;                   /*  出るまで j を減少させる */
		if (i >= j)                /* i >= j なら */
			break;                 /* 無限ループから抜ける */

		Swap(x,y, i, j);             /* x[i] と x[j]を交換 */
		i++;                       /* 次のデータ */
		j--;
	}
	if (left < i - 1)              /* 基準値の左に 2 以上要素があれば */
		QSort(x,y, left, i - 1);     /* 左の配列を Q ソートする */
	if (j + 1 <  right)            /* 基準値の右に 2 以上要素があれば */
		QSort(x,y, j + 1, right);    /* 右の配列を Q ソートする */
}

/* 配列の要素を交換する */
void Swap(double x[],int y[], int i, int j)
{
	double temp;
	int temp_n;

	temp = x[i];
	x[i] = x[j];
	x[j] = temp;

	temp_n = y[i];
	y[i] = y[j];
	y[j] = temp_n;
}
void ALL(void) {
	int ai1, ai2;
	int ai_w[AICOUNT] = { 0 }, ai_l[AICOUNT] = { 0 }, ai_d[AICOUNT] = {0};
	int dre[AICOUNT] = { 0 };

	for (ai1 = 0; ai1 < AICOUNT; ai1++) {
		sprintf(filename, "AI/%d.dat", ai1);
		if ((file = fopen(filename, "rb")) == NULL) {
			printf("file open error!!\n");
			exit(0);
		}

		fread(w1, sizeof(double), I1 * J1, file);
		fread(w2, sizeof(double), I2 * J2, file);
		fread(w3, sizeof(double), I2 * J1, file);

	
		fclose(file);

		for (ai2 = 0; ai2 < AICOUNT; ai2++) {
			sprintf(filename, "AI/%d.dat", ai2);
			if ((fp = fopen(filename, "rb")) == NULL) {
				printf("file open error!!\n");
				exit(0);
			}
			fread(w1_T, sizeof(double), I1 * J1, fp);
			fread(w2_T, sizeof(double), I2 * J2, fp);
			fread(w3_T, sizeof(double), I2 * J1, fp);
		
			fclose(fp);

			Competition();

			if (player1.win == 1) {
				ai_w[ai1]++;
				//ai_l[ai2]++;
			}
			else if (player2.win == 1) {
				ai_l[ai1]++;
				//ai_w[ai2]++;
				dre[ai1] = ai2;
			}
			else {
				ai_d[ai1]++;
				//ai_d[ai2]++;
			}
		}
		char str12[128] = { 0 };
		sprintf_s(str12, "%d", ai1);
		OutputDebugString(str12);
		OutputDebugString("\n");
	}
	for (ai1 = 0; ai1 < AICOUNT; ai1++) {
		char str12[128] = { 0 };
		sprintf_s(str12, "%d:%d勝,%d敗,%d引き分け:率%2f:%d",ai1,ai_w[ai1],ai_l[ai1],ai_d[ai1],(double)ai_w[ai1]/(double)(ai_w[ai1]+ ai_l[ai1]+ ai_d[ai1])*100,dre[ai1]);
		OutputDebugString(str12);
		OutputDebugString("\n");
	}
	exit(0);
}
void playeraa(void) {
	//GetHitKeyStateAll(Buf);
}
