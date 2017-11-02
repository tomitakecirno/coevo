#pragma once
/*=======================================
�����̊e���@�܂Ƃ�
CoansMode0	���_�̎�@
CoansMode1	����@�F���]�����@(�]���l�ɕ␳��������)
CoansMode2	����@�F�O�]�����@(�]���l�ɕ␳�������Ȃ�)
CoansMode3	�O��@�F���]�����@
CoansMode4	�O��@�F�O�]�����@
=======================================*/
#include <time.h>
#include <string>
#include <fstream>
#include "config_nim.hpp"
#include "player_nim.h"
#include "coansmodule_nim.hpp"
#include "../header/Usual_Methods.hpp"

/*��@�̃N���X*/
class Coans{
//���J�����o
public :
	void	main_task(); 	//��@�̃��C�����[�`��
	int		Get_MatchUp_Num();			//�ΐ�񐔂��擾
	
	//�f�[�^���~�X�����Ƃ��Ɏg�����
	void	Stra_nitch_CSV();	//�헪�f�[�^��ǂݍ����
	//�ȉ������p
//����J�����o
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
	//���̃N���X�Ɣh���N���X����A�N�Z�X�\
protected:
	int method;						//��@��r�̎��Ɏg��
	int gene;						//���㐔
	int per;						//�Ԋu
	int trial;
	int cr_num;						//�N���X�^�[�ԍ�
	int	cru_k;						//�N���X�^�����O�p�����[�^�[
	int opp_num;
	int machup;						//�ΐ��

	std::string dir;
	std::vector<playerNim> pop;
	std::vector<playerNim> child;
	std::vector<playerNim> opp;

