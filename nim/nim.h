#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "config_nim.hpp"
#include "player_nim.h"
#include "../header/Usual_Methods.hpp"

class nim {
public:
	nim(int m)
	{
		nim_n = NIM;
		nim_status.resize(NIM);
		nim_status[0] = POLL1;
		nim_status[1] = POLL2;
		nim_status[2] = POLL3;
		stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);
		nim_status_vec.assign(stra_len,0);
		mont_max = *max_element(nim_status.begin(), nim_status.end());
		mont_size = mont_max;
		//cal_optimal();

		mode = m;
	}
	double nim_game(const int turn);
	double nim_evaluation();
	void input_stra_first(const std::vector<int> &pop);
	void input_stra_last(const std::vector<int> &opp);
	void output_stra_first(std::vector<int> &pop);
	void output_stra_last(std::vector<int> &pop);
	void cal_and(const std::vector<int> &binary1, const std::vector<int> &binary2, std::vector<int> &binary3);
protected:
	int mode;
	int nim_n;	//山の数
	int stra_len;
	int mont_max;
	int mont_size;
	std::vector<int> nim_status;	 //それぞれの山の石の数
	std::vector<int> nim_status_vec; //0:遷移不可能 1:遷移可能 2:現在の状態
	std::vector<int> opt;			 //最適解

	std::vector<int> pop_stra;
	std::vector<int> opp_stra;

	void Init_mont();
	int cal_index(const int a, const int b, const int c);
	void cal_binary_vec(const int n, std::vector<int> &input); //10進数を2進数のベクターに変換する
	void cal_xor_vec(std::vector<int> &one, std::vector<int> &ano, std::vector<int> &vec); //ベクター同士の排他的論理和
	void cal_optimal(); //最適解を求める
	void cal_move_vec();
	int cal_nimsum();
	void generate_mont(int a, int b, int c, std::vector<std::vector<int>> &mont);
	bool update_mont(int index); //戦略情報から山の状態を更新する
	int choose_stra(std::vector<int> &stra);
	void show_mont();
};
//0:プレイヤー先手，1:プレイヤー後手
double nim::nim_game(const int turn)
{
	Init_mont(); //山の初期化
	//show_mont();
	int stra_index;
	while (1)
	{
		stra_index = choose_stra(pop_stra);
		if (stra_index < 0) {
			if (turn == 0) {
				return -1;
			}
			else {
				return 0;
			}
		}
		else {
			//std::cout << "stra_index = " << stra_index << std::endl;
			if (!update_mont(stra_index)) {
				if (turn == 0) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}

		stra_index = choose_stra(opp_stra);
		if (stra_index < 0) {
			if (turn == 0) {
				return 0;
			}
			else {
				return -1;
			}
		}
		else {
			//std::cout << "stra_index = " << stra_index << std::endl;
			if (!update_mont(stra_index)) {
				if (turn == 0) {
					//先手の場合
					return 0;
				}
				else {
					return 1;
				}
			}
		}
	}
}
double nim::nim_evaluation()
{
	//show_mont();
	int stra_index;
	double optimal_c = 0;
	double battle_c = 0;
	//cal_optimal();
	//int count = int(std::count(opt.begin(), opt.end(), 1));
	//std::cout << "最適手 :" << count << std::endl;
	std::vector<std::vector<int>> mont;
	for (int i = 0; i < POLL1 + 1; i++) {
		for (int j = 0; j < POLL2 + 1; j++) {
			for (int k = 0; k < POLL3 + 1; k++) {
				nim_status = { i,j,k };
				int nim_sum = cal_nimsum();
				//ニム和が0でないときに評価
				if (nim_sum != 0) {
					//generate_mont(nim_status[0], nim_status[1], nim_status[2], mont);
					cal_move_vec(); //遷移可能状態を格納
					//show_vec_1(pop_stra);
					//show_vec_1(nim_status_vec);
					stra_index = choose_stra(pop_stra); //戦略を選ぶ
					//std::cout << "stra_index : " << stra_index << std::endl;
					if (stra_index != -1) {
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
	std::cout << "optimal_c : " << optimal_c << std::endl;
	std::cout << "battle_c : " << battle_c << std::endl;

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
void nim::cal_binary_vec(const int n, std::vector<int> &input) 
{
	int zero_len = 0;
	input.assign(stra_len,0);

	int syou = n;
	int amari;
	while (0 < syou) {
		amari = syou % 2;
		syou = syou / 2;
		input[stra_len - zero_len - 1] = amari;
		zero_len++;
	}
	if (zero_len < stra_len) {
		for (int i = zero_len; i < stra_len; i++) {
			input[stra_len - zero_len - 1] = 0;
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
void nim::cal_optimal()
{
	opt.assign(stra_len,0);
	for (int i = 0; i < POLL1 + 1; i++) {
		for (int j = 0; j < POLL2 + 1; j++) {
			for (int k = 0; k < POLL3 + 1; k++) {
				nim_status = { i,j,k };
				int num_sum = cal_nimsum();
				if (num_sum == 0) {
					int index = i * (POLL2*POLL3) + j * POLL3 + k * 1; //最適解に加える
					if (stra_len < index) {
						std::cout << "戦略のサイズを超えています : index = " << index << std::endl;
						exit(EXIT_FAILURE);
					}
					opt[index] = 1;
				}
			}
		}
	}
//	show_vec_1(opt);
}
int nim::cal_nimsum() 
{
	std::vector<std::vector<int>> mont_vec(NIM);

	for (int i = 0; i < NIM; i++) {
		cal_binary_vec(nim_status[i], mont_vec[i]);
	}
	std::vector<int> sum(STRA_LEN, 0);
	for (int i = 0; i < STRA_LEN; i++) {
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
void nim::cal_and(const std::vector<int> &binary1, const std::vector<int> &binary2, std::vector<int> &binary3) 
{
	int bina = 0;
	for (int i = 0; i < STRA_LEN; i++)
	{
		const int sum = binary1[i] + binary2[i] + bina;
		binary3[i] = sum % 2;
		if (sum == 2) {
			bina = 1;
		}
		else {
			bina = 0;
		}
		
	}
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
int nim::choose_stra(std::vector<int> &stra) {
	if (stra.empty()) {
		std::cout << "戦略が空です : choose_stra" << std::endl;
		exit(EXIT_FAILURE);
	}
	int index;
	std::vector<int> tmp_stra(stra_len);
	for (int i = 0; i < stra_len; i++) {
		if (nim_status_vec[i] == 1 && stra[i] == 1) {
			tmp_stra[i] = 1;
		}
		else {
			tmp_stra[i] = 0;
		}
	}
	const int count = int(std::count(tmp_stra.begin(), tmp_stra.end(), 1));
	if (count > 0) {
		//1個の場合
		auto result_t = std::find(tmp_stra.begin(), tmp_stra.end(), 1);
		index = int(std::distance(tmp_stra.begin(), result_t));
		/*
		if (count == 1)
		{
			auto result_t = std::find(tmp_stra.begin(), tmp_stra.end(), 1);
			index = int(std::distance(tmp_stra.begin(), result_t));
		}
		//複数ある場合
		else if (1 < count)
		{
			std::vector<int> tmp_index(count);

			auto result_t = std::find(tmp_stra.begin(), tmp_stra.end(), 1);
			tmp_index[0] = int(std::distance(tmp_stra.begin(), result_t));

			for (int j = 1; j < count; j++) {
				result_t = std::find(result_t, tmp_stra.end(), 1);
				tmp_index[j] = int(std::distance(tmp_stra.begin(), result_t));
			}
			const int tmp_rand = GetRand_Int(count);
			index = tmp_index[tmp_rand];
		}
		*/
	}
	//リペアオペレーター
	else {
		if (mode == 1) {
			return -1;
		}
		else {
			tmp_stra = nim_status_vec;
			const int tmp_count = int(std::count(tmp_stra.begin(), tmp_stra.end(), 1));

			std::vector<int> tmp_index(tmp_count);

			auto result_t = std::find(tmp_stra.begin(), tmp_stra.end(), 1);
			tmp_index[0] = int(std::distance(tmp_stra.begin(), result_t));

			for (int i = 1; i < tmp_count; i++) {
				result_t = std::find(result_t + 1, tmp_stra.end(), 1);
				tmp_index[i] = int(std::distance(tmp_stra.begin(), result_t));
			}
			index = tmp_index[GetRand_Int(tmp_count)];
			//std::cout << "index : " << index << std::endl;
			//show_vec_1(stra);
			if (stra[index] == 0) {
				stra[index] = 1;
			}
			//show_vec_1(stra);
		}
	}
	return index;
}
void nim::input_stra_first(const std::vector<int> &pop) 
{
	pop_stra = pop;
}
void nim::output_stra_first(std::vector<int> &pop)
{ 
	pop = pop_stra;
}
void nim::input_stra_last(const std::vector<int> &opp) 
{
	opp_stra = opp;
}
void nim::output_stra_last(std::vector<int> &opp)
{
	opp = opp_stra;
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
