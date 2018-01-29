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
	void exp_BestRate(const std::vector<int> &method);
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
	std::vector<double> cr_eval;
	std::vector<std::vector<int>> cr_index;

	int Choice_Best_Index(); //一番良い個体のインデックスを返す

	//実験用
	std::vector<std::vector<int>> cr_a_d;
	void input_stra(int g);
	void output_stra(int g);
	void cal_best(int g = 0);
	void cal_rate();
	void asort_index_H();
	double pseudoF();
	void cal_gravity_and_disper();
	void output_opp_stra(int g);
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
void Coans_base::output_opp_stra(int g)
{
	std::stringstream tmp_fname;
	for (int i = 0; i < opp_num; i++) {
		std::stringstream fname;

		fname << tmp_fname.str() << "/opp_" << i << ".dat";
		if (!opp[i].output_stra(fname.str())) {
			std::cout << "error : output_stra -> opp" << std::endl;
			exit(EXIT_FAILURE);
		}
		//クリア
		fname.str("");
		fname.clear(std::stringstream::goodbit);
	}
	printf("%d:%d:%d opp strategy out put ...\n", method, trial, g);
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
			std::cout << "error : output_stra -> pop" << std::endl;
			exit(EXIT_FAILURE);
		}
		//クリア
		fname.str("");
		fname.clear(std::stringstream::goodbit);
	}
	printf("%d:%d:%d strategy out put ...\n", method, trial, g);
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
		for (int j = 0; j < KO; j++) {
			//1回目
			if (nim2.nim_game(pop[i].stra, pop[j].stra)) {
				pop[i].Result[j]++;
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
	std::vector<double> pop_sum_eval(KO);
	for (int i = 0; i < KO; i++) {
		pop_sum_eval[i] = nim2.nim_evaluation(pop[i].stra) * 100;
	}
	//show_vec_1(sum_eval);
	double max = *max_element(pop_sum_eval.begin(), pop_sum_eval.end());
	double min = *min_element(pop_sum_eval.begin(), pop_sum_eval.end());
	double sum = std::accumulate(pop_sum_eval.begin(), pop_sum_eval.end(), 0.0)/ double(KO);
	std::cout << "　pop max currect : " << max << " %" << std::endl;
	std::cout << "　pop min currect : " << min << " %" << std::endl;
	std::cout << "　pop ave currect : " << sum << " %" << std::endl;

	std::vector<double> opp_eval(opp_num);
	for (int i = 0; i < opp_num; i++) {
		opp_eval[i] = nim2.nim_evaluation(opp[i].stra) * 100;
	}
	max = *max_element(opp_eval.begin(), opp_eval.end());
	min = *min_element(opp_eval.begin(), opp_eval.end());
	sum = std::accumulate(opp_eval.begin(), opp_eval.end(), 0.0)/ opp_num;

	std::cout << "　opp max currect : " << max << " %" << std::endl;
	std::cout << "　opp min currect : " << min << " %" << std::endl;
	std::cout << "　opp ave currect : " << sum << " %" << std::endl;
}
void Coans_base::asort_index_H()
{
	std::vector<int> cr_pop(KO);
	for (int i = 0; i < KO; i++) {
		cr_pop[i] = pop[i].nitch;
	}
	const int cr_len = *max_element(cr_pop.begin(), cr_pop.end());
	cr_index.resize(cr_len + 1);
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
double Coans_base::pseudoF() 
{
	/*
		クラスタの評価関数
		分子:クラスタ間分散，分母:クラスタ内分散
		値が大きいほど良い
	*/
	asort_index_H();

	int size = 0;
	double dis_T = 0;
	//std::vector<double> all_dis(KO*(KO - 1));
	for (int i = 0; i < KO; i++) {
		for (int j = i; j < KO; j++, size++) {
			dis_T += cal_euclidean(pop[i].stra, pop[j].stra);
		}
	}

	const int cr_len = int(cr_index.size());
	std::vector<double> cr_dis(cr_len, 0);
	for (int i = 0; i < cr_len; i++) {
		const int cr_len2 = int(cr_index[i].size());
		for (int j = 0; j < cr_len2; j++) {
			for (int k = j; k < cr_len2; k++) {
				cr_dis[i] += cal_euclidean(pop[cr_index[i][j]].stra, pop[cr_index[i][k]].stra);
			}
		}
	}
	const double W_k = std::accumulate(cr_dis.begin(), cr_dis.end(), 0.0);
	/*実験用*/
	const double de = W_k / (KO - cr_len);
	const double mo = (dis_T - W_k) / (cr_len - 1);
	const double value_pseudoF = mo / de;
	return value_pseudoF;
}
void Coans_base::cal_gravity_and_disper() 
{
	asort_index_H();

	const int cr_len = int(cr_index.size());
	std::vector<std::vector<double>> cr_gra(cr_len);
	std::vector<double> cr_disper(cr_len);
	std::vector<double> num(cr_len);

	for (int i = 0; i < cr_len; i++) {
		int size = 0;
		const int cr_len2 = int(cr_index[i].size());
		num[i] = cr_len2;
		std::vector<std::vector<double>> tmp_stra(cr_len2);
		for (int j = 0; j < cr_len2; j++) {
			tmp_stra[j] = pop[cr_index[i][j]].stra;
		}
		std::vector<double> tmp_dis(cr_len2*(cr_len2 - 1));
		if (cr_len2 > 1) {
			for (int j = 0; j < cr_len2; j++) {
				for (int k = j; k < cr_len2; k++, size++) {
					tmp_dis[size] = cal_euclidean(pop[cr_index[i][j]].stra, pop[cr_index[i][k]].stra);
				}
			}
		}
		else {
			tmp_dis.resize(1);
			tmp_dis[size] = 0;
			size++;
		}
		cr_disper[i] = cal_dispersion_1(tmp_dis);
		cal_gravity(tmp_stra, cr_gra[i]);
	}
	const double cr_num_disper = cal_dispersion_1(num);
	const double cr_disper_uchi = std::accumulate(cr_disper.begin(), cr_disper.end(), 0.0) / cr_disper.size();
	const double cr_disper_soto = cal_dispersion_2(cr_gra);
	std::cout << "disper( in : out : n ) = " << cr_disper_uchi << " : " << cr_disper_soto << " : " << cr_num_disper << std::endl;
}
void Coans_base::exp_BestRate(const std::vector<int> &method) 
{
	pop.resize(KO);
	nim nim(1);
	char fname[50];
	const int method_len = int(method.size());
	std::vector<int> gene(method_len);
	for (int i = 0; i < method_len; i++) {
		sprintf_s(fname, "%s/%d/%d", STRA_DIR, method[i], 0);
		gene[i] = count_folder(fname);
	}
	const int min_num = *min_element(gene.begin(), gene.end());

	std::vector<std::vector<double>> csv_max;
	std::vector<std::vector<double>> csv_min;
	std::vector<std::vector<double>> csv_ave;
	csv_max = std::vector<std::vector<double>>(min_num, std::vector<double>(method_len, 0));
	csv_min = csv_max;
	csv_ave = csv_max;
	//m
	for (int m = 0; m < method_len; m++) {
		sprintf_s(fname, "%s/%d", STRA_DIR, method[m]);
		const int trial = count_folder(fname);
		int gene;
		//t
		for (int t = 0; t < trial; t++) {
			sprintf_s(fname, "%s/%d/%d", STRA_DIR, method[m], t);
			gene = count_folder(fname);
			//g
			for (int g = 0; g < gene; g++) {
				printf("method:%d trial:%d generation:%d\n", method[m], t, g);
				std::vector<double> tmp_PopEval(KO);
				for (int i = 0; i < KO; i++) {
					//個体情報インプット
					sprintf_s(fname, "%s/%d/%d/%d/%d.dat", STRA_DIR, method[m], t, g, i);
					pop[i].input_stra(fname);
					//pop[i].Init_stra();
					tmp_PopEval[i] = nim.nim_evaluation(pop[i].stra) * 100;
				}
				csv_max[g][m] += *max_element(tmp_PopEval.begin(), tmp_PopEval.end());
				csv_min[g][m] += *min_element(tmp_PopEval.begin(), tmp_PopEval.end());
				csv_ave[g][m] += accumulate(tmp_PopEval.begin(), tmp_PopEval.end(), 0.0) / KO;
			}
		}
		for (int g = 0; g < gene; g++) {
			csv_max[g][m] /= trial;
			csv_min[g][m] /= trial;
			csv_ave[g][m] /= trial;
		}
	}
	CsvModules::csv_fwrite("method_max.csv", csv_max);
	CsvModules::csv_fwrite("method_min.csv", csv_min);
	CsvModules::csv_fwrite("method_ave.csv", csv_ave);
}

class Coans : public Coans_base{
//公開メンバ
public :
	void main_task(); 	//手法のメインルーチン
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
protected :
	std::vector<double> pop_eval;
	void Crustering1();
	void Crustering2();
	void Generate_Opp_all();
	void Generate_Opp_per();
	bool end_decision();
//以下実験用
};
void Coans::main_task()
{
	std::cout << "method     :" << method << std::endl;
	std::cout << "trial      :" << trial << std::endl;
	std::cout << "pop   size   :" << KO << std::endl;
	std::cout << "pare  size   :" << PARENT << std::endl;
	std::cout << "child size :" << CHILD << std::endl;

	//集団初期化
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init_pn();
		pop[i].Init_stra();
	}
	pop_eval.assign(KO, 0);
	std::cout << pop[0].stra.size() << std::endl;

	nim nim(2); //ニムのクラス定義

	std::cout << "Initiarized..." << std::endl;
	output_stra(0);
	// > nim_2_4_0.logcal_cal_rate();
	std::cout << "Strategy0..." << std::endl;

	machup = 0;
	int machup_index = 1;
	double loop_start = clock();

	while (END_GA > machup) {

		Crustering(); //クラスタリング。手法によって変わる
		//主親選ぶ
		const int main = GetRand_Int(KO);

		//副親があれば以下の処理を行う
		if (pop[main].List1.empty()) {
			std::cout << "list1が空です main_tasks" << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
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
					if (nim.nim_game(child[i].stra, opp[j].stra)) {
						child[i].Result[j] += WIN_FIRST;
					}
					//2回目
					if (!nim.nim_game(opp[j].stra, child[i].stra)) {
						child[i].Result[j] += WIN_LAST;
					}
				}
			}
			//適応度計算
			for (int i = 0; i < CHILD + 1; i++) {
				child[i].cal_fitness();
			}
			//Best個体を集団へ
			const int index = Choice_Best_Index();
			if (pop[main].eval < child[index].eval){
				pop[main] = child[index];
			}
			//実験用 : 戦略書き出し
			if (BATTLE_PER*machup_index < machup) {
				const double loop_end = clock();
				printf("%d:%d:%I64d/%d　...%d[sec]\n", method, trial, machup, END_GA, int((loop_end - loop_start) / double(CLOCKS_PER_SEC)));

				output_stra(machup_index);
				//cal_best(machup_index);
				//cal_rate();
				machup_index++;
				loop_start = clock();
				end_decision();
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
	MakeList(pop, K_List1, K_List2, 0);
	cr_num = 1;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(cr_num, i, pop) == 1) {
			cr_num++;
		}
	}
	//Cru_Upgma(pop);
	//cr_num = K_UPGMA;
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
bool Coans::end_decision()
{
	if (machup == 0) {
		return true;
	}
	std::vector<double> eval(KO);
	for (int i = 0; i < KO; i++) {
		eval[i] = pop[i].eval;
	}
	const double eval_diff = cal_euclidean(eval, pop_eval);
	std::cout << "diff_eval :" << eval_diff << std::endl;
	if (eval_diff > 0.1) {
		pop_eval = eval;
		return true;
	}
	else {
		return false;
	}
}

/*
	method2
	主親		:ランダム
	対戦相手	:クラスタの代表点
	評価		:ニムの評価に基づく
*/
class coans_mode2 : public Coans {
public:
	coans_mode2(std::string str, int t, int k = 0) {
		dir = str;
		method = 2;
		trial = t;
		machup = 0;					//対戦回数
		cru_k = k;						//クラスタリングパラメーター
	}
private:
	//クラスタリングに使うパラメーター
	virtual void Crustering();
	virtual void Generate_Opp();
};
void coans_mode2::Crustering() 
{
	MakeList(pop, K_List1, K_List2, 0);
	cr_num = 0;
	for (int i = 0; i < KO; i++) {
		if (SetNitch(cr_num, i, pop) == 1) {
			cr_num++;
		}
	}
}
void coans_mode2::Generate_Opp() 
{
	choice_oppoment(pop, opp, cr_num);
	opp_num = int(opp.size());
}

/*
	method3
	主親		:ランダム
	対戦相手	:集団全体
	評価		:ニムの評価に基づく
*/

class coans_mode3 : public Coans {
public:
	coans_mode3(std::string str, int t, int k = 0) {
		dir = str;
		method = 3;
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
	opp = pop;
	opp_num = KO;
}

class Coans_s : public Coans_base {
	//公開メンバ
public:
	void main_task(); 	//手法のメインルーチン
	void main_task2(); 	//改良案

	//実験用
	void exp_pop_disper();
	void exp_upgma();
private:
	virtual void Crustering(int index) = 0;
	virtual void Generate_Opp() = 0;
protected:
	std::vector<double> pop_eval;

	void child2sub(const std::vector<int> &subpare, const std::vector<playerNim> &child);
	void create_pop();
	void ans_nitch();
	int get_best();
	bool end_decision();
	bool improve_decision(std::vector<playerNim> &child_1, std::vector<playerNim> &child_2);
	void update_opp(const playerNim &player);
	int choice_main();
	void all_comp();
	void cal_cr_eval();
};
void Coans_s::main_task() {
	std::cout << "method     :" << method << std::endl;
	std::cout << "trial      :" << trial << std::endl;
	std::cout << "pop_size   :" << KO << std::endl;
	std::cout << "child_size :" << CHILD << std::endl;

	create_pop();
	pop_eval.assign(KO, 0);
	output_stra(0);
	Cru_Upgma(pop);
	cal_gravity_and_disper();
	//std::cout << "pseudoF : " << pseudoF() << std::endl;
	nim nim(2);
	machup = 0;
	int machup_index = 1;
	for (int i = 0; i < KO; i++) {
		pop[i].Result.resize(KO);
		for (int j = 0; j < KO; j++) {
			if (nim.nim_game(pop[i].stra, pop[j].stra)) {
				pop[i].Result[j]++;
			}
		}
		pop[i].cal_fitness();
		pop[i].Init_Result();
	}
	cal_rate();
	int end_flag = 1;
	double loop_start = clock();
	while (end_flag) {
		//std::cout << "1,";
		//main parent
		const int main = choice_main();
		Crustering(main); //crustering
		 //sub parent
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
			//std::cout << "2,";
			child.resize(CHILD);
			//ExtensionXLM(main, sub, pop, child);
			Generate_Opp(); //choice opp

			//std::cout << "3,";
			//crossover
			//EXLM_S(main, sub, pop, child, STEP_SIZE);
			machup += PARENT*(PARENT - 1);

			pop[main].Result.assign(opp_num, 0);
			for (int c = 0; c < CHILD; c++) {
				child[c].Result.assign(opp_num, 0);
			}
			for (auto &pi : sub) {
				pop[pi].Result.assign(opp_num, 0);
			}
			//std::cout << "opp_num=" << opp_num << ",";
			for (int i = 0; i < opp_num; i++) {
				for (int c = 0; c < CHILD; c++, machup++) {
					//std::cout << "(c,r) = " << "(" << i << "," << j << ")" << std::endl;
					if (nim.nim_game(child[c].stra, opp[i].stra)) {
						child[c].Result[i]++;
					}
				}
				if (nim.nim_game(pop[main].stra, opp[i].stra)) {
					pop[main].Result[i]++;
					machup++;
				}
				for (auto &pi : sub) {
					if (nim.nim_game(pop[pi].stra, opp[i].stra)) {
						pop[pi].Result[i]++;
					}
					machup++;
				}
			}

			//std::cout << "5,";
			//cal_fitness
			pop[main].cal_fitness();
			for (int i = 0; i < CHILD; i++) {
				child[i].cal_fitness();
			}
			for (auto &pi : sub) {
				pop[pi].cal_fitness();
			}
			//Best -> pop
			const int index = Choice_Best_Index();
			if (index != 0) {
				if (pop[main].eval < child[index].eval) {
					pop[main] = child[index];
				}
			}
			//child -> sub
			child2sub(sub, child);
			//std::cout << "6,";
			//for experiment
			if (BATTLE_PER*machup_index / 5 < machup) {
				std::cout << std::endl;
				const double loop_end = clock();
				printf("%d:%d:%I64d/%d　...%d[sec]\n", method, trial, machup, END_GA, int((loop_end - loop_start) / CLOCKS_PER_SEC));
				std::cout << "　max rate :" << child[index].eval / K_UPGMA * 100 << std::endl;

				double eval = 0.0;
				for (auto &pi : child) {
					eval += (pi.eval / K_UPGMA);
				}
				std::cout << "　average eval : " << eval / (CHILD + 1) * 100 << "[%]" << std::endl;
				//output_stra(machup_index);
				cal_rate();
				Cru_Upgma(pop);
				cal_gravity_and_disper();
				machup_index++;
				loop_start = clock();

				if (!end_decision()) {
					end_flag = 0;
				}
			}
			//reInit
			for (int i = 0; i < KO; i++) {
				pop[i].Init_0();
			}
			//exit(EXIT_FAILURE);
			//std::cout << std::endl;
		}
	}
}
void Coans_s::main_task2() {
	/*
		始めだけクラスタリングして敵を選ぶ
	*/
	std::cout << "method     :" << method << std::endl;
	std::cout << "trial      :" << trial << std::endl;
	std::cout << "pop_size   :" << KO << std::endl;
	std::cout << "child_size :" << CHILD << std::endl;
	std::cout << "w_size     :" << W_SIZE << std::endl;
	std::cout << "K          :" << K_UPGMA << std::endl;

	create_pop();
	pop_eval.assign(KO, 0);
	output_stra(0);

	Cru_Upgma(pop);
	cr_num = int(opp.size());
	Generate_Opp();
	all_comp();

	double opp_disper;
	std::vector<std::vector<double>> opp_stra(opp_num);
	for (int i = 0; i < opp_num; i++) {
		opp_stra[i] = opp[i].stra;
	}
	opp_disper = cal_dispersion_2(opp_stra);
	std::cout << "　opp_num : opp_disper = " << opp_num << " : " << opp_disper << std::endl;
	machup = 0;
	int machup_index = 1;
	cal_rate();

	double rate = 65.0;
	int end_flag = 1;
	double loop_start = clock();
	//ループスタート
		//std::cout << "1,";
	while (END_GA > machup) {
		//std::cout << "1,";
		//main parent
		//const int cr_tmp_index = cal_minIndex(cr_eval);
		//const int cr_len = int(cr_index[cr_tmp_index].size());
		const int main = choice_main();
		Crustering(main); //crustering
		//sub parent
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
			//std::cout << "2,";
			child.resize(CHILD);

			//cross
			EXLM_S(main, sub, pop, child, opp, STEP_SIZE);
			machup += (PARENT * 2 * opp_num) * 2;

			//std::cout << "3,";

			std::vector<double> opp_eval(opp_num, 0);
			competition_single(pop[main], opp);
			machup += opp_num * 2;
			for (int i = 0; i < opp_num; i++) {
				opp[i].cal_fitness();
				opp_eval[i] += opp[i].eval;
			}
			for (auto &pi : sub) {
				competition_single(pop[pi], opp);
				for (int i = 0; i < opp_num; i++) {
					opp[i].cal_fitness();
					opp_eval[i] += opp[i].eval;
				}
			}
			machup += (PARENT*opp_num) * 2;
			competition_multi(child, opp);
			machup += (CHILD*opp_num) * 2;
			for (int i = 0; i < opp_num; i++) {
				opp[i].cal_fitness();
				opp_eval[i] += opp[i].eval;
			}
			//std::cout << "5,";
			//cal_fitness
			pop[main].cal_fitness();
			for (int i = 0; i < CHILD; i++) {
				child[i].cal_fitness();
			}
			for (auto &pi : sub) {
				pop[pi].cal_fitness();
			}
			//Best -> pop
			const int index = Choice_Best_Index();
			if (index != 0) {
				if (pop[main].eval < child[index].eval) {
					//const int tmp_nitch = pop[main].nitch;
					pop[main] = child[index];
					//pop[main].nitch = tmp_nitch;
				}
			}
			//child -> sub
			child2sub(sub, child);
			for (auto &pi : sub) {
				pop_eval[pi] = pop[pi].eval;
			}
			const int min_index = cal_minIndex(opp_eval);
			const double opp_min = opp_eval[min_index] / double(CHILD + PARENT + 1);
			if (opp_min < child[index].eval / opp_num) {
				opp[min_index] = child[index];
			}
			//std::cout << "6, " << ": machup = " << machup;
			//for experiment
			if (BATTLE_PER*machup_index < machup) {
				std::cout << "machup = " << machup;
				std::cout << std::endl;
				const double loop_end = clock();
				printf("%d:%d:%I64d/%d　...%d[sec]\n", method, trial, machup, END_GA, int((loop_end - loop_start) / CLOCKS_PER_SEC));
				for (int i = 0; i < opp_num; i++) {
					opp_stra[i] = opp[i].stra;
				}
				opp_disper = cal_dispersion_2(opp_stra);
				std::cout << "　opp_num : opp_disper = " << opp_num << " : " << opp_disper << std::endl;
				output_stra(machup_index);
				output_opp_stra(machup_index);
				cal_rate();
				all_comp();
				//Cru_Upgma(pop);
				//cal_gravity_and_disper();
				machup_index++;
				loop_start = clock();
			}
			//reInit
			pop[main].Init_0();
			for (auto &pi : sub) {
				pop[pi].Init_0();
			}
			//exit(EXIT_FAILURE);
		}
	}
}
void Coans_s::child2sub(const std::vector<int> &subpare, const std::vector<playerNim> &child)
{
	for (auto &pi : subpare) {
		std::vector<double> dis(CHILD);

		for (int j = 0; j < CHILD; j++) {
			dis[j] = cal_euclidean(pop[pi].stra, child[j].stra);
		}
		const auto min = min_element(dis.begin(), dis.end());
		const int min_index = int(std::distance(dis.begin(), min));

		if (pop[pi].eval < child[min_index].eval) {
			pop[pi] = child[min_index];
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
	if (tmp_pop.size() != KO) {
		std::cout << "集団サイズが合いません -> creat_pop" << std::endl;
		exit(EXIT_FAILURE);
	}
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].stra = tmp_pop[i].stra;
		pop[i].Init_pn();
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
int Coans_s::get_best()
{
	nim nim2(1);
	for (int i = 0; i < KO; i++) {
		pop[i].Result.assign(KO, 0);
	}

	for (int i = 0; i < KO; i++) {
		for (int j = 0; j < KO; j++) {
			//1回目
			if (nim2.nim_game(pop[i].stra, pop[j].stra)) {
				pop[i].Result[j]++;
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
bool Coans_s::end_decision()
{
	if (machup == 0) {
		return true;
	}
	std::vector<double> eval(KO);
	for (int i = 0; i < KO; i++) {
		eval[i] = pop[i].eval;
	}
	const double eval_diff = cal_euclidean(eval, pop_eval);
	std::cout << "　diff_eval :" << eval_diff << std::endl;
	if (eval_diff > 0.1) {
		return true;
	}
	else {
		return false;
	}
}
bool Coans_s::improve_decision(std::vector<playerNim> &child_1, std::vector<playerNim> &child_2)
{
	if (child_1[0].eval == 0.0) {
		return true;
	}
	std::cout << "　child size : " << child_1.size() << std::endl;
	double ave_1 = 0.0;
	for (int i = 0; i < CHILD; i++) {
		ave_1 += (child_1[i].eval / K_UPGMA);
	}
	ave_1 /= CHILD;
	std::cout << "　child average : " << ave_1 << std::endl;
	if (0.6 < ave_1) {
		return false;
	}
	if (child_2.empty()) {
		child_2.resize(CHILD);
		return true;
	}
	std::cout << "　tmp_child size : " << child_2.size() << std::endl;
	double ave_2 = 0.0;
	for (int i = 0; i < CHILD; i++) {
		ave_2 += (child_2[i].eval / K_UPGMA);
	}
	ave_2 /= CHILD;
	std::cout << "　tmp_child average : " << ave_2 << std::endl;
	if (ave_1 > ave_2 && 0.6 < ave_1) {
		return false;
	}
	else if(ave_1 < ave_2){
		for (int i = 0; i < CHILD; i++) {
			child_1[i + 1] = child_2[i];
		}
		return true;
	}
	else {
		return true;
	}
}
void Coans_s::update_opp(const playerNim &player) 
{
	std::vector<double> eval(opp_num);
	for (int i = 0; i < opp_num; i++) {
		eval[i] = opp[i].eval;
	}
	const int index = cal_minIndex(eval);
	opp[index] = player;
}
void Coans_s::exp_pop_disper()
{
	std::stringstream fname;
	fname << "./nim/" << method;
	const int trial_num = count_folder(fname.str());

	//フォルダ数取得
	std::vector<int> f_num(trial_num);
	for (int i = 0; i < trial_num; i++) {
		char fname[50];
		sprintf_s(fname, "./nim/%d/%d", method, i);
		f_num[i] = count_folder(fname);
	}

	pop.resize(KO);
	std::vector<std::vector<double>> disper(trial_num);
	std::vector<std::vector<double>> stra(KO);
	for (int t = 0; t < trial_num; t++) {
		disper[t].resize(f_num[t]);
		std::cout << "trial : folder = " << trial_num << " : " << f_num[t] << std::endl;
		for (int f = 0; f < f_num[t]; f++) {
			for (int i = 0; i < KO; i++) {
				//個体情報インプット
				char fname[50];
				sprintf_s(fname, "./nim/%d/%d/%d/%d.dat", method, 0, f, i);
				pop[i].input_stra(fname);
				stra[i] = pop[i].stra;
			}
			//cal_disper
			disper[t][f] = cal_dispersion_2(stra);
			std::cout << "folder : " << f << "...end" << std::endl;
		}
	}
	CsvModules::csv_fwrite("disper_4.csv", disper, KO*(CHILD + 1) * 2 * 10);
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
		Cru_Upgma(pop);

		asort_index_H();

		const int size = int(cr_index.size());
		//重心
		std::vector<std::vector<double>> cr_gra(size);
		//分散
		std::vector<double> cr_disper(size);
		for (int i = 0; i < size; i++) {
			//分散と重心
			//cr_disper[i] = distance_disper(cr_index[i], cr_gra[i]);
		}
		//const double disper = cal_dispersion_1(cr_gra);
		show_vec_1(cr_disper);
		std::cout << "folder : " << f << "...end" << std::endl;
	}
}
int Coans_s::choice_main() 
{
	double sum_eval = 0;
	for (int i = 0; i < KO; i++) {
		sum_eval += pop[i].eval;
	}
	std::vector<double> ration(KO);
	for (int i = 0; i < KO; i++) {
		double tmp_eval;
		if (pop[i].eval <= 0) {
			tmp_eval = 1.0;
		}
		else {
			tmp_eval = pop[i].eval;
		}
		ration[i] = sum_eval / pop[i].eval;
	}
	sum_eval = std::accumulate(ration.begin(), ration.end(), 0.0);
	const double rand = GetRand_Real_0(sum_eval);

	int index = 0;
	double sum = ration[0];
	while (sum < rand) {
		index++;
		sum += ration[index];
	}
	if (KO <= index) {
		std::cout << "indexが集団サイズより大きいです -> choice_main" << std::endl;
		exit(EXIT_FAILURE);
	}
	//show_vec_1(ration);
	//show_vec_1(ration);
	//std::cout << "index : " << index << std::endl;
	//std::cout << "pop eval : " << pop[index].eval << std::endl;
	return index;
}
void Coans_s::all_comp() 
{
	const double pop_size = KO;
	competition_multi(pop, opp);
	for (int i = 0; i < KO; i++) {
		pop[i].cal_fitness();
		pop_eval[i] = pop[i].eval;
		pop[i].Init_Result();
	}
	const double max = *max_element(pop_eval.begin(), pop_eval.end());
	const double min = *min_element(pop_eval.begin(), pop_eval.end());
	const double ave = (std::accumulate(pop_eval.begin(), pop_eval.end(), 0.0) / double(KO));
	std::cout << "　pop max : " << (max / (opp_num * 2)) * 100 << std::endl;
	std::cout << "　pop ave : " << (ave / (opp_num * 2)) * 100 << std::endl;
	std::cout << "　pop min : " << (min / (opp_num * 2)) * 100 << std::endl;

}
void Coans_s::cal_cr_eval() 
{
	const int cr_len = int(cr_index.size());
	cr_eval.resize(cr_len);
	for (int i = 0; i < cr_len; i++) {
		double eval = 0;
		for (auto &pi : cr_index[i]) {
			eval += pop[pi].eval;
		}
		cr_eval[i] = eval / double(cr_index[i].size()) / opp_num * 100;
	}
}

class mode4 : public Coans_s {
public:
	mode4(std::string str, int t) {
		dir = str;
		method = 4;
		trial = t;
		machup = 0;					//対戦回数
	}
private:
	virtual void Crustering(int index);
	virtual void Generate_Opp();
};
void mode4::Crustering(int index) {
	//近傍リスト生成
	std::vector<double> dis(KO);
	pop[index].List1.resize(K_List1);
	for (int i = 0; i < KO; i++) {
		if (i == index) {
			dis[i] = 10000;
		}
		else {
			dis[i] = cal_euclidean(pop[index].stra, pop[i].stra);
		}
	}
	for (int i = 0; i < K_List1; i++) {
		const int min_index = cal_minIndex(dis);
		pop[index].List1[i] = min_index;
		dis[min_index] = 10000;
	}
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