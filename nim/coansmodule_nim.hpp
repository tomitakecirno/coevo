#pragma once
/*-----------------------------------
ANSを使った提案手法の関数まとめ
-----------------------------------*/
#include "config_nim.hpp"
#include "player_nim.h"

template<class Vec>
Vec cal_euclidean(const Vec &one, const Vec &ano);
void AnsList1(std::vector<std::vector<int>> &IndexSave, std::vector<playerNim> &pop);
void AnsList2(std::vector<std::vector<int>> &IndexSave, std::vector<playerNim> &pop);
void AnsList3(std::vector<std::vector<int>> &IndexSave, std::vector<playerNim> &pop);
void MakeList(std::vector<playerNim> &pop, int Para_KL1, int Para_KL2, int Para_KL3);
int SetNitch(int nitch_number, int kotai, std::vector<playerNim> &pop);
void binaryEXLM(const int main_pare, const std::vector<int> &sub_pare, std::vector<playerNim> &pop, std::vector<playerNim> &child);
void two_point_cross(const std::vector<int> &main, const std::vector<int> &sub, std::vector<std::vector<int>> &c_stra);
void choice_oppoment(std::vector<playerNim> &pop, std::vector<playerNim> &opp, const int count_nitch);

int cal_haming(const std::vector<int> &one, const std::vector<int> &ano)
{
	int one_size = int(one.size());
	int ano_size = int(ano.size());
	if (one_size != ano_size) {
		std::cout << "ベクターのサイズが違います" << std::endl;
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
		std::cout << "ベクターのサイズが違います" << std::endl;
		exit(EXIT_FAILURE);
	}
	int sum = 0;
	for (int i = 0; i < one_size; i++) {
		sum += (one[i] - ano[i]) * (one[i] - ano[i]);
	}
	return sqrt(sum);
}

//近傍リストを作る
void MakeList(std::vector<playerNim> &pop, int Para_KL1, int Para_KL2, int Para_KL3)
{
	using namespace std;
	vector<vector<int>>	DisSaveList1(KO);
	vector<vector<int>>	DisSaveList2(KO);
	vector<vector<int>>	DisSaveList3(KO);
	vector<vector<int>>	IndexSaveList1(KO); //K番目以内の個体を記録しておく
	vector<vector<int>>	IndexSaveList2(KO); //K番目以内の個体を記録しておく
	vector<vector<int>>	IndexSaveList3(KO); //K番目以内の個体を記録しておく

	/*自身以外の個体との距離を総当たりで計測.自身も含む.*/
	/*ついでに近い順に個体の番号も取得*/
	std::vector<int> stra1;
	std::vector<int> stra2;
	for (int i = 0; i < KO; i++) {
		pop[i].get_stra(stra1);
		DisSaveList1[i].resize(KO);
		for (int j = 0; j < KO; j++) {
			if (i != j) {
				pop[j].get_stra(stra2);
				DisSaveList1[i][j] = cal_haming(stra1, stra2);

			}
			else {
				DisSaveList1[i][j] = 100000;
			}
		}
	}
	//std::cout << "1-1-1" << ',';
	//一旦移しておく
	DisSaveList2 = DisSaveList1;
	DisSaveList3 = DisSaveList1;
	for (int i = 0; i < KO; i++) {
		if (Para_KL1) {
			//IndexSaveList1を作る
			IndexSaveList1[i].resize(Para_KL1);
			for (int j = 0; j < Para_KL1; j++) {
				auto min = min_element(DisSaveList1[i].begin(), DisSaveList1[i].end());
				size_t index = distance(DisSaveList1[i].begin(), min);
				IndexSaveList1[i][j] = int(index);
				DisSaveList1[i][index] = 100000;
			}
		}
		//IndexSaveList2を作る
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
	if(Para_KL1){
		AnsList1(IndexSaveList1, pop);
	}
	if (Para_KL2) {
		AnsList2(IndexSaveList2, pop);
	}
	if (Para_KL3) {
		AnsList3(IndexSaveList3, pop);
	}
	//std::cout << "1-1-3" << ',';
}
//List1
void AnsList1(std::vector<std::vector<int>> &IndexSave, std::vector<playerNim> &pop)
{
	//近傍リスト1
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //空だったら何もしない
		else {
			std::vector<int> tmp_list1;
			int IndexSave_len = int(IndexSave[i].size());
			for (int j = 0; j<IndexSave_len; j++) {
				tmp_list1.push_back(IndexSave[i][j]);
			}
			pop[i].input_list(tmp_list1, 1);
		}
	}
}
//List2
void AnsList2(std::vector< std::vector<int> > &IndexSave, std::vector<playerNim> &pop)
{
	//近傍リスト2
	for (int i = 0; i<KO; i++) {
		if (IndexSave[i].empty()) {} //空だったら何もしない
		else {
			std::vector<int> tmp_list2;
			int IndexSave_len = int(IndexSave[i].size());
			for (int j = 0; j<IndexSave_len; j++) {
				int tmp = IndexSave[i][j]; //一旦インデックスを保存
				if (count(IndexSave[tmp].begin(), IndexSave[tmp].end(), i) != 0)
					tmp_list2.push_back(tmp); //相手にも存在すれば近傍リストへ加える
			}
			pop[i].input_list(tmp_list2, 2);
		}
	}
}
//List3
void AnsList3(std::vector< std::vector<int> > &IndexSave, std::vector<playerNim> &pop)
{
	//近傍リスト1
	for (int i = 0; i < KO; i++) {
		int IndexSave_len = int(IndexSave[i].size());
		for (int j = 0; j < IndexSave_len; j++) {
			//i=jのときは何もしない
			if (i == j) {}
			else {
				std::vector<int> tmp_list3;
				if ( count(IndexSave[j].begin(), IndexSave[j].end(), i) != 0) {
					tmp_list3.push_back(j);
				}
				pop[i].input_list(tmp_list3, 3);
			}
		}
	}
}

