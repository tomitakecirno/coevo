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
#include "CrusteringMethod_nim.h"

#define TIME_PER 100

/*��@�̃N���X*/
class Coans_base {
public:
	__int64		Get_MatchUp_Num();			//�ΐ�񐔂��擾
	//�f�[�^���~�X�����Ƃ��Ɏg�����
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

	int Choice_Best_Index(); //��ԗǂ��̂̃C���f�b�N�X��Ԃ�

	//�����p
	std::vector<std::vector<int>> cr_p;
	void input_stra(int g);
	void output_stra(int g);
	void output_cr_pop();
	void cal_best(int g);
	void cal_rate();
	void asort_index_H(std::vector<std::vector<int>> &cr_index);
};
__int64 Coans_base::Get_MatchUp_Num() {
	return machup;
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
int Coans_base::Choice_Best_Index()
{
	const int child_len = int(child.size());
	std::vector<double> tmp_eval(child_len);
	//const int index_size = int(index_vec.size());

	//��U�i�[
	for (int i = 0; i < child_len; i++) {
		tmp_eval[i] = child[i].eval;
	}
	auto max = max_element(tmp_eval.begin() + 1, tmp_eval.end());
	int index;
	//��e(index = 0)���]���l�̍����̂�����Ή��P
	//�����]���n�̌̂���������ꍇ�̓����_��
	const int count = int(std::count(tmp_eval.begin() + 1, tmp_eval.end(), *max));
	//cout << "count_Num:" << count_Num << endl;
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
		index = tmp_index[rand];
	}
	return index;
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
			nim2.input_stra(pop[i].stra, pop[j].stra);
			value = nim2.nim_game(0);
			if (value == 1) {
				pop[i].Result[j] += WIN_FIRST;
			}
			else {
				pop[j].Result[i] += WIN_LAST;
			}
			//���������ւ���2���
			nim2.input_stra(pop[j].stra, pop[i].stra);
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
		pop[i].cal_fitness();
		sum_eval[i] = pop[i].eval;
	}
	const auto max_t = max_element(sum_eval.begin(), sum_eval.end());
	const int max_index = int(std::distance(sum_eval.begin(), max_t));
	char max_name[50];
	sprintf_s(max_name, "./nim/%d/%d/%d/best.dat", method, trial, g);
	pop[max_index].output_stra(max_name);
	nim2.nim_evaluation(pop[max_index].stra);
}
void Coans_base::cal_rate() 
{
	nim nim2(1);
	std::vector<double> sum_eval(KO);
	for (int i = 0; i < KO; i++) {
		sum_eval[i] = nim2.nim_evaluation(pop[i].stra)*100;
	}
	const auto max = max_element(sum_eval.begin(), sum_eval.end());
	const auto min = min_element(sum_eval.begin(), sum_eval.end());
	const double ave = std::accumulate(sum_eval.begin(), sum_eval.end(), 0.0)/KO;
	std::cout << "�@max currect : " << *max << " %" << std::endl;
	std::cout << "�@min currect : " << *min << " %" << std::endl;
	std::cout << "�@ave currect : " << ave << " %" << std::endl;
}
void Coans_base::asort_index_H(std::vector<std::vector<int>> &cr_index)
{
	std::vector<int> cr_pop(KO);
	for (int i = 0; i < KO; i++) {
		cr_pop[i] = pop[i].nitch;
	}
	const int cr_len = *max_element(cr_pop.begin(), cr_pop.end());
	cr_index.resize(cr_len);
	//�N���X�^���Ɍ̂̃C���f�b�N�X���d�������ē����
	int size = 0;
	for (int i = 0; i < cr_len + 1; i++) {
		const int count = int(std::count(cr_pop.begin(), cr_pop.end(), i));
		if (count) {
			cr_index[size].resize(count);
			auto itr = std::find(cr_pop.begin(), cr_pop.end(), i);
			cr_index[size][0] = int(std::distance(cr_pop.begin(), itr));

			for (int j = 1; j < count; j++) {
				itr = std::find(itr + 1, cr_pop.end(), i);
				cr_index[size][j] = int(std::distance(cr_pop.begin(), itr));
			}
			size++;
		}
	}
	if (size < cr_len + 1) {
		const int delta = cr_len - size;
		for (int i = 0; i < delta; i++) {
			cr_index.erase(cr_index.end());
		}
	}
}

