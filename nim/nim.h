#pragma once
#include<iostream>
#include<vector>
#include <algorithm>
#include<cmath>
#include"config_nim.hpp"
#include"../header/Usual_Methods.hpp"

class nim {
public:
	nim()
	{
		nim_n = NIM;
		nim_status.resize(NIM);
		stra_len = (POLL1 + 1)*(POLL2 + 1)*(POLL3 + 1);
		nim_status_vec.resize(stra_len);
		Init();
		cal_optimal();
	}
	void main_task();
protected:
	int nim_n;	//山の数
	int stra_len;
	std::vector<int> nim_status;	 //それぞれの山の石の数
	std::vector<int> nim_status_vec; //
	std::vector<int> opt;			 //最適解

	std::vector<int> pop_stra;
	std::vector<int> opp_stra;

	void Init();
	void cal_binary_vec(const int n, std::vector<int> &input); //10進数を2進数のベクターに変換する
	void xor_vec(std::vector<int> &one, std::vector<int> &ano, std::vector<int> &vec); //ベクター同士の排他的論理和
	void cal_optimal(); //最適解を求める
	void generate_mont(int a, int b, int c, std::vector<std::vector<int>> &mont);
};
void nim::Init() {
	nim_status[0] = POLL1;
	nim_status[1] = POLL2;
	nim_status[2] = POLL3;
	nim_status_vec[stra_len - 1] = 1;
}
void nim::main_task() {
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
void nim::xor_vec(std::vector<int> &one, std::vector<int> &ano, std::vector<int> &input) 
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
	const auto max = *max_element(nim_status.begin(), nim_status.end());
	std::vector<std::vector<int>> mont;

	for (int i = 0; i < nim_status[0] + 1; i++) {
		for (int j = 0; j < nim_status[1] + 1; j++) {
			for (int k = 0; k < nim_status[2] + 1; k++) {
				mont = std::vector<std::vector<int>>(max, std::vector<int>(NIM, 0));
				generate_mont(i, j, k, mont);
				//show_vec_2(mont);

				std::vector<int> tmp_xor_sum;
				std::vector<int> xor_sum;
				tmp_xor_sum = mont[0];
				for (int i = 1; i < max; i++) {
					xor_vec(tmp_xor_sum, mont[i], xor_sum);
					tmp_xor_sum = xor_sum;
				}
				for (int i = 1; i < NIM; i++) {
					if (xor_sum[i - 1] == xor_sum[i]) {
						xor_sum[i] = 0;
					}
					else {
						xor_sum[i] = 1;
					}
				}
				if (xor_sum[NIM - 1] == 0) {
					//最適解に加える
					int index = i * (nim_status[0]*nim_status[0]) + j * nim_status[1] + k * 1;
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
void nim::generate_mont(int a, int b, int c, std::vector<std::vector<int>> &mont) {
	if (mont.empty()) {
		std::cout << "montが空です" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::vector<int> tmp_nim_status = { a,b,c };
	std::cout << "(a,b,c) = ";
	show_vec_1(tmp_nim_status);
	const auto max = *max_element(tmp_nim_status.begin(), tmp_nim_status.end());
	for (int i = 1; i < max+1; i++) {
		for (int j = 0; j < NIM; j++) {
			if (i <= tmp_nim_status[j]) {
				mont[i-1][j] = 1;
			}
		}
	}
}
