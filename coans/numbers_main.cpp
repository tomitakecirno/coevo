#include <iostream>
#include "../header/numbers_main.h"

void RexStar(playerTK &pare, playerTK &child);

int main(){
	return 0;
}


/************
JGG�{REX
�����|�C���g�
************/
void RexStar(std::vector<playerTK> &pare, std::vector<playerTK> &child)
{
	double base_gra[DEM] = { 0 }; /*��ƂȂ�d�S*/
	double sum_n[DEM] = { 0 }; /*x,y���ꂼ��̘a���ꎞ�I�ɕۑ����Ă����z��*/
	/*�x�[�X�ƂȂ�d�S�����߂�*/
	int pare_size = pare.size();
	for (int i = 0; i<pare_size; i++){
		for (int j = 0; j<DEM; j++){
			sum_n[j] += pare[i].n[j]; /*�e��x,y�����ꂼ�ꑫ��*/
		}
	}
	for (int i = 0; i<DEM; i++){
		base_gra[i] = sum_n[i] / pare_size; /*�e�̂��ꂼ��x,y�̘a������*/
	}
	/*�d�S����e�ɑ΂���x�N�g�������߂�*/
	double vector[pare_size][DEM];
	for (int i = 0; i<pare_size; i++){
		for (int j = 0; j<DEM; j++){
			vector[i][j] = pare[i].n[j] - base_gra[j];
		}
	}
	/*�d�S�����Ƃɐe�𕡐�����*/
	playerTK sub_pare[pare_size];

	for (int i = 0; i<pare_size; i++){
		sub_pare[i].Init();
		for (int j = 0; j<DEM; j++){
			sub_pare[i].n[j] = pare[i].n[j] - 2 * vector[i].n[j];
		}
	}
	/*�e���܂Ƃ߂�*/
	playerTK sum_pare[pare_size];
	for (i = 0; i<Np; i++){
		/*�O������pare�\���̂��A�㔼���ɕ����̂�����*/		sum_pare[i] = pare[i];		sum_pare[i].Init();

		sum_pare[pare_size + i] = sub_pare[i];		sum_pare[pare_size + i].Init();
	}
	/*�i���o�[�Y��K�p���ĕ]���̂������Ƀ\�[�g*/
	Pare_Numbers(sum_pare);
	sort_win(sum_pare, Np * 2); 
	/*��ʔ����̗��̂̏d�S�A���ʔ����̗��̂̏d�S�����߂�*/
	double high_gra[DEM] = { 0 };
	for (j = 0; j<DEM; j++){
		sum_n[j] = 0;
	}
	for (i = 0; i<Np; i++){
		for (j = 0; j<DEM; j++){
			sum_n[j] += sum_pare[i].n[j];
		}
	}

	for (i = 0; i<DEM; i++){
		high_gra[i] = sum_n[i] / Np;
	}
	/*�q�̐���*/
	double diag[DEM];
	double sum_coe[DEM] = { 0 }, coe = 0; /*�e�̃x�N�g��*�����̑��a*/
	double child_save[DEM];
	int count = 0;
	true_flag = 1;
	while (true_flag){
		/*��11�̑�Q���̒l����*/
		for (i = 0; i<DEM; i++){
			diag[i] = genrand_real1()*T;
			sum_coe[i] = 0;
		}
		/*��(11)�̑�R���̓���*/
		for (i = 0; i<Np; i++){
			coe = GetRand_Real(sqrt(3 / (DEM + 1)));
			for (j = 0; j<DEM; j++){
				sum_coe[j] += coe * (sum_pare[i].n[j] - base_gra[j]);
			}
		}
		for (i = 0; i<DEM; i++){
			child_save[i] = base_gra[i] + diag[i] * (high_gra[i] - base_gra[i]) + sum_coe[i];
		}
		if (fabs(child_save[0]) <= 100.00 && fabs(child_save[1]) <= 100.00){
			for (i = 0; i<DEM; i++){
				child[count].n[i] = child_save[i]; /*��(11)�̓���*/
			}
			count++;
		}
		if (count == Nc){
			true_flag = 0;
		}
	}
}