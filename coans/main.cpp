#pragma once
#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
#include "../header/MatchUpMethod.h"
#include "../header/Floreano.h"
#include "../header/config.hpp"
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <cstring>

using namespace std;

/****************************
���C�����[�`��
DE
0:�ΐ푊��w�K
1:�v���C���[VS�v���C���[
2:ANS�N���X�^�����O��@�w�K
3:�K�w�I�N���X�^�����O��@
***************************/
void MatchUp_CSV(std::vector<int> &Count);
void Show_Time(clock_t Start, clock_t End);

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Main_Mode;
	int Main_Method;
	int Main_Trial;
	int Main_K;
	if (__argc < 3) {
		cout << "not enough Parameter" << endl;
		exit(1);
	}else if(__argc == 4) {
		Main_Mode = atoi(__argv[1]);
		Main_Method = atoi(__argv[2]);
		Main_Trial = atoi(__argv[3]);
		Main_K = 0;
	}
	else if (__argc == 5) {
		Main_Mode = atoi(__argv[1]);
		Main_Method = atoi(__argv[2]);
		Main_Trial = atoi(__argv[3]);
		Main_K = atoi(__argv[4]);
	}

	/*	
		argv[1]:Method
		argv[2]:Trial
		argv[3]:k
	*/
	cout << "���[�h:" << Main_Mode << endl;
	cout << "��@:" << Main_Method << endl;
	cout << "���s��:" << Main_Trial << endl;
	cout << "�N���X�^��:" << Main_K << endl;
	cout << "���㐔:" << KU << endl;
	cout << "��؂�:" << PER << endl;

	int MatchUp_Count;

	clock_t Start_Main = clock();
	//�����p�ΐ푊��w�K
	if (Main_Mode == 0) {
		Make_Directory_AIT(0,TRIAL);
		FloreMethods Flore_1;
		for (int t = 4; t < TRIAL; t++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano(t);
		}
	}
	//�w�K�ŋL�^�����f�[�^�����ƂɎ��ۂɑΐ�
	else if (Main_Mode == 1) {
		Match Match_0;
		Make_CSV_Directory(Main_Method);
		Match_0.Init_Parameter(Main_Method, 0, 50, 30, KU, PER, Main_K);

		for (int Opp_t = 0; Opp_t < F_TRIAL; Opp_t++) {
			//����@vsFloreano ���㐔:2000(100����Ԋu) ����@�W�c50 Floreano�W�c30
			Match_0.Match_And_SetDATA(Main_Trial, Opp_t);
			Match_0.File_Write_CSV(Main_Trial, Opp_t);
		}
		std::cout << "Process time:" << time << std::endl;
		Match_0.Decide_Best(Main_Trial);
	}
	//����@�w�K
	else if (Main_Mode == 2) {
		//����@
		if (Main_Method == 0) {
			CoansMode0 Mode0("AI", Main_Method);
			Mode0.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode0.Get_MatchUp_Num();
		}
		//�K�w�I�N���X�^�����O�𐷂荞�񂾎�@
		else if (Main_Method == 1) {
			CoansMode1 Mode1("TEST", Main_Method);
			Mode1.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode1.Get_MatchUp_Num();
		}
		//�K�w�I�N���X�^�����O�{List3
		else if (Main_Method == 2) {
			CoansMode2 Mode2("AI", Main_K);
			Mode2.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode2.Get_MatchUp_Num();
		}else if (Main_Method == 3) {
			CoansMode3 Mode3("AI", Main_K);
			Mode3.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode3.Get_MatchUp_Num();
		}
		else if (Main_Method == 4) {
			CoansMode4 Mode4("AI", Main_Method);
			Mode4.Coans_Tasks(Main_Trial);
			MatchUp_Count = Mode4.Get_MatchUp_Num();
		}
	}
	clock_t End_Main = clock();
	Show_Time(Start_Main, End_Main);
	std::cout << "match_up num:" << MatchUp_Count << std::endl;

	//�v���C
	//WaitKey();		// �L�[���͑҂�
	DxLib_End();			// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}

void Show_Time(clock_t Start, clock_t End) {
	int time = (Start - Start) / CLOCKS_PER_SEC;
	std::cout << "Process time:" << time << "[sec]" << std::endl;
}


void MatchUp_CSV(std::vector<int> &Count) {
	//�t�@�C�����ݒ�
	char fname[30];
	sprintf(fname, "MatchUp_Log.csv");

	//�t�@�C���o�̓X�g���[��
	std::ofstream ofs(fname);
	for (int i = 0; i < TRIAL; i++) {
		ofs << Count[i] << ',';
	}
	ofs << std::endl;
}