	int  Choice_Best_Index(bool random = true);
	void input_stra(int g);
	void output_stra(int g);
	void Crustering1();
	void Crustering2();
};
void Coans::main_task()
{
	std::cout << "��@:" << method << std::endl;
	std::cout << "���s��:" << trial << std::endl;
	std::cout << "�N���X�^��:" << cru_k << std::endl;
	std::cout << "���㐔:" << gene << std::endl;
	std::cout << "��؂�:" << per << std::endl;
	std::cout << "�W�c�̐�:" << KO << std::endl;
	std::cout << "�q�̐�:" << PARENT << std::endl;
	std::cout << "�e�̐�:" << CHILD << std::endl;

	//�W�c�錾
	pop.resize(KO);
	init_genrand((unsigned)time(NULL)); /*����������*/

	//�W�c������
	for (int i = 0; i < KO; i++) {
		pop[i].Init_pn();
		pop[i].Init_stra();
	}
	Make_Directory(dir, method, trial, gene, per, cru_k);
	//Csv_exp csv_exp(dir, method, trial, gene, per);

	//��������̐헪���L�^
	machup = 0;
	//�C�ӂ̐��㐔���[�v
	int Loop_Time_Start;
	int Loop_Time_End;
	int Loop_Time;
	//Crustering();

	std::cout << "Initiarized..." << std::endl;
	output_stra(0);
	std::cout << "Strategy0..." << std::endl;
	for (int Gene_Loop = 1; Gene_Loop < gene+1; Gene_Loop++) {
		Loop_Time_Start = clock();
		std::cout << method << ":" << trial << ":" << cru_k << ":" << Gene_Loop;
		std::cout << "  |  ";
		std::cout << "1" << ',';
		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��

		std::cout << "2" << ',';
		int tmp_index;
		int tmp_sub;
		std::vector<int> sub_pare;
		//��e�I��
		const int main_pare = GetRand_Int(KO);
		//���e��I��

		std::vector<int> list1;
		if (!pop[main_pare].get_list(list1, 1)) {
			std::cout << "error main_task -> get_list" << std::endl;
			exit(EXIT_FAILURE);
		}
		int list1_len = int(list1.size());
		for (int i = 0; i < PARENT; i++) {
			if (list1.empty()) {
				std::cout << "�ߖT���X�g�ɏ\���Ȍ̂�����܂���" << std::endl;
				exit(EXIT_FAILURE);
			}
			tmp_index = GetRand_Int(list1_len);
			tmp_sub = list1[tmp_index];
			sub_pare.push_back(tmp_sub); //�v�f��ǉ�
			list1.erase(list1.begin() + tmp_index); //�ǉ������v�f���폜
			list1_len--;
		}
		//���e������Έȉ��̏������s��
		if (!sub_pare.empty()) {
			//�q�̐���
			child.resize(CHILD + 1);
			std::cout << "3" << ',';
			//�g��XLM
			child[0] = pop[main_pare];
			ExtensionXLM(main_pare, sub_pare, pop, child);
			//�ΐ푊��̌̂�I��
			std::cout << "4" << ',';
			Generate_Opp();

			//w1,w2,w3:�q�̂̐헪�Cw1_T,w2_T,w3_T:�ΐ푊��̐헪
			//player1:�q�́Cplayer2:�ΐ푊��0
			std::cout << "5" << ',';
			int opp_len = int(opp.size());

			//�����őΐ�
			/*
			
			*/
			std::cout << "6" << ',';
			//�K���x�v�Z
			for (int i = 0; i < CHILD; i++) {
				child[i].cal_fitness();
			}

			std::cout << "7" << ',';
			//Best�̂��W�c��
			int index = Choice_Best_Index(false);
			pop[main_pare] = child[index];

			//�����p
			if (Gene_Loop % PER == 0) {
				std::cout << "10" << ',';
			}
			std::cout << "8";
			std::cout << "  [";
			for (int c = 0; c < CHILD + 1; c++) {
				std::cout << child[c].get_eval() << ",";
			}
			std::cout << "]" << std::endl;
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
				pop[i].Init_pn();
			}
		}
		Loop_Time_End = clock();
		Loop_Time = (Loop_Time_End - Loop_Time_Start) / CLOCKS_PER_SEC;
		std::cout << "Time per gene : " << Loop_Time << " [sec]" << std::endl;
	}
	if (method == 2) {
		//csv_exp.fwrite_Cr_P();
	}
}
int Coans::Get_MatchUp_Num() {
	return machup;
}
void Coans::input_stra(int g)
{
	std::stringstream tmp_fname;
	tmp_fname << "./" << dir << "/" << method << "/" << trial << "/" << g;

	for (int i = 0; i < KO; i++) {
		std::stringstream fname;
		fname << tmp_fname.str() << "/" << i << ".dat";

		if (!pop[i].input_stra(fname.str())) {
			std::cout << "error : in_stra -> in_stra" << std::endl;
			exit(EXIT_FAILURE);
		}
		//�N���A
		fname.str("");
		fname.clear(std::stringstream::goodbit);
	}
}
void Coans::output_stra(int g) 
{
	std::stringstream tmp_fname;
	tmp_fname << "./" << dir << "/" << method << "/" << trial << "/" << g;

	for (int i = 0; i < KO; i++) {
		std::stringstream fname;
		fname << tmp_fname.str() << "/" << i << ".dat";

		if (!pop[i].output_stra(fname.str())) {
			std::cout << "error : output_stra -> output_stra" << std::endl;
			exit(EXIT_FAILURE);
		}
		//�N���A
		fname.str("");
		fname.clear(std::stringstream::goodbit);
	}

}
void Coans::Stra_nitch_CSV() {
	/*
	std::cout << "���[�h:" << 4 << std::endl;
	std::cout << "��@:" << method << std::endl;
	std::cout << "���s��:" << trial << std::endl;
	std::cout << "�N���X�^��:" << cru_k << std::endl;
	std::cout << "���㐔:" << gene << std::endl;
	std::cout << "��؂�:" << per << std::endl;
	std::cout << "�W�c�̐�:" << KO << std::endl;

	//Init
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
	}
	Csv_exp csv_exp(dir, method, trial, gene, per);
	for (int g = 0; g < gene / per + 1; g++) {
		//�헪�i�[
		for (int i = 0; i < KO; i++) {
			char fname[50];
			if (cru_k == 0) {
				sprintf_s(fname, ("%s/%d/%d/%d/%d.dat"), dir.c_str(), method, trial, g, i);
			}
			else if (0 < cru_k) {
				sprintf_s(fname, ("%s/%d/%d/%d/%d/%d.dat"), dir.c_str(), method, trial, cru_k, g, i);
			}
			//���v���C���[�̐헪�i�[
			pop[i].input_stra(fname);
		}
		//�N���X�^�����O
		Crustering();
		csv_exp.SetVec_Cr_Pop(pop, g);
	}
	csv_exp.fwrite_Cr_P();
	*/
}
void Coans::Crustering1() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(pop, K_List1, K_List2, 0);
	cr_num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(cr_num, i, pop) == 1) {
			cr_num++;
		}
	}
}
void Coans::Crustering2() {
	//�ߖT���X�g����
	//MakeList(Pop, true, false, false);
	//�N���X�^�ԍ�����U��
	/*
	Cru_Upgma(pop, cru_k);
	cr_num = cru_k;
	*/
}
int Coans::Choice_Best_Index(bool random) {
	std::vector<double> tmp_eval(CHILD);

	//��U�i�[
	for (int i = 0; i < CHILD; i++) {
		tmp_eval[i] = child[i].get_eval();
	}

	int index = 0;
	auto max = max_element(tmp_eval.begin(), tmp_eval.end());
	//�����]���n�̌̂���������ꍇ�̓����_��
	int count_Num = int(count(tmp_eval.begin(), tmp_eval.end(), *max));
	//cout << "count_Num:" << count_Num << endl;
	if (random) {
		if (count_Num == 1) {
			//�C���f�b�N�X���擾
			index = int(std::distance(tmp_eval.begin(), max));
		}
		else if (1 < count_Num) {
			std::vector<int> tmp_max_index(count_Num);
			tmp_max_index[0] = int(std::distance(tmp_eval.begin(), max));
			for (int j = 1; j < count_Num; j++) {
				auto Index_Iterator = find(tmp_eval.begin() + tmp_max_index[j - 1] + 1, tmp_eval.end(), *max);
				tmp_max_index[j] = int(std::distance(tmp_eval.begin(), Index_Iterator));
			}
			index = int(tmp_max_index[GetRand_Int(count_Num)]);
		}
	}
	else {
		index = int(std::distance(tmp_eval.begin(), max));
	}
	return index;
}

/*
	mode1
	�E�H�[�h�@�̊K�w�I�N���X�^�����O�Ŗ����I�ɃN���X�^�����O
	�]����HP�̍���
*/
class coans_mode1 : public Coans {
public:
	coans_mode1(std::string str, int g, int g_p, int k = 0) {
		dir = str;
		method = 1;
		gene = g;						//���㐔
		per = g_p;						//�Ԋu
		machup = 0;					//�ΐ��
		cru_k = k;						//�N���X�^�����O�p�����[�^�[
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int Child_Num, int Opp_Num);
	virtual void Cal_Fitness();
};
void coans_mode1::Crustering() {
	//�ߖT���X�g����
	//MakeList(Pop, true, false, false);
	//�N���X�^�ԍ�����U��
	Crustering1();
}
void coans_mode1::Generate_Opp() {
	choice_oppoment(pop, opp, cr_num);
	opp_num = cr_num;
}
void coans_mode1::Return_Re(int Child_Num, int Opp_Num){}
void coans_mode1::Cal_Fitness(){}

