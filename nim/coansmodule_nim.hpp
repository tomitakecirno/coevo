#pragma once
/*-----------------------------------
ANS���g������Ď�@�̊֐��܂Ƃ�
-----------------------------------*/
#include "config_nim.hpp"
#include "player_nim.h"

template<class Vec>
Vec cal_euclidean(const Vec &one, const Vec &ano);
void AnsList1(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list1, int index);
void AnsList2(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list2, int index);
void AnsList3(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list3, int index);
void MakeList(std::vector<playerNim> &pop, int Para_KL1, int Para_KL2, int Para_KL3);
int SetNitch(int nitch_number, int kotai, std::vector<playerNim> &pop);
void binaryEXLM(const int main_pare, const std::vector<int> &sub_pare, const std::vector<playerNim> &pop, std::vector<playerNim> &child);
void two_point_cross(const std::vector<int> &main, const std::vector<int> &sub, std::vector<std::vector<int>> &c_stra);
void choice_oppoment(std::vector<playerNim> &pop, std::vector<playerNim> &opp, const int count_nitch);
void mutation(std::vector<int> &stra);

//�n�~���O����
int cal_haming(const std::vector<int> &one, const std::vector<int> &ano)
{
	int one_size = int(one.size());
	int ano_size = int(ano.size());
	if (one_size != ano_size) {
		std::cout << "�x�N�^�[�̃T�C�Y���Ⴂ�܂�" << std::endl;
		exit(EXIT_FAILURE);
	}
	int sum = 0;
	for (int i = 0; i < one_size; i++) {
		if (one[i] != ano[i]) {
			sum++;
		}
	}
	return sum;
}
template<class Vec>
Vec cal_euclidean(const Vec &one, const Vec &ano) {
	int one_size = int(one.size());
	int ano_size = int(ano.size());
	if (one_size != ano_size) {
		std::cout << "�x�N�^�[�̃T�C�Y���Ⴂ�܂�" << std::endl;
		exit(EXIT_FAILURE);
	}
	int sum = 0;
	for (int i = 0; i < one_size; i++) {
		sum += (one[i] - ano[i]) * (one[i] - ano[i]);
	}
	return sqrt(sum);
}

