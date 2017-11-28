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
#include "nim.h"
#include "../header/Usual_Methods.hpp"

#define TIME_PER 100

/*��@�̃N���X*/
class Coans_base {
public:
	__int64		Get_MatchUp_Num();			//�ΐ�񐔂��擾
	//�f�[�^���~�X�����Ƃ��Ɏg�����
	void	Stra_nitch_CSV();	//�헪�f�[�^��ǂݍ����
protected:
	int method;						//��@��r�̎��Ɏg��
	int gene;						//���㐔
	int per;						//�Ԋu
	int trial;
	int cr_num;						//�N���X�^�[�ԍ�
	int	cru_k;						//�N���X�^�����O�p�����[�^�[
	int opp_num;
	int sel_child;
	__int64 machup;						//�ΐ��

	std::string dir;
	std::vector<playerNim> pop;
	std::vector<playerNim> child;
	std::vector<playerNim> opp;

	void Choice_Best_Index(std::vector<int> &index_vec); //��ԗǂ��̂̃C���f�b�N�X��Ԃ�
	void sub_servive(const std::vector<int> subpare, std::vector<int> &index_vec);

	//�����p
	std::vector<std::vector<int>> cr_p;
	void input_stra(int g);
	void output_stra(int g);
	void output_cr_pop();
	void cal_best(int g);
};
__int64 Coans_base::Get_MatchUp_Num() {
	return machup;
}
void Coans_base::Stra_nitch_CSV() {
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
void Coans_base::input_stra(int g)
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
void Coans_base::output_stra(int g)
{
	std::stringstream tmp_fname;
	tmp_fname << "./" << dir << "/" << method << "/" << trial << "/" << g;

	Make_Directory2(dir, method, trial, g);
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
void Coans_base::output_cr_pop()
{
	if (cr_p.empty()) {
		std::cout << "cr_p����ł�" << std::endl;
		exit(EXIT_FAILURE);
	}
	Make_CSV_Directory(method);
	const int vec_size = int(cr_p.size());
	std::vector<std::vector<int>> tmp_cr_p(vec_size);
	//�N���X�^���Ń��T�C�Y
	for (int i = 0; i < vec_size; i++) {
		auto max = *max_element(cr_p[i].begin(), cr_p[i].end());
		tmp_cr_p[i].assign(max + 1, 0);
	}
	//�N���X�^�̌̐����J�E���g��C�x�N�^�[��0�̗v�f���폜
	for (int i = 0; i < vec_size; i++) {
		for (int j = 0; j < KO; j++) {
			tmp_cr_p[i][cr_p[i][j]]++;
		}
		tmp_cr_p[i].erase(std::remove(tmp_cr_p[i].begin(), tmp_cr_p[i].end(), 0), tmp_cr_p[i].end());
	}

	char fname[50], fname2[50];
	if (cru_k == 0) {
		sprintf_s(fname, "%s/%d/cruster_%d_%d_%d.csv", CSV_DIR, method, method, gene, trial);
		sprintf_s(fname2, "%s/%d/cruster2_%d_%d_%d.csv", CSV_DIR, method, method, gene, trial);
	}
	else {
		sprintf_s(fname, "%s/%d/cruster_%d_%d_%d_%d.csv", CSV_DIR, method, method, gene, trial, cru_k);
		sprintf_s(fname2, "%s/%d/cruster2_%d_%d_%d_%d.csv", CSV_DIR, method, method, gene, trial, cru_k);
	}
	if (!CsvModules::csv_fwrite(fname, cr_p, per)) {
		exit(EXIT_FAILURE);
	}
	if (!CsvModules::csv_fwrite(fname2, tmp_cr_p, per)) {
		exit(EXIT_FAILURE);
	}
	std::cout << "ccr_pop csv done..." << std::endl;
}
void Coans_base::Choice_Best_Index(std::vector<int> &index_vec)
{
	std::vector<double> tmp_eval(CHILD);
	int index_size = int(index_vec.size());

	//��U�i�[
	for (int i = 0; i < CHILD; i++) {
		tmp_eval[i] = child[i].eval;
	}
	for (int i = 0; i < index_size; i++) {
		int index = 0;
		auto max = max_element(tmp_eval.begin() + 1, tmp_eval.end());
		//��e(index = 0)���]���l�̍����̂�����Ή��P
		//�����]���n�̌̂���������ꍇ�̓����_��
		int count = int(std::count(tmp_eval.begin() + 1, tmp_eval.end(), *max));
		//cout << "count_Num:" << count_Num << endl;
		if (count) {
			if (count == 1) {
				//�C���f�b�N�X���擾
				index = int(std::distance(tmp_eval.begin(), max));
			}
			else if (1 < count) {
				std::vector<int> tmp_index(count);
				auto index_t = tmp_eval.begin();

				for (int j = 0; j < count; j++) {
					index_t = std::find(index_t + 1, tmp_eval.end(), *max);
					tmp_index[j] = int(std::distance(tmp_eval.begin(), index_t));
				}
				const int rand = GetRand_Int(count);
				const int index = tmp_index[rand];
			}
		}
		index_vec[i] = index;
		tmp_eval[index] = -10000;
	}
}
void Coans_base::sub_servive(const std::vector<int> subpare, std::vector<int> &index_vec)
{
	const int index_size = int(index_vec.size());
	const int sub_size = int(subpare.size());

	for (int i = 1; i < index_size; i++) {

		int child_index = index_vec[i];
		std::vector<double> dis(sub_size);

		for (int j = 0; j < sub_size; j++) {
			const int sub_index = subpare[j];
			dis[j] = cal_haming(pop[sub_index].stra, child[child_index].stra);
		}

		auto min = min_element(dis.begin(), dis.end());
		int min_index = int(std::distance(dis.begin(), min));
		if (pop[subpare[min_index]].eval < child[child_index].eval) {
			pop[subpare[min_index]] = child[child_index];
		}
	}
}
void Coans_base::cal_best(int g) 
{
	nim nim2(1);
	std::vector<double> sum_eval(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Result.assign(KO, 0);
	}
	for (int i = 0; i < KO; i++) {
		for (int j = i; j < KO; j++) {
			//1���
			double value;
			nim2.input_stra_first(pop[i].stra);	//���
			nim2.input_stra_last(pop[j].stra);	//���
			value = nim2.nim_game(0);
			if (value == 1) {
				pop[i].Result[j] += WIN_FIRST;
			}
			else {
				pop[j].Result[i] += WIN_LAST;
			}
			//���������ւ���2���
			nim2.input_stra_first(pop[j].stra);		//���
			nim2.input_stra_last(pop[i].stra);		//���
			value = nim2.nim_game(1);
			if (value == 0) {
				pop[i].Result[j] += WIN_LAST;
			}
			else {
				pop[j].Result[i] += WIN_FIRST;
			}
		}
	}
	for (int i = 0; i < KO; i++) {
		sum_eval[i] = std::accumulate(pop[i].Result.begin(), pop[i].Result.end(), 0.0);
	}
	//show_vec_1(sum_eval);
	auto max_t = max_element(sum_eval.begin(), sum_eval.end());
	const int max_index = int(std::distance(sum_eval.begin(), max_t));
	char max_name[50];
	sprintf_s(max_name, "./nim/%d/%d/%d/best.dat", method, trial, g);
	pop[max_index].output_stra(max_name);

	std::cout << "�@best written ... " << std::endl;
	//show_vec_1(pop[max_index].stra);
	//p_data opp;
	//opp.input_stra(max_name);
	//show_vec_1(opp.stra);
	//exit(0);
}

class Coans : public Coans_base{
//���J�����o
public :
	void main_task(); 	//��@�̃��C�����[�`��
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void child_select(int main, const std::vector<int> &sub) = 0;
protected :
	void Crustering1();
	void Crustering2();
	void Generate_Opp_all();
	void Generate_Opp_per();
//�ȉ������p
};
void Coans::main_task()
{
	std::cout << "��@:" << method << std::endl;
	std::cout << "���s��:" << trial << std::endl;
	std::cout << "�N���X�^��:" << cru_k << std::endl;
	std::cout << "���㐔:" << gene << std::endl;
	std::cout << "��؂�:" << BATTLE_PER << std::endl;
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
	std::cout << pop[0].stra.size() << std::endl;
	cr_p.resize(gene / per);
	cr_p = std::vector<std::vector<int>>(gene / per + 1, std::vector<int>(KO, 0));

	nim nim(2); //�j���̃N���X��`
	//Csv_exp csv_exp(dir, method, trial, gene, per);

	//�C�ӂ̐��㐔���[�v
	//Crustering();
	int Loop_Time_Start, Loop_Time_End, Loop_Time;
	std::cout << "Initiarized..." << std::endl;
	output_stra(0);
	std::cout << "Strategy0..." << std::endl;

	Loop_Time_Start = clock();
	machup = 0;
	int machup_index = 1;
	for (int Gene_Loop = 1; Gene_Loop < gene + 1; Gene_Loop++) {
		if (Gene_Loop % TIME_PER == 0) {
			std::cout << method << ":" << trial << ":" << Gene_Loop << std::endl;
		}
		//std::cout << "1" << ',';
		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��

		//�����p : csv�Ńj�b�`���o��
		if (Gene_Loop % per == 0) {
			if (method == 2) {
				for (int i = 0; i < KO; i++) {
					cr_p[Gene_Loop / per][i] = pop[i].nitch;
				}
			}
		}

		//std::cout << "2" << ',';
		//��e�I��
		const int main_pare = GetRand_Int(KO);
		//���e��I��

		//���e������Έȉ��̏������s��
		if (pop[main_pare].List1.empty()) {
			std::cout << "list1����ł� main_tasks" << std::endl;
			exit(EXIT_FAILURE);
		}
		else  {
			//�q�̐���
			std::vector<int> sub_pare;
			sub_pare = pop[main_pare].List1;
			child.resize(CHILD + 1);
			//std::cout << "3" << ',';

			//�g��XLM
			child[0] = pop[main_pare];
			binaryEXLM(main_pare, sub_pare, pop, child);

			//std::cout << "4" << ',';
			Generate_Opp(); //�ΐ푊��̌̂�I��

			//std::cout << "5" << ',';
			if (Gene_Loop % TIME_PER == 0) {
				std::cout << "�@opp_num = " << opp_num << std::endl;
			}

			//�����őΐ�
			for (int i = 0; i < CHILD + 1; i++) {
				child[i].Result.assign(opp_num, 0);
				for (int j = 0; j < opp_num; j++) {
					//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
					//1���
					nim.input_stra_first(child[i].stra);	//���
					nim.input_stra_last(opp[j].stra);		//���
					child[i].Result[j] += nim.nim_game(0)*WIN_FIRST;
					machup++;
					//���������ւ���2���
					nim.input_stra_first(opp[j].stra);		//���
					nim.input_stra_last(child[i].stra);		//���
					child[i].Result[j] += nim.nim_game(1)*WIN_LAST;
					machup++;
				}
			}

			//std::cout << "6" << ',';
			//�K���x�v�Z
			for (int i = 0; i < CHILD + 1; i++) {
				child[i].cal_fitness();
			}
			//std::cout << "7" << ',';
			//Best�̂��W�c��
			child_select(main_pare, sub_pare);

			//�����p : �헪�����o��
			if ( BATTLE_PER*machup_index < machup ) {
				//std::cout << "10" << ',';
				output_stra(machup_index);
				cal_best(machup_index);
				std::cout << "�@output strategies : " << machup_index << std::endl;
				std::cout << "�@mach_up : " << machup << std::endl;
				machup_index++;
			}
			//std::cout << "8";
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
				pop[i].Init_pn();
			}
		}
		if (Gene_Loop % TIME_PER == 0) {
			Loop_Time_End = clock();
			Loop_Time = (Loop_Time_End - Loop_Time_Start);
			std::cout << "�@Time per gene : " << Loop_Time/CLOCKS_PER_SEC << " [sec]" << std::endl;
			Loop_Time_Start = clock();
		}
	}
	if (method == 2) {
		output_cr_pop();
	}
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
void Coans::Generate_Opp_all() 
{
	opp = pop;
	opp_num = KO;
}
void Coans::Generate_Opp_per() 
{
	choice_oppoment(pop, opp, cr_num);
	opp_num = int( opp.size() );
}

/*
	method2
	��e		:�����_��
	�ΐ푊��	:�N���X�^�̑�\�_
	�]��		:�j���̕]���Ɋ�Â�
*/
class coans_mode2 : public Coans {
public:
	coans_mode2(std::string str, int g, int g_p, int t, int k = 0) {
		dir = str;
		method = 2;
		gene = g;						//���㐔
		per = g_p;						//�Ԋu
		trial = t;
		machup = 0;					//�ΐ��
		cru_k = k;						//�N���X�^�����O�p�����[�^�[
		sel_child = 1;
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void child_select(int main, const std::vector<int> &sub);
};
void coans_mode2::Crustering() {
	//�ߖT���X�g����
	//MakeList(Pop, true, false, false);
	//�N���X�^�ԍ�����U��
	Crustering1();
}
void coans_mode2::Generate_Opp() 
{
	Generate_Opp_per();
}
void coans_mode2::child_select(int main, const std::vector<int> &sub)
{
	std::vector<int> index_vec(sel_child);
	Choice_Best_Index(index_vec);
	pop[main] = child[index_vec[0]];
}

/*
	method3
	��e		:�����_��
	�ΐ푊��	:�W�c�S��
	�]��		:�j���̕]���Ɋ�Â�
*/
class coans_mode3 : public Coans {
public:
	coans_mode3(std::string str, int g, int g_p, int t, int k = 0) {
		dir = str;
		method = 3;
		gene = g;						//���㐔
		per = g_p;						//�Ԋu
		trial = t;
		machup = 0;					//�ΐ��
		cru_k = k;						//�N���X�^�����O�p�����[�^�[
		sel_child = 1;
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void child_select(int main, const std::vector<int> &sub);
};
void coans_mode3::Crustering() {
	//�ߖT���X�g����
	//MakeList(Pop, true, false, false);
	//�N���X�^�ԍ�����U��
	Crustering1();
}
void coans_mode3::Generate_Opp()
{
	Generate_Opp_all();
}
void coans_mode3::child_select(int main, const std::vector<int> &sub)
{
	std::vector<int> index_vec(sel_child);
	Choice_Best_Index(index_vec);
	pop[main] = child[index_vec[0]];
}

/*
method3
��e		:�����_��
�ΐ푊��	:�W�c�S��
�]��		:�j���̕]���Ɋ�Â�
*/
class coans_mode4 : public Coans {
public:
	coans_mode4(std::string str, int g, int g_p, int t, int k = 0) {
		dir = str;
		method = 3;
		gene = g;						//���㐔
		per = g_p;						//�Ԋu
		trial = t;
		machup = 0;					//�ΐ��
		cru_k = k;						//�N���X�^�����O�p�����[�^�[
		sel_child = K_List1;
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void child_select(int main, const std::vector<int> &sub);
};
void coans_mode4::Crustering() {
	//�ߖT���X�g����
	//MakeList(Pop, true, false, false);
	//�N���X�^�ԍ�����U��
	Crustering1();
}
void coans_mode4::Generate_Opp()
{
	Generate_Opp_all();
}
void coans_mode4::child_select(int main, const std::vector<int> &sub)
{
	std::vector<int> index_vec(sel_child);
	Choice_Best_Index(index_vec);
	pop[main] = child[index_vec[0]];
	sub_servive(sub, index_vec);
}

//�S���ΐ�̂�
class Coans_exp : public Coans_base {
public:
	void main_task(std::string str, int m, int g, int g_p, int t);
};
void Coans_exp::main_task(std::string str, int m, int g, int g_p, int t) 
{
	dir = str;
	method = m;
	gene = g;						//���㐔
	per = g_p;						//�Ԋu
	trial = t;
	machup = 0;					//�ΐ��
	
	int folder_num = 0;
	char fname[50];
	//�t�H���_�����擾
	while (1) {
		sprintf_s(fname, "./nim/%d/%d/%d/%d.dat", method, trial, folder_num, 0);
		std::ifstream ifs(fname);
		if (ifs.fail()) {
			break;
		}
		else {
			folder_num++;
		}
	}
	pop.resize(KO);
	//�S���ΐ�
	for (int f = 0; f < folder_num; f++) {
		std::cout << "trial:" << trial << "  " << "folder:" << f << std::endl;
		double start = clock();
		for (int i = 0; i < KO; i++) {
			sprintf_s(fname, "./nim/%d/%d/%d/%d.dat", method, trial, f, i);
			if (!pop[i].input_stra(fname)) {
				exit(EXIT_FAILURE);
			}
		}
		cal_best(f);
		double end = clock();

		double time = end - start;
		std::cout << "�@" << time / CLOCKS_PER_SEC << "[sec]" << std::endl;
	}
}

/*

���_��@�̃N���X

*/
/*
class Coans_GT2016 : public Coans {
	//���J�����o
public:
	Coans_GT2016(std::string str, int g, int g_p, int t, int k = 0) {
		dir = str;
		method = 0;
		gene = g;						//���㐔
		per = g_p;						//�Ԋu
		trial = t;
		machup = 0;					//�ΐ��
		cru_k = k;						//�N���X�^�����O�p�����[�^�[
	}
	//��@�̃��C�����[�`��
	void	Coans_GT2016_Tasks();
	int		Get_MatchUp_Num();
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
protected:
	std::vector<int> opp_flag;
	std::vector<int> opp_delete_flag;

	void Update_Opponent(playerNim child);
	int	 Cal_Gra_Nitch(playerNim child);
};
void Coans_GT2016::Coans_GT2016_Tasks()
{
	std::cout << "Initiarize" << std::endl;

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
	init_genrand((unsigned)time(NULL));

										//�W�c������
	for (int i = 0; i < KO; i++) {
		pop[i].Init_pn();
		pop[i].Init_stra();
	}

	nim nim; //�j���̃N���X��`
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
	for (int Gene_Loop = 1; Gene_Loop < gene + 1; Gene_Loop++) {
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
			binaryEXLM(main_pare, sub_pare, pop, child);

			std::cout << "4" << ',';
			Generate_Opp(); //�ΐ푊��̌̂�I��


			std::cout << "5" << ',';

			//�����őΐ�
			for (int i = 0; i < CHILD + 1; i++) {
				std::vector<double> result(opp_num, 0);
				for (int j = 0; j < opp_num; j++) {
					//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
					//1���
					nim.input_stra_first(child[i]);	//���
					nim.input_stra_last(opp[j]);	//���
					result[j] += nim.nim_game()*WIN_FIRST;

					//���������ւ���2���
					nim.input_stra_first(opp[j]);	//���
					nim.input_stra_last(child[i]);	//���
					result[j] += nim.nim_game()*WIN_LAST;
				}
				child[i].put_result(result);
			}

			std::cout << "6" << ',';
			//�K���x�v�Z
			for (int i = 0; i < CHILD; i++) {
				child[i].cal_fitness();
			}

			std::cout << "7" << ',';
			//Best�̂��W�c��
			int index = Choice_Best_Index();
			pop[main_pare] = child[index];

			std::cout << "8" << ',';
			//�폜�\��̌̂������
			if (index != 0) {
				int k;
				for (k = 0; k < KO; k++) {
					if (opp[k].delete_flag != 0) {
						break;
					}
				}
				if (k != Pop_n) {
					Opponent[k] = Child[Index];
					Opponent[k].comp_flag = 1;
					Opponent[k].delete_flag = 0;
				}if (k == Pop_n) {
					std::cout << "8-1" << ',';
					Update_Opponent(Child[Index]);
				}
			}
			std::cout << "9" << ',';
			for (int i = 0; i < Pop_n; i++) {
				if (Opponent[i].delete_flag != 1) {
					if (Opponent[i].comp_flag == 0) {
						Opponent[i].gene_count++;
					}
					else if (Opponent[i].comp_flag == 1) {
						Opponent[i].gene_count = 0;
					}
					Opponent[i].comp_flag = 0;
					if (DELETE <= Opponent[i].gene_count) {
						Opponent[i].delete_flag = 1;
						Opponent[i].nitch = 0;
					}
				}
			}

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
}
int Coans_GT2016::Get_MatchUp_Num() 
{
	return Machup_Num;
}
void Coans_GT2016::Crustering() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(opp, K_List1, K_List2, 0);
	cr_num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(cr_num, i, opp) == 1) {
			cr_num++;
		}
	}
}
void Coans_GT2016::Generate_Opp() {}
void Coans_GT2016::Update_Opponent(playerNim child)
{
	//����Ȃ�������c���ƗL�Q�B�������Ŏc���̔��f���Ă���̂ŁA�]�v�ȏ��������J�E���g�����.
	//�������ŏ��̃j�b�`�̏d�S�������āA�j�b�`�ԍ����擾
	int nitch_count = 0;
	const int child_nitch = Cal_Gra_Nitch(child);
	//��ԋ߂��N���X�^�̔ԍ�������

	//�퓬�f�[�^������
	std::vector<int> opp_nitch(opp_num);
	for (int i = 0; i < opp_num; i++) {
		opp_nitch[i] = opp[i].get_nitch();
		if (opp_nitch[i] == child_nitch) {
			opp_flag[i] = 0;
			nitch_count++;
		}
	}
	//�ΐ�p�̃x�N�^�[�����
	std::vector<playerNim> oppc(nitch_count + 1);
	std::vector<double> oppc_eval(nitch_count + 1);
	for (int i = 0; i < KO; i++) {
		if (opp_nitch[i] == child_nitch) {
			oppc[nitch_count] = opp[i];
			nitch_count--;
		}
	}
	assert(0 <= Nit_Count);
	child.eval = 0;
	child.flag = 0;
	Oppc[0] = child;

	//�Y������j�b�`�̌̂Ǝq�̂Ń��[�O��
	int Oppc_size = int(Oppc.size());
	for (int i = 0; i < Oppc_size; i++) {
		//���N���X�^���̌̂Ƒΐ�
		Oppc[i].Result.resize(Oppc_size - 1);
		StrategySet_M(Oppc[i]);
		for (int j = i + 1; j < Oppc_size - 1; j++) {
			StrategySet_T(Oppc[j]);
			Competition();//�ΐ� player1 = �q�� palyer2 = �ΐ푊��H
			Oppc[i].Result[j] = (player1.hp - player2.hp) / 300;
			Oppc[j].Result[i] = (player2.hp - player1.hp) / 300;
			Machup_Num++;
		}
		//�q�̂Ƒΐ�
		Opponent[i].flag = 1;
		Opponent[i].comp_flag = 1;
	}
	for (int i = 0; i < Oppc_size; i++) {
		Oppc_Eval[i] = std::accumulate(Oppc[i].Result.begin(), Oppc[i].Result.end(), 0.0);
	}

	//�]���l�̍ŏ��̂����߂�
	auto min = min_element(Oppc_Eval.begin(), Oppc_Eval.end());
	int min_index = int(distance(Oppc_Eval.begin(), min));

	//�폜�\��̌̘̂g�֎q�̂�����A�܂��͑���W�c�̒��Ŏq�̂��ア����̂Ƃ���ɓ����
	int j;
	for (int i = 0; i < Oppc_size; i++) {
		if (min_index != i) {
			for (j = 0; j < KO && Opponent[j].nitch != Nit_Num; j++) {}
			Opponent[j] = Oppc[i];
			Opponent[j].comp_flag = 1;
			Opponent[j].delete_flag = 0;
		}
	}
	for (int i = 0; i < KO; i++) {
		Opponent[i].flag = 0;
		Opponent[i].eval = 0;
		Opponent[i].nitch = 0;
		Opponent[i].List2.clear();
	}
	MakeList(Opponent, 0, K_List2, 0);
	Cr_Num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(Cr_Num, i, Opponent) == 1) {
			Cr_Num++;
		}
	}
}
int Coans_GT2016::Cal_Gra_Nitch(playerTK child)
{
	std::vector<int> Nitch_Num;
	std::vector< std::vector < std::vector<double> > > Sum_N_W1;
	std::vector< std::vector < std::vector<double> > > Sum_N_W2;
	std::vector< std::vector < std::vector<double> > > Sum_N_W3;

	Nitch_Num = std::vector<int>(Cr_Num, 0);
	Sum_N_W1 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(I1, std::vector<double>(J1, 0.0)));
	Sum_N_W2 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(I2, std::vector<double>(J2, 0.0)));
	Sum_N_W3 = std::vector<std::vector<std::vector<double>>>(Cr_Num + 1, std::vector<std::vector<double>>(J1, std::vector<double>(I2, 0.0)));

	
	//std::cout << "8-1-4" << ',';
	auto min = min_element(Dis_Vector.begin() + 1, Dis_Vector.end());
	auto min_Nit = int(distance(Dis_Vector.begin(), min));
	return min_Nit;
}
*/