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
#include "../header/Usual_Methods.hpp"

/*手法のクラス*/
class Coans{
//公開メンバ
public :
	void	main_task(); 	//手法のメインルーチン
	int		Get_MatchUp_Num();			//対戦回数を取得
	
	//データ取りミスったときに使うやつ
	void	Stra_nitch_CSV();	//戦略データを読み込んで
	//以下実験用
//非公開メンバ
private:
	virtual void Crustering() = 0;
	virtual void Generate_Opp() = 0;
	virtual void Return_Re(int Child_Num, int Opp_Num) = 0;
	virtual void Cal_Fitness() = 0;
	//このクラスと派生クラスからアクセス可能
protected:
	int method;						//手法比較の時に使う
	int gene;						//世代数
	int per;						//間隔
	int trial;
	int cr_num;						//クラスター番号
	int	cru_k;						//クラスタリングパラメーター
	int opp_num;
	int machup;						//対戦回数

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
	init_genrand((unsigned)time(NULL)); /*乱数初期化*/

	//集団初期化
	for (int i = 0; i < KO; i++) {
		pop[i].Init_pn();
		pop[i].Init_stra();
	}
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
	for (int Gene_Loop = 1; Gene_Loop < gene+1; Gene_Loop++) {
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
			ExtensionXLM(main_pare, sub_pare, pop, child);
			//対戦相手の個体を選ぶ
			std::cout << "4" << ',';
			Generate_Opp();

			//w1,w2,w3:子個体の戦略，w1_T,w2_T,w3_T:対戦相手の戦略
			//player1:子個体，player2:対戦相手0
			std::cout << "5" << ',';
			int opp_len = int(opp.size());

			//ここで対戦
			/*
			
			*/
			std::cout << "6" << ',';
			//適応度計算
			for (int i = 0; i < CHILD; i++) {
				child[i].cal_fitness();
			}

			std::cout << "7" << ',';
			//Best個体を集団へ
			int index = Choice_Best_Index(false);
			pop[main_pare] = child[index];

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
		//クリア
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
		//クリア
		fname.str("");
		fname.clear(std::stringstream::goodbit);
	}

}
void Coans::Stra_nitch_CSV() {
	/*
	std::cout << "モード:" << 4 << std::endl;
	std::cout << "手法:" << method << std::endl;
	std::cout << "試行回数:" << trial << std::endl;
	std::cout << "クラスタ数:" << cru_k << std::endl;
	std::cout << "世代数:" << gene << std::endl;
	std::cout << "区切り:" << per << std::endl;
	std::cout << "集団個体数:" << KO << std::endl;

	//Init
	pop.resize(KO);
	for (int i = 0; i < KO; i++) {
		pop[i].Init();
	}
	Csv_exp csv_exp(dir, method, trial, gene, per);
	for (int g = 0; g < gene / per + 1; g++) {
		//戦略格納
		for (int i = 0; i < KO; i++) {
			char fname[50];
			if (cru_k == 0) {
				sprintf_s(fname, ("%s/%d/%d/%d/%d.dat"), dir.c_str(), method, trial, g, i);
			}
			else if (0 < cru_k) {
				sprintf_s(fname, ("%s/%d/%d/%d/%d/%d.dat"), dir.c_str(), method, trial, cru_k, g, i);
			}
			//自プレイヤーの戦略格納
			pop[i].input_stra(fname);
		}
		//クラスタリング
		Crustering();
		csv_exp.SetVec_Cr_Pop(pop, g);
	}
	csv_exp.fwrite_Cr_P();
	*/
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
int Coans::Choice_Best_Index(bool random) {
	std::vector<double> tmp_eval(CHILD);

	//一旦格納
	for (int i = 0; i < CHILD; i++) {
		tmp_eval[i] = child[i].get_eval();
	}

	int index = 0;
	auto max = max_element(tmp_eval.begin(), tmp_eval.end());
	//同じ評価地の個体が複数ある場合はランダム
	int count_Num = int(count(tmp_eval.begin(), tmp_eval.end(), *max));
	//cout << "count_Num:" << count_Num << endl;
	if (random) {
		if (count_Num == 1) {
			//インデックスを取得
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
	ウォード法の階層的クラスタリングで明示的にクラスタリング
	評価はHPの差分
*/
class coans_mode1 : public Coans {
public:
	coans_mode1(std::string str, int g, int g_p, int k = 0) {
		dir = str;
		method = 1;
		gene = g;						//世代数
		per = g_p;						//間隔
		machup = 0;					//対戦回数
		cru_k = k;						//クラスタリングパラメーター
	}
private:
	//クラスタリングに使うパラメーター
	virtual void Crustering();
	virtual void Generate_Opp();
	virtual void Return_Re(int Child_Num, int Opp_Num);
	virtual void Cal_Fitness();
};
void coans_mode1::Crustering() {
	//近傍リスト生成
	//MakeList(Pop, true, false, false);
	//クラスタ番号割り振り
	Crustering1();
}
void coans_mode1::Generate_Opp() {
	choice_oppoment(pop, opp, cr_num);
	opp_num = cr_num;
}
void coans_mode1::Return_Re(int Child_Num, int Opp_Num){}
void coans_mode1::Cal_Fitness(){}