//�ߖT���X�g�����
void MakeList(std::vector<playerNim> &pop, int Para_KL1, int Para_KL2, int Para_KL3)
{
	using namespace std;
	vector<vector<int>>	DisSaveList1(KO);
	vector<vector<int>>	DisSaveList2(KO);
	vector<vector<int>>	DisSaveList3(KO);
	vector<vector<int>>	IndexSaveList1(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���
	vector<vector<int>>	IndexSaveList2(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���
	vector<vector<int>>	IndexSaveList3(KO); //K�Ԗڈȓ��̌̂��L�^���Ă���

	/*���g�ȊO�̌̂Ƃ̋����𑍓�����Ōv��.���g���܂�.*/
	/*���łɋ߂����Ɍ̂̔ԍ����擾*/
	for (int i = 0; i < KO; i++) {
		DisSaveList1[i].resize(KO);
		for (int j = 0; j < KO; j++) {
			if (i != j) {
				DisSaveList1[i][j] = cal_haming(pop[i].stra, pop[j].stra);

			}
			else {
				DisSaveList1[i][j] = 100000;
			}
		}
	}
	//std::cout << "1-1-1" << ',';
	//��U�ڂ��Ă���
	DisSaveList2 = DisSaveList1;
	DisSaveList3 = DisSaveList1;
	for (int i = 0; i < KO; i++) {
		if (Para_KL1) {
			//IndexSaveList1�����
			IndexSaveList1[i].resize(Para_KL1);
			for (int j = 0; j < Para_KL1; j++) {
				auto min = min_element(DisSaveList1[i].begin(), DisSaveList1[i].end());
				size_t index = distance(DisSaveList1[i].begin(), min);
				IndexSaveList1[i][j] = int(index);
				DisSaveList1[i][index] = 100000;
			}
		}
		//IndexSaveList2�����
		if (Para_KL2) {
			IndexSaveList2[i].resize(Para_KL2);
			for (int j = 0; j < Para_KL2; j++) {
				auto min = min_element(DisSaveList2[i].begin(), DisSaveList2[i].end());
				size_t index = distance(DisSaveList2[i].begin(), min);
				IndexSaveList2[i][j] = int(index);
				DisSaveList2[i][index] = 100000;
			}
		}if (Para_KL3) {
			IndexSaveList3[i].resize(Para_KL3);
			for (int j = 0; j < Para_KL3; j++) {
				auto min = min_element(DisSaveList3[i].begin(), DisSaveList3[i].end());
				size_t index = distance(DisSaveList3[i].begin(), min);
				IndexSaveList3[i][j] = int(index);
				DisSaveList3[i][index] = 100000;
			}
		}
	}
	//std::cout << "1-1-2" << ',';
	//AnsList1
	for (int i = 0; i < KO; i++) {
		if (Para_KL1) {
			AnsList1(IndexSaveList1, pop[i].List1, i);
		}
		if (Para_KL2) {
			AnsList2(IndexSaveList2, pop[i].List2, i);
		}
		if (Para_KL3) {
			AnsList3(IndexSaveList3, pop[i].List3, i);
		}
	}
	//std::cout << "1-1-3" << ',';
}
//List1
void AnsList1(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list1, int index)
{
	//�ߖT���X�g1
	if (IndexSave[index].empty()) {} //�󂾂����牽�����Ȃ�
	else {
		int IndexSave_len = int(IndexSave[index].size());
		for (int j = 0; j < IndexSave_len; j++) {
			list1.push_back(IndexSave[index][j]);
		}
	}
}
//List2
void AnsList2(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list2, int index)
{
	//�ߖT���X�g2
	if (IndexSave[index].empty()) {} //�󂾂����牽�����Ȃ�
	else {
		int IndexSave_len = int(IndexSave[index].size());
		for (int j = 0; j < IndexSave_len; j++) {
			int tmp = IndexSave[index][j]; //��U�C���f�b�N�X��ۑ�
			if (count(IndexSave[tmp].begin(), IndexSave[tmp].end(), index) != 0) {
				list2.push_back(tmp); //����ɂ����݂���΋ߖT���X�g�։�����
			}
		}
	}
}
//List3
void AnsList3(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list3, int index)
{
	//�ߖT���X�g1
	int IndexSave_len = int(IndexSave[index].size());
	for (int j = 0; j < IndexSave_len; j++) {
		//i=j�̂Ƃ��͉������Ȃ�
		if (index == j) {}
		else {
			if (count(IndexSave[j].begin(), IndexSave[j].end(), index) != 0) {
				list3.push_back(j);
			}
		}
	}
}
//�ċA�I�Ƀj�b�`�����蓖�ĂĂ���
int SetNitch(int nitch_number, int kotai, std::vector<playerNim> &pop)
{
	if (pop[kotai].nitch == 0) {
		pop[kotai].nitch = nitch_number;
		if (!pop[kotai].List2.empty()) {
			const int list_len = int(pop[kotai].List2.size());
			for (int i = 0; i < list_len; i++) {
				int tmp = pop[kotai].List2[i];
				SetNitch(nitch_number, tmp, pop);
			}
		}
		return 1;
	}
	return 0;
}

//�g�UELM.w1,w2,w3���ꂼ�ꕪ���Đ�������.
void binaryEXLM(const int main_pare, const std::vector<int> &sub_pare, const std::vector<playerNim> &pop, std::vector<playerNim> &child)
{
	int sub_len = int(sub_pare.size());

	std::vector<std::vector<int>> sub_stra(sub_len);

	for (int c = 1; c < CHILD / 2 + 1; c++) {
		int rand = GetRand_Int(sub_len);
		std::vector<std::vector<int>> child_stra(2);

		two_point_cross(pop[main_pare].stra, pop[sub_pare[rand]].stra, child_stra);

		
		child[c].stra			  = child_stra[0];
		child[CHILD / 2 + c].stra = child_stra[1];
		mutation(child[c].stra);
		mutation(child[CHILD / 2 + c].stra);
		child[c].Init_pn();
		child[CHILD / 2 + c].Init_pn();;
	}
}
void two_point_cross(const std::vector<int> &parent1, const std::vector<int> &parent2, std::vector<std::vector<int>> &c_stra) 
{
	if (c_stra.size() < 2) {
		std::cout << "�q�̂̐헪������܂��� : two_point_cross" << std::endl;
		exit(EXIT_FAILURE);
	}
	int stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);
	//2�_������
	int one = 0;
	int two = 0;
	while (one == two) {
		one = GetRand_Int(stra_len);
		two = GetRand_Int(stra_len);
	}
	if (two < one) {
		int tmp;
		tmp = one;
		one = two;
		two = tmp;
	}

	//2�_����
	c_stra[0] = parent1;
	c_stra[1] = parent2;
	for (int i = one; i <= two; i++) {
		c_stra[0][i] = parent2[i];
		c_stra[1][i] = parent1[i];
	}
}
//�ˑR�ψ�
void mutation(std::vector<int> &stra) 
{
	for (int i = 0; i < STRA_LEN; i++) {
		const int rand = GetRand_Int(1000);
		if (1 < MUTATION*rand) {
			if (stra[i] == 0) {
				stra[i] = 1;
			}
			else {
				stra[i] = 0;
			}
		}
	}
}
//�ΐ푊���I��
void choice_oppoment(std::vector<playerNim> &pop, std::vector<playerNim> &opp, const int count_nitch)
{
	std::vector<int> tmpindex;
	std::vector<int> tmp_nitch(KO);
	for (int i = 0; i < KO; i++) {
		tmp_nitch[i] = pop[i].nitch;
	}
	int size = 0;
	for (int i = 0; i < count_nitch; i++) {
		if (std::count(tmp_nitch.begin(), tmp_nitch.end(), i)) {
			size++;
		}
	}
	int tmp_size = size;
	std::vector<std::vector<int>> tmpCluster(size); //index������
	for (int i = 0; i < count_nitch; i++) {
		//���T�C�Y
		int count = int(std::count(tmp_nitch.begin(), tmp_nitch.end(), i));
		if (count > 0) {
			size--;
			tmpCluster[size].resize(count);
			//�i�[
			for (int j = 0; j < KO; j++) {
				if (tmp_nitch[j] == i) {
					count--;
					tmpCluster[size][count] = j;
				}
			}
		}
	}
	opp.resize(tmp_size);
	for (int i = 0; i < tmp_size; i++) {
		int len = int(tmpCluster[i].size());
		int rand = GetRand_Int(len);
		int tmp_index = tmpCluster[i][rand];
		opp[i] = pop[tmp_index];
	}
}