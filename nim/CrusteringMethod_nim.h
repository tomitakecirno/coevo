/*
分けるクラスタの数を固定化して実験してみる
（ツリー構造にすると大がかりなため）
*/
#pragma once
#include "config_nim.hpp"
#include <math.h>
#include <forward_list>
#include "coansmodule_nim.hpp"

#define M 2
#define C 3
#define DEM 2

double Range_Ward(double D_io, double D_jo, double D_ij, int N_i, int N_j, int N_o);
bool diff(std::vector<std::vector<double>> &Uk_1, std::vector<std::vector<double>> &Uk_2);

//階層的クラスタリング
void Cru_Upgma(std::vector<playerNim> &Pop) {
	const int Pop_Length = int(Pop.size());				//自集団のサイズ
	int N = Pop_Length;								//現在のクラスタの数

	//距離Matrix
	std::vector<std::vector<double> > Vec_Dis(Pop_Length);	//距離を格納
	std::vector<int> Group_Index(Pop_Length);	//個体のクラスタ番号
	std::vector<int> Group_Num(Pop_Length);		//クラスタ毎の個体数

	//std::cout << "1-1" << ',';
	for (int i = 0; i < Pop_Length; i++) {
		Vec_Dis[i].resize(Pop_Length);
		Group_Index[i] = i;
		Group_Num[i] = 1;
	}
	//std::cout << "1-2" << ',';
	//距離Matrix生成
	for (int i = 0; i < Pop_Length; i++) {
		Vec_Dis[i][i] = 10000;
		for (int j = i + 1; j < Pop_Length; j++) {
			//距離計算
			Vec_Dis[i][j] = cal_euclidean(Pop[i].stra, Pop[j].stra);
			assert(0 <= Vec_Dis[i][j]);
			Vec_Dis[j][i] = Vec_Dis[i][j];
		}
	}
	//std::cout << "1-3" << ',';
	//クラスタの数が既定数以下になったら終了
	while (N > K_UPGMA) {
		//最小値を求める
		auto Min = min_element(Vec_Dis[0].begin(), Vec_Dis[0].end());
		double Min_Value = *Min;
		int Min_Index_A = 0;
		int Min_Index_B = int(distance(Vec_Dis[0].begin(), Min));
		for (int i = 1; i < N; i++) {
			if (*(Min = min_element(Vec_Dis[i].begin(), Vec_Dis[i].end())) < Min_Value) {
				Min_Value = *Min;
				Min_Index_A = i;
				Min_Index_B = int(distance(Vec_Dis[i].begin(), Min));
			}
		}
		//距離更新
		for (int i = 0; i < Pop_Length; i++) {
			//選ばれた個体と同じクラスタの個体
			if (Group_Index[i] == Group_Index[Min_Index_A] || Group_Index[i] == Group_Index[Min_Index_B]) {
				for (int j = 0; j < Pop_Length; j++) {
					if (Group_Index[j] != Group_Index[Min_Index_A] && Group_Index[j] != Group_Index[Min_Index_B]) {
						//ウォード法で距離更新
						Vec_Dis[i][j] = Range_Ward(Vec_Dis[Min_Index_A][j], Vec_Dis[Min_Index_B][j], Vec_Dis[Min_Index_A][Min_Index_B],
							Group_Num[Min_Index_A], Group_Num[Min_Index_B], Group_Num[j]);
						assert(0 <= Vec_Dis[i][j]);
						Vec_Dis[j][i] = Vec_Dis[i][j];
					}
				}
			}
		}
		//クラスタ数更新andクラスタ番号更新
		const int tmp_index_A = Group_Index[Min_Index_A];
		const int tmp_index_B = Group_Index[Min_Index_B];
		if (tmp_index_A < tmp_index_B) {
			Group_Num[Min_Index_A] += Group_Num[Min_Index_B];
			Group_Num[Min_Index_B] = 0;
			for (int i = 0; i < Pop_Length; i++) {
				if (Group_Index[i] == tmp_index_B) {
					Group_Index[i] = tmp_index_A;
				}
			}
		}
		else if (tmp_index_A > tmp_index_B) {
			Group_Num[Min_Index_B] += Group_Num[Min_Index_A];
			Group_Num[Min_Index_A] = 0;
			for (int i = 0; i < Pop_Length; i++) {
				if (Group_Index[i] == tmp_index_A) {
					Group_Index[i] = tmp_index_B;
				}
			}
		}
		//同じクラスタに属している個体同士の距離を10000くらいにする
		for (int i = 0; i < Pop_Length; i++) {
			if (Group_Index[i] == Group_Index[Min_Index_A]) {
				for (int j = 0; j < Pop_Length; j++) {
					if (Group_Index[j] == Group_Index[Min_Index_A]) {
						Vec_Dis[i][j] = 10000;
						Vec_Dis[j][i] = 10000;
					}
				}
			}
		}
		N--;
		//std::cout << "Min_Index:" << "[" << Min_Index_A << "," << Min_Index_B << "]" << std::endl;
		//std::cout << "Group_Num:";
		//Show_Vector_1(Group_Num);
		//std::cout << "Group_Index:";
		//Show_Vector_1(Group_Index);
		//std::cout << "Vec_Dis:";
		//Show_Vector_2(Vec_Dis);
	}
	//std::cout << "1-4" << ',';
	int size = 0;
	std::vector<int> pop_index(KO);
	const auto max = *max_element(Group_Index.begin(), Group_Index.end());
	for (int i = 0; i < max + 1; i++) {
		if (std::count(Group_Index.begin(), Group_Index.end(), i) > 0) {
			for (int j = 0; j < Pop_Length; j++) {
				if (Group_Index[j] == i) {
					Pop[j].nitch = size;
					pop_index[j] = size;
				}
			}
			size++;
		}
	}
	//show_vec_1(pop_index);
}
double Range_Ward(double D_io, double D_jo, double D_ij, int N_i, int N_j, int N_o) {
	int nk;
	double r = 0;
	nk = N_i + N_j;
	r = ((N_i + N_o) * D_io + (N_j + N_o) * D_jo - N_o * D_ij) / ((double)nk + N_o);

	return r;
}