//再帰的にニッチを割り当てていく
int SetNitch(int nitch_number, int kotai, std::vector<playerNim> &pop)
{
	std::vector<int> list;
	if (pop[kotai].get_nitch() == 0) {
		pop[kotai].input_nitch(nitch_number);
		if (!pop[kotai].get_list(list,2)) {
			int list_len = int(list.size());
			for (int i = 0; i < list_len; i++) {
				int tmp = list[i];
				SetNitch(nitch_number, tmp, pop);
			}
		}
		return 1;
	}
	return 0;
}

//拡散ELM.w1,w2,w3それぞれ分けて生成する.
void binaryEXLM(const int main_pare, const std::vector<int> &sub_pare, std::vector<playerNim> &pop, std::vector<playerNim> &child)
{
	int sub_len = int(sub_pare.size());

	std::vector<int> main_stra;
	pop[main_pare].get_stra(main_stra);

	std::vector<std::vector<int>> sub_stra(sub_len);
	for (int i = 0; i < sub_len; i++) {
		pop[ sub_pare[i] ].get_stra(sub_stra[i]);
	}

	for (int c = 1; c < CHILD / 2 + 1; c++) {
		int rand = GetRand_Int(sub_len);
		std::vector<std::vector<int>> child_stra(2);

		two_point_cross(main_stra, sub_stra[rand], child_stra);

		child[c].playerNim::put_stra(child_stra[0]);
		child[CHILD / 2 + c].playerNim::put_stra(child_stra[1]);
	}
}
void two_point_cross(const std::vector<int> &main, const std::vector<int> &sub, std::vector<std::vector<int>> &c_stra) 
{
	if (c_stra.size() < 2) {
		std::cout << "子個体の戦略が足りません : two_point_cross" << std::endl;
		exit(EXIT_FAILURE);
	}

	int stra_len = int(std::pow(2, POLL1 + POLL2 + POLL3));

	//2点を決定
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

	//2点交叉
	c_stra[0] = main;
	c_stra[1] = sub;
	for (int i = one; i <= two; i++) {
		c_stra[0][i] = sub[i];
		c_stra[1][i] = main[i];
	}
}
//対戦相手を選ぶ
void choice_oppoment(std::vector<playerNim> &pop, std::vector<playerNim> &opp, const int count_nitch)
{
	std::vector<int> tmpindex;
	std::vector<int> tmp_nitch(KO);
	for (int i = 0; i < KO; i++) {
		tmp_nitch[i] = pop[i].get_nitch();
	}
	int size = 0;
	for (int i = 0; i < count_nitch; i++) {
		if (std::count(tmp_nitch.begin(), tmp_nitch.end(), i)) {
			size++;
		}
	}
	int tmp_size = size;
	std::vector<std::vector<int>> tmpCluster(size); //indexを入れる
	for (int i = 0; i < count_nitch; i++) {
		//リサイズ
		int count = int(std::count(tmp_nitch.begin(), tmp_nitch.end(), i));
		if (count > 0) {
			size--;
			tmpCluster[size].resize(count);
			//格納
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