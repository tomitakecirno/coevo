#pragma once
/*-----------------------------------
ANSを使った提案手法の関数まとめ
-----------------------------------*/
#include "config_nim.hpp"
#include "player_nim.h"
#include "nim.h"

template<class Vec>
double cal_euclidean(const Vec &one, const Vec &ano);
void AnsList1(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list1, int index);
void AnsList2(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list2, int index);
void AnsList3(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list3, int index);
void MakeList(std::vector<playerNim> &pop, int Para_KL1, int Para_KL2, int Para_KL3);
int SetNitch(int nitch_number, int kotai, std::vector<playerNim> &pop);
void ExtensionXLM(const int main_pare, const std::vector<int> &sub_pare, std::vector<playerNim> &pop, std::vector<playerNim> &child);
void choice_oppoment(std::vector<playerNim> &pop, std::vector<playerNim> &opp, const int count_nitch);

template<class Vec>
double cal_euclidean(const Vec &one, const Vec &ano) {
	int one_size = int(one.size());
	int ano_size = int(ano.size());
	if (one_size != ano_size) {
		std::cout << "ベクターのサイズが違います" << std::endl;
		exit(EXIT_FAILURE);
	}
	double sum = 0;
	for (int i = 0; i < one_size; i++) {
		sum += (one[i] - ano[i]) * (one[i] - ano[i]);
	}
	return std::sqrt(sum);
}
void cal_gravity(const std::vector<std::vector<double>> &w, std::vector<double> &gra)
{
	const int w_len = int(w.size());
	gra.assign(W_SIZE, 0);

	for (auto &pi : w) {
		for (int i = 0; i < W_SIZE; i++) {
			gra[i] += pi[i];
		}
	}
	for (int i = 0; i < W_SIZE; i++) {
		gra[i] /= w_len;
	}
}
double cal_dispersion_1(const std::vector<double> &eval) 
{
	const double ave = std::accumulate(eval.begin(), eval.end(), 0.0)/ eval.size();

	double disper = 0;
	for (auto &pi : eval) {
		disper += (pi - ave) * (pi - ave);
	}
	return disper;
}
double cal_dispersion_2(const std::vector<std::vector<double>> &eval)
{
	const int len = int(eval.size());
	const int len2 = int(eval[0].size());

	std::vector<double> sum(len2, 0);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len2; j++) {
			sum[j] += eval[i][j];
		}
	}
	for (int i = 0; i < len2; i++) {
		sum[i] /= len;
	}
	std::vector<double> dis(len);
	for (int i = 0; i < len; i++) {
		dis[i] = cal_euclidean(eval[i],sum);
	}
	double disper;
	return disper;
}