class Coans : public Coans_base{
//���J�����o
public :
	void main_task(); 	//��@�̃��C�����[�`��
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
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

	//�W�c������
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init_pn();
		pop[i].Init_stra();
	}
	std::cout << pop[0].stra.size() << std::endl;

	cr_p = std::vector<std::vector<int>>(gene / per + 1, std::vector<int>(KO, 0));
	nim nim(2); //�j���̃N���X��`

	std::cout << "Initiarized..." << std::endl;
	output_stra(0);
	cal_rate();
	std::cout << "Strategy0..." << std::endl;

	machup = 0;
	int machup_index = 1;
	double loop_start = clock();

	for (int Gene_Loop = 1; Gene_Loop < gene + 1; Gene_Loop++) {

		Crustering(); //�N���X�^�����O�B��@�ɂ���ĕς��

		//��e�I��
		const int main = GetRand_Int(KO);

		//���e������Έȉ��̏������s��
		if (pop[main].List1.empty()) {
			std::cout << "list1����ł� main_tasks" << std::endl;
			exit(EXIT_FAILURE);
		}
		else  {
			std::vector<int> sub(PARENT);
			std::vector<int> tmp_list = pop[main].List1;

			for (int i = 0; i < PARENT; i++) {
				const int index = GetRand_Int(int(tmp_list.size()));
				sub[i] = tmp_list[index];
				tmp_list.erase(tmp_list.begin() + index);
			}

			child.resize(CHILD + 1);
			//�g��XLM
			child[0] = pop[main];
			ExtensionXLM(main, sub, pop, child);

			Generate_Opp(); //�ΐ푊��̌̂�I��

			//�����őΐ�
			for (int i = 0; i < CHILD + 1; i++) {
				child[i].Result.assign(opp_num, 0);
				for (int j = 0; j < opp_num; j++, machup += 2) {
					//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
					//1���
					nim.input_stra(child[i].stra, opp[j].stra);	//���
					child[i].Result[j] += nim.nim_game(0)*WIN_FIRST;
					//���������ւ���2���
					nim.input_stra(opp[j].stra, child[i].stra);	//���
					child[i].Result[j] += nim.nim_game(1)*WIN_LAST;
				}
			}
			//�K���x�v�Z
			for (int i = 0; i < CHILD + 1; i++) {
				child[i].cal_fitness();
			}
			//Best�̂��W�c��
			const int index = Choice_Best_Index();
			if (WIN_RATE <= (child[index].eval/opp_num)*100 ) {
				pop[main] = child[index];
			}
			//�����p : �헪�����o��
			if ( BATTLE_PER*machup_index < machup ) {
				const double loop_end = clock();
				printf("%d:%d:%d/%d:%I64d�@...%d[sec]\n", method, trial, Gene_Loop, KU, machup, int((loop_end - loop_start) / CLOCKS_PER_SEC));

				output_stra(machup_index);
				cal_best(machup_index);
				cal_rate();
				machup_index++;
				loop_start = clock();
			}
			//�W�c�̉��ȊO������
			for (int i = 0; i < KO; i++) {
				pop[i].Init_pn();
			}
		}
	}
}
void Coans::Crustering1() {
	//�ߖT���X�g�������N���X�^�ԍ�����U��
	MakeList(pop, K_List1, 0, 0);
	/*
	cr_num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(cr_num, i, pop) == 1) {
			cr_num++;
		}
	}
	*/
	Cru_Upgma(pop);
	cr_num = K_UPGMA;
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
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
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
	}
private:
	//�N���X�^�����O�Ɏg���p�����[�^�[
	virtual void Crustering();
	virtual void Generate_Opp();
};
void coans_mode3::Crustering() {
	//�ߖT���X�g����
	MakeList(pop, K_List1, false, false);
	//�N���X�^�ԍ�����U��
	//Crustering1();
}
void coans_mode3::Generate_Opp()
{
	Generate_Opp_all();
}

class Coans_s : public Coans_base {
	//���J�����o
public:
	void main_task(); 	//��@�̃��C�����[�`��
	void exp_upgma();
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
protected:
	std::vector<playerNim> local_s;

