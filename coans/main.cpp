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
2:�d�݊w�K	
***************************/
void MatchUp_CSV(std::vector<int> &Count);

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (__argc < 2) {
		cout << "Please Set Trial" << endl;
		exit(0);
	}
	int Main_Mode = atoi(__argv[1]);
	int Main_Trial = atoi(__argv[2]);

	cout << "���[�h:" << Main_Mode << endl;
	cout << "���s��:" << Main_Trial << endl;
	//�����p�ΐ푊��w�K
	if (Main_Mode == 0) {
		Make_Directory_AIT(0,TRIAL);
		FloreMethods Flore_1;
		for (int t = 4; t < TRIAL; t++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano(t);
		}
		exit(0);
	}
	//�w�K�ŋL�^�����f�[�^�����ƂɎ��ۂɑΐ�
	else if (Main_Mode == 1) {
		gamemode();
	}
	//���C���W�c�w�K
	else if (Main_Mode == 2) {
		std::vector<int> MatchUp_Count(Main_Trial);
		CoansMode1 Mode1;
		Make_Directory_AI(0, Main_Trial, KU, PER);
		Make_CSV_Directory();

		clock_t Start_Main = clock();
		Mode1.Coans_Tasks(Main_Trial);
		clock_t End_Main = clock();
		//Mode1.File_Write_Pop(t, true);
		MatchUp_Count[Main_Trial] = Mode1.Get_MatchUp_Num();

		int time = (End_Main - Start_Main) / CLOCKS_PER_SEC;
		std::cout << "Process time:" << time << "[sec]" << std::endl;
		std::cout << "match_up num:" << MatchUp_Count[Main_Trial] << std::endl;

		//�ΐ퐔��������
		MatchUp_CSV(MatchUp_Count);
		exit(0);
	}
	//�w�K�ŋL�^�����f�[�^�����Ƃɑΐ�.
	else if (Main_Mode == 3) {
		Match Match_1;
		for (int Opp_t = 0; Opp_t < F_TRIAL; Opp_t++) {
			//����@vsFloreano ���㐔:2000(100����Ԋu) ����@�W�c50 Floreano�W�c30
			Match_1.Init_Parameter(0, 0, 50, 30, KU, PER);
			Match_1.Match_And_SetDATA(Main_Trial, Opp_t);
			Match_1.File_Write_CSV(Main_Trial, Opp_t);
		}
		exit(0);
	}
	//�v���C
	WaitKey();		// �L�[���͑҂�
	DxLib_End();			// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
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
