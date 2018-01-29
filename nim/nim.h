#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "config_nim.hpp"
#include "player_nim.h"
#include "../header/Usual_Methods.hpp"
#include "NeuralNetwork.hpp"

class Numbers {
public:
	bool game(const std::vector<double> &pop, const std::vector<double> &opp);
protected:
};
bool Numbers::game(const std::vector<double> &pop, const std::vector<double> &opp)
{
	std::vector<double> diff(W_SIZE);
	for (int i = 0; i < W_SIZE; i++) {
		diff[i] = std::sqrt((pop[i] - opp[i])*(pop[i] - opp[i]));
	}
	const int index = cal_maxIndex(diff);
	if (pop[index] > opp[index]) {
		return true;
	}
	else {
		return false;
	}
}

class nim {
public:
	nim(int m)
	{
		nim_n = NIM;
		nim_status.resize(NIM);
		nim_status[0] = POLL1;
		nim_status[1] = POLL2;
		nim_status[2] = POLL3;
		stra_len = EVALUATION_VEC;
		nim_status_vec.assign(stra_len, 0);
		mont_max = *max_element(nim_status.begin(), nim_status.end());
		mont_size = mont_max;
		//cal_optimal();
	}
	bool nim_game(const std::vector<double> &pop, const std::vector<double> &opp);
	double nim_evaluation(const std::vector<double>& stra);
	void input_stra(const std::vector<double> &pop, const std::vector<double> &opp);
protected:
	int mode;
	int nim_n;	//山の数
	int stra_len;
	int mont_max;
	int mont_size;
	std::vector<int> nim_status;	 //それぞれの山の石の数
	std::vector<int> nim_status_vec; //0:遷移不可能 1:遷移可能 2:現在の状態
	std::vector<int> opt;			 //最適解

	std::vector<double> pop_stra;
	std::vector<double> opp_stra;