//fuzzy-c-means
void fuzzy_x_means()
{
	std::random_device rd;
	std::mt19937 mt(rd());

	//test
	std::uniform_real_distribution<> dist_n(0, 10);
	std::vector<std::vector<double>> pop;
	pop = std::vector<std::vector<double>>(KO, std::vector<double>(DEM));
	for (int i = 0; i < KO; i++) {
		for (int j = 0; j < DEM; j++) {
			pop[i][j] = dist_n(mt);
		}
	}
	//初期値セット
	std::uniform_real_distribution<> dist(0, 1);
	std::vector<std::vector<double>> Uk;
	std::vector<std::vector<double>> delta_Uk;
	Uk = std::vector<std::vector<double>>(KO, std::vector<double>(C));
	for (int i = 0; i < KO; i++) {
		for (int j = 0; j < C; j++) {
			Uk[i][j] = dist(mt);
		}
	}
	while (diff(Uk, delta_Uk)) {
		//クラスタの中心を更新
		std::vector<std::vector<double>> _x;
		_x = std::vector<std::vector<double>>(C, std::vector<double>(DEM, 0));
		for (int i = 0; i < C; i++) {
			std::vector<double> sum_1(DEM, 0);
			double sum_2 = 0;
			for (int j = 0; j < KO; j++) {
				const double delta = std::pow(Uk[j][i], M);
				for (int k = 0; k < DEM; k++) {
					sum_1[k] += delta*pop[j][k]; //分子
				}
				sum_2 += delta; //分母
			}
			for (int j = 0; j < DEM; j++) {
				_x[i][j] = sum_1[j] / sum_2;
			}
		}
		delta_Uk = std::vector<std::vector<double>>(KO, std::vector<double>(C, 0));
		for (int k = 0; k < C; k++) {
			for (int i = 0; i < KO; i++) {
				const double n = 2 / (M - 1);
				const double dis_ik = cal_euclidean(pop[i], _x[k]);
				double sum = 0;
				for (int j = 0; j < C; j++) {
					const double dis_ij = cal_euclidean(pop[i], _x[j]);
					sum += std::pow(dis_ik / dis_ij, n);
				}
				delta_Uk[i][k] = 1 / sum;
			}
		}
	}
}
bool diff(std::vector<std::vector<double>> &Uk_1, std::vector<std::vector<double>> &Uk_2) 
{
	if (Uk_2.empty()) {
		return true;
	}
	double sum = 0;
	for (int i = 0; i < KO; i++) {
		sum += cal_euclidean(Uk_1[i], Uk_2[i]);
	}
	std::cout << "sum : " << sum << std::endl;
	if (sum <  0.01) {
		return false;
	}
	else {
		Uk_1 = Uk_2;
		return true;
	}
}
bool gath_geva(std::vector<std::vector<double>> &x, std::vector<std::vector<double>> &Uk_1, std::vector<std::vector<double>> &_x)
{
	const int cru_size = int(Uk_1[0].size());
	std::vector<double> F(cru_size);
	for (int i = 0; i < cru_size; i++) {
		double sum_1 = 0;
		double sum_2 = 0;
		for (int j = 0; j < KO; j++) {
			const double delta = std::pow(Uk_1[j][i], M);
			double diff_xv = 0;
			for (int k = 0; k < DEM; k++) {
				diff_xv += (x[j][k] - _x[i][k])*(x[j][k] - _x[i][k]);
			}
			sum_1 += delta*diff_xv; //分母
			sum_2 += delta;
		}
		F[i] = sum_1 / sum_2;
	}
}