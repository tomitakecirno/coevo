#pragma once
#include "config_nim.hpp"
#include "player_nim.h"
#include "nim.h"
#include <iostream>
#include <time.h>
#include "coansmodule_nim.hpp"

class floreano {
public :
	floreano(int t = 0){
		method_t = 0;
		trial_t = t;
		Make_Directory_AIT(method_t, trial_t);
	}
	void main_task();
private:
	std::vector<p_data> pop;
	std::vector<p_data> opp;
protected:
	int method_t;
	int trial_t;
	bool cross(std::vector<p_data> &Tmp_Pop); //����BLX
	int Choice_Best_Index(); //�ŗǌ̂̃C���f�b�N�X��Ԃ�
	void output_stra(); //�헪�o��
};

//2�_�����ő�p
bool floreano::cross(std::vector<p_data> &tmp)
{
	if (tmp.empty()) {
		std:: cout << "tmp_pop����ł�" << std::endl;
		return false;
	}
	int tmp_size = int(tmp.size());
	tmp.resize(KOT);
	for (int p = tmp_size; p < KOT; p++) {
		int index1 = 0;
		int index2 = 0;
		while (index1 == index2) {
			index1 = GetRand_Int(KOT / 5);
			index2 = GetRand_Int(KOT / 5);
		}
		std::vector<std::vector<int>> child(2);
		two_point_cross(tmp[index1].stra, tmp[index2].stra, child);
		mutation(child[0]);
		mutation(child[1]);
		tmp[p].stra = child[0];
		tmp[p + 1].stra = child[1];

		tmp[p].stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);
		tmp[p + 1].stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);

		p++;
	}
	tmp_size = int(tmp.size());
	if (tmp_size != KOT) {
		return false;
	}
	return true;
}
void floreano::main_task(void) {
	//�����W�c����

	pop.resize(KOT);
	opp.resize(FLORET);

	init_genrand((unsigned)time(NULL));
	//�W�c������

	std::vector<int> tmp_n(KOT);
	for (int i = 0; i < KOT; i++) {
		pop[i].Init();
		pop[i].Init_stra();
		tmp_n[i] = i;
	}
	//����W�c�擾
	for (int i = 0; i<FLORET; i++) {
		const int index = GetRand_Int( int(tmp_n.size()) );
		//����W�c�։�����
		opp[i] = pop[ tmp_n[index] ];
		tmp_n.erase(tmp_n.begin() + index);
	}
	nim nim(0);
	//�ȉ����[�v
	for (int e = 0; e < F_KU; e++) {
		std::cout << method_t << ":" << trial_t << ":" << ":" << e;
		//std::cout << "  |  ";
		//std::cout << "1" << ',';

		/*�ΐ�*/
		for (int i = 0; i < KOT; i++) {
			pop[i].Result.assign(FLORET,0);
		}
		//std::cout << "2" << ',';
		/*
		for (int i = 0; i < FLORET; i++) {
			opp[i].Result.resize(KO);
		}
		*/
		for (int i = 0; i < KOT; i++) {
			for (int j = 0; j < FLORET; j++) {
				//1���
				nim.input_stra_first(pop[i].stra);
				nim.input_stra_last(opp[j].stra);
				pop[i].Result[j] += nim.nim_game(0)*WIN_FIRST;

				//2���
				nim.input_stra_first(opp[j].stra);
				nim.input_stra_last(pop[i].stra);
				pop[i].Result[j] += nim.nim_game(1)*WIN_LAST;
			}
		}
		//std::cout << "3" << ',';
		//�]���l�v�Z
		for (int i = 0; i < KOT; i++) {
			pop[i].cal_fitness();
		}
		//std::cout << "3" << ',';
		//�m�F�p
		std::vector<double> pop_eval(KOT);
		for (int i = 0; i < KOT; i++) {
			pop_eval[i] = pop[i].eval;
		}

		std::vector<p_data> tmp_pop(KOT / 5);
		//���1/5���c��
		//int count_Num;
		for (int i = 0; i < KOT / 5; i++) {
			const int index = Choice_Best_Index();
			//cout << "index:" << index << endl;
			tmp_pop[i] = pop[index];
			pop.erase(pop.begin() + index); //�v�f���폜
		}

		//std::cout << "4" << ',';
		//�ΐ푊��̍X�V.�ŌÂ̌̂�u��������
		//e%FLORET�Œu��������̂��擾�ł���D
		opp[e%FLORET] = tmp_pop[0];
		//����
		cross(tmp_pop);
		pop = tmp_pop;
		//std::cout << "5" << ',';
		for (int i = 0; i < KOT; i++) {
			pop[i].Init();
		}
		for (int j = 0; j < FLORET; j++) {
			opp[j].Init();
		}

		show_vec_1(pop_eval);
		std::cout << std::endl;
	}
	output_stra();
}
int floreano::Choice_Best_Index()
{
	int pop_size = int(pop.size());
	std::vector<double> tmp_eval(pop_size);

	//��U�i�[
	for (int i = 0; i < pop_size; i++) {
		tmp_eval[i] = pop[i].eval;
	}

	int index = 0;
	auto max = max_element(tmp_eval.begin(), tmp_eval.end());
	//�����]���n�̌̂���������ꍇ�̓����_��
	int count = int(std::count(tmp_eval.begin(), tmp_eval.end(), *max));
	//cout << "count_Num:" << count_Num << endl;
	if (count) {
		if (count == 1) {
			//�C���f�b�N�X���擾
			index = int(std::distance(tmp_eval.begin(), max));
		}
		else if (1 < count) {
			int rand = GetRand_Int(count);
			auto itrater = tmp_eval.begin();

			for (int j = 0; j < rand + 1; j++) {
				itrater = std::find(itrater, tmp_eval.end(), *max);
			}
			index = int(std::distance(tmp_eval.begin(), itrater));
		}
	}
	return index;
}
void floreano::output_stra() 
{
	for (int i = 0; i < KOT; i++) {
		char fname[50];
		sprintf_s(fname, "./AIT/%d/%d/%d.dat", method_t, trial_t, i);
		pop[i].output_stra(fname);
	}
}
