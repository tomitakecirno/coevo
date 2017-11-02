#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include"config_nim.hpp"

class nim {
public:
	nim()
	{
		nim_n = NIM;
		nim_status.resize(NIM);
		nim_status[0] = POLL1;
		nim_status[1] = POLL2;
		nim_status[2] = POLL3;

		stra_len = int(std::pow(POLL1+ POLL2+ POLL3,2));
		/*
		for (int i = 0; i < POLL1 + 1; i++) {
			for (int j = 0; j < POLL2 + 1; j++) {
				for (int k = 0; k < POLL3 + 1; k++) {
					if()
				}
			}
		}
		*/
	}

	void main_task();
protected:
	int nim_n;	//R‚Ì”
	int stra_len;
	std::vector<int> nim_status; //‚»‚ê‚¼‚ê‚ÌR‚ÌÎ‚Ì”
	std::vector<int> opt;		 //Å“K‰ğ

	std::vector<int> pop_stra;
	std::vector<int> opp_stra;

	void cal_binary_vec(const int n, std::vector<int> &input);
	void xor(std::vector<int> &one, std::vector<int> &ano, std::vector<int> &vec);
};
void nim::cal_binary_vec(const int n, std::vector<int> &input) 
{
	int zero_len = 0;
	input.resize(stra_len);

	int syou = n;
	int amari;
	while (1 < syou) {
		amari = syou % 2;
		syou  = syou / 2;
		input[stra_len - zero_len - 1] = amari;
		zero_len++;
	}
	if (zero_len < stra_len) {
		for (int i = zero_len; i < stra_len; i++) {
			input[stra_len - zero_len - 1] = 0;
		}
	}
	int sum = 0;
	for (int i = stra_len; i > -1; i--) {

	}
}
void nim::xor(std::vector<int> &one, std::vector<int> &ano, std::vector<int> &vec) {
	vec.resize(stra_len);
	for (int i = 0; i < stra_len; i++) {
		if (one[i] == ano[i]) {
			vec[i] = 0;
		}
		else {
			vec[i] = 1;
		}
	}
}
void nim::main_task() {

}
