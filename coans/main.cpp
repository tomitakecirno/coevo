#include "../header/coansmodule.hpp"
#include "../header/Floreano.h"

using namespace std;

int coans(int mode, int trial);
/****************************
���C�����[�`��
***************************/
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int battle_count[4][TRIAL] = { 0 };
	//�v���C
	if (DE == 1) {
		gamemode();
	}
	//??
	else if (DE == 0) {
		FloreanoEval();
	}
	//�w�K
	else if (DE == 2) {
		for (int t = 0; t < TRIAL; t++) {
			for (int m = 0; m < 1; m++) {
				battle_count[m][t] = coans(m, t);
				cout << battle_count[m][t] << " ";
			}
			//mode:0 ����@�F���]�����@(���������Ń|�C���g��^���Ȃ�)
			//mode:1 ����@�F�O�]�����@(���������Ń|�C���g��^����)
			//mode:2 �O��@�F���]�����@
			//mode:3 �O��@�F�O�]�����@
		}
		exit(0);
	}
	else if (DE == 3) {
		exit(0);
	}
	else if (DE == 4) {
		for (int t = 0; t < TRIAL; t++) {
			cout << "���s��:" << t << endl;
			for (int m = 0; m < 4; m++) {
				ALL(m, 4, t);
			}
		}
	}
	else if (DE == 5) {
		for (int t = 0; t < TRIAL; t++) {
			for (int m = 0; m < 4; m++) {
				ALLWatch(m, t);
			}
		}
	}
	WaitKey();		// �L�[���͑҂�
	DxLib_End();			// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}