	void child2sub(const std::vector<int> &subpare, const std::vector<playerNim> &child);
	void create_pop();
	void create_pop_with_sol();
	void cal_fitness_alfa(std::vector<int>& sub, std::vector<playerNim>& child);
	int get_best();
	void ans_nitch();
	double distance_disper(const std::vector<int> &index, std::vector<double> &gra);
	void cal_gravity_nitch(const std::vector<int> &index, std::vector<double> &gra);
};
void Coans_s::main_task() {
	std::cout << "��@:" << method << std::endl;
	std::cout << "���㐔:" << gene << std::endl;
	std::cout << "���s��:" << trial << std::endl;
	std::cout << "�W�c�̐�:" << KO << std::endl;
	std::cout << "�q�̐�:" << CHILD << std::endl;
	int save_index = 0;
	for (int cycle1 = 0; cycle1 < CYCLE; cycle1++) {
		std::cout << "reboot" << std::endl;
		if (!local_s.empty()) {
			create_pop_with_sol();
		}
		else {
			create_pop();
		}
		cal_rate();

		machup = 0;
		int machup_index = 1;

		output_stra(save_index);
		double loop_start = clock();
		while (machup < END_GA) {
			Crustering(); //�N���X�^�����O
			//��e
			const int main = GetRand_Int(KO);
			//���e������Έȉ��̏������s��
			if (pop[main].List1.empty()) {
				std::cout << "list1����ł� main_tasks" << std::endl;
				exit(EXIT_FAILURE);
			}
			else {
				std::vector<int> sub(PARENT);
				std::vector<int> tmp_list1;
				tmp_list1 = pop[main].List1;
				for (int i = 0; i < PARENT; i++) {
					const int index = GetRand_Int(int(tmp_list1.size()));
					sub[i] = tmp_list1[index];
					tmp_list1.erase(tmp_list1.begin() + index);
				}

				child.resize(CHILD + 1);
				//�g��XLM
				child[0] = pop[main];
				ExtensionXLM(main, sub, pop, child);
				//EXLM_S(main, sub, pop, child);
				Generate_Opp(); //�ΐ푊��̌̂�I��

				nim nim(2); //�j���̃N���X��`
				for (int c = 0; c < CHILD + 1; c++) {
					child[c].Result.assign(opp_num, 0);
				}
				for (auto &pi : sub) {
					pop[pi].Result.assign(opp_num, 0);
				}
				for (int i = 0; i < opp_num; i++) {
					for (int c = 0; c < CHILD + 1; c++, machup += 2) {
						//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
						//1���
						nim.input_stra(child[c].stra, opp[i].stra);	//���
						child[c].Result[i] += nim.nim_game(0)*WIN_FIRST;
						//���������ւ���2���
						nim.input_stra(opp[i].stra, child[c].stra);	//���
						child[c].Result[i] += nim.nim_game(1)*WIN_LAST;
					}
					for (auto &pi : sub) {
						nim.input_stra(pop[pi].stra, opp[i].stra);	//���
						pop[pi].Result[i] += nim.nim_game(0)*WIN_FIRST;
						//���������ւ���2���
						nim.input_stra(opp[i].stra, pop[pi].stra);	//���
						pop[pi].Result[i] += nim.nim_game(1)*WIN_LAST;
						machup += 2;
					}
				}
				//�K���x�v�Z
				/*
				for (int i = 0; i < CHILD + 1; i++) {
					child[i].cal_fitness();
				}
				*/
				cal_fitness_alfa(sub, child);
				//Best�̂��W�c��
				const int index = Choice_Best_Index();
				if (index != 0) {
					if (child[0].eval < child[index].eval) {
						pop[main] = child[index];
					}
				}
				child2sub(sub, child);
				//�����p : �헪�����o��
				if (BATTLE_PER*machup_index < machup) {
					const double loop_end = clock();
					printf("%d:%d:%I64d/%d�@...%d[sec]\n", method, trial, machup, END_GA, int((loop_end - loop_start) / CLOCKS_PER_SEC));
					std::cout << "max rate :" << child[index].eval << std::endl;

					output_stra(save_index);
					cal_rate();
					machup_index++;
					save_index++;
					loop_start = clock();
				}
				//�W�c�̉��ȊO������
				for (int i = 0; i < KO; i++) {
					pop[i].Init_pn();
				}
			}
		}
		const int max_index = get_best();
		local_s.push_back(pop[max_index]);
	}
	
}
void Coans_s::child2sub(const std::vector<int> &subpare, const std::vector<playerNim> &child)
{
	for (auto &pi : subpare) {
		std::vector<double> dis(CHILD);

		for (int j = 1; j < CHILD + 1; j++) {
			dis[j - 1] = cal_euclidean(pop[pi].stra, child[j].stra);
		}
		const auto min = min_element(dis.begin(), dis.end());
		const int min_index = int(std::distance(dis.begin(), min));

		if (pop[pi].eval < child[min_index + 1].eval) {
			pop[pi] = child[min_index + 1];
		}
	}
}
void Coans_s::create_pop() 
{
	//�헪����
	std::vector<p_data> tmp_pop(KO_L);
	for (int i = 0; i < KO_L; i++) {
		tmp_pop[i].Init_stra();
	}
	//��������ŋ��������߂�
	int n = KO_L;
	std::vector<double> dis;
	while (KO < n) {
		dis.resize(n*n);
		for (int i = 0; i < n; i++) {
			for (int j = i; j < n; j++) {
				if (i == j) {
					dis[i*n + j] = 10000;
				}
				else {
					dis[i*n + j] = cal_euclidean(tmp_pop[i].stra, tmp_pop[j].stra);
					dis[j*n + i] = dis[i*n + j];
				}
			}
		}
		const auto min = min_element(dis.begin(), dis.end());
		const int index = int(std::distance(dis.begin(), min));
		const int pair[2] = { index / n, index % n };
		const int de_index = pair[ GetRand_Int(2) ];
		tmp_pop.erase(tmp_pop.begin() + de_index);
		n--;
	}
	std::cout << tmp_pop.size() << std::endl;
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].stra = tmp_pop[i].stra;
		pop[i].Init_pn();
	}
}
void Coans_s::create_pop_with_sol()
{
	const int sol_len = int(local_s.size());

	std::vector<p_data> tmp_pop(KO_L + sol_len);
	for (int i = 0; i < KO_L; i++) {
		tmp_pop[i].Init_stra();
	}
	for (int i = 0; i < sol_len; i++) {
		tmp_pop[KO_L + i].stra = local_s[i].stra;
	}

	int n = KO_L;
	std::vector<double> dis;
	while (KO < n) {
		dis.resize(n*n + sol_len*n);
		for (int i = 0; i < n; i++) {
			for (int j = i; j < n; j++) {
				if (i == j) {
					dis[i*n + j] = 10000;
				}
				else {
					dis[i*n + j] = cal_euclidean(tmp_pop[i].stra, tmp_pop[j].stra);
					dis[j*n + i] = dis[i*n + j];
				}
			}
		}
		for (int i = 0; i < sol_len; i++) {
			for (int j = 0; j < n; j++) {
				dis[n*n + i*j] = cal_euclidean(tmp_pop[n+i].stra, tmp_pop[j].stra);
			}
		}
		const auto min = min_element(dis.begin(), dis.end());
		const int index = int(std::distance(dis.begin(), min));

		int de_index;
		if (n*n <= index) {
			de_index = index % n;
		}
		else {
			const int pair[2] = { index / n, index % n };
			de_index = pair[GetRand_Int(2)];
		}
		tmp_pop.erase(tmp_pop.begin() + de_index);

		n--;
	}
	std::cout << tmp_pop.size() << std::endl;
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].stra = tmp_pop[i].stra;
		pop[i].Init_pn();
	}
}
void Coans_s::cal_fitness_alfa(std::vector<int>& sub, std::vector<playerNim>& child)
{
	for (int i = 0; i < opp_num; i++) {
		opp[i].cal_fitness();
	}
	const int child_len = int(child.size());
	for (int i = 0; i < child_len; i++) {
		child[i].eval = 0;
		for (int j = 0; j < opp_num; j++) {
			child[i].eval += child[i].Result[j] + opp[j].eval*ALFA;
		}
	}
	for (auto &pi : sub) {
		for (int j = 0; j < opp_num; j++) {
			pop[pi].eval += pop[pi].Result[j] + opp[j].eval*ALFA;
		}
	}
}
void Coans_s::cal_gravity_nitch(const std::vector<int> &index, std::vector<double> &gra)
{
	const int index_len = int(index.size());
	std::vector<std::vector<double>> w(index_len);

	for (int i = 0; i < index_len; i++) {
		w[i] = pop[index[i]].stra;
	}
	cal_gravity(w, gra);
}
double Coans_s::distance_disper(const std::vector<int> &index, std::vector<double> &gra)
{
	//�d�S����̋����̕��U�����߂�
	const int index_len = int(index.size());
	//�d�S
	cal_gravity_nitch(index, gra);
	//���U
	std::vector<double> dis2gra(index_len);
	for (int i = 0; i < index_len; i++) {
		dis2gra[i] = cal_euclidean(pop[index[i]].stra, gra);
	}
	const double disper = cal_dispersion_1(dis2gra);
	return disper;
}
int Coans_s::get_best()
{
	nim nim2(1);
	for (int i = 0; i < KO; i++) {
		pop[i].Result.assign(KO, 0);
	}

	for (int i = 0; i < KO; i++) {
		for (int j = i; j < KO; j++) {
			//1���
			double value;
			nim2.input_stra(pop[i].stra, pop[j].stra);
			value = nim2.nim_game(0);
			if (value == 1) {
				pop[i].Result[j] += WIN_FIRST;
			}
			else {
				pop[j].Result[i] += WIN_LAST;
			}
			//���������ւ���2���
			nim2.input_stra(pop[j].stra, pop[i].stra);
			value = nim2.nim_game(1);
			if (value == 0) {
				pop[i].Result[j] += WIN_LAST;
			}
			else {
				pop[j].Result[i] += WIN_FIRST;
			}
		}
	}
	std::vector<double> sum_eval(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].cal_fitness();
		sum_eval[i] = pop[i].eval;
	}
	const auto max_t = max_element(sum_eval.begin(), sum_eval.end());
	const int max_index = int(std::distance(sum_eval.begin(), max_t));
	return max_index;
}
void Coans_s::exp_upgma()
{
	std::stringstream fname;
	fname << "./nim/" << method << "0";
	const int folder_num = count_folder(fname.str());

	std::vector<std::vector<double>> cr_disper(folder_num);

	std::cout << "folder : " << folder_num << std::endl;

	pop.resize(KO);
	nim nim2(1);
	for (int f = 0; f < folder_num; f++) {
		for (int i = 0; i < KO; i++) {
			//�̏��C���v�b�g
			char fname[50];
			sprintf_s(fname, "./nim/%d/%d/%d/%d.dat", method, 0, f, i);
			pop[i].input_stra(fname);
			pop[i].Init_pn();
			pop[i].eval = nim2.nim_evaluation(pop[i].stra) * 100;
		}
		Crustering(); //�N���X�^�����O

		std::vector<std::vector<int>> cr_index;
		asort_index_H(cr_index);

		const int size = int(cr_index.size());
		//�d�S
		std::vector<std::vector<double>> cr_gra(size);
		//���U
		std::vector<double> cr_disper(size);
		for (int i = 0; i < size; i++) {
			//���U�Əd�S
			cr_disper[i] = distance_disper(cr_index[i], cr_gra[i]);
		}
		//const double disper = cal_dispersion_1(cr_gra);
		show_vec_1(cr_disper);
		std::cout << "folder : " << f << "...end" << std::endl;
	}
}
void Coans_s::ans_nitch() {
	MakeList(pop, 0, K_List2, 0);
	int cr_para = 0;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(cr_para, i, pop) == 1) {
			cr_para++;
		}
	}
}

class mode4 : public Coans_s {
public:
	mode4(std::string str, int t) {
		dir = str;
		method = 4;
		trial = t;
		machup = 0;					//�ΐ��
		local_s.clear();
	}
private:
	virtual void Crustering();
	virtual void Generate_Opp();
};
void mode4::Crustering() {
	//�ߖT���X�g����
	MakeList(pop, K_List1, 0, 0);
	Cru_Upgma(pop);
	cr_num = K_UPGMA;
}
void mode4::Generate_Opp()
{
	choice_oppoment(pop, opp, cr_num);
	opp_num = int(opp.size());
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