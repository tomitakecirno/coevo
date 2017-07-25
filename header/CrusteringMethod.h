/*
分けるクラスタの数を固定化して実験してみる
（ツリー構造にすると大がかりなため）
*/
#pragma once
//#include "Cruster_Config.h"
#include "config.hpp"
#include <math.h>
#include <forward_list>
#include "coansmodule.hpp"

//double Cal_Uclidean(std::vector<double> &a, std::vector<double> &b);
double Range_Ward(double D_io, double D_jo, double D_ij, int N_i, int N_j, int N_o);

//int Cal_Vec_Length(int Pop_Size);
void Cru_Upgma(std::vector<playerTK> &Pop, int k) {
	int Pop_Length = int(Pop.size());				//自集団のサイズ
	int N = Pop_Length;								//現在のクラスタの数

	//距離Matrix
	std::vector<std::vector<double> > Vec_Dis(Pop_Length);	//距離を格納
	std::vector<int> Group_Index(Pop_Length);	//個体のクラスタ番号
	std::vector<int> Group_Num(Pop_Length);		//クラスタ毎の個体数

	for (int i = 0; i < Pop_Length; i++) {
		Vec_Dis[i].resize(Pop_Length);
		Group_Index[i] = i;
		Group_Num[i] = 1;
	}
	//距離Matrix生成
	for (int i = 0; i < Pop_Length; i++) {
		Vec_Dis[i][i] = 10000;
		for (int j = i+1; j < Pop_Length; j++) {
			//距離計算
			Vec_Dis[i][j] = cal_kotai_distance(Pop[i], Pop[j]);
			Vec_Dis[j][i] = Vec_Dis[i][j];
		}
	}
	//クラスタの数が既定数以下になったら終了
	while (N > k) {
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
			if (Group_Index[i] == Group_Index[Min_Index_A] || Group_Index[i] == Group_Index[Min_Index_B]) {
				for (int j = 0; j < Pop_Length; j++) {
					if (Group_Index[j] != Group_Index[Min_Index_A] && Group_Index[j] != Group_Index[Min_Index_B]) {
						//ウォード法で距離更新
						Vec_Dis[i][j] = Range_Ward(Vec_Dis[Min_Index_A][j], Vec_Dis[Min_Index_B][j], Vec_Dis[Min_Index_A][Min_Index_B],
							Group_Num[Min_Index_A], Group_Num[Min_Index_B], Group_Num[j]);
						Vec_Dis[j][i] = Vec_Dis[i][j];
					}
				}
			}
		}

		//クラスタ数更新
		Group_Num[Min_Index_A] += Group_Num[Min_Index_B];
		Group_Num[Min_Index_B] = Group_Num[Min_Index_A];
		//クラスタ番号更新
		for (int i = 0; i < Pop_Length; i++) {
			if (Group_Index[i] == Group_Index[Min_Index_B] && i != Min_Index_B) {
				Group_Index[i] = Group_Index[Min_Index_A];
			}
		}
		Group_Index[Min_Index_B] = Group_Index[Min_Index_A];

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
		/*
		std::cout << "距離表示" << std::endl;
		Show_Distance(Vec_Dis);
		//表示
		std::cout << "2-4" << std::endl;;
		std::cout << "Pop_Num = ";
		Show_Vector(Group_Index);
		std::cout << "Group_Num = ";
		Show_Vector(Group_Num);
		*/
		N--;
	}
	for (int i = 0; i < Pop_Length; i++) {
		Pop[i].nitch = Group_Index[i];
	}
}

/*
double Cal_Uclidean(std::vector<double> &a, std::vector<double> &b) {
	int a_Length = int(a.size());
	double Tmp_Dis_sum = 0;
	for (int k = 0; k < a_Length; k++) {
		Tmp_Dis_sum += (b[k] - a[k])*(b[k] - a[k]);
	}
	return sqrt(Tmp_Dis_sum);
}
*/

double Range_Ward(double D_io, double D_jo, double D_ij, int N_i, int N_j, int N_o) {
	int nk;
	double r = 0;
	nk = N_i + N_j;
	r = ((N_i + N_o) * D_io + (N_j + N_o) * D_jo - N_o * D_ij) / ((double)nk + N_o);

	return r;
}