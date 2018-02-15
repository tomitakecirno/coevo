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
void choice_oppoment(std::vector<playerNim> &pop, std::vector<playerNim> &opp, int count_nitch = 0);

template<class Vec>
double cal_euclidean(const Vec &one, const Vec &ano) {
	const int one_size = int(one.size());
	const int ano_size = int(ano.size());
	if (one_size != ano_size) {
		std::cout << "ベクターのサイズが違います -> cal_euclidean" << std::endl;
		std::cout << "one_size:" << one_size << std::endl;
		std::cout << "ano_size:" << ano_size << std::endl;
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
	const int w_len2 = int(w[0].size());

	gra.assign(w_len2, 0);

	for (auto &pi : w) {
		for (int i = 0; i < w_len2; i++) {
			gra[i] += pi[i];
		}
	}
	for (int i = 0; i < w_len2; i++) {
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

	std::vector<double> gra;
	std::vector<double> dis2gra(len);

	//重心と重心からの分散
	cal_gravity(eval, gra);
	for (int i = 0; i < len; i++) {
		dis2gra[i] = cal_euclidean(eval[i], gra);
	}
	const double disper = cal_dispersion_1(dis2gra);
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
	const int child_len = int(child.size());
	const int plus = (child_len == CHILD) ? 0 : 1;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<> dist(0.0, 1.0 / (std::sqrt(PARENT)));
	//子個体の戦略生成
	for (int c = plus; c < CHILD + plus; c++) {
		//初期化
		child[c].Init_pn();
		child[c].stra = pop[main_pare].stra;
		for (int i = 0; i < PARENT; i++) {
			const double coe = dist(mt);
			for (int j = 0; j < W_SIZE; j++) {
				child[c].stra[j] += sub_delta[i][j] * coe;
			}
		}
		if (GAME_NUM == 1) {
			vec2evalvec_nim(child[c].stra, child[c].nim_evaluation_vec);
		}
	}
}
//EXLM_REX
//斎藤さんのEXLMを共進化的に修正したもの
//ステップサイズを広げたい
void EXLM_S(const int main_pare, const std::vector<int> &sub_pare, const std::vector<playerNim> &pop, std::vector<playerNim> &child ,std::vector<playerNim> &opp, double t = STEP_SIZE)
{
	const int sub_len = int(sub_pare.size());

	//重心を求める
	double sub_g[W_SIZE] = { 0 };
	for (int i = 0; i < W_SIZE; i++) {
		for (int j = 0; j < sub_len; j++) {
			sub_g[i] += pop[sub_pare[j]].stra[i];
		}
		sub_g[i] /= sub_len * 2;
	}

	std::vector<playerNim> sub_2(sub_len * 2);
	for (int i = 0; i < sub_len; i++) {
		sub_2[i].stra = pop[sub_pare[i]].stra;

		sub_2[i + sub_len].stra.resize(W_SIZE);
		for (int j = 0; j < W_SIZE; j++) {
			const double tmp_w = pop[sub_pare[i]].stra[j] - sub_g[j];
			sub_2[i + sub_len].stra[j] = sub_g[j] - tmp_w;
		}
	}
	if (GAME_NUM == 1) {
		for (int i = 0; i < sub_len * 2; i++) {
			vec2evalvec_nim(sub_2[i].stra, sub_2[i].nim_evaluation_vec);
		}
	}
	//対戦で評価値を決める
	const int opp_len = int(opp.size());
	std::vector<double> sub_eval(sub_len * 2);

	competition_multi(sub_2, opp);
	for (int i = 0; i < sub_len * 2; i++) {
		sub_2[i].cal_fitness();
		sub_eval[i] = sub_2[i].eval;
	}
	std::vector<int> max_sub(sub_len);
	for (int i = 0; i < sub_len; i++) {
		const auto max = max_element(sub_eval.begin(), sub_eval.end());
		const int index = int(std::distance(sub_eval.begin(), max));
		max_sub[i] = index;
		sub_eval[index] = -10000;
	}
	double sub_max_g[W_SIZE] = { 0 };
	for (int i = 0; i < W_SIZE; i++) {
		for (auto &pi : max_sub) {
			sub_max_g[i] += sub_2[pi].stra[i];
		}
		sub_max_g[i] /= sub_len;
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(-std::sqrt((3 / (sub_len - 1)) + 1), std::sqrt((3 / (sub_len - 1)) + 1)); //乱数の範囲
	std::uniform_real_distribution<double> dist_t(0.0, STEP_SIZE); //乱数の範囲

	//子個体の戦略生成
	for (int c = 0; c < CHILD; c++) {
		//初期化
		child[c].Init_pn();
		child[c].stra = pop[main_pare].stra;
		//重心スライド追加
		for (int i = 0; i < W_SIZE; i++) {
			child[c].stra[i] += dist_t(mt)*(sub_max_g[i] - pop[main_pare].stra[i]);
		}
		//差分ベクトル
		for (int i = 0; i < sub_len * 2; i++) {
			const double coe = dist(mt);
			for (int j = 0; j < W_SIZE; j++) {
				child[c].stra[j] += (sub_2[i].stra[j] - pop[main_pare].stra[j]) * coe;
			}
		}
		if (GAME_NUM == 1) {
			vec2evalvec_nim(child[c].stra, child[c].nim_evaluation_vec);
		}
		//show_vec_1(child[c].stra);
	}
}
void rexSter_C(std::vector<playerNim> &child, std::vector<playerNim> &tmp_child, double t = STEP_SIZE)
{
	tmp_child.resize(CHILD);
	std::vector<double> tmp_eval(CHILD);
	for (int i = 0; i < CHILD + 1; i++) {
		tmp_child[i - 1] = child[i];
		tmp_eval[i - 1] = child[i].eval;
	}

	std::vector<int> better_index(CHILD / 2);
	for (int i = 0; i < CHILD / 2; i++) {
		const auto max = max_element(tmp_eval.begin(), tmp_eval.end());
		const int index = int(std::distance(tmp_eval.begin(), max));
		better_index[i] = index;
		tmp_eval[index] = -10000;
	}

	double sub_max_g[W_SIZE] = { 0 };
	for (int i = 0; i < W_SIZE; i++) {
		for (auto &pi : better_index) {
			sub_max_g[i] += tmp_child[pi].stra[i];
		}
		sub_max_g[i] /= (CHILD / 2);
	}
	double sub_g[W_SIZE] = { 0 };
	for (int i = 0; i < W_SIZE; i++) {
		for (int j = 0; j < CHILD; j++) {
			sub_g[i] += tmp_child[j].stra[i];
		}
		sub_g[i] /= CHILD;
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(-std::sqrt((3 / W_SIZE) + 1), std::sqrt((3 / W_SIZE) + 1)); //乱数の範囲
	std::uniform_real_distribution<double> dist_t(0.0, t); //乱数の範囲

	for (int c = 0; c < CHILD; c++) {
		//初期化
		child[c].Init_pn();
		for (int i = 0; i < W_SIZE; i++) {
			child[c].stra[i] = sub_g[i];
		}
		//重心スライド追加
		for (int i = 0; i < W_SIZE; i++) {
			child[c].stra[i] += dist_t(mt)*(sub_max_g[i] - sub_g[i]);
		}
		//差分ベクトル
		for (int i = 0; i < CHILD; i++) {
			const double coe = dist(mt);
			for (int j = 0; j < W_SIZE; j++) {
				child[c].stra[j] += (tmp_child[i].stra[j] - sub_g[j]) * coe;
			}
		}
		if (GAME_NUM == 1) {
			vec2evalvec_nim(child[c].stra, child[c].nim_evaluation_vec);
		}
		//show_vec_1(child[c].stra);
	}

}
//対戦相手を選ぶ
void choice_oppoment(std::vector<playerNim> &pop, std::vector<playerNim> &opp, int count_nitch)
{
	std::vector<int> tmpindex;
	std::vector<int> tmp_nitch(KO);
	for (int i = 0; i < KO; i++) {
		tmp_nitch[i] = pop[i].nitch;
	}
	int size = 0;
	const auto max = *max_element(tmp_nitch.begin(), tmp_nitch.end());
	for (int i = 0; i <= max; i++) {
		if (std::count(tmp_nitch.begin(), tmp_nitch.end(), i)) {
			if (i != size) {
				for (int j = 0; j < KO; i++) {
					if (tmp_nitch[j] == i) {
						tmp_nitch[j] = size;
						pop[j].nitch = size;
					}
				}
			}
			size++;
		}
	}
	std::vector<int> cr_num(size);
	for (int i = 0; i < size; i++) {
		cr_num[i] = int(std::count(tmp_nitch.begin(), tmp_nitch.end(), i));
	}
	std::vector<std::vector<int>> pop_index(size);
	int count_size = size;
	for (int i = 0; i < size; i++) {
		pop_index[i].resize(cr_num[i]);
		auto f_itr = std::find(tmp_nitch.begin(), tmp_nitch.end(), i);
		int index = int(std::distance(tmp_nitch.begin(), f_itr));
		pop_index[i][0] = index;
		for (int j = 1; j < cr_num[i]; j++) {
			f_itr = std::find(f_itr + 1, tmp_nitch.end(), i);
			index = int(std::distance(tmp_nitch.begin(), f_itr));
			pop_index[i][j] = index;
		}
	}
	opp.resize(size);
	for (int i = 0; i < size; i++) {
		const int len = cr_num[i];
		std::vector<std::vector<double>> stra(len);
		std::vector<double> gra, dis(len);
		for (int j = 0; j < cr_num[i]; j++) {
			stra[j] = pop[pop_index[i][j]].stra;
		}
		cal_gravity(stra, gra);
		for (int j = 0; j < len; j++) {
			dis[j] = cal_euclidean(stra[j], gra);
		}
		auto min = min_element(dis.begin(), dis.end());
		const int index = int(std::distance(dis.begin(), min));
		opp[i] = pop[pop_index[i][index]];
	}
}