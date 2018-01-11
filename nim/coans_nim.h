#pragma once
/*=======================================
自分の各種手法まとめ
CoansMode0	卒論の手法
CoansMode1	現手法：現評価方法(評価値に補正を加える)
CoansMode2	現手法：前評価方法(評価値に補正を加えない)
CoansMode3	前手法：現評価方法
CoansMode4	前手法：前評価方法
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

/*手法のクラス*/
class Coans_base {
public:
	__int64		Get_MatchUp_Num();			//対戦回数を取得
	//データ取りミスったときに使うやつ
protected:
	int method;						//手法比較の時に使う
	int gene;						//世代数
	int per;						//間隔
	int trial;
	int cr_num;						//クラスター番号
	int	cru_k;						//クラスタリングパラメーター
	int opp_num;
	int sel_child;
	__int64 machup;						//対戦回数

	std::string dir;
	std::vector<playerNim> pop;
	std::vector<playerNim> child;
	std::vector<playerNim> opp;

	int Choice_Best_Index(); //一番良い個体のインデックスを返す

	//実験用
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
		//クリア
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
		//クリア
		fname.str("");
		fname.clear(std::stringstream::goodbit);
	}

}
void Coans_base::output_cr_pop()
{
	if (cr_p.empty()) {
		std::cout << "cr_pが空です" << std::endl;
		exit(EXIT_FAILURE);
	}
	Make_CSV_Directory(method);
	const int vec_size = int(cr_p.size());
	std::vector<std::vector<int>> tmp_cr_p(vec_size);
	//クラスタ数でリサイズ
	for (int i = 0; i < vec_size; i++) {
		auto max = *max_element(cr_p[i].begin(), cr_p[i].end());
		tmp_cr_p[i].assign(max + 1, 0);
	}
	//クラスタの個体数をカウント後，ベクターの0の要素を削除
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

	//一旦格納
	for (int i = 0; i < child_len; i++) {
		tmp_eval[i] = child[i].eval;
	}
	auto max = max_element(tmp_eval.begin() + 1, tmp_eval.end());
	int index;
	//主親(index = 0)より評価値の高い個体がいれば改善
	//同じ評価地の個体が複数ある場合はランダム
	const int count = int(std::count(tmp_eval.begin() + 1, tmp_eval.end(), *max));
	//cout << "count_Num:" << count_Num << endl;
	if (count == 1) {
		//インデックスを取得
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
			//1回目
			double value;
			nim2.input_stra(pop[i].stra, pop[j].stra);
			value = nim2.nim_game(0);
			if (value == 1) {
				pop[i].Result[j] += WIN_FIRST;
			}
			else {
				pop[j].Result[i] += WIN_LAST;
			}
			//先手後手を入れ替えて2回目
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
	std::cout << "　max currect : " << *max << " %" << std::endl;
	std::cout << "　min currect : " << *min << " %" << std::endl;
	std::cout << "　ave currect : " << ave << " %" << std::endl;
}
void Coans_base::asort_index_H(std::vector<std::vector<int>> &cr_index)
{
	std::vector<int> cr_pop(KO);
	for (int i = 0; i < KO; i++) {
		cr_pop[i] = pop[i].nitch;
	}
	const int cr_len = *max_element(cr_pop.begin(), cr_pop.end());
	cr_index.resize(cr_len);
	//クラスタ毎に個体のインデックスを仕分けして入れる
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
//公開メンバ
public :
	void main_task(); 	//手法のメインルーチン
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
protected :
	void Crustering1();
	void Crustering2();
	void Generate_Opp_all();
	void Generate_Opp_per();
//以下実験用
};
void Coans::main_task()
{
	std::cout << "手法:" << method << std::endl;
	std::cout << "試行回数:" << trial << std::endl;
	std::cout << "クラスタ数:" << cru_k << std::endl;
	std::cout << "世代数:" << gene << std::endl;
	std::cout << "区切り:" << BATTLE_PER << std::endl;
	std::cout << "集団個体数:" << KO << std::endl;
	std::cout << "子個体数:" << PARENT << std::endl;
	std::cout << "親個体数:" << CHILD << std::endl;

	//集団初期化
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init_pn();
		pop[i].Init_stra();
	}
	std::cout << pop[0].stra.size() << std::endl;

	cr_p = std::vector<std::vector<int>>(gene / per + 1, std::vector<int>(KO, 0));
	nim nim(2); //ニムのクラス定義

	std::cout << "Initiarized..." << std::endl;
	output_stra(0);
	cal_rate();
	std::cout << "Strategy0..." << std::endl;

	machup = 0;
	int machup_index = 1;
	double loop_start = clock();

	for (int Gene_Loop = 1; Gene_Loop < gene + 1; Gene_Loop++) {

		Crustering(); //クラスタリング。手法によって変わる

		//主親選ぶ
		const int main = GetRand_Int(KO);

		//副親があれば以下の処理を行う
		if (pop[main].List1.empty()) {
			std::cout << "list1が空です main_tasks" << std::endl;
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
			//拡張XLM
			child[0] = pop[main];
			ExtensionXLM(main, sub, pop, child);

			Generate_Opp(); //対戦相手の個体を選ぶ

			//ここで対戦
			for (int i = 0; i < CHILD + 1; i++) {
				child[i].Result.assign(opp_num, 0);
				for (int j = 0; j < opp_num; j++, machup += 2) {
					//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
					//1回目
					nim.input_stra(child[i].stra, opp[j].stra);	//先手
					child[i].Result[j] += nim.nim_game(0)*WIN_FIRST;
					//先手後手を入れ替えて2回目
					nim.input_stra(opp[j].stra, child[i].stra);	//先手
					child[i].Result[j] += nim.nim_game(1)*WIN_LAST;
				}
			}
			//適応度計算
			for (int i = 0; i < CHILD + 1; i++) {
				child[i].cal_fitness();
			}
			//Best個体を集団へ
			const int index = Choice_Best_Index();
			if (WIN_RATE <= (child[index].eval/opp_num)*100 ) {
				pop[main] = child[index];
			}
			//実験用 : 戦略書き出し
			if ( BATTLE_PER*machup_index < machup ) {
				const double loop_end = clock();
				printf("%d:%d:%d/%d:%I64d　...%d[sec]\n", method, trial, Gene_Loop, KU, machup, int((loop_end - loop_start) / CLOCKS_PER_SEC));

				output_stra(machup_index);
				cal_best(machup_index);
				cal_rate();
				machup_index++;
				loop_start = clock();
			}
			//集団の解以外初期化
			for (int i = 0; i < KO; i++) {
				pop[i].Init_pn();
			}
		}
	}
}
void Coans::Crustering1() {
	//近傍リスト生成＆クラスタ番号割り振り
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
	//近傍リスト生成
	//MakeList(Pop, true, false, false);
	//クラスタ番号割り振り
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
	主親		:ランダム
	対戦相手	:クラスタの代表点
	評価		:ニムの評価に基づく
*/
class coans_mode2 : public Coans {
public:
	coans_mode2(std::string str, int g, int g_p, int t, int k = 0) {
		dir = str;
		method = 2;
		gene = g;						//世代数
		per = g_p;						//間隔
		trial = t;
		machup = 0;					//対戦回数
		cru_k = k;						//クラスタリングパラメーター
	}
private:
	//クラスタリングに使うパラメーター
	virtual void Crustering();
	virtual void Generate_Opp();
};
void coans_mode2::Crustering() {
	//近傍リスト生成
	//MakeList(Pop, true, false, false);
	//クラスタ番号割り振り
	Crustering1();
}
void coans_mode2::Generate_Opp() 
{
	Generate_Opp_per();
}

/*
	method3
	主親		:ランダム
	対戦相手	:集団全体
	評価		:ニムの評価に基づく
*/

class coans_mode3 : public Coans {
public:
	coans_mode3(std::string str, int g, int g_p, int t, int k = 0) {
		dir = str;
		method = 3;
		gene = g;						//世代数
		per = g_p;						//間隔
		trial = t;
		machup = 0;					//対戦回数
		cru_k = k;						//クラスタリングパラメーター
	}
private:
	//クラスタリングに使うパラメーター
	virtual void Crustering();
	virtual void Generate_Opp();
};
void coans_mode3::Crustering() {
	//近傍リスト生成
	MakeList(pop, K_List1, false, false);
	//クラスタ番号割り振り
	//Crustering1();
}
void coans_mode3::Generate_Opp()
{
	Generate_Opp_all();
}

class Coans_s : public Coans_base {
	//公開メンバ
public:
	void main_task(); 	//手法のメインルーチン
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
	std::cout << "手法:" << method << std::endl;
	std::cout << "世代数:" << gene << std::endl;
	std::cout << "試行回数:" << trial << std::endl;
	std::cout << "集団個体数:" << KO << std::endl;
	std::cout << "子個体数:" << CHILD << std::endl;
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
			Crustering(); //クラスタリング
			//主親
			const int main = GetRand_Int(KO);
			//副親があれば以下の処理を行う
			if (pop[main].List1.empty()) {
				std::cout << "list1が空です main_tasks" << std::endl;
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
				//拡張XLM
				child[0] = pop[main];
				ExtensionXLM(main, sub, pop, child);
				//EXLM_S(main, sub, pop, child);
				Generate_Opp(); //対戦相手の個体を選ぶ

				nim nim(2); //ニムのクラス定義
				for (int c = 0; c < CHILD + 1; c++) {
					child[c].Result.assign(opp_num, 0);
				}
				for (auto &pi : sub) {
					pop[pi].Result.assign(opp_num, 0);
				}
				for (int i = 0; i < opp_num; i++) {
					for (int c = 0; c < CHILD + 1; c++, machup += 2) {
						//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
						//1回目
						nim.input_stra(child[c].stra, opp[i].stra);	//先手
						child[c].Result[i] += nim.nim_game(0)*WIN_FIRST;
						//先手後手を入れ替えて2回目
						nim.input_stra(opp[i].stra, child[c].stra);	//先手
						child[c].Result[i] += nim.nim_game(1)*WIN_LAST;
					}
					for (auto &pi : sub) {
						nim.input_stra(pop[pi].stra, opp[i].stra);	//先手
						pop[pi].Result[i] += nim.nim_game(0)*WIN_FIRST;
						//先手後手を入れ替えて2回目
						nim.input_stra(opp[i].stra, pop[pi].stra);	//先手
						pop[pi].Result[i] += nim.nim_game(1)*WIN_LAST;
						machup += 2;
					}
				}
				//適応度計算
				/*
				for (int i = 0; i < CHILD + 1; i++) {
					child[i].cal_fitness();
				}
				*/
				cal_fitness_alfa(sub, child);
				//Best個体を集団へ
				const int index = Choice_Best_Index();
				if (index != 0) {
					if (child[0].eval < child[index].eval) {
						pop[main] = child[index];
					}
				}
				child2sub(sub, child);
				//実験用 : 戦略書き出し
				if (BATTLE_PER*machup_index < machup) {
					const double loop_end = clock();
					printf("%d:%d:%I64d/%d　...%d[sec]\n", method, trial, machup, END_GA, int((loop_end - loop_start) / CLOCKS_PER_SEC));
					std::cout << "max rate :" << child[index].eval << std::endl;

					output_stra(save_index);
					cal_rate();
					machup_index++;
					save_index++;
					loop_start = clock();
				}
				//集団の解以外初期化
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
	//戦略生成
	std::vector<p_data> tmp_pop(KO_L);
	for (int i = 0; i < KO_L; i++) {
		tmp_pop[i].Init_stra();
	}
	//総当たりで距離を求める
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
	//重心からの距離の分散を求める
	const int index_len = int(index.size());
	//重心
	cal_gravity_nitch(index, gra);
	//分散
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
			//1回目
			double value;
			nim2.input_stra(pop[i].stra, pop[j].stra);
			value = nim2.nim_game(0);
			if (value == 1) {
				pop[i].Result[j] += WIN_FIRST;
			}
			else {
				pop[j].Result[i] += WIN_LAST;
			}
			//先手後手を入れ替えて2回目
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
			//個体情報インプット
			char fname[50];
			sprintf_s(fname, "./nim/%d/%d/%d/%d.dat", method, 0, f, i);
			pop[i].input_stra(fname);
			pop[i].Init_pn();
			pop[i].eval = nim2.nim_evaluation(pop[i].stra) * 100;
		}
		Crustering(); //クラスタリング

		std::vector<std::vector<int>> cr_index;
		asort_index_H(cr_index);

		const int size = int(cr_index.size());
		//重心
		std::vector<std::vector<double>> cr_gra(size);
		//分散
		std::vector<double> cr_disper(size);
		for (int i = 0; i < size; i++) {
			//分散と重心
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
		machup = 0;					//対戦回数
		local_s.clear();
	}
private:
	virtual void Crustering();
	virtual void Generate_Opp();
};
void mode4::Crustering() {
	//近傍リスト生成
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

卒論手法のクラス

*/
/*
class Coans_GT2016 : public Coans {
	//公開メンバ
public:
	Coans_GT2016(std::string str, int g, int g_p, int t, int k = 0) {
		dir = str;
		method = 0;
		gene = g;						//世代数
		per = g_p;						//間隔
		trial = t;
		machup = 0;					//対戦回数
		cru_k = k;						//クラスタリングパラメーター
	}
	//手法のメインルーチン
	void	Coans_GT2016_Tasks();
	int		Get_MatchUp_Num();
private:
	//クラスタリングに使うパラメーター
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

	std::cout << "手法:" << method << std::endl;
	std::cout << "試行回数:" << trial << std::endl;
	std::cout << "クラスタ数:" << cru_k << std::endl;
	std::cout << "世代数:" << gene << std::endl;
	std::cout << "区切り:" << per << std::endl;
	std::cout << "集団個体数:" << KO << std::endl;
	std::cout << "子個体数:" << PARENT << std::endl;
	std::cout << "親個体数:" << CHILD << std::endl;

	//集団宣言
	pop.resize(KO);
	init_genrand((unsigned)time(NULL));

										//集団初期化
	for (int i = 0; i < KO; i++) {
		pop[i].Init_pn();
		pop[i].Init_stra();
	}

	nim nim; //ニムのクラス定義
	Make_Directory(dir, method, trial, gene, per, cru_k);
	//Csv_exp csv_exp(dir, method, trial, gene, per);

	//初期世代の戦略を記録
	machup = 0;
	//任意の世代数ループ
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
		Crustering(); //クラスタリング。手法によって変わる

		std::cout << "2" << ',';
		int tmp_index;
		int tmp_sub;
		std::vector<int> sub_pare;
		//主親選ぶ
		const int main_pare = GetRand_Int(KO);
		//副親を選ぶ

		std::vector<int> list1;
		if (!pop[main_pare].get_list(list1, 1)) {
			std::cout << "error main_task -> get_list" << std::endl;
			exit(EXIT_FAILURE);
		}
		int list1_len = int(list1.size());
		for (int i = 0; i < PARENT; i++) {
			if (list1.empty()) {
				std::cout << "近傍リストに十分な個体がありません" << std::endl;
				exit(EXIT_FAILURE);
			}
			tmp_index = GetRand_Int(list1_len);
			tmp_sub = list1[tmp_index];
			sub_pare.push_back(tmp_sub); //要素を追加
			list1.erase(list1.begin() + tmp_index); //追加した要素を削除
			list1_len--;
		}
		//副親があれば以下の処理を行う
		if (!sub_pare.empty()) {
			//子個体生成
			child.resize(CHILD + 1);
			std::cout << "3" << ',';

			//拡張XLM
			child[0] = pop[main_pare];
			binaryEXLM(main_pare, sub_pare, pop, child);

			std::cout << "4" << ',';
			Generate_Opp(); //対戦相手の個体を選ぶ


			std::cout << "5" << ',';

			//ここで対戦
			for (int i = 0; i < CHILD + 1; i++) {
				std::vector<double> result(opp_num, 0);
				for (int j = 0; j < opp_num; j++) {
					//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
					//1回目
					nim.input_stra_first(child[i]);	//先手
					nim.input_stra_last(opp[j]);	//後手
					result[j] += nim.nim_game()*WIN_FIRST;

					//先手後手を入れ替えて2回目
					nim.input_stra_first(opp[j]);	//先手
					nim.input_stra_last(child[i]);	//後手
					result[j] += nim.nim_game()*WIN_LAST;
				}
				child[i].put_result(result);
			}

			std::cout << "6" << ',';
			//適応度計算
			for (int i = 0; i < CHILD; i++) {
				child[i].cal_fitness();
			}

			std::cout << "7" << ',';
			//Best個体を集団へ
			int index = Choice_Best_Index();
			pop[main_pare] = child[index];

			std::cout << "8" << ',';
			//削除予定の個体があれば
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

			//実験用
			if (Gene_Loop % PER == 0) {
				std::cout << "10" << ',';
			}
			std::cout << "8";
			std::cout << "  [";
			for (int c = 0; c < CHILD + 1; c++) {
				std::cout << child[c].get_eval() << ",";
			}
			std::cout << "]" << std::endl;
			//集団の解以外初期化
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
	//近傍リスト生成＆クラスタ番号割り振り
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
	//いらない相手を残すと有害。勝ち数で残す個体判断しているので、余計な勝ち数がカウントされる.
	//距離が最小のニッチの重心を見つけて、ニッチ番号を取得
	int nitch_count = 0;
	const int child_nitch = Cal_Gra_Nitch(child);
	//一番近いクラスタの番号を入れる

	//戦闘データ初期化
	std::vector<int> opp_nitch(opp_num);
	for (int i = 0; i < opp_num; i++) {
		opp_nitch[i] = opp[i].get_nitch();
		if (opp_nitch[i] == child_nitch) {
			opp_flag[i] = 0;
			nitch_count++;
		}
	}
	//対戦用のベクターを作る
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

	//該当するニッチの個体と子個体でリーグ戦
	int Oppc_size = int(Oppc.size());
	for (int i = 0; i < Oppc_size; i++) {
		//自クラスタ内の個体と対戦
		Oppc[i].Result.resize(Oppc_size - 1);
		StrategySet_M(Oppc[i]);
		for (int j = i + 1; j < Oppc_size - 1; j++) {
			StrategySet_T(Oppc[j]);
			Competition();//対戦 player1 = 子個体 palyer2 = 対戦相手？
			Oppc[i].Result[j] = (player1.hp - player2.hp) / 300;
			Oppc[j].Result[i] = (player2.hp - player1.hp) / 300;
			Machup_Num++;
		}
		//子個体と対戦
		Opponent[i].flag = 1;
		Opponent[i].comp_flag = 1;
	}
	for (int i = 0; i < Oppc_size; i++) {
		Oppc_Eval[i] = std::accumulate(Oppc[i].Result.begin(), Oppc[i].Result.end(), 0.0);
	}

	//評価値の最小個体を求める
	auto min = min_element(Oppc_Eval.begin(), Oppc_Eval.end());
	int min_index = int(distance(Oppc_Eval.begin(), min));

	//削除予定の個体の枠へ子個体を入れる、または相手集団の中で子個体より弱い相手のところに入れる
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