	void Init_mont();
	int cal_index(const int a, const int b, const int c);
	void cal_binary_vec(const int n, std::vector<int> &input, const int size = EVALUATION_VEC); //10進数を2進数のベクターに変換する
	void cal_xor_vec(std::vector<int> &one, std::vector<int> &ano, std::vector<int> &vec); //ベクター同士の排他的論理和
	void cal_move_vec();
	int cal_nimsum();
	void generate_mont(int a, int b, int c, std::vector<std::vector<int>> &mont);
	bool update_mont(int index); //戦略情報から山の状態を更新する
	int choose_stra(const std::vector<double> &stra);
	void vec2evalvec(const std::vector<double>& stra, std::vector<double>& eval_vec);
	void show_mont();
};
//0:プレイヤー先手，1:プレイヤー後手
bool nim::nim_game(const std::vector<double> &pop, const std::vector<double> &opp)
{
	vec2evalvec(pop, pop_stra);
	vec2evalvec(opp, opp_stra);
	Init_mont(); //山の初期化
	//show_mont();
	int stra_index;

	while (1)
	{
		stra_index = choose_stra(pop_stra);
		if (!update_mont(stra_index)) {
			return true;
		}
		stra_index = choose_stra(opp_stra);
		if (!update_mont(stra_index)) {
			return false;
		}
	}
}
double nim::nim_evaluation(const std::vector<double>& stra)
{
	//show_mont();
	int stra_index;
	double optimal_c = 0;
	double battle_c = 0;
	//cal_optimal();
	//int count = int(std::count(opt.begin(), opt.end(), 1));
	//std::cout << "最適手 :" << count << std::endl;
	std::vector<std::vector<int>> mont;
	vec2evalvec(stra, pop_stra);
	for (int i = 0; i < POLL1 + 1; i++) {
		for (int j = 0; j < POLL2 + 1; j++) {
			for (int k = 0; k < POLL3 + 1; k++) {
				nim_status = { i,j,k };
				int nim_sum = cal_nimsum();
				//ニム和が0でないときに評価
				if (nim_sum != 0) {
					cal_move_vec(); //遷移可能状態を格納
					stra_index = choose_stra(pop_stra); //戦略を選ぶ
					if (stra_index > -1) {
						nim_status[0] = stra_index / ((POLL2 + 1)*(POLL3 + 1));
						nim_status[1] = (stra_index % ((POLL2 + 1)*(POLL3 + 1))) / (POLL3 + 1);
						nim_status[2] = stra_index % (POLL3 + 1);

						//generate_mont(nim_status[0], nim_status[1], nim_status[2], mont);
						nim_sum = cal_nimsum();
						if (nim_sum == 0) {
							optimal_c++;
						}
					}
					battle_c++;
				}
			}
		}
	}
	//std::cout << "optimal_c : " << optimal_c << std::endl;
	//std::cout << "battle_c : " << battle_c << std::endl;
	//std::cout << "　currect : " << (optimal_c/battle_c)*100 << " %" << std::endl;
	if (optimal_c / battle_c > 1) {
		std::cout << "optimal_c > battle_c -> nim_evaluation" << std::endl;
		exit(EXIT_FAILURE);
	}
	return optimal_c / battle_c;
}
void nim::Init_mont() {
	nim_status[0] = POLL1;
	nim_status[1] = POLL2;
	nim_status[2] = POLL3;
	nim_status_vec.assign(stra_len, 0);
	cal_move_vec();
}
bool nim::update_mont(int index) {
	if (stra_len < index) {
		std::cout << "indexが大きすぎます->update_mont : " << index << std::endl;
		exit(EXIT_FAILURE);
	}

	//山の個数を更新
	nim_status[0] = index / ((POLL2 + 1)*(POLL3 + 1));
	nim_status[1] = (index % ((POLL2 + 1)*(POLL3 + 1))) / (POLL3 + 1);
	nim_status[2] = index % (POLL3 + 1);

	//show_mont();

	cal_move_vec();
	int sum = std::accumulate(nim_status.begin(), nim_status.end(),0);
	if (sum) {
		return true;
	}
	else {
		return false;
	}
}
int nim::cal_index(const int a, const int b, const int c) {
	if (POLL1 < a || POLL2 < b || POLL3 < c) {
		std::cout << "パラメーターが大きすぎます->cal_index :" << a << "," << b << "," << c << std::endl;
		exit(EXIT_FAILURE);
	}
	int tmp_index = a * ((POLL2 + 1) * (POLL3 + 1)) + b * (POLL3 + 1) + c * 1;
	if (stra_len < tmp_index) {
		std::cout << "tmp_indexが大きすぎます ->cal_move_vec" << std::endl;
		std::cout << "tmp_index:" << tmp_index << std::endl;
		std::cout << "nim_status:";
		show_vec_1(nim_status);
		exit(EXIT_FAILURE);
	}
	return tmp_index;
}
void nim::cal_binary_vec(const int n, std::vector<int> &input, const int size) 
{
	int zero_len = 0;
	input.assign(size, 0);

	int syou = n;
	int amari;
	while (0 < syou) {
		amari = syou % 2;
		syou = syou / 2;
		input[size - zero_len - 1] = amari;
		zero_len++;
	}
	if (zero_len < size) {
		for (int i = zero_len; i < size; i++) {
			input[size - zero_len - 1] = 0;
		}
	}
}
void nim::cal_xor_vec(std::vector<int> &one, std::vector<int> &ano, std::vector<int> &input) 
{
	int one_size = int(one.size());
	input.resize(one_size);
	for (int i = 0; i < one_size; i++) {
		if (one[i] == ano[i]) {
			input[i] = 0;
		}
		else {
			input[i] = 1;
		}
	}
}
int nim::cal_nimsum() 
{
	std::vector<std::vector<int>> mont_vec(NIM);

	for (int i = 0; i < NIM; i++) {
		cal_binary_vec(nim_status[i], mont_vec[i]);
	}
	std::vector<int> sum(EVALUATION_VEC, 0);
	for (int i = 0; i < EVALUATION_VEC; i++) {
		int tmp_sum = 0;
		for (int j = 0; j < NIM; j++) {
			tmp_sum += mont_vec[j][i];
		}
		sum[i] = tmp_sum % 2;
	}
	int nim_sum = int(std::accumulate(sum.begin(), sum.end(), 0));
	return nim_sum;
}
void nim::cal_move_vec()
{
	int index;
	nim_status_vec.assign(stra_len, 0);
	for (int i = 0; i < NIM; i++) {
		for (int j = 0; j < nim_status[i]; j++) {
			switch (i) {
			case 0:
				index = cal_index(j, nim_status[1], nim_status[2]);
				break;
			case 1:
				index = cal_index(nim_status[0], j, nim_status[2]);
				break;
			case 2:
				index = cal_index(nim_status[0], nim_status[1], j);
				break;
			}
			nim_status_vec[index] = 1;
		}
	}
	index = cal_index(nim_status[0], nim_status[1], nim_status[2]);
	nim_status_vec[index] = 2;
	//show_vec_1(nim_status_vec);
}
void nim::generate_mont(int a, int b, int c, std::vector<std::vector<int>> &mont) 
{
	std::vector<int> tmp_nim_status = { a,b,c };
	const auto max = *max_element(tmp_nim_status.begin(), tmp_nim_status.end());
	mont = std::vector<std::vector<int>>(max, std::vector<int>(NIM, 0));
	for (int i = 1; i < max+1; i++) {
		for (int j = 0; j < NIM; j++) {
			if (i <= tmp_nim_status[j]) {
				mont[i-1][j] = 1;
			}
		}
	}
	show_vec_2(mont);
}
int nim::choose_stra(const std::vector<double> &stra) 
{
	if (stra.empty()) {
		std::cout << "戦略が空です : choose_stra" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::vector<double> tmp_stra(stra_len);
	for (int i = 0; i < stra_len; i++) {
		if (nim_status_vec[i] == 1) {
			tmp_stra[i] = stra[i];
		}
		else {
			tmp_stra[i] = 10000;
		}
	}
	const auto min = min_element(tmp_stra.begin(), tmp_stra.end());
	const int index = int(std::distance(tmp_stra.begin(), min));
	return index;
}
void nim::input_stra(const std::vector<double> &pop, const std::vector<double> &opp)
{
	vec2evalvec(pop, pop_stra);
	vec2evalvec(opp, opp_stra);
}
void nim::vec2evalvec(const std::vector<double>& stra, std::vector<double>& eval_vec)
{
	NN::NeuralNetwork<INPUT, MIDDLE, OUTPUT> nn;

	const auto s = stra.size();
	Eigen::VectorXd vec = Eigen::VectorXd::Zero(s);
	for (auto i = 0U; i < s; ++i) {
		vec[i] = stra[i];
	}
	nn.setWeight(vec); //重みセット

	std::vector<int> poll1(3, 0);
	std::vector<int> poll2(3, 0);
	std::vector<int> poll3(3, 0);
	std::vector<int> input_vec(9, 0);

	eval_vec.resize(EVALUATION_VEC);
	int len = 0;
	for (int i = 0; i < POLL3 + 1; i++) {
		cal_binary_vec(i, poll3, 3);
		//show_vec_1(poll3);

		for (int j = 0; j < POLL2 + 1; j++) {
			cal_binary_vec(j, poll2, 3);

			for (int k = 0; k < POLL1 + 1; k++, len++) {
				cal_binary_vec(k, poll1, 3);

				for (int l = 0; l < 3; l++) {
					input_vec[l + 0] = poll1[l];
					input_vec[l + 3] = poll2[l];
					input_vec[l + 6] = poll3[l];
				}
				for (int l = 0; l < 9; l++) {
					if (input_vec[l] == 0) {
						input_vec[l] = -1;
					}
				}
				//show_vec_1(input_vec);
				nn.setInput(input_vec); //入力
				eval_vec[len] = nn.getOutPut(); //出力
			}
		}
	}
	//show_vec_1(eval_vec);
}
void nim::show_mont()
{
	std::vector<std::vector<int>> tmp_mont_vec;
	tmp_mont_vec = std::vector<std::vector<int>>(mont_size, std::vector<int>(NIM, 0));

	generate_mont(nim_status[0], nim_status[1], nim_status[2], tmp_mont_vec);
	std::cout << "山の状態" << std::endl;
	show_vec_2(tmp_mont_vec);

	std::cout << "vec = [";
	for (auto &p : nim_status_vec) {
		std::cout << p << ",";
	}
	std::cout << "]" << std::endl;
	std::cout << "(0,1,2) = (" << nim_status[0] << "," << nim_status[1] << "," << nim_status[2] << ")" << std::endl << std::endl;
}

int competition_single(playerNim &player_1, std::vector<playerNim> &player_2) {
	int mach = 0;
	nim nim(1);
	Numbers numbers;
	const int len_2p = int(player_2.size());

	player_1.Result.assign(len_2p, 0);
	for (int i = 0; i < len_2p; i++) {
		player_2[i].Result.assign(1, 0);
	}
	switch (GAME_NUM) {
		//Numbers
	case 0:
		for (int j = 0; j < len_2p; j++, mach++) {
			const double diff = numbers.game(player_1.stra, player_2[j].stra);
			player_1.Result[j] = diff;
			player_2[j].Result[0] = diff*(-1);
		}
		break;
		//Nim
	case 1:
		for (int j = 0; j < len_2p; j++, mach += 2) {
			//1回目
			if (nim.nim_game(player_1.stra, player_2[j].stra)) {
				player_1.Result[j] += 0.8;
			}
			else {
				player_2[j].Result[0] += 1.2;
			}
			//2回目
			if (nim.nim_game(player_2[j].stra, player_1.stra)) {
				player_2[j].Result[0] += 0.8;
			}
			else {
				player_1.Result[j] += 1.2;
			}
		}
		break;
	default:
		exit(EXIT_FAILURE);
		break;
	}
	return mach;
}
int competition_multi(std::vector<playerNim> &player_1, std::vector<playerNim> &player_2)
{
	int mach = 0;
	nim nim(1);
	Numbers numbers;
	const int len_1p = int(player_1.size());
	const int len_2p = int(player_2.size());
	for (int i = 0; i < len_1p; i++) {
		player_1[i].Result.assign(len_2p, 0);
	}
	for (int i = 0; i < len_2p; i++) {
		player_2[i].Result.assign(len_1p, 0);
	}
	switch (GAME_NUM) {
		//Numbers
	case 0:
		for (int i = 0; i < len_1p; i++, mach++) {
			for (int j = 0; j < len_2p; j++) {
				const double diff = numbers.game(player_1[i].stra, player_2[j].stra);
				player_1[i].Result[j] = diff;
				player_2[j].Result[i] = diff*(-1);
			}
		}
		break;
		//Nim
	case 1:
		for (int i = 0; i < len_1p; i++) {
			for (int j = 0; j < len_2p; j++, mach += 2) {
				if (nim.nim_game(player_1[i].stra, player_2[j].stra)) {
					player_1[i].Result[j] += 0.8;
				}
				else {
					player_2[j].Result[i] += 1.2;
				}
				if (nim.nim_game(player_2[j].stra, player_1[i].stra)) {
					player_2[j].Result[i] += 0.8;
				}
				else {
					player_1[i].Result[j] += 1.2;
				}
			}
		}
		break;
	default:
		exit(EXIT_FAILURE);
		break;
	}
	return mach;
}