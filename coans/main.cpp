#pragma once
#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
#include "../header/MatchUpMethod.h"
#include "../header/Floreano.h"
#include "../header/config.hpp"
#include <fstream>
#include <time.h>

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
	//�����p�ΐ푊��w�K
	if (DE == 0) {
		Make_Directory_AIT(0,TRIAL);
		FloreMethods Flore_1;
		for (int t = 4; t < TRIAL; t++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano(t);
		}
		exit(0);
	}
	//�w�K�ŋL�^�����f�[�^�����ƂɎ��ۂɑΐ�
	else if (DE == 1) {
		gamemode();
	}
	//���C���W�c�w�K
	else if (DE == 2) {
		std::vector<int> MatchUp_Count(TRIAL);
		CoansMode1 Mode1;
		Make_Directory_AI(0, TRIAL, KU, PER);
		Make_CSV_Directory();
		for (int t = 3; t < TRIAL; t++) {
			std::cout << "trial:" << t << "  ";
			clock_t start = clock();
			Mode1.Coans_Tasks(t);
			clock_t end = clock();
			//Mode1.File_Write_Pop(t, true);
			MatchUp_Count[t] = Mode1.Get_MatchUp_Num();

			int time = (end - start) / CLOCKS_PER_SEC;
			int hour = time / 3600;
			int minute = time / 3600 / 60;
			int second = time % 60;
			std::cout << "Process time:" << oct << showbase << hour << ':';
			std::cout << oct << showbase << minute << ':';
			std::cout << oct << showbase << second << std::endl;
			std::cout << "match_up num:" << MatchUp_Count[t] << std::endl;
		}
		//�ΐ퐔��������
		MatchUp_CSV(MatchUp_Count);
		exit(0);
	}
	//�w�K�ŋL�^�����f�[�^�����Ƃɑΐ�.
	else if (DE == 3) {
		Match Match_1;
		for (int Pop_t = 0; Pop_t < TRIAL; Pop_t++) {
			for (int Opp_t = 0; Opp_t < F_TRIAL; Opp_t++) {
				//����@vsFloreano ���㐔:2000(100����Ԋu) ����@�W�c50 Floreano�W�c30
				Match_1.Init_Parameter(0, 0, 50, 30, 2000, 100);
				Match_1.Match_And_SetDATA(Pop_t, Opp_t);
				Match_1.File_Write_CSV(Pop_t, Opp_t);
			}
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
