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
	CoansMode1 Mode1;
	switch(DE) {
	case 0:
		FloreanoEval();
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