//近傍リストを作る
void MakeList(std::vector<playerNim> &pop, int Para_KL1, int Para_KL2, int Para_KL3)
{
	using namespace std;
	vector<vector<double>>	DisSaveList1(KO);
	vector<vector<double>>	DisSaveList2(KO);
	vector<vector<double>>	DisSaveList3(KO);
	vector<vector<int>>	IndexSaveList1(KO); //K番目以内の個体を記録しておく
	vector<vector<int>>	IndexSaveList2(KO); //K番目以内の個体を記録しておく
	vector<vector<int>>	IndexSaveList3(KO); //K番目以内の個体を記録しておく

	/*自身以外の個体との距離を総当たりで計測.自身も含む.*/
	/*ついでに近い順に個体の番号も取得*/
	for (int i = 0; i < KO; i++) {
		DisSaveList1[i].resize(KO);
		for (int j = 0; j < KO; j++) {
			if (i != j) {
				DisSaveList1[i][j] = cal_euclidean(pop[i].stra, pop[j].stra);

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
	//近傍リスト1
	if (IndexSave[index].empty()) {} //空だったら何もしない
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
	//近傍リスト2
	if (IndexSave[index].empty()) {} //空だったら何もしない
	else {
		int IndexSave_len = int(IndexSave[index].size());
		for (int j = 0; j < IndexSave_len; j++) {
			int tmp = IndexSave[index][j]; //一旦インデックスを保存
			if (count(IndexSave[tmp].begin(), IndexSave[tmp].end(), index) != 0) {
				list2.push_back(tmp); //相手にも存在すれば近傍リストへ加える
			}
		}
	}
}
//List3
void AnsList3(const std::vector<std::vector<int>> &IndexSave, std::vector<int> &list3, int index)
{
	//近傍リスト1
	int IndexSave_len = int(IndexSave[index].size());
	for (int j = 0; j < IndexSave_len; j++) {
		//i=jのときは何もしない
		if (index == j) {}
		else {
			if (count(IndexSave[j].begin(), IndexSave[j].end(), index) != 0) {
				list3.push_back(j);
			}
		}
	}
}
//再帰的にニッチを割り当てていく
int SetNitch(int nitch_number, int kotai, std::vector<playerNim> &pop)
{
	if (pop[kotai].nitch < 0) {
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
//EXLM
void ExtensionXLM(const int main_pare, const std::vector<int> &sub_pare, std::vector<playerNim> &pop, std::vector<playerNim> &child)
{
	const int sub_len = int(sub_pare.size());

	//重心から親に対するベクトルを求める
	std::vector<std::vector<double>> sub_delta(sub_len);
	for (int i = 0; i<sub_len; i++) {
		sub_delta[i].resize(W_SIZE);
		for (int j = 0; j < W_SIZE; j++) {
			sub_delta[i][j] = pop[sub_pare[i]].stra[j] - pop[main_pare].stra[j];
		}
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<> dist(0.0, 1.0 / (std::sqrt(PARENT)));
	//子個体の戦略生成
	for (int c = 1; c < CHILD + 1; c++) {
		//初期化
		child[c].Init_pn();
		child[c].stra.resize(W_SIZE);
		for (int i = 0; i < PARENT; i++) {
			const double coe = dist(mt);
			for (int j = 0; j < W_SIZE; j++) {
				child[c].stra[j] += sub_delta[i][j] * coe;
			}
		}
		//show_vec_1(child[c].stra);
	}
}
//斎藤さんのEXLM
void EXLM_S(const int main_pare, const std::vector<int> &sub_pare, const std::vector<playerNim> &pop, std::vector<playerNim> &child)
{
	const int sub_len = int(sub_pare.size());
	
	std::vector<p_data> sub_2(sub_len*2);
	for (int i = 0; i < sub_len; i++) {
		sub_2[i].stra = pop[sub_pare[i]].stra;

		sub_2[i + sub_len].stra.resize(W_SIZE);
		for (int j = 0; j < W_SIZE; j++) {
			const double tmp_w = pop[sub_pare[i]].stra[j] - pop[main_pare].stra[j];
			sub_2[i + sub_len].stra[j] = pop[main_pare].stra[j] - tmp_w;
		}
	}
	std::vector<double> tmp_eval(sub_len * 2, 0);
	nim nim(2);
	for (int i = 0; i < sub_len*2; i++) {
		for (int j = i; j < sub_len*2; j++) {
			//先行
			nim.input_stra(sub_2[i].stra, sub_2[j].stra);
			if (nim.nim_game(0)) {
				tmp_eval[i] += WIN_FIRST;
			}
			else {
				tmp_eval[j] += WIN_LAST;
			}
			//後攻
			nim.input_stra(sub_2[j].stra, sub_2[i].stra);
			if (nim.nim_game(1)) {
				tmp_eval[j] += WIN_FIRST;
			}
			else {
				tmp_eval[i] += WIN_LAST;
			}
		}
	}
	std::vector<int> max_sub(sub_len);
	for (int i = 0; i < sub_len; i++) {
		const auto max = max_element(tmp_eval.begin(), tmp_eval.end());
		const int index = int(std::distance(tmp_eval.begin(), max));
		max_sub[i] = index;
		tmp_eval[index] = -10000;
	}
	//重心を求める
	double sub_max_g[W_SIZE] = { 0 };
	for (int i = 0; i < W_SIZE; i++) {
		for (auto &pi : max_sub) {
			sub_max_g[i] += sub_2[pi].stra[i];
		}
		sub_max_g[i] /= sub_len;
	}
	//各副親から主親に対するベクトルを求める
	std::vector<std::vector<double>> sub_delta(sub_len * 2);
	for (int i = 0; i < sub_len * 2; i++) {
		sub_delta[i].resize(W_SIZE);
		for (int j = 0; j < W_SIZE; j++) {
			sub_delta[i][j] = sub_2[i].stra[j] - pop[main_pare].stra[j];
		}
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<> dist(0.0, 1.0 / (std::sqrt(sub_len * 2)));
	//子個体の戦略生成
	for (int c = 1; c < CHILD + 1; c++) {
		//初期化
		child[c].Init_pn();
		child[c].stra.resize(W_SIZE);
		for (int i = 0; i < sub_len * 2; i++) {
			const double coe = dist(mt);
			for (int j = 0; j < W_SIZE; j++) {
				child[c].stra[j] += sub_delta[i][j] * coe;
			}
		}
		//重心をスライド
		for (int i = 0; i < W_SIZE; i++) {
			child[c].stra[i] += sub_max_g[i];
		}
		//show_vec_1(child[c].stra);
	}
}
//対戦相手を選ぶ
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