#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
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
		FloreMethods Flore_1;
		for (int i = 0; i < TRIAL; i++) {
			Flore_1.Main_Tasks();
			Flore_1.Fwrite_Floreano();
			if (i == TRIAL) {
				break;
			}
		}
	}
	//�w�K�ŋL�^�����f�[�^�����ƂɎ��ۂɑΐ�
	else if (DE == 1) {
		gamemode();
	}
	//���C���W�c�w�K
	else if (DE == 2) {
		std::vector<int> MatchUp_Count(TRIAL);
		CoansMode1 Mode1;
		for (int t = 0; t < TRIAL; t++) {
			std::cout << "trial:" << t << "  ";
			clock_t start = clock();
			Mode1.Coans_Tasks(t);
			clock_t end = clock();
			Mode1.File_Write_Pop(t, true);
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
