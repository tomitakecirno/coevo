#include "../header/coansmodule.hpp"
#include "../header/CoansMethods.h"
#include "../header/Floreano.h"
#include "../header/config.hpp"

using namespace std;

/****************************
���C�����[�`��
DE
0:�ΐ푊��w�K
1:�v���C���[VS�v���C���[
2:�d�݊w�K	
***************************/
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

	}
	//���C���W�c�w�K
	else if (DE == 2) {

	}
	CoansMode1 Mode1;
	switch(DE) {
	case 0:
		break;
	case 1:
		gamemode();
		break;
	//�w�K
	case 2:
		for (int t = 0; t < TRIAL; t++) {
			Mode1.Coans_Tasks(t);
			cout << "�ΐ��:" << Mode1.Get_MatchUp_Num() << endl;
		}
		break;
	}
	//�v���C
	WaitKey();		// �L�[���͑҂�
	DxLib_End();			// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}